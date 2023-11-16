#include <algorithm>
#include "textureApi.h"
#include "texture.h"
#include "converters.h"
#include "logging.h"
#include "pixelFormat.h"

sTexture *createTexture() {
    sTexture *texture = new sTexture();
    texture->m_rawPixelData.resize(0);
    texture->m_width = 0;
    texture->m_height = 0;
    texture->m_pixelFormat = ePixelFormat::INVALID;

    return texture;
}


sTexture *createTexture(uint32_t width, uint32_t height, ePixelFormat pixelFormat) {
    sTexture *texture = new sTexture();
    initTexture(texture, width, height, pixelFormat);
    return texture;
}


void initTexture(sTexture *texture, uint32_t width, uint32_t height, ePixelFormat pixelFormat) {
    texture->m_rawPixelData.resize(calculateTextureSize(width, height, pixelFormat));
    texture->m_width = width;
    texture->m_height = height;
    texture->m_pixelFormat = pixelFormat;
}

sTexture *createTexture(const uint8_t *data, size_t dataSize,
                        uint32_t width, uint32_t height,
                        ePixelFormat pixelFormat) {

    if (calculateTextureSize(width, height, pixelFormat) > dataSize) {
        loggerEx(eLogLevel::ERROR,
                 std::format("Textures size doesn't match to calculated size for {}x{} {}. Expected {}, but got {}\n",
                             width, height, getPixelFormatName(pixelFormat),
                             calculateTextureSize(width, height, pixelFormat), dataSize));
        return nullptr;
    }
    sTexture *texture = new sTexture();
    texture->m_rawPixelData.assign(data, data + dataSize);
    texture->m_width = width;
    texture->m_height = height;
    texture->m_pixelFormat = pixelFormat;

    return texture;
}


#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

/**
 * Converts a texture from one pixel format to another.
 *
 * @param fromTexture The source texture to be converted.
 * @param toTexture The destination texture where the converted data will be stored.
 * @return A boolean value indicating the success or failure of the conversion.
 */
