#include <cstring>
#include "api.h"
#include "texture.h"
#include "dds/ddsSupport.h"
#include "png/pngSupport.h"
#include "hdr/hdrSupport.h"
#include "tga/tgaSupport.h"

int64_t get_buffer_size_from_texture_format(uint32_t width, uint32_t height, ePixelFormat pixelFormat) {
    return calculateTextureSize(width, height, pixelFormat);
}

int64_t get_buffer_size_from_texture(const sTexture *texture) {
    return calculateTextureSize(texture);
}

sTexture *
create_texture(const uint8_t *data, size_t dataSize, uint32_t width, uint32_t height, ePixelFormat pixelFormat) {
    return createTexture(data, dataSize, width, height, pixelFormat);
}

sTexture *create_empty_texture(uint32_t width, uint32_t height, ePixelFormat pixelFormat) {
    return createTexture(width, height, pixelFormat);
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

uint32_t get_texture_width(const sTexture *texture) {
    return texture->m_width;
}

uint32_t get_texture_height(const sTexture *texture) {
    return texture->m_height;
}

ePixelFormat get_texture_pixel_format(const sTexture *texture) {
    return texture->m_pixelFormat;
}

