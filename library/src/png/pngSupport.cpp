#include <filesystem>
#include <cstring>
#include "png/pngSupport.h"
#include "texture.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "logging.h"
#include "fileAbstraction.h"

namespace fs = std::filesystem;


bool loadPNG(const fs::path &filename, sTexture *texture, uint8_t expectedChannels) {
    int realChannelCount;
    texture->m_pixelFormat = ePixelFormat::INVALID;
    FILE *file = openFile(filename, "rb");
    if (file == nullptr)
        return false;
    uint8_t *data = stbi_load_from_file(file, (int *) &texture->m_width, (int *) &texture->m_height, &realChannelCount,
                                        0);
    if (data == nullptr) {
        return false;
    }
    fclose(file);
    if (expectedChannels == 0) {
        expectedChannels = realChannelCount;
    }
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
    texture->m_rawPixelData.assign(data, data + calculateTextureSize(texture));
    stbi_image_free(data);
    loggerEx(eLogLevel::INFO, std::format("Loaded PNG {}x{} {}\n", texture->m_width, texture->m_height,
                                          getPixelFormatName(texture->m_pixelFormat)));
    return true;
}

bool loadPNG(uint8_t *data, size_t dataSize, sTexture *texture, uint8_t expectedChannels) {
    int realChannelCount;
    uint8_t *iData = stbi_load_from_memory(data, dataSize, (int *) &texture->m_width, (int *) &texture->m_height,
                                           &realChannelCount,
                                           expectedChannels);
    if (iData == nullptr) {
        texture->m_pixelFormat = ePixelFormat::INVALID;
        return false;
    }
    if (expectedChannels == 0) {
        expectedChannels = realChannelCount;
    }
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
    texture->m_rawPixelData.assign(iData, iData + calculateTextureSize(texture));
    loggerEx(eLogLevel::INFO, std::format("Loaded PNG {}x{} {}\n", texture->m_width, texture->m_height,
                                          getPixelFormatName(texture->m_pixelFormat)));
    return true;
}

bool writePNG(const std::filesystem::path &filename, const sTexture *texture) {
    if (texture->m_pixelFormat != ePixelFormat::RGBA8888 &&
        texture->m_pixelFormat != ePixelFormat::RGB888 &&
        texture->m_pixelFormat != ePixelFormat::RG88 &&
        texture->m_pixelFormat != ePixelFormat::R8
            ) {
        loggerEx(eLogLevel::WARN, std::format(
                "Input format({}) for PNG isn't an RGBA8888/RGB888/RG88/R8. Will convert it to compatible format\n",
                getPixelFormatName(texture->m_pixelFormat)));
        ePixelFormat tmpPixelFormat = ePixelFormat::RGBA8888;
        loggerEx(eLogLevel::DEBUG, std::format("Converting {} texture with {} channels to {} with {} channels\n",
                                               getPixelFormatName(texture->m_pixelFormat),
                                               getChannelCount(texture->m_pixelFormat),
                                               getPixelFormatName(tmpPixelFormat),
                                               getChannelCount(tmpPixelFormat)
        ));
        switch (getChannelCount(texture->m_pixelFormat)) {
            case 4:
                tmpPixelFormat = ePixelFormat::RGBA8888;
                break;
            case 3:
                tmpPixelFormat = ePixelFormat::RGB888;
                break;
            case 2:
                tmpPixelFormat = ePixelFormat::RG88;
                break;
            case 1:
                tmpPixelFormat = ePixelFormat::R8;
                break;
        }
        sTexture *tmpTexture = createTexture(texture->m_width, texture->m_height, tmpPixelFormat);
        if (!convertTexture(texture, tmpTexture)) {
            loggerEx(eLogLevel::ERROR, "Failed to save PNG\n");
            return false;
        }
        bool res = writePNG(filename, tmpTexture);
        freeTexture(tmpTexture);
        return res;

    }


    FILE *file = openFile(filename, "wb");
    if (!file) {
        return false;
    }
    loggerEx(eLogLevel::DEBUG,
             std::format("Saving PNG with {} format {} channels\n",
                         getPixelFormatName(texture->m_pixelFormat),
                         getChannelCount(texture->m_pixelFormat)));
    int stride_in_bytes =
            texture->m_width * getChannelCount(texture->m_pixelFormat); // Assuming 4 bytes per pixel (RGBA)
    bool result = stbi_write_png_to_func(fileWriteFunc, file,
                                         texture->m_width, texture->m_height,
                                         getChannelCount(texture->m_pixelFormat), // Number of channels, assuming RGBA
                                         texture->m_rawPixelData.data(),
                                         stride_in_bytes);

    fclose(file);
    loggerEx(eLogLevel::INFO, std::format("Wrote PNG {}x{} {}\n", texture->m_width, texture->m_height,
                                          getPixelFormatName(texture->m_pixelFormat)));
    return result;
}