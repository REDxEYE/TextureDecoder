#include <cstring>
#include "api.h"
#include "texture.h"

int64_t calculate_texture_size_whpf(uint32_t width, uint32_t height, ePixelFormat pixelFormat) {
    return calculateTextureSize(width, height, pixelFormat);
}

int64_t calculate_texture_size(sTexture *texture) {
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

bool get_texture_data(sTexture *texture, char *buffer, uint32_t buffer_size) {
    if (calculateTextureSize(texture) < buffer_size) {
        return false;
    }
    memcpy(buffer, texture->m_rawPixelData.data(), texture->m_rawPixelData.size());
    return true;
}

void free_texture(sTexture *texture) {
    freeTexture(texture);
}

sTexture *load_dds(char *filename) {
    sTexture *texture = createTexture();
    if (loadDDS(filename, texture)) {
        return texture;
    }
    freeTexture(texture);
    return nullptr;
}

sTexture *load_png(char *filename, int expected_channels) {
    sTexture *texture = createTexture();
    if (loadPNG(filename, texture, expected_channels)) {
        return texture;
    }
    freeTexture(texture);
    return nullptr;
}

sTexture *load_hdr(char *filename) {
    sTexture *texture = createTexture();
    if (loadHDR(filename, texture)) {
        return texture;
    }
    freeTexture(texture);
    return nullptr;
}

uint32_t get_texture_width(sTexture *texture) {
    return texture->m_width;
}

uint32_t get_texture_height(sTexture *texture) {
    return texture->m_height;
}

ePixelFormat get_texture_pixel_format(sTexture *texture) {
    return texture->m_pixelFormat;
}
