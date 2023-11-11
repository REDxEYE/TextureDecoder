//
// Created by AORUS on 13.07.2022.
//

#pragma once


#include <cstdint>
#include <cmath>
#include <valarray>

struct RGB565;
struct RGB888;
struct RGBA8888;

static inline uint8_t lerp(uint8_t val0, uint8_t val1, uint8_t val0_fac, uint8_t val1_fac);

static inline RGBA8888
rgb_lerp(const RGBA8888 &color0, const RGBA8888 &color1, uint8_t color0_fac, uint8_t color1_fac);

static inline RGB888 rgb_lerp(const RGB888 &color0, const RGB888 &color1, uint8_t color0_fac, uint8_t color1_fac);


struct RGB888 {
    uint8_t r, g, b;

    RGB888() : r(0), g(0), b(0) {};

    RGB888(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {};

};

struct RGB565 {
    uint16_t r: 5;
    uint16_t g: 6;
    uint16_t b: 5;

    [[nodiscard]] inline RGB888 to_rgb888() const {
        return {static_cast<uint8_t>(b * (255 / 31)),
                static_cast<uint8_t>(g * (255 / 63)),
                static_cast<uint8_t>(r * (255 / 31))};
    }

    [[nodiscard]] inline uint16_t as_int() const {
        return *(uint16_t *) (this);
    }

    [[nodiscard]] inline RGB565 lerp(RGB565 other, uint8_t val0_fac, uint8_t val1_fac) const {
        return {
                ::lerp((uint8_t) r, (uint8_t) other.r, val0_fac, val1_fac),
                ::lerp((uint8_t) g, (uint8_t) other.g, val0_fac, val1_fac),
                ::lerp((uint8_t) b, (uint8_t) other.b, val0_fac, val1_fac),
        };
    }

    [[nodiscard]] inline uint8_t diff(const RGB565 &other) const {
        return std::abs(r - other.r) + std::abs(g - other.g) + std::abs(b - other.b);
    }

};

struct RGBA8888 {
    union {
        struct {
            uint8_t r, g, b;
        };
        RGB888 rgb;
    };
    uint8_t a;

    RGBA8888(RGB888 _rgb) : rgb(_rgb), a(255) {};

    explicit RGBA8888(RGB565 rgb) : RGBA8888(rgb.to_rgb888()) {};

    RGBA8888() : r(0), g(0), b(0), a(255) {};

    RGBA8888(uint8_t r, uint8_t g, uint8_t b) : rgb(r, g, b), a(255) {};

    RGBA8888(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : rgb(r, g, b), a(a) {};

    inline uint32_t as_int() {
        return *(uint32_t *) (this);
    }

    inline RGB888 as_rgb() {
        return *(RGB888 *) (this);
    }

    inline void set_rgb(RGB888 rgb) {
        this->rgb = rgb;
    }

    [[nodiscard]] inline RGB565 to_rgb565() const {
        return {static_cast<uint8_t>((uint16_t) b / (255 / 31)),
                static_cast<uint8_t>((uint16_t) g / (255 / 63)),
                static_cast<uint8_t>((uint16_t) r / (255 / 31))};
    }
};

static inline uint8_t lerp(const uint8_t val0, const uint8_t val1, uint8_t val0_fac, uint8_t val1_fac) {
    return (val0 * val0_fac + val1 * val1_fac) / (val0_fac + val1_fac);
}

static inline RGBA8888
rgb_lerp(const RGBA8888 &color0, const RGBA8888 &color1, uint8_t color0_fac, uint8_t color1_fac) {
    return {lerp(color0.r, color1.r, color0_fac, color1_fac),
            lerp(color0.g, color1.g, color0_fac, color1_fac),
            lerp(color0.b, color1.b, color0_fac, color1_fac),
            lerp(color0.a, color1.a, color0_fac, color1_fac)
    };
}

static inline RGB888 rgb_lerp(const RGB888 &color0, const RGB888 &color1, uint8_t color0_fac, uint8_t color1_fac) {
    return {lerp(color0.r, color1.r, color0_fac, color1_fac),
            lerp(color0.g, color1.g, color0_fac, color1_fac),
            lerp(color0.b, color1.b, color0_fac, color1_fac)};
}

