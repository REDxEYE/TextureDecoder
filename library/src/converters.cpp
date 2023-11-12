#include <cstring>
#include "converters.h"
#include "bcdec.h"
#include "half.h"
#include "texture.h"


bool convertRGBA8888toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    memcpy(toTexture->m_rawPixelData.data(), fromTexture->m_rawPixelData.data(),
           reinterpret_cast<size_t>(fromTexture->m_rawPixelData.data()));
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
        *(FLOAT16*)&toTexture->m_rawPixelData[i * 6 + 0] = *(float*)&fromTexture->m_rawPixelData[i * 12 + 0];
        *(FLOAT16*)&toTexture->m_rawPixelData[i * 6 + 2] = *(float*)&fromTexture->m_rawPixelData[i * 12 + 4];
        *(FLOAT16*)&toTexture->m_rawPixelData[i * 6 + 4] = *(float*)&fromTexture->m_rawPixelData[i * 12 + 8];
    }
    return true;
}

bool convertRGB16FtoRGB32F(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        *(float*)&toTexture->m_rawPixelData[i * 12 + 0] = *(FLOAT16*)&fromTexture->m_rawPixelData[i * 6 + 0];
        *(float*)&toTexture->m_rawPixelData[i * 12 + 4] = *(FLOAT16*)&fromTexture->m_rawPixelData[i * 6 + 2];
        *(float*)&toTexture->m_rawPixelData[i * 12 + 8] = *(FLOAT16*)&fromTexture->m_rawPixelData[i * 6 + 4];
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
            bcdec_bc6h_half(src, dst, (int) fromTexture->m_width * 3,false);
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
            bcdec_bc6h_float(src, dst, (int) fromTexture->m_width * 3,false);
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
        toTexture->m_rawPixelData[i * 3 + 0] = fromTexture->m_rawPixelData[i];
        toTexture->m_rawPixelData[i * 3 + 1] = 0;
        toTexture->m_rawPixelData[i * 3 + 2] = 0;
        toTexture->m_rawPixelData[i * 3 + 3] = 255;

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
        toTexture->m_rawPixelData[i * 3 + 0] = fromTexture->m_rawPixelData[i * 2 + 0];
        toTexture->m_rawPixelData[i * 3 + 1] = fromTexture->m_rawPixelData[i * 2 + 1];
        toTexture->m_rawPixelData[i * 3 + 2] = 0;
        toTexture->m_rawPixelData[i * 3 + 3] = 255;

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


