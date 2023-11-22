#pragma once
#include <cstdint>

struct ETC1BlockFull {
    unsigned int pixelIndices:32; // 2 bits for each pixel index (16 pixels total)
    unsigned int flipBit: 1;
    unsigned int diffBit: 1;
    unsigned int tableCodeword1: 3;
    unsigned int tableCodeword0: 3;
    unsigned int b0: 4;
    unsigned int b1: 4;
    unsigned int g0: 4;
    unsigned int g1: 4;
    unsigned int r0: 4;
    unsigned int r1: 4;
};

struct ETC1BlockDiff {
    unsigned int pixelIndices:32; // 2 bits for each pixel index (16 pixels total)
    unsigned int flipBit: 1;
    unsigned int diffBit: 1;
    unsigned int tableCodeword1: 3;
    unsigned int tableCodeword0: 3;
    int b1d: 3;
    unsigned int b0b: 5;
    int g1d: 3;
    unsigned int g0b: 5;
    int r1d: 3;
    unsigned int r0b: 5;
};


void decode_etc1(uint64_t block, uint8_t* dst, uint64_t pitch);