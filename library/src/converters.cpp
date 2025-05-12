#include "converters.h"
#include "bcdec.h"
#include "half.h"
#include "texture.h"
#include "decoders/etc/etc1.h"
#include <cstring>
#include <functional>


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

using Decoder = std::function<void(void *src, void *dst, int len)>;

template<size_t blockSize, size_t pixelSize>
void
partialBCnToRGBA8888(const uint8_t *src, uint8_t *dst, uint32_t partialWidth, uint32_t partialHeight, uint32_t stride,
                     const Decoder &decoder) {
    static uint8_t tmpBuffer[blockSize * blockSize * pixelSize];
    decoder((void *) src, (void *) tmpBuffer, blockSize * pixelSize);

    for (int y = 0; y < partialHeight; y++) {
        memcpy(dst + y * stride, &tmpBuffer[y * blockSize], partialWidth * blockSize);
    }
}

template<size_t blockDataSize, size_t blockSize, size_t pixelSize>
bool convertBCnToRGBA8888(const sTexture *fromTexture, sTexture *toTexture, const Decoder &decoder) {
    const uint8_t *src = fromTexture->m_rawPixelData.data();
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    uint8_t *dst = toTexture->m_rawPixelData.data();
    uint8_t *output = dst;
    uint32_t stride = fromTexture->m_width * pixelSize;
    for (int y = 0; y < fromTexture->m_height; y += blockSize) {
        for (int x = 0; x < fromTexture->m_width; x += blockSize) {
            if (x >= toTexture->m_width) {
                continue;
            }

            dst = output + (y * fromTexture->m_width + x) * pixelSize;
            if (y + blockSize <= toTexture->m_height && x + blockSize <= toTexture->m_width) {
                decoder((void *) src, dst, (int) stride);
            } else {
                uint32_t partialWidth = std::min(fromTexture->m_width - x, (uint32_t) blockSize);
                uint32_t partialHeight = std::min(fromTexture->m_height - y, (uint32_t) blockSize);
                partialBCnToRGBA8888<blockSize, pixelSize>(src, dst, partialWidth, partialHeight, stride, &bcdec_bc1);
            }

            src += blockDataSize;
        }
    }
    return true;
}

bool convertBC1toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC1_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    convertBCnToRGBA8888<BCDEC_BC1_BLOCK_SIZE, 4, 4>(fromTexture, toTexture, bcdec_bc1);
    return true;
}

bool convertBC1atoRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC1_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    convertBCnToRGBA8888<BCDEC_BC1_BLOCK_SIZE, 4, 4>(fromTexture, toTexture, bcdec_bc1);
    return true;
}


bool convertBC2toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC2_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    convertBCnToRGBA8888<BCDEC_BC2_BLOCK_SIZE, 4, 4>(fromTexture, toTexture, bcdec_bc2);
    return true;
}

bool convertBC3toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC3_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    convertBCnToRGBA8888<BCDEC_BC3_BLOCK_SIZE, 4, 4>(fromTexture, toTexture, bcdec_bc3);
    return true;
}

bool convertBC4toR8(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC4_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    convertBCnToRGBA8888<BCDEC_BC4_BLOCK_SIZE, 4, 1>(fromTexture, toTexture, bcdec_bc4);
    return true;
}

bool convertBC5toRG88(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC5_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    convertBCnToRGBA8888<BCDEC_BC5_BLOCK_SIZE, 4, 2>(fromTexture, toTexture, bcdec_bc5);
    return true;
}

void bcdec_bc6h_half_unsigned(const void *compressedBlock, void *decompressedBlock, int destinationPitch) {
    bcdec_bc6h_half(compressedBlock, decompressedBlock, destinationPitch, false);
}

bool convertBC6toRGB16F(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC6H_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    convertBCnToRGBA8888<BCDEC_BC6H_BLOCK_SIZE, 4, 6>(fromTexture, toTexture,bcdec_bc6h_half_unsigned);
    return true;
}

void bcdec_bc6h_float_unsigned(const void *compressedBlock, void *decompressedBlock, int destinationPitch) {
    bcdec_bc6h_float(compressedBlock, decompressedBlock, destinationPitch, false);
}
bool convertBC6toRGB32F(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC6H_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    convertBCnToRGBA8888<BCDEC_BC6H_BLOCK_SIZE, 4, 12>(fromTexture, toTexture,bcdec_bc6h_float_unsigned);
    return true;
}

bool convertBC7toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    if (fromTexture->m_rawPixelData.size() <
        BCDEC_BC7_COMPRESSED_SIZE(fromTexture->m_width, fromTexture->m_height))
        return false;
    convertBCnToRGBA8888<BCDEC_BC7_BLOCK_SIZE, 4, 4>(fromTexture, toTexture,bcdec_bc7);
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

bool convertABGR8888toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 4 + 0] = fromTexture->m_rawPixelData[i * 4 + 3];
        toTexture->m_rawPixelData[i * 4 + 1] = fromTexture->m_rawPixelData[i * 4 + 2];
        toTexture->m_rawPixelData[i * 4 + 2] = fromTexture->m_rawPixelData[i * 4 + 1];
        toTexture->m_rawPixelData[i * 4 + 3] = fromTexture->m_rawPixelData[i * 4 + 0];
    }
    return true;
}

bool convertARGB8888toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 4 + 1] = fromTexture->m_rawPixelData[i * 4 + 0];
        toTexture->m_rawPixelData[i * 4 + 2] = fromTexture->m_rawPixelData[i * 4 + 1];
        toTexture->m_rawPixelData[i * 4 + 3] = fromTexture->m_rawPixelData[i * 4 + 2];
        toTexture->m_rawPixelData[i * 4 + 0] = fromTexture->m_rawPixelData[i * 4 + 3];
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

