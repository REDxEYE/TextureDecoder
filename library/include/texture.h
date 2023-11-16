#pragma once

#include <vector>
#include "pixelFormat.h"

struct sTexture {
    std::vector<uint8_t> m_rawPixelData;
    uint32_t m_width{0};
    uint32_t m_height{0};
    ePixelFormat m_pixelFormat{0};
};