/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_ERROR_HPP
#define SDL2XX_ERROR_HPP

#include <stdexcept>

namespace sdl {

    struct error : std::runtime_error {

        error();

        explicit
        error(const std::exception& e);

    };

} // namespace sdl

#endif
