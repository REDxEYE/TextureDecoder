//
// Created by AORUS on 13.07.2022.
//

#pragma once
#include <cstddef>
#include <cstdint>
#include <cassert>

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


#define MAKE_FOURCC(ch0, ch1, ch2, ch3) \
        ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) | \
        ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24))


/**
 * Aligns a pointer to the specified alignment.
 *
 * @param ptr The original pointer.
 * @param alignment The alignment boundary (must be a power of 2).
 * @return Aligned pointer.
 */
void* alignPointer(void* ptr, std::size_t alignment);

#define BSWAP_64(x)            \
  ((((x) & 0xff00000000000000ull) >> 56)    \
   | (((x) & 0x00ff000000000000ull) >> 40)    \
   | (((x) & 0x0000ff0000000000ull) >> 24)    \
   | (((x) & 0x000000ff00000000ull) >> 8)    \
   | (((x) & 0x00000000ff000000ull) << 8)    \
   | (((x) & 0x0000000000ff0000ull) << 24)    \
   | (((x) & 0x000000000000ff00ull) << 40)    \
   | (((x) & 0x00000000000000ffull) << 56))
