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
#include <string>

namespace sdl {

    struct error : std::runtime_error {

#if 0
        std::string sdl_msg;
#endif

        error();

#if 0
        error(const char* prefix);
        error(const std::string& prefix);
#endif

    };

} // namespace sdl

#endif
