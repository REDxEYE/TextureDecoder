#pragma once

#include <unordered_map>
#include "textureDecoder.h"

#define MAKE_PIXEL_PAIR(e1, e2) ((uint32_t)e1|((uint32_t)e2)>>16)

typedef bool (*tConverterFn)(const sTexture *from_texture, sTexture *to_texture);

bool convertRGBA8888toRGBA8888(const sTexture *from_texture, sTexture *to_texture);
bool convertBC1toRGBA8888(const sTexture *from_texture, sTexture *to_texture);
bool convertBC1atoRGBA8888(const sTexture *from_texture, sTexture *to_texture);

static std::unordered_map<uint32_t, tConverterFn> cConverters{
        {MAKE_PIXEL_PAIR(ePixelFormat::RGBA8888, ePixelFormat::RGBA8888), convertRGBA8888toRGBA8888},
        {MAKE_PIXEL_PAIR(ePixelFormat::BC1, ePixelFormat::RGBA8888), convertRGBA8888toRGBA8888},
        {MAKE_PIXEL_PAIR(ePixelFormat::BC1a, ePixelFormat::RGBA8888), convertRGBA8888toRGBA8888},
};