#pragma once
#include <cstdint>
#include <filesystem>
#include "textureApi.h"

bool loadTGA(uint8_t *data, size_t dataSize, sTexture *texture, uint8_t expectedChannels);

bool loadTGA(const std::filesystem::path&, sTexture *texture, uint8_t expectedChannels);

bool writeTGA(const std::filesystem::path& filename, const sTexture* texture);