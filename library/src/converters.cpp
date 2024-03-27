#include "converters.h"
#include "bcdec.h"
#include "half.h"
#include "texture.h"
#include "decoders/etc/etc1.h"


bool convertRGBA8888toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.assign(fromTexture->m_rawPixelData.begin(), fromTexture->m_rawPixelData.end());
    return true;
}

bool convertRGB888toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 4 + 0] = fromTexture->m_rawPixelData[i * 3 + 0];
        toTexture->m_rawPixelData[i * 4 + 1] = fromTexture->m_rawPixelData[i * 3 + 1];
        toTexture->m_rawPixelData[i * 4 + 2] = fromTexture->m_rawPixelData[i * 3 + 2];
        toTexture->m_rawPixelData[i * 4 + 3] = 255;
    }
    return true;
}

bool convertRGBA8888toRGB888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 3 + 0] = fromTexture->m_rawPixelData[i * 4 + 0];
        toTexture->m_rawPixelData[i * 3 + 1] = fromTexture->m_rawPixelData[i * 4 + 1];
        toTexture->m_rawPixelData[i * 3 + 2] = fromTexture->m_rawPixelData[i * 4 + 2];
    }
    return true;
}

bool convertRGB32FtoRGB16F(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        *(FLOAT16 *) &toTexture->m_rawPixelData[i * 6 + 0] = *(float *) &fromTexture->m_rawPixelData[i * 12 + 0];
        *(FLOAT16 *) &toTexture->m_rawPixelData[i * 6 + 2] = *(float *) &fromTexture->m_rawPixelData[i * 12 + 4];
        *(FLOAT16 *) &toTexture->m_rawPixelData[i * 6 + 4] = *(float *) &fromTexture->m_rawPixelData[i * 12 + 8];
    }
    return true;
}

bool convertRGB16FtoRGB32F(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        *(float *) &toTexture->m_rawPixelData[i * 12 + 0] = *(FLOAT16 *) &fromTexture->m_rawPixelData[i * 6 + 0];
        *(float *) &toTexture->m_rawPixelData[i * 12 + 4] = *(FLOAT16 *) &fromTexture->m_rawPixelData[i * 6 + 2];
        *(float *) &toTexture->m_rawPixelData[i * 12 + 8] = *(FLOAT16 *) &fromTexture->m_rawPixelData[i * 6 + 4];
    }
    return true;
}


bool convertBC1toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC1_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    int i, j;
    const uint8_t *src = fromTexture->m_rawPixelData.data();
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    uint8_t *dst = toTexture->m_rawPixelData.data();
    uint8_t *output = dst;
    for (i = 0; i < fromTexture->m_height; i += 4) {
        for (j = 0; j < fromTexture->m_width; j += 4) {
            dst = output + (i * fromTexture->m_width + j) * 4;
            bcdec_bc1(src, dst, (int) fromTexture->m_width * 4);
            src += BCDEC_BC1_BLOCK_SIZE;
        }
    }
    return true;
}

bool convertBC1atoRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC1_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    int i, j;
    const uint8_t *src = fromTexture->m_rawPixelData.data();
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    uint8_t *dst = toTexture->m_rawPixelData.data();
    uint8_t *output = dst;
    for (i = 0; i < fromTexture->m_height; i += 4) {
        for (j = 0; j < fromTexture->m_width; j += 4) {
            dst = output + (i * fromTexture->m_width + j) * 4;
            bcdec_bc1(src, dst, (int) fromTexture->m_width * 4);
            src += BCDEC_BC1_BLOCK_SIZE;
        }
    }
    return true;
}


bool convertBC2toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC2_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    int i, j;
    const uint8_t *src = fromTexture->m_rawPixelData.data();
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    uint8_t *dst = toTexture->m_rawPixelData.data();
    uint8_t *output = dst;
    for (i = 0; i < fromTexture->m_height; i += 4) {
        for (j = 0; j < fromTexture->m_width; j += 4) {
            dst = output + (i * fromTexture->m_width + j) * 4;
            bcdec_bc2(src, dst, (int) fromTexture->m_width * 4);
            src += BCDEC_BC2_BLOCK_SIZE;
        }
    }
    return true;
}

bool convertBC3toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC3_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    int i, j;
    const uint8_t *src = fromTexture->m_rawPixelData.data();
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    uint8_t *dst = toTexture->m_rawPixelData.data();
    uint8_t *output = dst;
    for (i = 0; i < fromTexture->m_height; i += 4) {
        for (j = 0; j < fromTexture->m_width; j += 4) {
            dst = output + (i * fromTexture->m_width + j) * 4;
            bcdec_bc3(src, dst, (int) fromTexture->m_width * 4);
            src += BCDEC_BC3_BLOCK_SIZE;
        }
    }
    return true;
}

