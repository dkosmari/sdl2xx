/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <algorithm>
#include <cmath>
#include <ostream>

#include <SDL2/SDL_stdinc.h>

#include "color.hpp"


namespace sdl {

    namespace {

        constexpr
        Uint8
        float_to_byte(float f)
            noexcept
        {
            float sf = std::floor(f * 256.0f);
            return std::clamp(int(sf), 0, 255);
        }

    } // namespace


    color
    color::from_rgb(float r,
                    float g,
                    float b)
        noexcept
    {
        return {
            float_to_byte(r),
            float_to_byte(g),
            float_to_byte(b),
            0xff
        };
    }


    color
    color::from_rgba(float r,
                     float g,
                     float b,
                     float a)
        noexcept
    {
        return {
            float_to_byte(r),
            float_to_byte(g),
            float_to_byte(b),
            float_to_byte(a)
        };
    }


    namespace {

        color
        from_rgb_bias(float r,
                      float g,
                      float b,
                      float m)
            noexcept
        {
            return color::from_rgb(r + m, g + m, b + m);
        }

    } // namespace


    color
    color::from_hsl(degreesf h,
                    float s,
                    float l)
        noexcept
    {
        h = wrap_positive(h);
        s = std::clamp(s, 0.0f, 1.0f);
        l = std::clamp(l, 0.0f, 1.0f);
        // C = (1 - |2L - 1|) × S
        float c = (1.0f - std::abs(2.0f * l - 1.0f)) * s;
        // X = C × (1 - |(H / 60°) mod 2 - 1|)
        float x = c * (1.0f - std::abs(std::fmod(h.value() / 60.0f, 2.0f) - 1.0f));
        // m = L - C/2
        float m = l - c / 2.0f;
        if (h < 60_degf)
            return from_rgb_bias(c, x, 0, m);
        if (h < 120_degf)
            return from_rgb_bias(x, c, 0, m);
        if (h < 180_degf)
            return from_rgb_bias(0, c, x, m);
        if (h < 240_degf)
            return from_rgb_bias(0, x, c, m);
        if (h < 300_degf)
            return from_rgb_bias(x, 0, c, m);
        return from_rgb_bias(c, 0, x, m);
    }


    color
    color::from_hsv(degreesf h,
                    float s,
                    float v)
        noexcept
    {
        h = wrap_positive(h);
        s = std::clamp(s, 0.0f, 1.0f);
        v = std::clamp(v, 0.0f, 1.0f);
        // C = V × S
        float c = v * s;
        // X = C × (1 - |(H / 60°) mod 2 - 1|)
        float x = c * (1.0f - std::abs(std::fmod(h.value() / 60.0f, 2.0f) - 1.0f));
        // m = V - C
        float m = v - c;
        if (h < 60_degf)
            return from_rgb_bias(c, x, 0, m);
        if (h < 120_degf)
            return from_rgb_bias(x, c, 0, m);
        if (h < 180_degf)
            return from_rgb_bias(0, c, x, m);
        if (h < 240_degf)
            return from_rgb_bias(0, x, c, m);
        if (h < 300_degf)
            return from_rgb_bias(x, 0, c, m);
        return from_rgb_bias(c, 0, x, m);
    }


    namespace {

        constexpr
        float
        calc_hue(float r,
                 float g,
                 float b,
                 float delta,
                 float v)
            noexcept
        {
            if (delta == 0.0f)
                return 0.0f;
            if (v == r)
                return 60.0f * std::fmod(((g - b) / delta), 6.0f);
            if (v == g)
                return 60.0f * ((b - r) / delta + 2.0f);
            return 60.0f * ((r - g) / delta + 4.0f);
        }


        constexpr
        float
        hsl_saturation(float delta,
                       float l)
            noexcept
        {
            if (delta == 0.0f)
                return 0.0f;
            return delta / (1.0f - std::abs(2.0f * l - 1.0f));
        }


        constexpr
        float
        hsv_saturation(float delta,
                       float v)
            noexcept
        {
            if (v == 0.0f)
                return 0.0f;
            return delta / v;
        }

    } // namespace


    hsl
    color::to_hsl()
        const noexcept
    {
        auto [r_, g_, b_] = to_rgb();
        auto [c_min, c_max] = std::minmax({r_, g_, b_});
        float delta = c_max - c_min;
        float l = (c_min + c_max) / 2.0f;
        float v = c_max;
        float s = hsl_saturation(delta, l);
        float h = calc_hue(r_, g_, b_, delta, v);
        return {degreesf{h}, s, l};
    }


    hsv
    color::to_hsv()
        const noexcept
    {
        auto [r_, g_, b_] = to_rgb();
        auto [c_min, c_max] = std::minmax({r_, g_, b_});
        float delta = c_max - c_min;
        float v = c_max;
        float s = hsv_saturation(delta, v);
        float h = calc_hue(r_, g_, b_, delta, v);
        return {degreesf{h}, s, v};
    }


    string
    to_string(const hsl& c)
    {
        char buf[64];
        SDL_snprintf(buf, sizeof buf,
                     "hsl(%.0f°, %.1f%%, %.1f%%)",
                     c.h.value(),
                     100 * c.s,
                     100 * c.l);
        return buf;
    }


    string
    to_string(const hsv& c)
    {
        char buf[64];
        SDL_snprintf(buf, sizeof buf,
                     "hsv(%.0f°, %.1f%%, %.1f%%)",
                     c.h.value(),
                     100 * c.s,
                     100 * c.v);
        return buf;
    }


    string
    to_string(color c)
    {
        char buf[1 + 8 + 1];
        if (c.a == 0xff)
            SDL_snprintf(buf, sizeof buf,
                         "#%02x%02x%02x",
                         c.r, c.g, c.b);
        else
            SDL_snprintf(buf, sizeof buf,
                         "#%02x%02x%02x%02x",
                         c.r, c.g, c.b, c.a);
        return buf;
    }


    std::ostream&
    operator <<(std::ostream& out,
                const hsl& c)
    {
        return out << to_string(c);
    }


    std::ostream&
    operator <<(std::ostream& out,
                const hsv& c)
    {
        return out << to_string(c);
    }


    std::ostream&
    operator <<(std::ostream& out,
                color c)
    {
        return out << to_string(c);
    }

} // namespace sdl
