#pragma once

#include <unordered_map>
#include "textureApi.h"

#define MAKE_PIXEL_PAIR(e1, e2) ((uint32_t)(e1)|((uint32_t)(e2))<<16)

typedef bool (*tConverterFn)(const sTexture *fromTexture, sTexture *toTexture);

bool convertRGBA8888toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);

bool convertR8toRG88(const sTexture *fromTexture, sTexture *toTexture);

bool convertR8toRGB888(const sTexture *fromTexture, sTexture *toTexture);

bool convertR8toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);

bool convertRG88toRGB888(const sTexture *fromTexture, sTexture *toTexture);

bool convertRG88toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);

bool convertRG88toR8(const sTexture *fromTexture, sTexture *toTexture);

bool convertRGB888toR8(const sTexture *fromTexture, sTexture *toTexture);

bool convertRGBA8888toR8(const sTexture *fromTexture, sTexture *toTexture);

bool convertRGB888toRG88(const sTexture *fromTexture, sTexture *toTexture);

bool convertRGBA8888toRG88(const sTexture *fromTexture, sTexture *toTexture);

bool convertRGBA8888toRGB888(const sTexture *fromTexture, sTexture *toTexture);

bool convertRGB888toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);

bool convertRGB32FtoRGB16F(const sTexture *fromTexture, sTexture *toTexture);

bool convertRGB16FtoRGB32F(const sTexture *fromTexture, sTexture *toTexture);

bool convertBC1toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);

bool convertBC1atoRGBA8888(const sTexture *fromTexture, sTexture *toTexture);

bool convertBC2toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);

bool convertBC3toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);

bool convertBC4toR8(const sTexture *fromTexture, sTexture *toTexture);

bool convertBC5toRG88(const sTexture *fromTexture, sTexture *toTexture);

bool convertBC6toRGB16F(const sTexture *fromTexture, sTexture *toTexture);

bool convertBC6toRGB32F(const sTexture *fromTexture, sTexture *toTexture);

bool convertBC7toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);

bool convertRGBA8888toBGRA8888(const sTexture *fromTexture, sTexture *toTexture);

bool convertBGRA8888toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);

bool convertRG16toRG88(const sTexture *fromTexture, sTexture *toTexture);

bool convertR16toR8(const sTexture *fromTexture, sTexture *toTexture);

bool convertRG16_SIGNEDtoRG16(const sTexture *fromTexture, sTexture *toTexture);

bool convertRGBA1010102toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);

bool convertRGBA1010102toRGBA16(const sTexture *fromTexture, sTexture *toTexture);

bool convertRA88toRG88(const sTexture *fromTexture, sTexture *toTexture);

bool convertRG88toRA88(const sTexture *fromTexture, sTexture *toTexture);

bool convertRA88toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);

bool convertRG16toRGBA16(const sTexture *fromTexture, sTexture *toTexture);
bool convertRGBA4444toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);
bool convertRGBA1111toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);
bool convertETC1toRGB888(const sTexture *fromTexture, sTexture *toTexture);
bool convertABGR8888toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);
bool convertARGB8888toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);
bool convertRGB565toRGB888(const sTexture *fromTexture, sTexture *toTexture);
bool convertRGBA5551toRGBA8888(const sTexture *fromTexture, sTexture *toTexture);
bool convertRGB888toBGR888(const sTexture *fromTexture, sTexture *toTexture);
bool convertBGR888toRGB888(const sTexture *fromTexture, sTexture *toTexture);
bool convertRGBA8888toARGB8888(const sTexture *fromTexture, sTexture *toTexture);
bool convertRGB32FtoRGBA32F(const sTexture *fromTexture, sTexture *toTexture);
bool convertRGB16FtoRGBA16F(const sTexture *fromTexture, sTexture *toTexture);

#define DEFINE_CONVERTER(fromFmt, toFmt) {MAKE_PIXEL_PAIR(fromFmt, toFmt), convert##fromFmt##to##toFmt}

#define DEFINE_TWO_WAY_CONVERTER(fromFmt, toFmt) DEFINE_CONVERTER(fromFmt, toFmt),DEFINE_CONVERTER(toFmt, fromFmt)

static std::unordered_map<uint32_t, tConverterFn> cConverters{
        DEFINE_CONVERTER(ETC1, RGB888),
        DEFINE_CONVERTER(RGBA1111, RGBA8888),
        DEFINE_CONVERTER(RGBA4444, RGBA8888),
        DEFINE_CONVERTER(RGBA8888, RGBA8888),
        DEFINE_CONVERTER(ABGR8888, RGBA8888),
        DEFINE_CONVERTER(RGB565, RGB888),
        DEFINE_CONVERTER(RGBA5551, RGBA8888),
        DEFINE_TWO_WAY_CONVERTER(RGB888, BGR888),
        DEFINE_TWO_WAY_CONVERTER(RGBA8888, BGRA8888),
        DEFINE_TWO_WAY_CONVERTER(RGBA8888, ARGB8888),
        DEFINE_CONVERTER(RG16, RG88),
        DEFINE_CONVERTER(RG16, RGBA16),
        DEFINE_CONVERTER(R16, R8),
        DEFINE_CONVERTER(RG16_SIGNED, RG16), // Remap signed to unsigned
        DEFINE_CONVERTER(RGBA1010102, RGBA8888),
        DEFINE_CONVERTER(RGBA1010102, RGBA16),
        DEFINE_TWO_WAY_CONVERTER(RGBA8888, BGRA8888),
        DEFINE_TWO_WAY_CONVERTER(RGB888, RGBA8888),
        DEFINE_TWO_WAY_CONVERTER(RGB16F, RGB32F),
        DEFINE_TWO_WAY_CONVERTER(R8, RG88),
        DEFINE_TWO_WAY_CONVERTER(R8, RGB888),
        DEFINE_TWO_WAY_CONVERTER(R8, RGBA8888),
        DEFINE_TWO_WAY_CONVERTER(RG88, RGB888),
        DEFINE_TWO_WAY_CONVERTER(RG88, RGBA8888),
        DEFINE_TWO_WAY_CONVERTER(RA88, RG88),
        DEFINE_CONVERTER(RA88, RGBA8888),
        DEFINE_CONVERTER(BC1, RGBA8888),
        DEFINE_CONVERTER(BC1a, RGBA8888),
        DEFINE_CONVERTER(BC2, RGBA8888),
        DEFINE_CONVERTER(BC3, RGBA8888),
        DEFINE_CONVERTER(BC4, R8),
        DEFINE_CONVERTER(BC5, RG88),
        DEFINE_CONVERTER(BC6, RGB16F),
        DEFINE_CONVERTER(BC6, RGB32F),
        DEFINE_CONVERTER(BC7, RGBA8888),
        DEFINE_CONVERTER(ETC1, RGB888),
        DEFINE_CONVERTER(RGB32F, RGBA32F),
        DEFINE_CONVERTER(RGB16F, RGBA16F),
};
