#pragma once
#include "formats/png/pngSupport.h"

static void assert_same(const sTexture *t, const sTexture *e, uint64_t c) {
    writePNG("test_data/assert_same_t.png", t);
    writePNG("test_data/assert_same_e.png", e);
    ASSERT_EQ(t->m_rawPixelData.size(), e->m_rawPixelData.size());
    ASSERT_EQ(t->m_pixelFormat, e->m_pixelFormat) << getPixelFormatName(t->m_pixelFormat) << " VS " <<
 getPixelFormatName(e->m_pixelFormat);
    for (uint64_t i = 0; i < e->m_width * e->m_height * c; ++i) {
        ASSERT_NEAR(t->m_rawPixelData[i], e->m_rawPixelData[i], 1.0f);
    }
}

static void assert_almost_same(const sTexture *t, const sTexture *e, uint64_t c, uint32_t max_error) {
    writePNG("test_data/assert_same_t.png", t);
    writePNG("test_data/assert_same_e.png", e);
    ASSERT_EQ(t->m_rawPixelData.size(), e->m_rawPixelData.size());
    ASSERT_EQ(t->m_pixelFormat, e->m_pixelFormat);
    for (uint64_t i = 0; i < e->m_width * e->m_height * c; ++i) {
        ASSERT_LE(std::abs(t->m_rawPixelData[i] - e->m_rawPixelData[i]), max_error) << "i=" << i;
    }
}

static void assert_same_float(const sTexture *t, const sTexture *e, uint64_t c) {
    ASSERT_EQ(t->m_rawPixelData.size(), e->m_rawPixelData.size());
    ASSERT_EQ(t->m_pixelFormat, e->m_pixelFormat);
    for (uint64_t i = 0; i < e->m_width * e->m_height * c; ++i) {
        ASSERT_NEAR(*(float *) &t->m_rawPixelData[i * 4], *(float *) &e->m_rawPixelData[i * 4], 0.25) << "i=" << i;
    }
}
