/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <cstdio>
#include <ostream>

#include "angle.hpp"


namespace sdl {

    string
    to_string(degrees d)
    {
        char buf[32];
        std::snprintf(buf, sizeof buf,
                      "%f°",
                      d.value());
        return buf;
    }


    string
    to_string(degreesf d)
    {
        char buf[32];
        std::snprintf(buf, sizeof buf,
                      "%f°",
                      d.value());
        return buf;
    }


    string
    to_string(radians d)
    {
        char buf[32];
        std::snprintf(buf, sizeof buf,
                      "%f",
                      d.value());
        return buf;
    }


    string
    to_string(radiansf d)
    {
        char buf[32];
        std::snprintf(buf, sizeof buf,
                      "%f",
                      d.value());
        return buf;
    }


    std::ostream&
    operator <<(std::ostream& out,
                degrees d)
    {
        return out << d.value() << "°";
    }


    std::ostream&
    operator <<(std::ostream& out,
                degreesf d)
    {
        return out << d.value() << "°";
    }


    std::ostream&
    operator <<(std::ostream& out,
                radians r)
    {
        return out << r.value();
    }


    std::ostream&
    operator <<(std::ostream& out,
                radiansf r)
    {
        return out << r.value();
    }

} // namespace sdl
