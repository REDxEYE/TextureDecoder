#include <cstring>
#include <cmath>
#include <array>
#include <algorithm>
#include "decoders/etc/etc1.h"
#include "shared.h"

int16_t codeWordTable[8][4]{
        {2,  8,   -2,  -8,},
        {5,  17,  -5,  -17,},
        {9,  29,  -9,  -29,},
        {13, 42,  -13, -42,},
        {18, 60,  -18, -60,},
        {24, 80,  -24, -80,},
        {33, 106, -33, -106,},
        {47, 183, -47, -183,},
};

uint8_t remapTable[16]{
        0, 4, 8, 12,
        1, 5, 9, 13,
        2, 6, 10, 14,
        3, 7, 11, 15
};

uint8_t extend4to8(uint8_t v) {
    return v << 4 | v;
}

uint8_t extend5to8(uint8_t v) {
    return v << 3 | v >> 2;
}

struct RGB {
    uint8_t r, g, b;
};

void decode_etc1(uint64_t block, uint8_t *dst, uint64_t pitch) {
    block = BSWAP_64(block);
    bool diffMode = (block >> 33) & 1;
    bool flipMode = (block >> 32) & 1;
    RGB *dstRgb = reinterpret_cast<RGB *>(dst);
    RGB rgb0{};
    RGB rgb1{};
    uint8_t tableIndex0;
    uint8_t tableIndex1;
    uint32_t pixelIndices;

    if (!diffMode) {
        ETC1BlockFull blockFull{};
        memcpy(&blockFull, &block, 8);
        rgb0.r = extend4to8(blockFull.r1);
        rgb0.g = extend4to8(blockFull.g1);
        rgb0.b = extend4to8(blockFull.b1);
        rgb1.r = extend4to8(blockFull.r0);
        rgb1.g = extend4to8(blockFull.g0);
        rgb1.b = extend4to8(blockFull.b0);
        tableIndex0 = blockFull.tableCodeword0;
        tableIndex1 = blockFull.tableCodeword1;
        pixelIndices = blockFull.pixelIndices;
    } else {
        ETC1BlockDiff blockDiff{};
        memcpy(&blockDiff, &block, 8);
        rgb0.r = extend5to8(blockDiff.r0b);
        rgb0.g = extend5to8(blockDiff.g0b);
        rgb0.b = extend5to8(blockDiff.b0b);
        rgb1.r = extend5to8(blockDiff.r0b + blockDiff.r1d);
        rgb1.g = extend5to8(blockDiff.g0b + blockDiff.g1d);
        rgb1.b = extend5to8(blockDiff.b0b + blockDiff.b1d);
        tableIndex0 = blockDiff.tableCodeword0;
        tableIndex1 = blockDiff.tableCodeword1;
        pixelIndices = blockDiff.pixelIndices;
    }

    for (uint32_t i = 0; i < 16; i++) {
        uint8_t modIndex = ((pixelIndices >> (15 + i)) & 2) | (pixelIndices >> (i) & 1);
        int16_t mod0 = codeWordTable[tableIndex0][modIndex];
        int16_t mod1 = codeWordTable[tableIndex1][modIndex];

        RGB color0{
                (uint8_t) std::clamp((int16_t) rgb0.r + mod0, 0, 255),
                (uint8_t) std::clamp((int16_t) rgb0.g + mod0, 0, 255),
                (uint8_t) std::clamp((int16_t) rgb0.b + mod0, 0, 255),
        };
        RGB color1{
                (uint8_t) std::clamp((int16_t) rgb1.r + mod1, 0, 255),
                (uint8_t) std::clamp((int16_t) rgb1.g + mod1, 0, 255),
                (uint8_t) std::clamp((int16_t) rgb1.b + mod1, 0, 255),
        };
        uint8_t x = (remapTable[i] % 4);
        uint8_t y = (remapTable[i] / 4);
        if (!flipMode) {
            dstRgb[x + pitch * y] = i > 7 ? color1 : color0;
        } else {
            dstRgb[x + pitch * y] = i % 4 > 1 ? color1 : color0;
        }
    }

}
