#pragma once

#include <cstdint>
#include "pixelFormats.h"

struct sTexture;


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


