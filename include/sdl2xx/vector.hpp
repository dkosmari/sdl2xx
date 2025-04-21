/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_VECTOR_HPP
#define SDL2XX_VECTOR_HPP

#include <vector>

#include "allocator.hpp"

namespace sdl {

#ifdef SDL2XX_USE_ALLOCATOR

    template<typename T>
    using vector = std::vector<T, allocator<T>>;

#else

    using std::vector;

#endif

} // namespace sdl

#endif
