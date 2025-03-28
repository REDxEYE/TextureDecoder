#include <gtest/gtest.h>
#include "formats/dds/ddsSupport.h"
#include "formats/png/pngSupport.h"
#include "formats/hdr/hdrSupport.h"
#include "texture.h"
#include "textureApi.h"
#include "shared.h"

TEST(DDSTests, TestR8) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/mode-l.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/mode-l.png", &eTexture, 1));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::R8);
    assert_same(&texture, &eTexture, 1);
}

TEST(DDSTests, TestRA88) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/mode-la.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/mode-la.png", &eTexture, 2));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::RA88);
    assert_same(&texture, &eTexture, 2);
}

TEST(DDSTests, TestRGB888) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/mode-rgb.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/mode-rgb.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::RGB888);
    assert_same(&texture, &eTexture, 3);
}

TEST(DDSTests, TestRGBA8888) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/mode-rgba.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/mode-rgba.png", &eTexture, 4));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::RGBA8888);
    assert_same(&texture, &eTexture, 4);
}

TEST(DDSTests, TestRGBA8888ToRGB888) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/mode-rgba.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/mode-rgb.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::RGBA8888);
    sTexture *rgb888Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RGB888);
    ASSERT_TRUE(convertTexture(&texture, rgb888Texture));
    assert_same(rgb888Texture, &eTexture, 3);
    freeTexture(rgb888Texture);
}

TEST(DDSTests, TestRGB888ToRGBA8888) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/mode-rgb.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/mode-rgb.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::RGB888);
    sTexture *rgba8888Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RGBA8888);
    sTexture *ergba8888Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RGBA8888);
    ASSERT_TRUE(convertTexture(&texture, rgba8888Texture));
    ASSERT_TRUE(convertTexture(&eTexture, ergba8888Texture));
    assert_same(rgba8888Texture, ergba8888Texture, 4);
    freeTexture(rgba8888Texture);
    freeTexture(ergba8888Texture);
}

TEST(DDSTests, TestBC1) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/dxt1-rgb-4bbp-noalpha_MipMaps-1.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/dxt1-rgb-4bbp-noalpha_MipMaps-1.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC1);
    sTexture *rgb888Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RGB888);
    ASSERT_TRUE(convertTexture(&texture, rgb888Texture));
    assert_same(rgb888Texture, &eTexture, 3);
    freeTexture(rgb888Texture);
}

TEST(DDSTests, TestBC2) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/dxt3-argb-8bbp-explicitalpha_MipMaps-1.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/dxt3-argb-8bbp-explicitalpha_MipMaps-1.png", &eTexture, 4));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC2);
    sTexture *rgb888Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RGBA8888);
    ASSERT_TRUE(convertTexture(&texture, rgb888Texture));
    assert_same(rgb888Texture, &eTexture, 4);
    freeTexture(rgb888Texture);
}

TEST(DDSTests, TestBC3) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/dxt5-argb-8bbp-interpolatedalpha_MipMaps-1.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/dxt5-argb-8bbp-interpolatedalpha_MipMaps-1.png", &eTexture, 4));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC3);
    sTexture *rgb888Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RGBA8888);
    ASSERT_TRUE(convertTexture(&texture, rgb888Texture));
    assert_same(rgb888Texture, &eTexture, 4);
    freeTexture(rgb888Texture);
}

TEST(DDSTests, TestBC4) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/ati1.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/ati1.png", &eTexture, 1));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC4);
    sTexture *r8Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::R8);
    ASSERT_TRUE(convertTexture(&texture, r8Texture));
    assert_same(r8Texture, &eTexture, 1);
    freeTexture(r8Texture);
}

TEST(DDSTests, TestBC4U) {
    sTexture texture;
    ASSERT_TRUE(loadDDS("test_data/bc4u.dds", &texture));
    sTexture eTexture;
    ASSERT_TRUE(loadPNG("test_data/bc4u.png", &eTexture, 1));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC4);
    sTexture *r8Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::R8);
    ASSERT_TRUE(convertTexture(&texture, r8Texture));
    assert_same(r8Texture, &eTexture, 1);
    freeTexture(r8Texture);
}

TEST(DDSTests, TestBC5) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/ati2.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/ati2.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC5);
    sTexture *rg88eTexture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RG88);
    sTexture *rg88Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RG88);
    ASSERT_TRUE(convertTexture(&texture, rg88Texture));
    ASSERT_TRUE(convertTexture(&eTexture, rg88eTexture));
    assert_same(rg88Texture, rg88eTexture, 2);
    freeTexture(rg88Texture);
    freeTexture(rg88eTexture);
}

