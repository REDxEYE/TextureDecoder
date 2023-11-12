#pragma once

#include "shared.h"
#include "textureDecoder.h"

extern "C" {

DLL_EXPORT int64_t get_buffer_size_from_texture(const sTexture *texture);
DLL_EXPORT int64_t get_buffer_size_from_texture_format(uint32_t width, uint32_t height, ePixelFormat pixelFormat);
DLL_EXPORT sTexture *create_texture(const uint8_t *data, size_t dataSize,
                                    uint32_t width, uint32_t height, ePixelFormat pixelFormat);
DLL_EXPORT sTexture *create_empty_texture(uint32_t width, uint32_t height, ePixelFormat pixelFormat);
DLL_EXPORT bool convert_texture(const sTexture *from_texture, sTexture *to_texture);
DLL_EXPORT bool get_texture_data(const sTexture *texture, char *buffer, uint32_t buffer_size);
DLL_EXPORT uint32_t get_texture_width(const sTexture *texture);
DLL_EXPORT uint32_t get_texture_height(const sTexture *texture);
DLL_EXPORT ePixelFormat get_texture_pixel_format(const sTexture *texture);
DLL_EXPORT void free_texture(sTexture *texture);
DLL_EXPORT sTexture *load_dds(const char *filename);
DLL_EXPORT sTexture *load_png(const char *filename, int expected_channels);
DLL_EXPORT sTexture *load_tga(const char *filename, int expected_channels);
DLL_EXPORT sTexture *load_hdr(const char *filename);
DLL_EXPORT bool write_png(const char *filename, const sTexture* texture);
DLL_EXPORT bool write_tga(const char *filename, const sTexture* texture);

}
