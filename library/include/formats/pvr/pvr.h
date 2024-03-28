#pragma once

#include <cstdint>
#include <vector>

enum ePvrFlags : uint32_t {
    NONE = 0x00,
    PREMULTIPLIED_ALPHA = 0x02,
};

enum class ePvrPixelFormat : uint32_t {
    PVRTC_2bpp_RGB = 0,
    PVRTC_2bpp_RGBA = 1,
    PVRTC_4bpp_RGB = 2,
    PVRTC_4bpp_RGBA = 3,
    PVRTCII_2bpp = 4,
    PVRTCII_4bpp = 5,
    ETC1 = 6,
    DXT1 = 7,
    DXT2 = 8,
    DXT3 = 9,
    DXT4 = 10,
    DXT5 = 11,
    BC1 = 12, // Equivalent to DXT1
    BC2 = 13, // Equivalent to DXT3
    BC3 = 14, // Equivalent to DXT5
    BC4 = 15,
    BC5 = 16,
    BC6 = 17,
    BC7 = 18,
    UYVY = 19,
    YUY2 = 20,
    BW1bpp = 21,
    R9G9B9E5_SharedExponent = 22,
    RGBG8888 = 23,
    GRGB8888 = 24,
    ETC2_RGB = 25,
    ETC2_RGBA = 26,
    ETC2_RGB_A1 = 27,
    EAC_R11_Unsigned = 28,
    EAC_R11_Signed = 29,
    EAC_RG11_Unsigned = 30,
    EAC_RG11_Signed = 31,
    ASTC_4x4 = 32,
    ASTC_5x4 = 33,
    ASTC_5x5 = 34,
    ASTC_6x5 = 35,
    ASTC_6x6 = 36,
    ASTC_8x5 = 37,
    ASTC_8x6 = 38,
    ASTC_8x8 = 39,
    ASTC_10x5 = 40,
    ASTC_10x6 = 41,
    ASTC_10x8 = 42,
    ASTC_10x10 = 43,
    ASTC_12x10 = 44,
    ASTC_12x12 = 45,
};

struct sPvrHeader {
    uint32_t version;
    ePvrFlags flags;
    union {
        uint8_t pixelFormatBytes[8];
        struct {
            ePvrPixelFormat pixelFormat;
            uint32_t unused;
        };
        struct {
            char channelOrder[4];
            uint8_t channelBBP[4];
        };
    };
    uint32_t colorSpace;
    uint32_t channelType;
    uint32_t width, height;
    uint32_t depth;
    uint32_t surfaceCount;
    uint32_t faceCount;
    uint32_t mipCount;
    uint32_t metaDataSize;
};

struct sPvrMetaData {
    uint32_t ident;
    uint32_t key;
    uint32_t size;
    std::vector<uint8_t> data;
};