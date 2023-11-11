#pragma once
#include <cstdint>
#include "textureDecoder.h"

bool loadHDR(uint8_t *data, size_t dataSize, sTexture *texture);

bool loadHDR(const char *filename, sTexture *texture);