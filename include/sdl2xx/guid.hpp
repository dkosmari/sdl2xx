/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_GUID_HPP
#define SDL2XX_GUID_HPP

#include <compare>
#include <iosfwd>

#include <SDL_guid.h>

#include "string.hpp"


namespace sdl {

    struct guid : SDL_GUID {

        constexpr
        guid()
            noexcept :
            SDL_GUID{}
        {}


        constexpr
        guid(const SDL_GUID& id)
            noexcept :
            SDL_GUID{id}
        {}


        explicit
        guid(const char* str)
            noexcept;


        inline
        explicit
        guid(const concepts::string auto& str)
            noexcept :
            guid{str.data()}
        {}


        [[nodiscard]]
        explicit
        operator bool()
            const noexcept;


        [[nodiscard]]
        bool
        operator ==(const guid& other)
            const noexcept;


        [[nodiscard]]
        std::strong_ordering
        operator <=>(const guid& other)
            const noexcept;

    };


    std::istream&
    operator >>(std::istream& in,
                guid& id);


    [[nodiscard]]
    string
    to_string(const guid& id);


    std::ostream&
    operator <<(std::ostream& out,
                const guid& id);

} // namespace sdl

#endif
