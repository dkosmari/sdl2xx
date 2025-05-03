/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_IMPL_UTILS_HPP
#define SDL2XX_IMPL_UTILS_HPP

#include <SDL_types.h>


namespace sdl::impl::utils {

    [[nodiscard]]
    unsigned
    map_to_uint(float x,
                unsigned max)
        noexcept;


    [[nodiscard]]
    Uint8
    map_to_uint8(float x)
        noexcept;


    [[nodiscard]]
    Uint16
    map_to_uint16(float x)
        noexcept;


    [[nodiscard]]
    int
    map_to_int(double x,
               int min,
               int max)
        noexcept;


    [[nodiscard]]
    Sint16
    map_to_int16(double x)
        noexcept;


    [[nodiscard]]
    double
    map_to_double(int x,
                  int min,
                  int max)
        noexcept;


    [[nodiscard]]
    double
    map_to_double(Sint16 x)
        noexcept;

} // namespace sdl::impl::utils

#endif
