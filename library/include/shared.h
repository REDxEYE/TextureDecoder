//
// Created by AORUS on 13.07.2022.
//

#pragma once
#include "cstdint"

#if defined(_MSC_VER)
//  Microsoft
    #if defined(LIB)
        #define  DLL_EXPORT   extern "C"  __declspec(dllexport)
        #define  DLL_IMPORT   extern "C"  __declspec(dllimport)
    #else
        #define  DLL_EXPORT   __declspec(dllexport)
        #define  DLL_IMPORT   __declspec(dllimport)
    #endif
#elif defined(__GNUC__)
//  GCC
    #if defined(LIB)
        #define DLL_EXPORT   extern "C"  __attribute__((visibility("default")))
        #define DLL_IMPORT
    #else
        #define DLL_EXPORT  __attribute__((visibility("default")))
        #define DLL_IMPORT
    #endif
#else
//  do nothing and hope for the best?
#define DLL_EXPORT
#define DLL_IMPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif

#define BIT_MASK(bit_count) ((1 << (bit_count)) - 1)
#define GET_BIT(bit, value) ((value>>bit)&1)
#define SET_BITS(target, bit_offset, bit_count, value) target|= (((value)&BIT_MASK(bit_count)) << (bit_offset))

#define SWAP(TYPE, A, B)  do{TYPE TMP=A;A=B;B=TMP;}while(0)

enum ERROR_CODES {
    NO_ERROR,
    INPUT_BUFFER_SMALL,
    OUTPUT_BUFFER_SMALL,
};


class BitBuffer {
    uint32_t m_position;
    uint8_t *m_data;
    uint32_t m_data_size;

public:
    BitBuffer(uint8_t *data, uint32_t size) : m_data(data), m_data_size(size), m_position(0) {};

    uint32_t get(uint32_t count) {
        uint32_t bits = get(m_position, count);
        m_position += count;
        return bits;
    }

    uint32_t get(uint32_t position, uint32_t count) {
        if (position + count >= m_data_size) {
            return 0;
        }
        uint32_t idx = position / 8;
        uint32_t bit = position % 8;
        uint16_t chunk;
        if (bit + count <= 8) {
            chunk = m_data[idx];
        } else {
            chunk = uint16_t(m_data[idx]) | m_data[idx + 1] << 8;
        }
        return chunk >> bit & BIT_MASK(count);

    }

    [[nodiscard]] uint32_t position() const {
        return m_position;
    }


    [[nodiscard]] uint32_t length() const {
        return m_data_size * 8;
    }
};