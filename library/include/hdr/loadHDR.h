#pragma once
#include <cstdint>
#include "textureDecoder.h"

bool loadPNG(uint8_t *data, size_t dataSize, sTexture *texture, uint8_t expectedChannels);

bool loadPNG(const char *filename, sTexture *texture, uint8_t expectedChannels);