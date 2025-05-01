/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <SDL2/SDL_error.h>

#include "error.hpp"


using namespace std::string_literals;

using std::runtime_error;


namespace sdl {

    error::error() :
        runtime_error{SDL_GetError()}
    {
        SDL_ClearError();
    }


    error::error(const char* msg) :
        runtime_error{msg}
    {}


    error::error(const std::exception& e) :
        runtime_error{e.what()}
    {}

} // namespace sdl
