#include <algorithm>
#include <cstring>
#include "texture.h"
#include "formats/png/pngSupport.h"
#include "stb_image.h"
#include "logging.h"

bool loadHDR(const std::filesystem::path &filename, sTexture *texture) {
    int c;
    float *data = stbi_loadf(filename.string().c_str(), (int *) &texture->m_width, (int *) &texture->m_height, &c, 3);
    if (data == nullptr) {
        texture->m_pixelFormat = ePixelFormat::INVALID;
        return false;
    }
    texture->m_pixelFormat = ePixelFormat::RGB32F;
    uint8_t* bytePtr = reinterpret_cast<uint8_t*>(data);
    texture->m_rawPixelData.assign(bytePtr, bytePtr + calculateTextureSize(texture));
    stbi_image_free(data);
    loggerEx(eLogLevel::INFO, std::format("Loaded HDR {}x{} {}\n", texture->m_width, texture->m_height,
                                          getPixelFormatName(texture->m_pixelFormat)));
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
    uint8_t* bytePtr = reinterpret_cast<uint8_t*>(iData);
    texture->m_rawPixelData.assign(bytePtr, bytePtr + calculateTextureSize(texture));
    loggerEx(eLogLevel::INFO, std::format("Loaded HDR {}x{} {}\n", texture->m_width, texture->m_height,
                                          getPixelFormatName(texture->m_pixelFormat)));
    return true;
}

