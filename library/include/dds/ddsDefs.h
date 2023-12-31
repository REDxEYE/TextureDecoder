#pragma once


enum eDDSPixelFormatFlags : uint32_t {
    ALPHAPIXELS = 0x1,
    ALPHA = 0x2,
    FOURCC = 0x4,
    RGB = 0x40,
    YUV = 0x200,
    LUMINANCE = 0x20000,
};

struct sDDSPixelFormat {
    uint32_t m_size{0};
    eDDSPixelFormatFlags m_flags{eDDSPixelFormatFlags(0)};
    uint32_t m_fourCC{0};
    uint32_t m_rGBBitCount{0};
    uint32_t m_rBitMask{0};
    uint32_t m_gBitMask{0};
    uint32_t m_bBitMask{0};
    uint32_t m_aBitMask{0};
};
struct sDDSHeader {
    uint32_t m_size{0};
    uint32_t m_flags{0};
    uint32_t m_height{0};
    uint32_t m_width{0};
    uint32_t m_pitchOrLinearSize{0};
    uint32_t m_depth{0};
    uint32_t m_mipMapCount{0};
    uint32_t m_reserved1[11]{0};
    sDDSPixelFormat m_pixelFormat{0};
    uint32_t m_caps{0};
    uint32_t m_caps2{0};
    uint32_t m_caps3{0};
    uint32_t m_caps4{0};
    uint32_t m_reserved2{0};
};

