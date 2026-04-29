/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025-2026  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_ERROR_HPP
#define SDL2XX_ERROR_HPP

#include <stdexcept>
#include <string>


namespace sdl {

    struct error : std::runtime_error {

        error();

        error(const char* msg);

        error(const std::string& msg);

        explicit
        error(const std::exception& e);

    }; // struct error

} // namespace sdl

#endif
