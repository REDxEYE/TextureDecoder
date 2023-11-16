#pragma once
#include <cstdint>
#include <filesystem>
#include "textureApi.h"

bool loadHDR(uint8_t *data, size_t dataSize, sTexture *texture);

bool loadHDR(const std::filesystem::path&, sTexture *texture);