TEST(DDSTests, TestDX10BC5Snorm) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/bc5_snorm.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/bc5_snorm.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC5);
    sTexture *rg88eTexture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RG88);
    sTexture *rg88Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RG88);
    ASSERT_TRUE(convertTexture(&texture, rg88Texture));
    ASSERT_TRUE(convertTexture(&eTexture, rg88eTexture));
    assert_same(rg88Texture, rg88eTexture, 2);
    freeTexture(rg88Texture);
    freeTexture(rg88eTexture);
}

TEST(DDSTests, TestDX10BC5Typeless) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/bc5_typeless.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/bc5_typeless.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC5);
    sTexture *rg88eTexture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RG88);
    sTexture *rg88Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RG88);
    ASSERT_TRUE(convertTexture(&texture, rg88Texture));
    ASSERT_TRUE(convertTexture(&eTexture, rg88eTexture));
    assert_same(rg88Texture, rg88eTexture, 2);
    freeTexture(rg88Texture);
    freeTexture(rg88eTexture);
}

TEST(DDSTests, TestDX10BC5Unorm) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/bc5_unorm.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/bc5_unorm.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC5);
    sTexture *rg88eTexture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RG88);
    sTexture *rg88Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RG88);
    ASSERT_TRUE(convertTexture(&texture, rg88Texture));
    ASSERT_TRUE(convertTexture(&eTexture, rg88eTexture));
    assert_same(rg88Texture, rg88eTexture, 2);
    freeTexture(rg88Texture);
    freeTexture(rg88eTexture);
}

TEST(DDSTests, TestDX10BC1Unorm) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/bc1.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/bc1.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC1);
    sTexture *rgb888Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RGB888);
    ASSERT_TRUE(convertTexture(&texture, rgb888Texture));
    assert_same(rgb888Texture, &eTexture, 3);
    freeTexture(rgb888Texture);
}

TEST(DDSTests, TestDX10BC1Typeless) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/bc1_typeless.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/bc1_typeless.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC1);
    sTexture *rgb888Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RGB888);
    ASSERT_TRUE(convertTexture(&texture, rgb888Texture));
    assert_same(rgb888Texture, &eTexture, 3);
    freeTexture(rgb888Texture);
}

TEST(DDSTests, TestDX10BC7Unorm) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/bc7-argb-8bpp_MipMaps-1.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/bc7-argb-8bpp_MipMaps-1.png", &eTexture, 4));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC7);
    sTexture *rg88Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RGBA8888);
    ASSERT_TRUE(convertTexture(&texture, rg88Texture));
    assert_same(rg88Texture, &eTexture, 4);
    freeTexture(rg88Texture);
}

TEST(DDSTests, TestDX10BC6UF16) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/source.dds", &texture));
    ASSERT_TRUE(loadHDR("test_data/source.hdr", &eTexture));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC6);
    sTexture *rg88Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RGB32F);
    ASSERT_TRUE(convertTexture(&texture, rg88Texture));
    assert_same_float(rg88Texture, &eTexture, 3);
    freeTexture(rg88Texture);
}

TEST(DDSTests, TestDX10BC6SF16) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/bc6h_sf.dds", &texture));
    ASSERT_TRUE(loadHDR("test_data/bc6h_sf.hdr", &eTexture));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC6);
    sTexture *rg88Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RGB32F);
    ASSERT_TRUE(convertTexture(&texture, rg88Texture));
    assert_same_float(rg88Texture, &eTexture, 3);
    freeTexture(rg88Texture);
}

TEST(DDSTests, TestDX10RG16) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadDDS("test_data/finalmm1616m00.dds", &texture));
    ASSERT_TRUE(loadPNG("test_data/finalmm1616m00.png", &eTexture, 0));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::RG16_SIGNED);
    sTexture *rgba8888Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RG88);
    sTexture *ergba8888Texture = createTexture(eTexture.m_width, eTexture.m_height, ePixelFormat::RG88);
    ASSERT_TRUE(convertTexture(&texture, rgba8888Texture));
    ASSERT_TRUE(convertTexture(&eTexture, ergba8888Texture));
    writePNG("test_data/finalmm1616m002.png", rgba8888Texture);
    assert_almost_same(rgba8888Texture, ergba8888Texture, 2, 1);
    freeTexture(rgba8888Texture);
    freeTexture(ergba8888Texture);
}
