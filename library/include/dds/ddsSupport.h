#pragma once

#include <cstdint>
#include "textureApi.h"
#include <filesystem>
#include "dds/ddsDefs.h"

bool loadDDS(const std::filesystem::path &filename, sTexture *texture);

bool loadDDS(uint8_t *data, size_t dataSize, sTexture *texture);