bool convertTexture(const sTexture *fromTexture, sTexture *toTexture) {
    // Ensure the dimensions of both textures match
    if (fromTexture->m_width != toTexture->m_width || fromTexture->m_height != toTexture->m_height) {
        loggerEx(eLogLevel::ERROR, "Textures width or height doesn't match.\n");
        return false;
    }

    // Direct copy if pixel formats are the same
    if (fromTexture->m_pixelFormat == toTexture->m_pixelFormat) {
        toTexture->m_rawPixelData.assign(fromTexture->m_rawPixelData.begin(), fromTexture->m_rawPixelData.end());
    }

    // Key for the converter map based on source and target formats
    uint32_t key = MAKE_PIXEL_PAIR(fromTexture->m_pixelFormat, toTexture->m_pixelFormat);
    if (cConverters.contains(key)) {
        loggerEx(eLogLevel::DEBUG,
                 std::format("Converting from {} to {}\n", getPixelFormatName(fromTexture->m_pixelFormat),
                             getPixelFormatName(toTexture->m_pixelFormat)));
        return cConverters.at(key)(fromTexture, toTexture);
    } else {
        // Find possible intermediate formats for conversion
        std::vector<uint32_t> fromConverters, toConverters;
        for (const auto &item: cConverters) {
            if ((ePixelFormat(item.first & 0xFFFF)) == fromTexture->m_pixelFormat) {
                fromConverters.push_back(item.first);
            }
            if ((ePixelFormat(item.first >> 16)) == toTexture->m_pixelFormat) {
                toConverters.push_back(item.first);
            }
        }
        // Identify a suitable intermediate format
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
        // Handle the case when no intermediate converter is found
        if (matchedIntermediate == ePixelFormat::INVALID) {
            loggerEx(eLogLevel::ERROR, std::format("Failed to find intermediate converters from {} to {}\n",
                                                   getPixelFormatName(fromTexture->m_pixelFormat),
                                                   getPixelFormatName(toTexture->m_pixelFormat)));
            return false;
        }
        // Perform conversion via intermediate format
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
        // Create a temporary texture for intermediate conversion
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

#pragma clang diagnostic pop


void freeTexture(sTexture *texture) {
    // Safely delete the texture and set the pointer to nullptr to avoid dangling pointers
    delete texture;
    texture = nullptr;
}


int64_t calculateTextureSize(const sTexture *texture) {
    // Delegate to the primary calculateTextureSize function
    return calculateTextureSize(texture->m_width, texture->m_height, texture->m_pixelFormat);
}


int64_t calculateTextureSize(uint32_t width, uint32_t height, ePixelFormat pixelFormat) {
    // Calculate the number of 4x4 blocks for compressed formats
    int64_t numBlocksWide = (width + 3) / 4; // Round up to cover all pixels
    int64_t numBlocksHigh = (height + 3) / 4;

    switch (pixelFormat) {
        case INVALID:
            return -1;
            // Uncompressed formats
        case RGBA8888:
        case BGRA8888:
        case RGBA1010102:
        case R32F:
        case R32:
        case RG16:
        case RG16F:
            return static_cast<int64_t>(width) * height * 4;
        case RGB888:
            return static_cast<int64_t>(width) * height * 3;
        case RG88:
        case RA88:
        case RGB565:
        case RGBA5551:
        case R16F:
        case R16:
            return static_cast<int64_t>(width) * height * 2;
        case R8:
            return static_cast<int64_t>(width) * height;
        case RGBA32:
        case RGBA32F:
            return static_cast<int64_t>(width) * height * 16;
        case RGB32:
        case RGB32F:
            return static_cast<int64_t>(width) * height * 12;
        case RG32F:
        case RG32:
        case RGBA16:
        case RGBA16F:
            return static_cast<int64_t>(width) * height * 8;
        case RGB16:
        case RGB16F:
            return static_cast<int64_t>(width) * height * 6;

            // Compressed formats
        case BC1:
        case BC1a:
        case BC4:
            return numBlocksWide * numBlocksHigh * 8; // 8 bytes per block
        case BC2:
        case BC3:
        case BC5:
        case BC6:
        case BC7:
            return numBlocksWide * numBlocksHigh * 16; // 16 bytes per block
        default:
            // For any unexpected format
            return -1;
    }
}


#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"


bool flipTexture(const sTexture *inTexture, sTexture *outTexture, bool flipUD, bool flipLR) {
    // Validate input and output textures
    if (!inTexture || !outTexture) {
        loggerEx(eLogLevel::ERROR, "Input or output texture is nullptr!\n");
        return false;
    }

    // Handle compressed pixel formats by converting to an uncompressed variant
    if (isCompressedPixelFormat(inTexture->m_pixelFormat)) {
        loggerEx(eLogLevel::INFO,
                 std::format("Cannot flip compressed format({}) directly, converting to compatible format({}).\n",
                             getPixelFormatName(inTexture->m_pixelFormat),
                             getPixelFormatName(getUncompressedPixelFormatVariant(inTexture->m_pixelFormat))));
        sTexture *tmpTexture = createTexture(inTexture->m_width, inTexture->m_height,
                                             getUncompressedPixelFormatVariant(inTexture->m_pixelFormat));
        if (!convertTexture(inTexture, tmpTexture)) {
            freeTexture(tmpTexture);
            return false;
        }
        bool res = flipTexture(tmpTexture, outTexture, flipUD, flipLR);
        freeTexture(tmpTexture);
        return res;
    }

    initTexture(outTexture,inTexture->m_width,inTexture->m_height,inTexture->m_pixelFormat);

    uint32_t width = inTexture->m_width;
    uint32_t height = inTexture->m_height;
    uint32_t bytesPerPixel = getPixelFormatPixelSize(inTexture->m_pixelFormat);
    const auto &inPixels = inTexture->m_rawPixelData;
    auto &outPixels = outTexture->m_rawPixelData;

    // Perform vertical flip if required
    for (uint32_t y = 0; y < height; ++y) {
        uint32_t targetY = flipUD ? (height - 1 - y) : y;
        std::copy_n(inPixels.begin() + y * width * bytesPerPixel,
                    width * bytesPerPixel,
                    outPixels.begin() + targetY * width * bytesPerPixel);
    }

    // Perform horizontal flip if required
    if (flipLR) {
        for (uint32_t y = 0; y < height; ++y) {
            for (uint32_t x = 0; x < width / 2; ++x) {
                uint32_t flippedX = width - 1 - x;
                for (int byte = 0; byte < bytesPerPixel; ++byte) {
                    std::swap(outPixels[(y * width + x) * bytesPerPixel + byte],
                              outPixels[(y * width + flippedX) * bytesPerPixel + byte]);
                }
            }
        }
    }

    return true;
}

#pragma clang diagnostic pop