bool convertBC4toR8(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC4_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    int i, j;
    const uint8_t *src = fromTexture->m_rawPixelData.data();
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    uint8_t *dst = toTexture->m_rawPixelData.data();
    uint8_t *output = dst;
    for (i = 0; i < fromTexture->m_height; i += 4) {
        for (j = 0; j < fromTexture->m_width; j += 4) {
            dst = output + (i * fromTexture->m_width + j);
            bcdec_bc4(src, dst, (int) fromTexture->m_width);
            src += BCDEC_BC4_BLOCK_SIZE;
        }
    }
    return true;
}

bool convertBC5toRG88(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC5_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    int i, j;
    const uint8_t *src = fromTexture->m_rawPixelData.data();
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    uint8_t *dst = toTexture->m_rawPixelData.data();
    uint8_t *output = dst;
    for (i = 0; i < fromTexture->m_height; i += 4) {
        for (j = 0; j < fromTexture->m_width; j += 4) {
            dst = output + (i * fromTexture->m_width + j) * 2;
            bcdec_bc5(src, dst, (int) fromTexture->m_width * 2);
            src += BCDEC_BC5_BLOCK_SIZE;
        }
    }
    return true;
}

bool convertBC6toRGB16F(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC6H_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    int i, j;
    const uint8_t *src = fromTexture->m_rawPixelData.data();
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    uint8_t *dst = toTexture->m_rawPixelData.data();
    uint8_t *output = dst;
    for (i = 0; i < fromTexture->m_height; i += 4) {
        for (j = 0; j < fromTexture->m_width; j += 4) {
            dst = output + (i * fromTexture->m_width + j) * 6;
            bcdec_bc6h_half(src, dst, (int) fromTexture->m_width * 3, false);
            src += BCDEC_BC6H_BLOCK_SIZE;
        }
    }
    return true;
}

bool convertBC6toRGB32F(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC6H_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    int i, j;
    const uint8_t *src = fromTexture->m_rawPixelData.data();
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    uint8_t *dst = toTexture->m_rawPixelData.data();
    uint8_t *output = dst;
    for (i = 0; i < fromTexture->m_height; i += 4) {
        for (j = 0; j < fromTexture->m_width; j += 4) {
            dst = output + (i * fromTexture->m_width + j) * 12;
            bcdec_bc6h_float(src, dst, (int) fromTexture->m_width * 3, false);
            src += BCDEC_BC6H_BLOCK_SIZE;
        }
    }
    return true;
}

bool convertBC7toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC7_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    int i, j;
    const uint8_t *src = fromTexture->m_rawPixelData.data();
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    uint8_t *dst = toTexture->m_rawPixelData.data();
    uint8_t *output = dst;
    for (i = 0; i < fromTexture->m_height; i += 4) {
        for (j = 0; j < fromTexture->m_width; j += 4) {
            dst = output + (i * fromTexture->m_width + j) * 4;
            bcdec_bc7(src, dst, (int) fromTexture->m_width * 4);
            src += BCDEC_BC7_BLOCK_SIZE;
        }
    }
    return true;
}

bool convertR8toRG88(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 3 + 0] = fromTexture->m_rawPixelData[i];
        toTexture->m_rawPixelData[i * 3 + 1] = 0;
    }
    return true;
}

bool convertR8toRGB888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 3 + 0] = fromTexture->m_rawPixelData[i];
        toTexture->m_rawPixelData[i * 3 + 1] = 0;
        toTexture->m_rawPixelData[i * 3 + 2] = 0;
    }
    return true;
}

bool convertR8toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 4 + 0] = fromTexture->m_rawPixelData[i];
        toTexture->m_rawPixelData[i * 4 + 1] = 0;
        toTexture->m_rawPixelData[i * 4 + 2] = 0;
        toTexture->m_rawPixelData[i * 4 + 3] = 255;

    }
    return true;
}

bool convertRG88toRGB888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 3 + 0] = fromTexture->m_rawPixelData[i * 2 + 0];
        toTexture->m_rawPixelData[i * 3 + 1] = fromTexture->m_rawPixelData[i * 2 + 1];
        toTexture->m_rawPixelData[i * 3 + 2] = 0;

    }
    return true;
}

bool convertRG88toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 4 + 0] = fromTexture->m_rawPixelData[i * 2 + 0];
        toTexture->m_rawPixelData[i * 4 + 1] = fromTexture->m_rawPixelData[i * 2 + 1];
        toTexture->m_rawPixelData[i * 4 + 2] = 0;
        toTexture->m_rawPixelData[i * 4 + 3] = 255;

    }
    return true;
}

