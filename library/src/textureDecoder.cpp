#include <cstdarg>
#include <cstring>
#include <cstdio>
#include "textureDecoder.h"
#include "converters.h"

loggerFn logger = &defaultLogger;

sTexture *createTexture(const uint8_t *data, size_t dataSize,
                        uint32_t width, uint32_t height,
                        ePixelFormat pixelFormat) {

    if (calculateTextureSize(width,height,pixelFormat) < dataSize) {
        logger(1, "Textures width or height doesn't match.\n");
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
        logger(1, "Textures width or height doesn't match.\n");
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
            logger(3, formatMessage("Failed to find intermediate converters from %s to %s\n",
                                    getPixelName(fromTexture->m_pixelFormat),
                                    getPixelName(toTexture->m_pixelFormat)));
            return false;
        }
        uint32_t key0 = MAKE_PIXEL_PAIR(fromTexture->m_pixelFormat, matchedIntermediate);
        uint32_t key1 = MAKE_PIXEL_PAIR(matchedIntermediate, toTexture->m_pixelFormat);
        if (!cConverters.contains(key0)) {
            logger(3, formatMessage("Failed to find converter from %s to %s\n",
                                    getPixelName(fromTexture->m_pixelFormat),
                                    getPixelName(matchedIntermediate)));
            return false;
        }
        if (!cConverters.contains(key1)) {
            logger(3, formatMessage("Failed to find intermediate converter from %s to %s\n",
                                    getPixelName(matchedIntermediate),
                                    getPixelName(toTexture->m_pixelFormat)));
            return false;
        }
        sTexture *tmpTexture = createTexture(fromTexture->m_width, fromTexture->m_height, matchedIntermediate);
        if (!convertTexture(fromTexture, tmpTexture)) {
            logger(3, formatMessage("Failed to convert from %s to %s\n",
                                    getPixelName(fromTexture->m_pixelFormat),
                                    getPixelName(tmpTexture->m_pixelFormat)));
            freeTexture(tmpTexture);
            return false;
        }
        if (!convertTexture(tmpTexture, toTexture)) {
            logger(3, formatMessage("Failed to convert from %s to %s\n",
                                    getPixelName(tmpTexture->m_pixelFormat),
                                    getPixelName(toTexture->m_pixelFormat)));
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

const char *getPixelName(ePixelFormat pixelFormat) {
    switch (pixelFormat) {
        case INVALID:
            return "INVALID";
        case RGBA32:
            return "RGBA32";
        case RGB32:
            return "RGB32";
        case RG32:
            return "RG32";
        case R32:
            return "R32";
        case RGBA16:
            return "RGBA16";
        case RGB16:
            return "RGB16";
        case RG16:
            return "RG16";
        case R16:
            return "R16";
        case RGBA32F:
            return "RGBA32F";
        case RGB32F:
            return "RGB32F";
        case RG32F:
            return "RG32F";
        case R32F:
            return "R32F";
        case RGBA16F:
            return "RGBA16F";
        case RGB16F:
            return "RGB16F";
        case RG16F:
            return "RG16F";
        case R16F:
            return "R16F";
        case RGBA8888:
            return "RGBA8888";
        case RGB888:
            return "RGB888";
        case RG88:
            return "RG88";
        case RA88:
            return "RA88";
        case R8:
            return "R8";
        case RGB565:
            return "RGB565";
        case RGBA5551:
            return "RGBA5551";
        case BC1:
            return "BC1";
        case BC1a:
            return "BC1a";
        case BC2:
            return "BC2";
        case BC3:
            return "BC3";
        case BC4:
            return "BC4";
        case BC5:
            return "BC5";
        case BC6:
            return "BC6";
        case BC7:
            return "BC7";
    }
}

char *formatMessage(const char *fmt, ...) {
    static char buffer[8192];
    va_list arg_list;
            va_start(arg_list, fmt);
#ifdef WIN32
    vsprintf_s(buffer, fmt, arg_list);
#else
    vsprintf(buffer, fmt, arg_list);
#endif

    return buffer;
}

void defaultLogger(uint8_t level, const char *message) {
    const char *levelName;
    switch (level) {
        case 0:
            levelName = "DEBUG";
            break;
        default:
        case 1:
            levelName = "INFO";
            break;
        case 2:
            levelName = "WARN";
            break;
        case 3:
            levelName = "ERROR";
            break;
    }
    printf("[%-5s]: %s", levelName, message);
}


