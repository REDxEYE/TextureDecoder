#include <algorithm>
#include <cstring>
#include "png/loadPNG.h"
#include "stb_image.h"

bool loadPNG(const char *filename, sTexture *texture, uint8_t expectedChannels) {
    int c;
    uint8_t *data = stbi_load(filename, (int *) &texture->m_width, (int *) &texture->m_height, &c, expectedChannels);
    if (data == nullptr) {
        texture->m_pixelFormat = ePixelFormat::INVALID;
        return false;
    }
    expectedChannels = std::max((uint8_t) c, expectedChannels);
    switch (expectedChannels) {
        case 1:
            texture->m_pixelFormat = ePixelFormat::R8;
            break;
        case 2:
            texture->m_pixelFormat = ePixelFormat::RA88;
            break;
        case 3:
            texture->m_pixelFormat = ePixelFormat::RGB888;
            break;
        case 4:
        default:
            texture->m_pixelFormat = ePixelFormat::RGBA8888;
            break;
    }
    texture->m_rawPixelData.resize(calculateTextureSize(texture));
    memcpy(texture->m_rawPixelData.data(), data, texture->m_rawPixelData.size());
    free(data);
    return true;
}

bool loadPNG(uint8_t *data, size_t dataSize, sTexture *texture, uint8_t expectedChannels) {
    int c;
    uint8_t *iData = stbi_load_from_memory(data, dataSize, (int *) &texture->m_width, (int *) &texture->m_height, &c,
                                           expectedChannels);
    if (iData == nullptr) {
        texture->m_pixelFormat = ePixelFormat::INVALID;
        return false;
    }
    expectedChannels = std::max((uint8_t) c, expectedChannels);
    switch (expectedChannels) {
        case 1:
            texture->m_pixelFormat = ePixelFormat::R8;
            break;
        case 2:
            texture->m_pixelFormat = ePixelFormat::RA88;
            break;
        case 3:
            texture->m_pixelFormat = ePixelFormat::RGB888;
            break;
        case 4:
        default:
            texture->m_pixelFormat = ePixelFormat::RGBA8888;
            break;
    }
    texture->m_rawPixelData.resize(calculateTextureSize(texture));
    memcpy(texture->m_rawPixelData.data(), iData, texture->m_rawPixelData.size());
    free(data);
    return true;
}