bool convertRG88toR8(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i] = fromTexture->m_rawPixelData[i * 2];

    }
    return true;
}

bool convertRGB888toR8(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i] = fromTexture->m_rawPixelData[i * 3];

    }
    return true;
}

bool convertRGBA8888toR8(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i] = fromTexture->m_rawPixelData[i * 4];

    }
    return true;
}

bool convertRGB888toRG88(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 2 + 0] = fromTexture->m_rawPixelData[i * 3 + 0];
        toTexture->m_rawPixelData[i * 2 + 1] = fromTexture->m_rawPixelData[i * 3 + 1];

    }
    return true;
}

bool convertRGBA8888toRG88(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 2 + 0] = fromTexture->m_rawPixelData[i * 4 + 0];
        toTexture->m_rawPixelData[i * 2 + 1] = fromTexture->m_rawPixelData[i * 4 + 1];

    }
    return true;
}

bool convertRGBA8888toBGRA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 4 + 2] = fromTexture->m_rawPixelData[i * 4 + 0];
        toTexture->m_rawPixelData[i * 4 + 1] = fromTexture->m_rawPixelData[i * 4 + 1];
        toTexture->m_rawPixelData[i * 4 + 0] = fromTexture->m_rawPixelData[i * 4 + 2];
        toTexture->m_rawPixelData[i * 4 + 3] = fromTexture->m_rawPixelData[i * 4 + 3];
    }
    return true;
}

bool convertBGRA8888toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 4 + 2] = fromTexture->m_rawPixelData[i * 4 + 0];
        toTexture->m_rawPixelData[i * 4 + 1] = fromTexture->m_rawPixelData[i * 4 + 1];
        toTexture->m_rawPixelData[i * 4 + 0] = fromTexture->m_rawPixelData[i * 4 + 2];
        toTexture->m_rawPixelData[i * 4 + 3] = fromTexture->m_rawPixelData[i * 4 + 3];
    }
    return true;
}

bool convertRG16toRG88(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    const uint16_t *fromData = reinterpret_cast<const uint16_t *>(fromTexture->m_rawPixelData.data());
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {

        toTexture->m_rawPixelData[i * 2 + 0] = (uint8_t) (fromData[i * 2 + 0] >> 8);
        toTexture->m_rawPixelData[i * 2 + 1] = (uint8_t) (fromData[i * 2 + 1] >> 8);
    }
    return true;
}

bool convertRGBA1010102toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        uint32_t pixel =
                ((uint32_t) fromTexture->m_rawPixelData[i * 4 + 3] << 24) |
                ((uint32_t) fromTexture->m_rawPixelData[i * 4 + 2] << 16) |
                ((uint32_t) fromTexture->m_rawPixelData[i * 4 + 1] << 8) |
                ((uint32_t) fromTexture->m_rawPixelData[i * 4 + 0]);
        uint16_t red = (pixel >> 0) & 0x3FF;
        uint16_t green = (pixel >> 10) & 0x3FF;
        uint16_t blue = (pixel >> 20) & 0x3FF;
        uint16_t alpha = (pixel >> 30) & 0x3;

        red = red >> 2;
        green = green >> 2;
        blue = blue >> 2;
        alpha = alpha * 85; // Remap 0-3 to 0-255

        toTexture->m_rawPixelData[i * 4 + 0] = (uint8_t) red;
        toTexture->m_rawPixelData[i * 4 + 1] = (uint8_t) green;
        toTexture->m_rawPixelData[i * 4 + 2] = (uint8_t) blue;
        toTexture->m_rawPixelData[i * 4 + 3] = alpha;
    }
    return true;
}

bool convertRGBA1010102toRGBA16(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        uint32_t pixel =
                ((uint32_t) fromTexture->m_rawPixelData[i * 4 + 3] << 24) |
                ((uint32_t) fromTexture->m_rawPixelData[i * 4 + 2] << 16) |
                ((uint32_t) fromTexture->m_rawPixelData[i * 4 + 1] << 8) |
                ((uint32_t) fromTexture->m_rawPixelData[i * 4 + 0]);
        uint16_t red = (pixel >> 0) & 0x3FF;
        uint16_t green = (pixel >> 10) & 0x3FF;
        uint16_t blue = (pixel >> 20) & 0x3FF;
        uint16_t alpha = (pixel >> 30) & 0x3;

        red = red << 6 | red >> 4;
        green = green << 6 | green >> 4;
        blue = blue << 6 | blue >> 4;
        alpha = alpha * 21845; // Remap 0-3 to 0-65535

        toTexture->m_rawPixelData[i * 4 + 0] = (uint8_t) red;
        toTexture->m_rawPixelData[i * 4 + 1] = (uint8_t) green;
        toTexture->m_rawPixelData[i * 4 + 2] = (uint8_t) blue;
        toTexture->m_rawPixelData[i * 4 + 3] = alpha;
    }
    return true;
}

