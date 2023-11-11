#pragma once

#include <vector>

enum ePixelFormat : uint16_t {
    INVALID,
    RGBA8888,
    RGB888,
    RG88,
    R8,
    RGB565,
    RGBA5551,
    BC1,
    BC1a,
    BC2,
    BC3,
    BC4,
    BC5,
    BC6,
    BC7

};

struct sTexture {
    std::vector<uint8_t> m_rawPixelData;
    uint32_t m_width;
    uint32_t m_height;
    ePixelFormat m_pixelFormat;
    bool m_sRGB;
    bool m_normalized;
    uint32_t m_flags;
};

int64_t calculateTextureSize(uint32_t width, uint32_t height, ePixelFormat pixelFormat);

sTexture *createTexture(const uint8_t *data, size_t dataSize,
                        uint32_t width, uint32_t height,
                        ePixelFormat pixelFormat, bool sRGB, bool normalized);

sTexture *createTexture(uint32_t width, uint32_t height,
                        ePixelFormat pixelFormat, bool sRGB, bool normalized);

bool convertTexture(const sTexture *from_texture, sTexture *to_texture);

void freeTexture(sTexture *texture);

