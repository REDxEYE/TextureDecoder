
#include <fstream>
#include "formats/vtf/vtfSupport.h"
#include "formats/vtf/vtflib.h"
#include "formats/vtf/enums.h"
#include "textureApi.h"
#include "logging.h"

namespace fs = std::filesystem;

bool loadVTF(const fs::path &filename, sTexture *texture) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        loggerEx(eLogLevel::ERROR, "Failed to open file\n");
        return false;
    }

    size_t fsize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> data(fsize);
    if (!file.read(reinterpret_cast<char *>(data.data()), fsize)) {
        return false;
    }
    bool res = loadVTF(data.data(), fsize, texture);
    file.close();
    return res;
}

bool loadVTF(uint8_t *data, size_t dataSize, sTexture *texture) {
    VTFFile vtf = VTFFile(data, dataSize);
    ePixelFormat pixel_format;
    bool special_case = false;
    switch (vtf.image_format()) {

        case VTFImageFormat::RGBA8888:
            pixel_format = ePixelFormat::RGBA8888;
            break;
        case VTFImageFormat::ABGR8888:
            pixel_format = ePixelFormat::ABGR8888;
            break;
        case VTFImageFormat::RGB888:
            pixel_format = ePixelFormat::RGB888;
            break;
        case VTFImageFormat::BGR888:
            pixel_format = ePixelFormat::BGR888;
            break;
        case VTFImageFormat::RGB565:
            pixel_format = ePixelFormat::RGB565;
            break;
        case VTFImageFormat::I8:
            pixel_format = ePixelFormat::R8;
            break;
        case VTFImageFormat::IA88:
            pixel_format = ePixelFormat::RA88;
            break;
        case VTFImageFormat::P8:
            pixel_format = ePixelFormat::R8;
            break;
        case VTFImageFormat::A8:
            pixel_format = ePixelFormat::R8;
            break;
        case VTFImageFormat::RGB888_BLUESCREEN:
            pixel_format = ePixelFormat::RGB888;
            break;
        case VTFImageFormat::BGR888_BLUESCREEN:
            pixel_format = ePixelFormat::RGB888;
            break;
        case VTFImageFormat::ARGB8888:
            pixel_format = ePixelFormat::ARGB8888;
            break;
        case VTFImageFormat::BGRA8888:
            pixel_format = ePixelFormat::BGRA8888;
            break;
        case VTFImageFormat::DXT1:
            pixel_format = ePixelFormat::BC1;
            break;
        case VTFImageFormat::DXT3:
            pixel_format = ePixelFormat::BC2;
            break;
        case VTFImageFormat::DXT5:
            pixel_format = ePixelFormat::BC3;
            break;
        case VTFImageFormat::BGRX8888:
            pixel_format = ePixelFormat::BGRA8888;
            break;
        case VTFImageFormat::BGR565:
            special_case = true;
            pixel_format = ePixelFormat::RGB565;
            break;
        case VTFImageFormat::BGRX5551:
            special_case = true;
            pixel_format = ePixelFormat::RGBA5551;
            break;
        case VTFImageFormat::BGRA4444:
            special_case = true;
            pixel_format = ePixelFormat::RGBA4444;
            break;
        case VTFImageFormat::DXT1_ONEBITALPHA:
            pixel_format = ePixelFormat::BC1a;
            break;
        case VTFImageFormat::BGRA5551:
            special_case = true;
            pixel_format = ePixelFormat::RGBA5551;
            break;
        case VTFImageFormat::UV88:
            pixel_format = ePixelFormat::RG88;
            break;
        case VTFImageFormat::UVWQ8888:
            pixel_format = ePixelFormat::RGBA8888;
            break;
        case VTFImageFormat::RGBA16161616F:
            pixel_format = ePixelFormat::RGBA16F;
            break;
        case VTFImageFormat::RGBA16161616:
            pixel_format = ePixelFormat::RGBA16;
            break;
        case VTFImageFormat::UVLX8888:
            pixel_format = ePixelFormat::RGBA8888;
            break;
        case VTFImageFormat::R32F:
            pixel_format = ePixelFormat::R32F;
            break;
        case VTFImageFormat::RGB323232F:
            pixel_format = ePixelFormat::RGB32F;
            break;
        case VTFImageFormat::RGBA32323232F:
            pixel_format = ePixelFormat::RGBA32F;
            break;
        case VTFImageFormat::ATI2N:
            pixel_format = ePixelFormat::BC5;
            break;
        case VTFImageFormat::ATI1N:
            pixel_format = ePixelFormat::BC4;
            break;
        default:
            break;
    }
    initTexture(texture,vtf.width(),vtf.height(),pixel_format);

    if (special_case) {
        sTexture cTexture;
        cTexture.m_height = texture->m_height;
        cTexture.m_width = texture->m_width;
        cTexture.m_pixelFormat = pixel_format;
        size_t buffer_size;
        uint8_t *buffer = vtf.get_highres_image(buffer_size);
        cTexture.m_rawPixelData.assign(buffer, buffer + buffer_size);
        ePixelFormat targetFormat;
        ePixelFormat flipFormat;
        switch (vtf.image_format()) {
            case VTFImageFormat::BGR565:
                targetFormat = ePixelFormat::RGB888;
                flipFormat = ePixelFormat::BGR888;
                break;
            case VTFImageFormat::BGRX5551:
            case VTFImageFormat::BGRA4444:
            case VTFImageFormat::BGRA5551:
                targetFormat = ePixelFormat::RGBA8888;
                flipFormat = ePixelFormat::BGRA8888;
                break;
            default:
                break;
        }
        texture->m_pixelFormat = targetFormat;
        if(!convertTexture(&cTexture,texture)){
            return false;
        }
        texture->m_pixelFormat = flipFormat;

    } else {
        size_t buffer_size;
        uint8_t *buffer = vtf.get_highres_image(buffer_size);
        texture->m_rawPixelData.assign(buffer, buffer + buffer_size);
    }
    loggerEx(eLogLevel::INFO, std::format("Loaded VTF {}x{} {}\n",vtf.width(),vtf.height(), getPixelFormatName(texture->m_pixelFormat)));

    return texture;

}
