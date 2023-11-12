#pragma once

#include "shared.h"
#include "dds/loadDDS.h"
#include "png/loadPNG.h"
#include "hdr/loadHDR.h"

extern "C" {

DLL_EXPORT int64_t get_buffer_size_from_texture(sTexture *texture);
DLL_EXPORT int64_t get_buffer_size_from_texture_format(uint32_t width, uint32_t height, ePixelFormat pixelFormat);
DLL_EXPORT sTexture *create_texture(const uint8_t *data, size_t dataSize,
                                    uint32_t width, uint32_t height, ePixelFormat pixelFormat);
DLL_EXPORT sTexture *create_empty_texture(uint32_t width, uint32_t height, ePixelFormat pixelFormat);
DLL_EXPORT bool convert_texture(const sTexture *from_texture, sTexture *to_texture);
DLL_EXPORT bool get_texture_data(sTexture *texture, char *buffer, uint32_t buffer_size);
DLL_EXPORT void free_texture(sTexture *texture);
DLL_EXPORT sTexture *load_dds(char *filename);
DLL_EXPORT sTexture *load_png(char *filename, int expected_channels);
DLL_EXPORT sTexture *load_hdr(char *filename);

}
