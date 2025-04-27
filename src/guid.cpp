/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <algorithm>
#include <istream>
#include <ostream>

#include "guid.hpp"


namespace sdl {

    guid::guid(const char* str)
        noexcept :
        SDL_GUID{SDL_GUIDFromString(str)}
    {

    }


    guid::operator bool()
        const noexcept
    {
        for (auto b : data)
            if (b)
                return true;
        return false;
    }


    bool
    guid::operator ==(const guid& other)
        const noexcept
    {
        return std::ranges::equal(data, other.data);
    }


    std::strong_ordering
    guid::operator <=>(const guid& other)
        const noexcept
    {
        return std::lexicographical_compare_three_way(std::begin(data),
                                                      std::end(data),
                                                      std::begin(other.data),
                                                      std::end(other.data));
    }


    std::istream&
    operator >>(std::istream& in,
                guid& id)
    {
        string token;
        in >> token;
        id = guid{token};
        return in;
    }


    string
    to_string(const guid& id)
    {
        char buf[32 + 1];
        SDL_GUIDToString(id, buf, sizeof buf);
        return buf;
    }


    std::ostream&
    operator <<(std::ostream& out,
                const guid& id)
    {
        return out << to_string(id);
    }

} // namespace sdl
