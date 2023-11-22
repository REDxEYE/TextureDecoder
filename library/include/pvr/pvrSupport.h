#pragma once
#include <filesystem>

#include "textureApi.h"
#include "pvr/pvr.h"

bool loadPVR(const std::filesystem::path &filename, sTexture *texture);

bool loadPVR(uint8_t *data, size_t dataSize, sTexture *texture);
