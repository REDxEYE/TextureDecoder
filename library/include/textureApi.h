#pragma once

#include <cstdint>
#include <cstddef>
#include "pixelFormat.h"

struct sTexture;

/**
 * Creates an empty texture with default values.
 *
 * @return A pointer to the newly created texture.
 */
sTexture *createTexture();

/**
 * Initializes a texture object with given dimensions and pixel format.
 *
 * @param texture A pointer to the texture to be initialized.
 * @param width The width of the texture in pixels.
 * @param height The height of the texture in pixels.
 * @param pixelFormat The pixel format of the texture.
 */
void initTexture(sTexture* texture, uint32_t width, uint32_t height, ePixelFormat pixelFormat);

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
sTexture *createTexture(const uint8_t *data, size_t dataSize, uint32_t width, uint32_t height, ePixelFormat pixelFormat);

/**
 * Creates a texture with specified width, height, and pixel format.
 *
 * @param width The width of the texture in pixels.
 * @param height The height of the texture in pixels.
 * @param pixelFormat The pixel format of the texture.
 * @return A pointer to the newly created texture.
 */
sTexture *createTexture(uint32_t width, uint32_t height, ePixelFormat pixelFormat);

/**
 * Flips a given texture either vertically, horizontally, or both, and writes the result to an output texture.
 *
 * @param inTexture A pointer to the source texture to flip.
 * @param outTexture A pointer to the destination texture where the flipped result will be stored. **!!RE-INITIALIZED INSIDE!!**
 * @param flipUD A boolean flag indicating whether to flip the texture upside down (vertically).
 * @param flipLR A boolean flag indicating whether to flip the texture left to right (horizontally).
 * @return A boolean value indicating the success or failure of the operation.
 */
bool flipTexture(const sTexture *inTexture, sTexture* outTexture, bool flipUD, bool flipLR);

/**
 * Calculates the size in bytes of a texture based on its width, height, and pixel format.
 * This function handles both uncompressed and compressed (BC) formats.
 * For compressed formats, it assumes block compression with a 4x4 block size.
 *
 * @param width The width of the texture in pixels.
 * @param height The height of the texture in pixels.
 * @param pixelFormat The pixel format of the texture.
 * @return The size of the texture in bytes, or -1 for invalid formats.
 */
int64_t calculateTextureSize(uint32_t width, uint32_t height, ePixelFormat pixelFormat);

/**
 * Calculates the size in bytes of a texture based on its properties.
 * This is an overloaded function that accepts a texture object directly.
 *
 * @param texture A pointer to the texture for which size calculation is desired.
 * @return The size of the texture in bytes, or -1 for invalid formats.
 */
int64_t calculateTextureSize(const sTexture *texture);

/**
 * Converts a texture from one pixel format to another.
 *
 * @param fromTexture The source texture to be converted.
 * @param toTexture The destination texture where the converted data will be stored.
 * @return A boolean value indicating the success or failure of the conversion.
 */
bool convertTexture(const sTexture *from_texture, sTexture *to_texture);

/**
 * Frees the memory allocated for a texture.
 *
 * @param texture A pointer to the texture to be freed.
 */
void freeTexture(sTexture *texture);


