#pragma once

#include <cstdint>

enum class VTFImageFormat : int32_t {
    RGBA8888 = 0,                //!<  = Red, Green, Blue, Alpha - 32 bpp
    ABGR8888,                    //!<  = Alpha, Blue, Green, Red - 32 bpp
    RGB888,                    //!<  = Red, Green, Blue - 24 bpp
    BGR888,                    //!<  = Blue, Green, Red - 24 bpp
    RGB565,                    //!<  = Red, Green, Blue - 16 bpp
    I8,                        //!<  = Luminance - 8 bpp
    IA88,                        //!<  = Luminance, Alpha - 16 bpp
    P8,                        //!<  = Paletted - 8 bpp
    A8,                        //!<  = Alpha- 8 bpp
    RGB888_BLUESCREEN,            //!<  = Red, Green, Blue, "BlueScreen" Alpha - 24 bpp
    BGR888_BLUESCREEN,            //!<  = Red, Green, Blue, "BlueScreen" Alpha - 24 bpp
    ARGB8888,                    //!<  = Alpha, Red, Green, Blue - 32 bpp
    BGRA8888,                    //!<  = Blue, Green, Red, Alpha - 32 bpp
    DXT1,                        //!<  = DXT1 compressed format - 4 bpp
    DXT3,                        //!<  = DXT3 compressed format - 8 bpp
    DXT5,                        //!<  = DXT5 compressed format - 8 bpp
    BGRX8888,                    //!<  = Blue, Green, Red, Unused - 32 bpp
    BGR565,                    //!<  = Blue, Green, Red - 16 bpp
    BGRX5551,                    //!<  = Blue, Green, Red, Unused - 16 bpp
    BGRA4444,                    //!<  = Red, Green, Blue, Alpha - 16 bpp
    DXT1_ONEBITALPHA,            //!<  = DXT1 compressed format with 1-bit alpha - 4 bpp
    BGRA5551,                    //!<  = Blue, Green, Red, Alpha - 16 bpp
    UV88,                        //!<  = 2 channel format for DuDv/Normal maps - 16 bpp
    UVWQ8888,                    //!<  = 4 channel format for DuDv/Normal maps - 32 bpp
    RGBA16161616F,                //!<  = Red, Green, Blue, Alpha - 64 bpp
    RGBA16161616,                //!<  = Red, Green, Blue, Alpha signed with mantissa - 64 bpp
    UVLX8888,                    //!<  = 4 channel format for DuDv/Normal maps - 32 bpp
    R32F,                        //!<  = Luminance - 32 bpp
    RGB323232F,                //!<  = Red, Green, Blue - 96 bpp
    RGBA32323232F,                //!<  = Red, Green, Blue, Alpha - 128 bpp
    NV_DST16,
    NV_DST24,
    NV_INTZ,
    NV_RAWZ,
    ATI_DST16,
    ATI_DST24,
    NV_NULL,
    ATI2N,
    ATI1N,
    /* XBox:
    X360_DST16,
    X360_DST24,
    X360_DST24F,
    LINEAR_BGRX8888,			//!<  = Blue, Green, Red, Unused - 32 bpp
    LINEAR_RGBA8888,			//!<  = Red, Green, Blue, Alpha - 32 bpp
    LINEAR_ABGR8888,			//!<  = Alpha, Blue, Green, Red - 32 bpp
    LINEAR_ARGB8888,			//!<  = Alpha, Red, Green, Blue - 32 bpp
    LINEAR_BGRA8888,			//!<  = Blue, Green, Red, Alpha - 32 bpp
    LINEAR_RGB888,				//!<  = Red, Green, Blue - 24 bpp
    LINEAR_BGR888,				//!<  = Blue, Green, Red - 24 bpp
    LINEAR_BGRX5551,			//!<  = Blue, Green, Red, Unused - 16 bpp
    LINEAR_I8,					//!<  = Luminance - 8 bpp
    LINEAR_RGBA16161616,		//!<  = Red, Green, Blue, Alpha signed with mantissa - 64 bpp
    LE_BGRX8888,				//!<  = Blue, Green, Red, Unused - 32 bpp
    LE_BGRA8888,				//!<  = Blue, Green, Red, Alpha - 32 bpp
    */
    COUNT,
    NONE = -1
};