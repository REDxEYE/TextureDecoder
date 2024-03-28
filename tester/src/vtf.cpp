#include <gtest/gtest.h>
#include "formats/vtf/vtfSupport.h"
#include "formats/png/pngSupport.h"
#include "shared.h"

void convertToAlpha(sTexture *texture) {
    ASSERT_EQ(texture->m_pixelFormat, ePixelFormat::RGBA8888);
    texture->m_pixelFormat = ePixelFormat::R8;
    std::vector<uint8_t> tmp(texture->m_rawPixelData);
    texture->m_rawPixelData.resize(tmp.size() / 4);
    for (int i = 0; i < texture->m_width * texture->m_height; ++i) {
        texture->m_rawPixelData[i] = tmp[i * 4 + 3];
    }
}

TEST(VTFTests, TestA8) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_a8.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_a8.png", &eTexture, 0));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::R8);
    convertToAlpha(&eTexture);
    assert_same(&texture, &eTexture, 1);
}

TEST(VTFTests, TestABGR8888) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_abgr8888.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_abgr8888.png", &eTexture, 0));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::ABGR8888);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGBA8888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 4);
}

TEST(VTFTests, TestARGB8888) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_argb8888.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_argb8888.png", &eTexture, 0));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::ARGB8888);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGBA8888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 4);
}

TEST(VTFTests, TestBGR888) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_bgr888.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_bgr888.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BGR888);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGB888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 3);
}

TEST(VTFTests, TestRGB565) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_rgb565.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_rgb565.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::RGB565);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGB888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 3);
}

TEST(VTFTests, TestBGR565) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_bgr565.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_bgr565.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BGR888);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGB888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 3);
}

TEST(VTFTests, TestBGRA4444) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_bgra4444.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_bgra4444.png", &eTexture, 4));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BGRA8888);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGBA8888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 4);
}

TEST(VTFTests, TestBGRA5551) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_bgra5551.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_bgra5551.png", &eTexture, 4));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BGRA8888);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGBA8888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 4);
}

TEST(VTFTests, TestBGRA8888) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_bgra8888.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_bgra8888.png", &eTexture, 4));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BGRA8888);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGBA8888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 4);
}

TEST(VTFTests, TestBGRX8888) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_bgrx8888.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_bgrx8888.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BGRA8888);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGB888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 3);
}

TEST(VTFTests, TestBGRX5551) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_bgrx5551.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_bgrx5551.png", &eTexture, 3));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BGRA8888);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGB888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 3);
}

TEST(VTFTests, TestDXT1) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_dxt1.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_dxt1.png", &eTexture, 0));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC1);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGB888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 3);
}

TEST(VTFTests, TestDXT1a) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_dxt1_onebitalpha.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_dxt1_onebitalpha.png", &eTexture, 0));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC1a);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGBA8888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 3);
}

TEST(VTFTests, TestDXT3) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_dxt3.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_dxt3.png", &eTexture, 0));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC2);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGBA8888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 4);
}

TEST(VTFTests, TestDXT5) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_dxt5.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_dxt5.png", &eTexture, 0));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::BC3);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGBA8888);
    ASSERT_TRUE(convertTexture(&texture, &tmp));
    assert_same(&tmp, &eTexture, 4);
}

TEST(VTFTests, TestI8) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_i8.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_i8.png", &eTexture, 0));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::R8);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::R8);
    convertTexture(&eTexture, &tmp);
    assert_same(&texture, &tmp, 1);
}

TEST(VTFTests, TestIA88) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_ia88.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_ia88.png", &eTexture, 0));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::RA88);
    assert_same(&texture, &eTexture, 2);
}

TEST(VTFTests, TestUV88) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_uv88.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_uv88.png", &eTexture, 0));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::RG88);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGB888);
    convertTexture(&texture, &tmp);
    assert_same(&tmp, &eTexture, 3);
}

TEST(VTFTests, TestUVLX8888) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_uvlx8888.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_uvlx8888.png", &eTexture, 0));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::RGBA8888);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGBA8888);
    convertTexture(&texture, &tmp);
    assert_same(&tmp, &eTexture, 4);
}

TEST(VTFTests, TestUVWQ8888) {
    sTexture texture;
    sTexture eTexture;
    ASSERT_TRUE(loadVTF("test_data/test_uvwq8888.vtf", &texture));
    ASSERT_TRUE(loadPNG("test_data/test_uvwq8888.png", &eTexture, 0));
    ASSERT_EQ(texture.m_width, eTexture.m_width);
    ASSERT_EQ(texture.m_height, eTexture.m_height);
    ASSERT_EQ(texture.m_pixelFormat, ePixelFormat::RGBA8888);
    sTexture tmp;
    initTexture(&tmp, texture.m_width, texture.m_height, ePixelFormat::RGBA8888);
    convertTexture(&texture, &tmp);
    assert_same(&tmp, &eTexture, 4);
}