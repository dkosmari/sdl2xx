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
    {}


#if 0
    error::error(const char* prefix) :
        runtime_error{prefix + ": "s + SDL_GetError()},
        sdl_msg{SDL_GetError()}
    {}


    error::error(const std::string prefix) :
        runtime_error{prefix + ": "s + SDL_GetError()},
        sdl_msg{SDL_GetError()}
    {}
#endif

} // namespace sdl
