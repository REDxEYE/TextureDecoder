#pragma once

#include <cstdint>
#include <filesystem>
#include "texture.h"

bool loadVTF(const std::filesystem::path &filename, sTexture *texture);

bool loadVTF(uint8_t *data, size_t dataSize, sTexture *texture);
