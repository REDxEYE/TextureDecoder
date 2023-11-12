#include <algorithm>
#include <cstring>
#include "texture.h"
#include "png/pngSupport.h"
#include "stb_image.h"

bool loadHDR(const std::filesystem::path& filename, sTexture *texture) {
    int c;
    float *data = stbi_loadf(filename.string().c_str(), (int *) &texture->m_width, (int *) &texture->m_height, &c, 3);
    if (data == nullptr) {
        texture->m_pixelFormat = ePixelFormat::INVALID;
        return false;
    }
    texture->m_pixelFormat = ePixelFormat::RGB32F;
    texture->m_rawPixelData.resize(calculateTextureSize(texture));
    memcpy(texture->m_rawPixelData.data(), data, texture->m_rawPixelData.size());
    stbi_image_free(data);
    return true;
}

bool loadHDR(uint8_t *data, size_t dataSize, sTexture *texture) {
    int c;
    float *iData = stbi_loadf_from_memory(data, dataSize, (int *) &texture->m_width,
                                                          (int *) &texture->m_height, &c,
                                                          3);
    if (iData == nullptr) {
        texture->m_pixelFormat = ePixelFormat::INVALID;
        return false;
    }
    texture->m_pixelFormat = ePixelFormat::RGB32F;
    texture->m_rawPixelData.resize(calculateTextureSize(texture));
    memcpy(texture->m_rawPixelData.data(), iData, texture->m_rawPixelData.size());
    free(data);
    return true;
}

