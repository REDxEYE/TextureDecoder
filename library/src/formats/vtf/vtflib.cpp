#include <algorithm>
#include "formats/vtf/vtflib.h"

VTFFile::VTFFile(uint8_t *data, size_t data_size) {
    m_data = data;
    m_data_size = data_size;
}

VTFResource *VTFFile::get_resource(uint32_t resource_ident) {
    if (version_major() == 7 && version_minor() >= 3) {
        uint32_t res_count = resource_count();
        if (res_count > 0) {
            for (int i = 0; i < res_count; i++) {
                VTFResource *res = (VTFResource *) ((m_data + sizeof(VTFHeader_73A)) + sizeof(VTFResource) * i);
                if ((res->type & 0x00FFFFFF) == resource_ident) {
                    return res;
                }
            }
        }
    }
    return nullptr;
}

static uint8_t block_sizes[(uint32_t) VTFImageFormat::COUNT]{
        4, 4, 3, 3, 2, 1, 2, 1, 1, 3, 3, 4, 4, 8, 16, 16,
        4, 2, 2, 2, 8, 2, 2, 4, 8, 8, 4, 4, 12, 16, 0, 0,
        0, 0, 0, 0, 0, 16, 8
};

uint8_t *VTFFile::get_highres_image(size_t &buffer_size) {
    uint8_t *raw_pixel_data = nullptr;
    if (version_major() == 7 && version_minor() >= 3) {
        VTFResource *image_res = get_resource(0x30);
        if (image_res != nullptr) {
            raw_pixel_data = m_data + image_res->data;
        }
    } else {
        raw_pixel_data = m_data + header()->header_size;
        if (header70()->low_res_image_width > 0 && header70()->low_res_image_height > 0) {
            if (header70()->low_res_image_format != VTFImageFormat::DXT1) {
                return nullptr;
            }
            raw_pixel_data += (header70()->low_res_image_height * header70()->low_res_image_width) / 2;
        }
    }
    if (raw_pixel_data == nullptr) {
        return nullptr;
    }
    uint32_t mip_count = header70()->mip_count;
    uint32_t min_block_size = 1;
    VTFImageFormat image_format = header70()->image_format;
    uint8_t block_size = block_sizes[(uint32_t) image_format];
    uint32_t w = width();
    uint32_t h = height();
    if (
            image_format == VTFImageFormat::DXT1 ||
            image_format == VTFImageFormat::DXT1_ONEBITALPHA ||
            image_format == VTFImageFormat::DXT3 ||
            image_format == VTFImageFormat::DXT5
            ) {
        min_block_size = 4;
    }

    for (uint32_t i = mip_count - 1; i > 0; i--) {
        size_t mip_size = (std::max((w >> i), min_block_size) * std::max((h >> i), min_block_size) * block_size) /
                          (min_block_size * min_block_size) * header70()->frames;
        raw_pixel_data += mip_size;

    }
    buffer_size = m_data_size - (raw_pixel_data - m_data);
    return raw_pixel_data;

}

VTFResource *VTFFile::resource(int32_t resource_id) {
    if (version_major() == 7 && version_minor() >= 3) {
        if (resource_id >= resource_count())
            return nullptr;
        return (VTFResource *) ((m_data + sizeof(VTFHeader_73A)) + sizeof(VTFResource) * resource_id);
    }
    return nullptr;
}
