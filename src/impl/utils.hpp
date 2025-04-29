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

    unsigned
    map_to_uint(float f,
                unsigned max)
        noexcept;


    Uint8
    map_to_uint8(float f)
        noexcept;

    Uint16
    map_to_uint16(float f)
        noexcept;

} // namespace sdl::impl::utils

#endif
