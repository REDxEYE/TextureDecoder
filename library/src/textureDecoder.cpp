#include "textureDecoder.h"
#include "converters.h"

sTexture *createTexture(const uint8_t *data, size_t dataSize,
                        uint32_t width, uint32_t height,
                        ePixelFormat pixelFormat, bool sRGB, bool normalized) {
    std::vector<uint8_t> vData(data, data + dataSize);
    sTexture *texture = new sTexture;
    texture->m_rawPixelData = std::move(vData);
    texture->m_width = width;
    texture->m_height = height;
    texture->m_pixelFormat = pixelFormat;
    texture->m_sRGB = sRGB;
    texture->m_normalized = normalized;

    return texture;
}

sTexture *createTexture(uint32_t width, uint32_t height, ePixelFormat pixelFormat, bool sRGB, bool normalized) {
    std::vector<uint8_t> vData;
    sTexture *texture = new sTexture;
    texture->m_rawPixelData = std::move(vData);
    texture->m_width = width;
    texture->m_height = height;
    texture->m_pixelFormat = pixelFormat;
    texture->m_sRGB = sRGB;
    texture->m_normalized = normalized;

    return texture;
}

bool convertTexture(const sTexture *from_texture, sTexture *to_texture) {
    uint32_t key = MAKE_PIXEL_PAIR(from_texture->m_pixelFormat, to_texture->m_pixelFormat);
    if (cConverters.contains(key)) {

    } else {

        uint32_t key0 = MAKE_PIXEL_PAIR(from_texture->m_pixelFormat, ePixelFormat::RGBA8888);
        uint32_t key1 = MAKE_PIXEL_PAIR(ePixelFormat::RGBA8888, to_texture->m_pixelFormat);
        if (!cConverters.contains(key0) || !cConverters.contains(key1)) {
            return false;
        }
        sTexture *tmpTexture = createTexture(from_texture->m_width, from_texture->m_height, ePixelFormat::RGBA8888,
                                             from_texture->m_sRGB, from_texture->m_normalized);
        if (!convertTexture(from_texture, tmpTexture)) {
            freeTexture(tmpTexture);
            return false;
        }
        if (!convertTexture(tmpTexture, to_texture)) {
            freeTexture(tmpTexture);
            return false;
        }
        freeTexture(tmpTexture);
    }
    return false;
}

void freeTexture(sTexture *texture) {
    delete texture;
}

int64_t calculateTextureSize(uint32_t width, uint32_t height, ePixelFormat pixelFormat) {
    switch (pixelFormat) {
        case INVALID:
            return -1;
        case RGBA8888:
            return width * height * 4;
        case RGB888:
            return width * height * 3;
        case RG88:
        case RGB565:
        case RGBA5551:
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
    }
}