bool convertRGB565toRGB888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        union {
            struct {
                uint16_t r: 5;
                uint16_t g: 6;
                uint16_t b: 5;
            } rgb;

            uint16_t d;
        } pixel;
        pixel.d = uint16_t(fromTexture->m_rawPixelData[i * 2 + 1]) << 8 |
                  fromTexture->m_rawPixelData[i * 2 + 0];

        toTexture->m_rawPixelData[i * 3 + 0] = ((uint16_t) pixel.rgb.r * 255) / 31;
        toTexture->m_rawPixelData[i * 3 + 1] = ((uint16_t) pixel.rgb.g * 255) / 63;
        toTexture->m_rawPixelData[i * 3 + 2] = ((uint16_t) pixel.rgb.b * 255) / 31;
    }
    return true;
}

bool convertRGB888toBGR888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 3 + 2] = fromTexture->m_rawPixelData[i * 3 + 0];
        toTexture->m_rawPixelData[i * 3 + 1] = fromTexture->m_rawPixelData[i * 3 + 1];
        toTexture->m_rawPixelData[i * 3 + 0] = fromTexture->m_rawPixelData[i * 3 + 2];
    }
    return true;
}

bool convertBGR888toRGB888(const sTexture *fromTexture, sTexture *toTexture) {
    return convertRGB888toBGR888(fromTexture, toTexture);
}

bool convertRGBA8888toARGB8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toTexture->m_rawPixelData[i * 4 + 1] = fromTexture->m_rawPixelData[i * 4 + 0];
        toTexture->m_rawPixelData[i * 4 + 2] = fromTexture->m_rawPixelData[i * 4 + 1];
        toTexture->m_rawPixelData[i * 4 + 3] = fromTexture->m_rawPixelData[i * 4 + 2];
        toTexture->m_rawPixelData[i * 4 + 0] = fromTexture->m_rawPixelData[i * 4 + 3];
    }
    return true;
}

bool convertRGBA5551toRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        union {
            struct {
                uint16_t r: 5;
                uint16_t g: 5;
                uint16_t b: 5;
                uint16_t a: 1;
            } rgb;

            uint16_t d;
        } pixel;
        pixel.d = uint16_t(fromTexture->m_rawPixelData[i * 2 + 1]) << 8 |
                  fromTexture->m_rawPixelData[i * 2 + 0];

        toTexture->m_rawPixelData[i * 4 + 0] = ((uint16_t) pixel.rgb.r * 255) / 31;
        toTexture->m_rawPixelData[i * 4 + 1] = ((uint16_t) pixel.rgb.g * 255) / 31;
        toTexture->m_rawPixelData[i * 4 + 2] = ((uint16_t) pixel.rgb.b * 255) / 31;
        toTexture->m_rawPixelData[i * 4 + 3] = ((uint16_t) pixel.rgb.a * 255);
    }
    return true;
}

bool convertRGB32FtoRGBA32F(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    const float *fromData = reinterpret_cast<const float *>(fromTexture->m_rawPixelData.data());
    float *toData = reinterpret_cast<float *>(toTexture->m_rawPixelData.data());
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toData[i * 4 + 0] = fromData[i * 3 + 0];
        toData[i * 4 + 1] = fromData[i * 3 + 1];
        toData[i * 4 + 2] = fromData[i * 3 + 2];
        toData[i * 4 + 3] = 1.f;
    }
    return true;
}

bool convertRGBA32FtoRGBA8888(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    const float *fromData = reinterpret_cast<const float *>(fromTexture->m_rawPixelData.data());
    uint8_t *toData = toTexture->m_rawPixelData.data();
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toData[i * 4 + 0] = (uint8_t) (fromData[i * 4 + 0] * 255);
        toData[i * 4 + 1] = (uint8_t) (fromData[i * 4 + 1] * 255);
        toData[i * 4 + 2] = (uint8_t) (fromData[i * 4 + 2] * 255);
        toData[i * 4 + 3] = (uint8_t) (fromData[i * 4 + 3] * 255);
    }
    return true;
}

bool convertR32FtoRGBA32F(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    const float *fromData = reinterpret_cast<const float *>(fromTexture->m_rawPixelData.data());
    float *toData = reinterpret_cast<float *>(toTexture->m_rawPixelData.data());
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toData[i * 4 + 0] = fromData[i];
        toData[i * 4 + 1] = fromData[i];
        toData[i * 4 + 2] = fromData[i];
        toData[i * 4 + 3] = 1.f;
    }
    return true;
}

bool convertR32FtoRGB32F(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    const float *fromData = reinterpret_cast<const float *>(fromTexture->m_rawPixelData.data());
    float *toData = reinterpret_cast<float *>(toTexture->m_rawPixelData.data());
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toData[i * 3 + 0] = fromData[i];
        toData[i * 3 + 1] = fromData[i];
        toData[i * 3 + 2] = fromData[i];
    }
    return true;
}

bool convertRGB16FtoRGBA16F(const sTexture *fromTexture, sTexture *toTexture) {
    toTexture->m_rawPixelData.resize(calculateTextureSize(toTexture));
    const uint16_t *fromData = reinterpret_cast<const uint16_t *>(fromTexture->m_rawPixelData.data());
    uint16_t *toData = reinterpret_cast<uint16_t *>(toTexture->m_rawPixelData.data());
    for (int i = 0; i < fromTexture->m_width * fromTexture->m_height; i++) {
        toData[i * 4 + 0] = fromData[i * 3 + 0];
        toData[i * 4 + 1] = fromData[i * 3 + 1];
        toData[i * 4 + 2] = fromData[i * 3 + 2];
        toData[i * 4 + 3] = 0x3c00;
    }
    return true;
}
