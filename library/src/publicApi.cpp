#include <cstring>

#include "zstd.h"
#include "lz4.h"

#include "publicApi.h"

#include "converters.h"
#include "logging.h"
#include "texture.h"
#include "formats/dds/ddsSupport.h"
#include "formats/png/pngSupport.h"
#include "formats/hdr/hdrSupport.h"
#include "formats/tga/tgaSupport.h"
#include "formats/pvr/pvrSupport.h"
#include "pixelFormat.h"

int64_t get_buffer_size_from_texture_format(uint32_t
                                            width,
                                            uint32_t height, ePixelFormat
                                            pixelFormat) {
    return calculateTextureSize(width, height, pixelFormat);
}

int64_t get_buffer_size_from_texture(const sTexture *texture) {
    return calculateTextureSize(texture);
}

bool is_compressed_pixel_format(ePixelFormat pixelFormat) {
    return isCompressedPixelFormat(pixelFormat);
}

sTexture *
create_texture(const uint8_t *data, size_t dataSize, uint32_t width, uint32_t height, ePixelFormat pixelFormat) {
    return createTexture(data, dataSize, width, height, pixelFormat);
}

sTexture *create_uninitialized_texture() {
    return createTexture();
}

sTexture *create_empty_texture(uint32_t
                               width,
                               uint32_t height, ePixelFormat
                               pixelFormat) {
    return
            createTexture(width, height, pixelFormat
            );
}

bool convert_texture(const sTexture *from_texture, sTexture *to_texture) {
    return convertTexture(from_texture, to_texture);
}

bool get_texture_data(const sTexture *texture, char *buffer, uint32_t buffer_size) {
    if (calculateTextureSize(texture) < buffer_size) {
        return false;
    }
    memcpy(buffer, texture->m_rawPixelData.data(), texture->m_rawPixelData.size());
    return true;
}

void free_texture(sTexture *texture) {
    freeTexture(texture);
}

sTexture *load_dds(const char *filename) {
    sTexture *texture = createTexture();
    if (loadDDS(filename, texture)) {
        return texture;
    }
    freeTexture(texture);
    return nullptr;
}

sTexture *load_dds_from_data(const char *data, uint32_t size) {
    sTexture *texture = createTexture();
    if (loadDDS(reinterpret_cast<const uint8_t *>(data), size, texture)) {
        return texture;
    }
    freeTexture(texture);
    return nullptr;
}

sTexture *load_tga(const char *filename, int expected_channels) {
    sTexture *texture = createTexture();
    if (loadTGA(filename, texture, expected_channels)) {
        return texture;
    }
    freeTexture(texture);
    return nullptr;
}

sTexture *load_png(const char *filename, int expected_channels) {
    sTexture *texture = createTexture();
    if (loadPNG(filename, texture, expected_channels)) {
        return texture;
    }
    freeTexture(texture);
    return nullptr;
}

bool write_png(const char *filename, const sTexture *texture) {
    return writePNG(filename, texture);
}

bool write_tga(const char *filename, const sTexture *texture) {
    return writeTGA(filename, texture);
}


sTexture *load_hdr(const char *filename) {
    sTexture *texture = createTexture();
    if (loadHDR(filename, texture)) {
        return texture;
    }
    freeTexture(texture);
    return nullptr;
}

sTexture *load_pvr(const char *filename) {
    sTexture *texture = createTexture();
    if (loadPVR(filename, texture)) {
        return texture;
    }
    freeTexture(texture);
    return nullptr;
}


uint32_t get_texture_width(const sTexture *texture) {
    return texture->m_width;
}

uint32_t get_texture_height(const sTexture *texture) {
    return texture->m_height;
}

ePixelFormat get_texture_pixel_format(const sTexture *texture) {
    return texture->m_pixelFormat;
}

bool flip_texture(const sTexture *in_texture, sTexture *out_texture, bool flip_ud, bool flip_lr) {
    return flipTexture(in_texture, out_texture, flip_ud, flip_lr);
}

ePixelFormat get_uncompressed_pixel_format_variant(ePixelFormat pixelFormat) {
    return getUncompressedPixelFormatVariant(pixelFormat);
}

size_t zstd_decompress(void *dst, size_t dstCapacity, const void *src, size_t compressedSize) {
    return ZSTD_decompress(dst, dstCapacity, src, compressedSize);
}

size_t lz4_decompress(void *dst, size_t dstCapacity, const void *src, size_t compressedSize) {
    return LZ4_decompress_safe((char *) src, (char *) dst, compressedSize, dstCapacity);
}

void print_all_converters() {
    for (auto c_converter: cConverters) {
        uint32_t key = c_converter.first;
        const char *f1 = getPixelFormatName(static_cast<ePixelFormat>(key & 0xFFFF));
        const char *f2 = getPixelFormatName(static_cast<ePixelFormat>(key >> 16));
        loggerEx(eLogLevel::INFO, std::format("Converter ({}) {} -> {}\n", key, f1, f2));
    }
}

extern eLogLevel logLevel;
void set_log_level(uint32_t level){
    logLevel = (eLogLevel)level;
}