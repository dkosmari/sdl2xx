/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <cmath>

#include "utils.hpp"


namespace sdl::impl::utils {

    unsigned
    map_to_uint(float f,
                unsigned max)
        noexcept
    {
        if (f <= 0)
            return 0;
        unsigned scaled = std::floor(f * (max + 1.0f));
        if (scaled > max)
            return max;
        return scaled;
    }


    Uint8
    map_to_uint8(float f)
        noexcept
    {
        return map_to_uint(f, 255);
    }


    Uint16
    map_to_uint16(float f)
        noexcept
    {
        return map_to_uint(f, 0xffff);
    }

} // namespace sdl::impl::utils
