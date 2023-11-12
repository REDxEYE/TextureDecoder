#include <cstring>
#include "textureDecoder.h"
#include "texture.h"
#include "converters.h"
#include "logging.h"

sTexture *createTexture(const uint8_t *data, size_t dataSize,
                        uint32_t width, uint32_t height,
                        ePixelFormat pixelFormat) {

    if (calculateTextureSize(width,height,pixelFormat) < dataSize) {
        loggerEx(eLogLevel::ERROR, "Textures width or height doesn't match.\n");
        return nullptr;
    }
    std::vector<uint8_t> vData(data, data + dataSize);
    sTexture *texture = new sTexture;
    texture->m_rawPixelData = std::move(vData);
    texture->m_width = width;
    texture->m_height = height;
    texture->m_pixelFormat = pixelFormat;

    return texture;
}

sTexture *createTexture(uint32_t width, uint32_t height, ePixelFormat pixelFormat) {
    std::vector<uint8_t> vData(calculateTextureSize(width, height, pixelFormat));
    sTexture *texture = new sTexture;
    texture->m_rawPixelData = std::move(vData);
    texture->m_width = width;
    texture->m_height = height;
    texture->m_pixelFormat = pixelFormat;

    return texture;
}

sTexture *createTexture() {
    std::vector<uint8_t> vData(0);
    sTexture *texture = new sTexture;
    texture->m_rawPixelData = std::move(vData);
    texture->m_width = 0;
    texture->m_height = 0;
    texture->m_pixelFormat = ePixelFormat::INVALID;

    return texture;
}

bool convertTexture(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_width != toTexture->m_width &&
        fromTexture->m_height != toTexture->m_height
            ) {
        loggerEx(eLogLevel::ERROR, "Textures width or height doesn't match.\n");
        return false;
    }

    if (fromTexture->m_pixelFormat == toTexture->m_pixelFormat) {
        memcpy(toTexture->m_rawPixelData.data(), fromTexture->m_rawPixelData.data(),
               fromTexture->m_rawPixelData.size());
    }

    uint32_t key = MAKE_PIXEL_PAIR(fromTexture->m_pixelFormat, toTexture->m_pixelFormat);
    if (cConverters.contains(key)) {
        return cConverters.at(key)(fromTexture, toTexture);
    } else {
        std::vector<uint32_t> fromConverters;
        std::vector<uint32_t> toConverters;
        for (const auto &item: cConverters) {
            if ((ePixelFormat(item.first & 0xFFFF)) == fromTexture->m_pixelFormat) {
                fromConverters.push_back(item.first);
            }
            if ((ePixelFormat(item.first >> 16)) == toTexture->m_pixelFormat) {
                toConverters.push_back(item.first);
            }
        }
        ePixelFormat matchedIntermediate = ePixelFormat::INVALID;
        for (auto fromKey: fromConverters) {
            ePixelFormat intermediate = ePixelFormat(fromKey >> 16);
            for (auto toKey: toConverters) {
                if (ePixelFormat(toKey & 0xFFFF) == intermediate) {
                    matchedIntermediate = intermediate;
                    break;
                }
            }
        }
        if (matchedIntermediate == ePixelFormat::INVALID) {
            loggerEx(eLogLevel::ERROR, std::format("Failed to find intermediate converters from {} to {}\n",
                                                   getPixelFormatName(fromTexture->m_pixelFormat),
                                                   getPixelFormatName(toTexture->m_pixelFormat)));
            return false;
        }
        uint32_t key0 = MAKE_PIXEL_PAIR(fromTexture->m_pixelFormat, matchedIntermediate);
        uint32_t key1 = MAKE_PIXEL_PAIR(matchedIntermediate, toTexture->m_pixelFormat);
        if (!cConverters.contains(key0)) {
            loggerEx(eLogLevel::ERROR, std::format("Failed to find converter from {} to {}\n",
                                                   getPixelFormatName(fromTexture->m_pixelFormat),
                                                   getPixelFormatName(matchedIntermediate)));
            return false;
        }
        if (!cConverters.contains(key1)) {
            loggerEx(eLogLevel::ERROR, std::format("Failed to find intermediate converter from {} to {}\n",
                                                   getPixelFormatName(matchedIntermediate),
                                                   getPixelFormatName(toTexture->m_pixelFormat)));
            return false;
        }
        sTexture *tmpTexture = createTexture(fromTexture->m_width, fromTexture->m_height, matchedIntermediate);
        if (!convertTexture(fromTexture, tmpTexture)) {
            loggerEx(eLogLevel::ERROR, std::format("Failed to convert from {} to {}\n",
                                                   getPixelFormatName(fromTexture->m_pixelFormat),
                                                   getPixelFormatName(tmpTexture->m_pixelFormat)));
            freeTexture(tmpTexture);
            return false;
        }
        if (!convertTexture(tmpTexture, toTexture)) {
            loggerEx(eLogLevel::ERROR, std::format("Failed to convert from {} to {}\n",
                                                   getPixelFormatName(tmpTexture->m_pixelFormat),
                                                   getPixelFormatName(toTexture->m_pixelFormat)));
            freeTexture(tmpTexture);
            return false;
        }
        freeTexture(tmpTexture);
        return true;
    }
}

void freeTexture(sTexture *texture) {
    delete texture;
}

int64_t calculateTextureSize(const sTexture *texture) {
    return calculateTextureSize(texture->m_width, texture->m_height, texture->m_pixelFormat);
}

int64_t calculateTextureSize(uint32_t width, uint32_t height, ePixelFormat pixelFormat) {
    switch (pixelFormat) {
        case INVALID:
            return -1;
        case RGBA8888:
        case R32F:
        case R32:
        case RG16:
        case RG16F:
            return width * height * 4;
        case RGB888:
            return width * height * 3;
        case RG88:
        case RA88:
        case RGB565:
        case RGBA5551:
        case R16F:
        case R16:
            return width * height * 2;
        case BC1:
        case BC1a:
        case BC4:
            return width * height / 2;
        case R8:
        case BC2:
        case BC3:
        case BC5:
        case BC6:
        case BC7:
            return width * height;
        case RGBA32:
        case RGBA32F:
            return width * height * 16;
        case RGB32:
        case RGB32F:
            return width * height * 12;
        case RG32F:
        case RG32:
        case RGBA16:
        case RGBA16F:
            return width * height * 8;
        case RGB16:
        case RGB16F:
            return width * height * 6;
    }
}

