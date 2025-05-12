#pragma once

#include "shared.h"
#include "textureApi.h"

extern "C" {

/**
* Gets the size of the buffer required to store the texture data.
*
* @param texture A pointer to the texture.
* @return The size of the buffer in bytes, or -1 if the texture format is invalid.
*/
DLL_EXPORT int64_t get_buffer_size_from_texture(const sTexture *texture);

/**
 * Calculates the buffer size required for a texture with given dimensions and pixel format.
 *
 * @param width The width of the texture in pixels.
 * @param height The height of the texture in pixels.
 * @param pixelFormat The pixel format of the texture.
 * @return The size of the buffer in bytes, or -1 if the pixel format is invalid.
 */
DLL_EXPORT int64_t get_buffer_size_from_texture_format(uint32_t width, uint32_t height, ePixelFormat pixelFormat);
/**
 * Determines if a given pixel format is compressed.
 *
 * @param pixelFormat The pixel format to check.
 * @return True if the pixel format is compressed, false otherwise.
 */
DLL_EXPORT bool is_compressed_pixel_format(ePixelFormat pixelFormat);

/**
 * Retrieves the uncompressed variant of a given pixel format.
 * If the provided pixel format is already uncompressed or if there is no
 * uncompressed variant, the function returns the original pixel format.
 *
 * @param pixelFormat The pixel format for which to find an uncompressed variant.
 * @return The uncompressed variant of the given pixel format, or the original
 *         pixel format if it is already uncompressed or if no uncompressed
 *         variant exists.
 */
DLL_EXPORT ePixelFormat get_uncompressed_pixel_format_variant(ePixelFormat pixelFormat);

/**
 * Creates a texture from raw pixel data.
 *
 * @param data Pointer to the raw pixel data.
 * @param dataSize Size of the raw pixel data in bytes.
 * @param width The width of the texture in pixels.
 * @param height The height of the texture in pixels.
 * @param pixelFormat The pixel format of the texture.
 * @return A pointer to the newly created texture, or nullptr if the data size is incorrect.
 */
DLL_EXPORT sTexture *create_texture(const uint8_t *data, size_t dataSize, uint32_t width, uint32_t height, ePixelFormat pixelFormat);

/**
 * Creates an uninitialized texture.
 *
 * @return A pointer to the newly created texture or empty texture.
 */
DLL_EXPORT sTexture *create_uninitialized_texture();

/**
 * Creates an empty texture with specified width, height, and pixel format.
 *
 * @param width The width of the texture in pixels.
 * @param height The height of the texture in pixels.
 * @param pixelFormat The pixel format of the texture.
 * @return A pointer to the newly created empty texture.
 */
DLL_EXPORT sTexture *create_empty_texture(uint32_t width, uint32_t height, ePixelFormat pixelFormat);

/**
 * Converts a texture from one pixel format to another.
 *
 * @param from_texture The source texture to be converted.
 * @param to_texture The destination texture where the converted data will be stored.
 * @return True if the conversion was successful, false otherwise.
 */
DLL_EXPORT bool convert_texture(const sTexture *from_texture, sTexture *to_texture);

/**
 * Flips a texture either vertically, horizontally, or both, and writes the result to an output texture.
 *
 * @param in_texture A pointer to the source texture to flip.
 * @param out_texture A pointer to the destination texture where the flipped result will be stored.
 * @param flip_ud A boolean flag indicating whether to flip the texture upside down (vertically).
 * @param flip_lr A boolean flag indicating whether to flip the texture left to right (horizontally).
 * @return True if the flip operation was successful, false otherwise.
 */
DLL_EXPORT bool flip_texture(const sTexture *in_texture, sTexture *out_texture, bool flip_ud, bool flip_lr);

/**
 * Retrieves the pixel data of a texture into a provided buffer.
 *
 * @param texture The texture to retrieve the data from.
 * @param buffer The buffer to write the texture data into.
 * @param buffer_size The size of the buffer in bytes.
 * @return True if data retrieval was successful, false otherwise.
 */
DLL_EXPORT bool get_texture_data(const sTexture *texture, char *buffer, uint32_t buffer_size);

/**
 * Retrieves the width of a texture.
 *
 * @param texture The texture to get the width of.
 * @return The width of the texture in pixels.
 */
DLL_EXPORT uint32_t get_texture_width(const sTexture *texture);

/**
 * Retrieves the height of a texture.
 *
 * @param texture The texture to get the height of.
 * @return The height of the texture in pixels.
 */
DLL_EXPORT uint32_t get_texture_height(const sTexture *texture);

/**
 * Retrieves the pixel format of a texture.
 *
 * @param texture The texture to get the pixel format of.
 * @return The pixel format of the texture.
 */
DLL_EXPORT ePixelFormat get_texture_pixel_format(const sTexture *texture);

/**
 * Frees the memory allocated for a texture.
 *
 * @param texture A pointer to the texture to be freed.
 */
DLL_EXPORT void free_texture(sTexture *texture);


DLL_EXPORT sTexture *load_dds(const char *filename);
DLL_EXPORT sTexture *load_dds_from_data(const char *data, uint32_t size);
DLL_EXPORT sTexture *load_png(const char *filename, int expected_channels);
DLL_EXPORT sTexture *load_tga(const char *filename, int expected_channels);
DLL_EXPORT sTexture *load_hdr(const char *filename);
DLL_EXPORT sTexture *load_pvr(const char *filename);
DLL_EXPORT bool write_png(const char *filename, const sTexture* texture);
DLL_EXPORT bool write_tga(const char *filename, const sTexture* texture);

// Expose ZSTD and LZ4 functions, since we already have them
DLL_EXPORT size_t zstd_decompress( void* dst, size_t dstCapacity, const void* src, size_t compressedSize);
DLL_EXPORT size_t lz4_decompress( void* dst, size_t dstCapacity, const void* src, size_t compressedSize);

DLL_EXPORT void print_all_converters();
DLL_EXPORT void set_log_level(uint32_t level);
}
