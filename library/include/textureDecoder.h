#pragma once

#include <vector>
#include <cstdint>

char *formatMessage(const char *fmt, ...);

void defaultLogger(uint8_t level, const char *message);

typedef void (*loggerFn)(uint8_t, const char *);

extern loggerFn logger;

enum ePixelFormat : uint16_t {
    INVALID,
    RGBA32,
    RGB32,
    RG32,
    R32,
    RGBA16,
    RGB16,
    RG16,
    R16,
    RGBA32F,
    RGB32F,
    RG32F,
    R32F,
    RGBA16F,
    RGB16F,
    RG16F,
    R16F,
    RGBA8888,
    RGB888,
    RG88,
    RA88,
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
    uint32_t m_width{0};
    uint32_t m_height{0};
    ePixelFormat m_pixelFormat{0};
};


const char *getPixelName(ePixelFormat pixelFormat);


int64_t calculateTextureSize(uint32_t width, uint32_t height, ePixelFormat pixelFormat);

int64_t calculateTextureSize(const sTexture *texture);

sTexture *createTexture(const uint8_t *data, size_t dataSize,
                        uint32_t width, uint32_t height,
                        ePixelFormat pixelFormat);

sTexture *createTexture(uint32_t width, uint32_t height,
                        ePixelFormat pixelFormat);

sTexture *createTexture();

bool convertTexture(const sTexture *from_texture, sTexture *to_texture);

void freeTexture(sTexture *texture);


