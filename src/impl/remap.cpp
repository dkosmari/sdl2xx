/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025-2026  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <cmath>

#include "remap.hpp"


namespace sdl::impl::remap {

    unsigned
    to_uint(float x,
            unsigned max)
        noexcept
    {
        if (x <= 0)
            return 0;
        unsigned scaled = std::floor(x * (max + 1.0f));
        if (scaled > max)
            return max;
        return scaled;
    }


    Uint8
    to_uint8(float x)
        noexcept
    {
        return to_uint(x, std::numeric_limits<Uint8>::max());
    }


    Uint16
    to_uint16(float x)
        noexcept
    {
        return to_uint(x, std::numeric_limits<Uint16>::max());
    }


    int
    to_int(double x,
           int min,
           int max)
        noexcept
    {
        double sx = x * (x < 0 ? min - 1 : max + 1);
        int scaled = std::floor(sx);
        if (x < 0) {
            if (-scaled < min)
                return min;
            return -scaled;
        } else {
            if (scaled > max)
                return max;
            return scaled;
        }
    }


    double
    to_norm(int x,
            int min,
            int max)
        noexcept
    {
        if (x < 0)
            return x / -static_cast<double>(min);
        else
            return x / static_cast<double>(max);
    }

} // namespace sdl::impl::remap
