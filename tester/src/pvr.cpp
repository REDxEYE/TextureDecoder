#include <gtest/gtest.h>
#include "formats/pvr/pvrSupport.h"
#include "formats/png/pngSupport.h"
#include "texture.h"
#include "textureApi.h"
#include "shared.h"


TEST(PVRTests, TestZSTD_ETC1) {
sTexture texture;
sTexture eTexture;
ASSERT_TRUE(loadPVR("test_data/chicken_df.pvr", &texture));
ASSERT_TRUE(loadPNG("test_data/chicken_df.png", &eTexture, 3));
ASSERT_EQ(texture.m_width, eTexture.m_width);
ASSERT_EQ(texture.m_height, eTexture.m_height);
ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::RGB888);
sTexture *rgba8888Texture = createTexture(texture.m_width, texture.m_height, ePixelFormat::RGB888);
ASSERT_TRUE(convertTexture(&eTexture, rgba8888Texture));
writePNG("test_data/finalmm1616m002.png", rgba8888Texture);
assert_almost_same(rgba8888Texture, &eTexture, 2, 1);
freeTexture(rgba8888Texture);
}
TEST(PVRTests, TestETC1) {
sTexture texture;
sTexture eTexture;
ASSERT_TRUE(loadPVR("test_data/etc1.pvr", &texture));
ASSERT_TRUE(loadPNG("test_data/etc1.png", &eTexture, 3));
ASSERT_EQ(texture.m_width, eTexture.m_width);
ASSERT_EQ(texture.m_height, eTexture.m_height);
ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::RGB888);
sTexture *rgba8888Texture = createTexture(texture.m_width, texture.m_height, ePixelFormat::RGB888);
ASSERT_TRUE(convertTexture(&texture, rgba8888Texture));
assert_almost_same(rgba8888Texture, &eTexture, 2, 1);
freeTexture(rgba8888Texture);
}