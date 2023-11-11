#include "converters.h"
#include "bcdec.h"

bool convertRGBA8888toRGBA8888(const sTexture *from_texture, sTexture *to_texture) {
    if (from_texture->m_width != to_texture->m_width &&
        from_texture->m_height != to_texture->m_height
            ) {
        return false;
    }
    to_texture->m_rawPixelData.resize(from_texture->m_rawPixelData.size());
    memcpy(to_texture->m_rawPixelData.data(), from_texture->m_rawPixelData.data(),
           reinterpret_cast<size_t>(from_texture->m_rawPixelData.data()));
    return true;
}

bool convertBC1toRGBA8888(const sTexture *from_texture, sTexture *to_texture) {
    if (from_texture->m_width != to_texture->m_width &&
        from_texture->m_height != to_texture->m_height
            ) {
        return false;
    }
    if (from_texture->m_rawPixelData.size() < BCDEC_BC1_COMPRESSED_SIZE(from_texture->m_width, from_texture->m_height))return false;
    int i,j;
    const uint8_t *src = from_texture->m_rawPixelData.data();
    uint8_t *dst = to_texture->m_rawPixelData.data();
    uint8_t *output = dst;
    for (i = 0; i < from_texture->m_height; i += 4) {
        for (j = 0; j < from_texture->m_width; j += 4) {
            dst = output + (i * from_texture->m_width + j) * 4;
            bcdec_bc1(src, dst, from_texture->m_width * 4);
            src += BCDEC_BC1_BLOCK_SIZE;
        }
    }
    return true;
}

bool convertBC1atoRGBA8888(const sTexture *from_texture, sTexture *to_texture) {
    if (from_texture->m_width != to_texture->m_width &&
        from_texture->m_height != to_texture->m_height
            ) {
        return false;
    }
    if (from_texture->m_rawPixelData.size() < BCDEC_BC1_COMPRESSED_SIZE(from_texture->m_width, from_texture->m_height))return false;
    int i,j;
    const uint8_t *src = from_texture->m_rawPixelData.data();
    uint8_t *dst = to_texture->m_rawPixelData.data();
    uint8_t *output = dst;
    for (i = 0; i < from_texture->m_height; i += 4) {
        for (j = 0; j < from_texture->m_width; j += 4) {
            dst = output + (i * from_texture->m_width + j) * 4;
            bcdec_bc1(src, dst, from_texture->m_width * 4);
            src += BCDEC_BC1_BLOCK_SIZE;
        }
    }
    return true;
}
