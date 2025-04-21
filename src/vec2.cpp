/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <cmath>
#include <ostream>

#include <SDL_stdinc.h>

#include "vec2.hpp"


namespace sdl {


    float
    length(vec2 v)
        noexcept
    {
        return std::hypot(v.x, v.y);
    }


    string
    to_string(vec2 v)
    {
        char buf[32];
        SDL_snprintf(buf, sizeof buf,
                     "(%d, %d)",
                     v.x, v.y);
        return buf;
    }


    std::ostream&
    operator <<(std::ostream& out,
                vec2 v)
    {
        return out << '('
                   << v.x
                   << ", "
                   << v.y
                   << ')';
    }


    float
    length(vec2f v)
        noexcept
    {
        return std::hypot(v.x, v.y);
    }


    vec2f
    normalized(vec2f v)
        noexcept
    {
        return v / length(v);
    }


    string
    to_string(vec2f v)
    {
        char buf[32];
        SDL_snprintf(buf, sizeof buf,
                     "(%f, %f)",
                     v.x, v.y);
        return buf;
    }


    std::ostream&
    operator <<(std::ostream& out,
                vec2f v)
    {
        return out << '('
                   << v.x
                   << ", "
                   << v.y
                   << ')';
    }

} // namespace sdl
