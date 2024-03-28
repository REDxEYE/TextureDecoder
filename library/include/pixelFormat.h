#pragma once

#include <cstdint>

enum ePixelFormat : uint16_t {
    INVALID,
    RGBA32,
    RGB32,
    RG32,
    R32,
    RGBA16,
    RGB16,
    RG16,
    RG16_SIGNED,
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
    BGRA8888,
    ABGR8888,
    ARGB8888,
    RGB888,
    BGR888,
    RG88,
    RA88,
    R8,
    RGB565,
    RGBA5551,
    RGBA1010102,
    RGBA4444,
    BC1,
    BC1a,
    BC2,
    BC3,
    BC4,
    BC5,
    BC6,
    BC7,
    ETC1,
    RGBA1111,
};

const char *getPixelFormatName(ePixelFormat pixelFormat);

int getChannelCount(ePixelFormat pixelFormat);
int getPixelFormatPixelSize(ePixelFormat pixelFormat);

bool isCompressedPixelFormat(ePixelFormat pixelFormat);

ePixelFormat getUncompressedPixelFormatVariant(ePixelFormat pixelFormat);