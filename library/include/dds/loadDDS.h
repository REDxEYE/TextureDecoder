#pragma once

#include <cstdint>
#include "textureDecoder.h"
#include "dds/dds.h"

bool loadDDS(uint8_t *data, size_t dataSize, sTexture *texture);

bool loadDDS(const char *filename, sTexture *texture);