#include <gtest/gtest.h>
#include "dds/loadDDS.h"
#include "textureDecoder.h"

TEST(DDSTests, CanLoad) {
    sTexture texture;
    ASSERT_TRUE(loadDDS("test_data/ARGB_8888_1.DDS", &texture));
}