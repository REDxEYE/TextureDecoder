#pragma once
#include <cstdint>
#include <filesystem>
#include "textureDecoder.h"

bool loadPNG(uint8_t *data, size_t dataSize, sTexture *texture, uint8_t expectedChannels);

bool loadPNG(const std::filesystem::path& filename, sTexture *texture, uint8_t expectedChannels);

bool writePNG(const std::filesystem::path& filename, const sTexture* texture);