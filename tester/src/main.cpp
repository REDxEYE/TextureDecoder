#include <gtest/gtest.h>
#include "texture.h"
#include "textureApi.h"
#include "stb_image.h"



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



TEST(Library, TestIsCompressedFormat) {
    ASSERT_TRUE(isCompressedPixelFormat(ePixelFormat::BC1));
    ASSERT_TRUE(isCompressedPixelFormat(ePixelFormat::BC1a));
    ASSERT_TRUE(isCompressedPixelFormat(ePixelFormat::BC2));
    ASSERT_TRUE(isCompressedPixelFormat(ePixelFormat::BC3));
    ASSERT_TRUE(isCompressedPixelFormat(ePixelFormat::BC4));
    ASSERT_TRUE(isCompressedPixelFormat(ePixelFormat::BC5));
    ASSERT_TRUE(isCompressedPixelFormat(ePixelFormat::BC6));
    ASSERT_TRUE(isCompressedPixelFormat(ePixelFormat::BC7));
    for (uint32_t i = ePixelFormat::RGBA32; i < ePixelFormat::RGBA1010102; i++) {
        ASSERT_FALSE(isCompressedPixelFormat(ePixelFormat(i)))
                                    << "Checking " << getPixelFormatName(ePixelFormat(i)) << "\n";
    }
}