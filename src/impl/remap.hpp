/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025-2026  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_IMPL_REMAP_HPP
#define SDL2XX_IMPL_REMAP_HPP

#include <concepts>
#include <limits>

#include <SDL_types.h>


namespace sdl::impl::remap {

    [[nodiscard]]
    unsigned
    to_uint(float x,
            unsigned max)
        noexcept;


    [[nodiscard]]
    Uint8
    to_uint8(float x)
        noexcept;


    [[nodiscard]]
    Uint16
    to_uint16(float x)
        noexcept;


    [[nodiscard]]
    int
    to_int(double x,
           int min,
           int max)
        noexcept;


    [[nodiscard]]
    Sint16
    to_int16(double x)
        noexcept;


    [[nodiscard]]
    double
    to_norm(int x,
            int min,
            int max)
        noexcept;


    template<std::signed_integral I>
    [[nodiscard]]
    double
    to_norm(I x)
        noexcept
    {
        return to_norm(x,
                       std::numeric_limits<I>::min(),
                       std::numeric_limits<I>::max());
    }

} // namespace sdl::impl::remap

#endif