bool convertRG16_SIGNEDtoRG16(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    const int16_t *fromData = reinterpret_cast<const int16_t *>(fromTexture->m_rawPixelData.data());
    uint16_t *toData = reinterpret_cast<uint16_t *>(toTexture->m_rawPixelData.data());

    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toData[i * 2 + 0] = static_cast<uint16_t>(fromData[i * 2 + 0] + 32768);
        toData[i * 2 + 1] = static_cast<uint16_t>(fromData[i * 2 + 1] + 32768);
    }
    return true;
}

bool convertR16toR8(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    const uint16_t *fromData = reinterpret_cast<const uint16_t *>(fromTexture->m_rawPixelData.data());
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {

        toTexture->m_rawPixelData[i] = (uint8_t) (fromData[i] >> 8);
    }
    return true;
}

bool convertRA88toRG88(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.assign(fromTexture->m_rawPixelData.begin(), fromTexture->m_rawPixelData.end());
    return true;
}

bool convertRG88toRA88(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.assign(fromTexture->m_rawPixelData.begin(), fromTexture->m_rawPixelData.end());
    return true;
}

bool convertRA88toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 4 + 0] = fromTexture->m_rawPixelData[i * 2 + 0];
        toTexture->m_rawPixelData[i * 4 + 1] = fromTexture->m_rawPixelData[i * 2 + 0];
        toTexture->m_rawPixelData[i * 4 + 2] = fromTexture->m_rawPixelData[i * 2 + 0];
        toTexture->m_rawPixelData[i * 4 + 3] = fromTexture->m_rawPixelData[i * 2 + 1];
    }
    return true;
}

bool convertRG16toRGBA16(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    const uint16_t *fromData = reinterpret_cast<const uint16_t *>(fromTexture->m_rawPixelData.data());
    uint16_t *toData = reinterpret_cast<uint16_t *>(toTexture->m_rawPixelData.data());

    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toData[i * 4 + 0] = fromData[i * 2 + 0];
        toData[i * 4 + 1] = fromData[i * 2 + 1];
        toData[i * 4 + 2] = 0;
        toData[i * 4 + 3] = 0xFFFF;
    }
    return true;
}

bool convertRGBA4444toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 4 + 0] = (fromTexture->m_rawPixelData[i * 2 + 0] & 0xF) * 17;
        toTexture->m_rawPixelData[i * 4 + 1] = ((fromTexture->m_rawPixelData[i * 2 + 0] >> 4) & 0xF) * 17;
        toTexture->m_rawPixelData[i * 4 + 2] = (fromTexture->m_rawPixelData[i * 2 + 1] & 0xF) * 17;
        toTexture->m_rawPixelData[i * 4 + 3] = ((fromTexture->m_rawPixelData[i * 2 + 1] >> 4) & 0xF) * 17;
    }
    return true;
}


bool convertRGBA1111toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        int bitOffset = i % 2 == 0 ? 0 : 4;
        toTexture->m_rawPixelData[i * 4 + 0] = ((fromTexture->m_rawPixelData[i / 2] >> (bitOffset + 0)) & 1) * 255;
        toTexture->m_rawPixelData[i * 4 + 1] = ((fromTexture->m_rawPixelData[i / 2] >> (bitOffset + 1)) & 1) * 255;
        toTexture->m_rawPixelData[i * 4 + 2] = ((fromTexture->m_rawPixelData[i / 2] >> (bitOffset + 2)) & 1) * 255;
        toTexture->m_rawPixelData[i * 4 + 3] = ((fromTexture->m_rawPixelData[i / 2] >> (bitOffset + 3)) & 1) * 255;
    }
    return true;
}

bool convertETC1toRGB888(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC1_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    const uint64_t *src = reinterpret_cast<const uint64_t *>(fromTexture->m_rawPixelData.data());
    uint8_t *dst = toTexture->m_rawPixelData.data();
    uint8_t *output = dst;
    uint32_t i, j;
    for (i = 0; i < toTexture->m_height; i += 4) {
        for (j = 0; j < toTexture->m_width; j += 4) {
            dst = output + (i * toTexture->m_width + j) * 3;
            decode_etc1(*src, dst, toTexture->m_width);
            src++;
        }
    }
    return true;
}