enum eDX10PixelFormat : uint32_t {
    DX10_UNKNOWN = 0,
    DX10_R32G32B32A32_TYPELESS = 1,
    DX10_R32G32B32A32_FLOAT = 2,
    DX10_R32G32B32A32_UINT = 3,
    DX10_R32G32B32A32_SINT = 4,
    DX10_R32G32B32_TYPELESS = 5,
    DX10_R32G32B32_FLOAT = 6,
    DX10_R32G32B32_UINT = 7,
    DX10_R32G32B32_SINT = 8,
    DX10_R16G16B16A16_TYPELESS = 9,
    DX10_R16G16B16A16_FLOAT = 10,
    DX10_R16G16B16A16_UNORM = 11,
    DX10_R16G16B16A16_UINT = 12,
    DX10_R16G16B16A16_SNORM = 13,
    DX10_R16G16B16A16_SINT = 14,
    DX10_R32G32_TYPELESS = 15,
    DX10_R32G32_FLOAT = 16,
    DX10_R32G32_UINT = 17,
    DX10_R32G32_SINT = 18,
    DX10_R32G8X24_TYPELESS = 19,
    DX10_D32_FLOAT_S8X24_UINT = 20,
    DX10_R32_FLOAT_X8X24_TYPELESS = 21,
    DX10_X32_TYPELESS_G8X24_UINT = 22,
    DX10_R10G10B10A2_TYPELESS = 23,
    DX10_R10G10B10A2_UNORM = 24,
    DX10_R10G10B10A2_UINT = 25,
    DX10_R11G11B10_FLOAT = 26,
    DX10_R8G8B8A8_TYPELESS = 27,
    DX10_R8G8B8A8_UNORM = 28,
    DX10_R8G8B8A8_UNORM_SRGB = 29,
    DX10_R8G8B8A8_UINT = 30,
    DX10_R8G8B8A8_SNORM = 31,
    DX10_R8G8B8A8_SINT = 32,
    DX10_R16G16_TYPELESS = 33,
    DX10_R16G16_FLOAT = 34,
    DX10_R16G16_UNORM = 35,
    DX10_R16G16_UINT = 36,
    DX10_R16G16_SNORM = 37,
    DX10_R16G16_SINT = 38,
    DX10_R32_TYPELESS = 39,
    DX10_D32_FLOAT = 40,
    DX10_R32_FLOAT = 41,
    DX10_R32_UINT = 42,
    DX10_R32_SINT = 43,
    DX10_R24G8_TYPELESS = 44,
    DX10_D24_UNORM_S8_UINT = 45,
    DX10_R24_UNORM_X8_TYPELESS = 46,
    DX10_X24_TYPELESS_G8_UINT = 47,
    DX10_R8G8_TYPELESS = 48,
    DX10_R8G8_UNORM = 49,
    DX10_R8G8_UINT = 50,
    DX10_R8G8_SNORM = 51,
    DX10_R8G8_SINT = 52,
    DX10_R16_TYPELESS = 53,
    DX10_R16_FLOAT = 54,
    DX10_D16_UNORM = 55,
    DX10_R16_UNORM = 56,
    DX10_R16_UINT = 57,
    DX10_R16_SNORM = 58,
    DX10_R16_SINT = 59,
    DX10_R8_TYPELESS = 60,
    DX10_R8_UNORM = 61,
    DX10_R8_UINT = 62,
    DX10_R8_SNORM = 63,
    DX10_R8_SINT = 64,
    DX10_A8_UNORM = 65,
    DX10_R1_UNORM = 66,
    DX10_R9G9B9E5_SHAREDEXP = 67,
    DX10_R8G8_B8G8_UNORM = 68,
    DX10_G8R8_G8B8_UNORM = 69,
    DX10_BC1_TYPELESS = 70,
    DX10_BC1_UNORM = 71,
    DX10_BC1_UNORM_SRGB = 72,
    DX10_BC2_TYPELESS = 73,
    DX10_BC2_UNORM = 74,
    DX10_BC2_UNORM_SRGB = 75,
    DX10_BC3_TYPELESS = 76,
    DX10_BC3_UNORM = 77,
    DX10_BC3_UNORM_SRGB = 78,
    DX10_BC4_TYPELESS = 79,
    DX10_BC4_UNORM = 80,
    DX10_BC4_SNORM = 81,
    DX10_BC5_TYPELESS = 82,
    DX10_BC5_UNORM = 83,
    DX10_BC5_SNORM = 84,
    DX10_B5G6R5_UNORM = 85,
    DX10_B5G5R5A1_UNORM = 86,
    DX10_B8G8R8A8_UNORM = 87,
    DX10_B8G8R8X8_UNORM = 88,
    DX10_R10G10B10_XR_BIAS_A2_UNORM = 89,
    DX10_B8G8R8A8_TYPELESS = 90,
    DX10_B8G8R8A8_UNORM_SRGB = 91,
    DX10_B8G8R8X8_TYPELESS = 92,
    DX10_B8G8R8X8_UNORM_SRGB = 93,
    DX10_BC6H_TYPELESS = 94,
    DX10_BC6H_UF16 = 95,
    DX10_BC6H_SF16 = 96,
    DX10_BC7_TYPELESS = 97,
    DX10_BC7_UNORM = 98,
    DX10_BC7_UNORM_SRGB = 99,
    DX10_AYUV = 100,
    DX10_Y410 = 101,
    DX10_Y416 = 102,
    DX10_NV12 = 103,
    DX10_P010 = 104,
    DX10_P016 = 105,
    DX10_420_OPAQUE = 106,
    DX10_YUY2 = 107,
    DX10_Y210 = 108,
    DX10_Y216 = 109,
    DX10_NV11 = 110,
    DX10_AI44 = 111,
    DX10_IA44 = 112,
    DX10_P8 = 113,
    DX10_A8P8 = 114,
    DX10_B4G4R4A4_UNORM = 115,
    DX10_P208 = 130,
    DX10_V208 = 131,
    DX10_V408 = 132,
};

enum eDX10ResourceDimmension : uint32_t {
    UNKNOWN = 0,
    BUFFER = 1,
    TEXTURE1D = 2,
    TEXTURE2D = 3,
    TEXTURE3D = 4
};
struct sDX10Header {
    eDX10PixelFormat m_dxgiFormat{0};
    eDX10ResourceDimmension m_resourceDimension{0};
    uint32_t m_miscFlag{0};
    uint32_t m_arraySize{0};
    uint32_t m_miscFlags2{0};
};