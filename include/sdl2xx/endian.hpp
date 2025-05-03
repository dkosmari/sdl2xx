/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_ENDIAN_HPP
#define SDL2XX_ENDIAN_HPP

#include <SDL_endian.h>


namespace sdl::endian {


    [[nodiscard]]
    constexpr
    Sint8
    swap(Sint8 x)
        noexcept
    {
        return x;
    }


    [[nodiscard]]
    constexpr
    Uint8
    swap(Uint8 x)
        noexcept
    {
        return x;
    }


    [[nodiscard]]
    constexpr
    Sint16
    swap(Sint16 x)
        noexcept
    {
        return SDL_Swap16(x);
    }


    [[nodiscard]]
    constexpr
    Uint16
    swap(Uint16 x)
        noexcept
    {
        return SDL_Swap16(x);
    }


    [[nodiscard]]
    constexpr
    Sint32
    swap(Sint32 x)
        noexcept
    {
        return SDL_Swap32(x);
    }


    [[nodiscard]]
    constexpr
    Uint32
    swap(Uint32 x)
        noexcept
    {
        return SDL_Swap32(x);
    }


    [[nodiscard]]
    constexpr
    Sint64
    swap(Sint64 x)
        noexcept
    {
        return SDL_Swap64(x);
    }


    [[nodiscard]]
    constexpr
    Uint64
    swap(Uint64 x)
        noexcept
    {
        return SDL_Swap64(x);
    }


    [[nodiscard]]
    inline
    float
    swap(float x)
        noexcept
    {
        return SDL_SwapFloat(x);
    }


    // Loading data.


    [[nodiscard]]
    constexpr
    Sint8
    from_be(Sint8 x)
        noexcept
    {
        return x;
    }


    [[nodiscard]]
    constexpr
    Uint8
    from_be(Uint8 x)
        noexcept
    {
        return x;
    }


    [[nodiscard]]
    constexpr
    Sint16
    from_be(Sint16 x)
        noexcept
    {
        return SDL_SwapBE16(x);
    }


    [[nodiscard]]
    constexpr
    Uint16
    from_be(Uint16 x)
        noexcept
    {
        return SDL_SwapBE16(x);
    }


    [[nodiscard]]
    constexpr
    Sint32
    from_be(Sint32 x)
        noexcept
    {
        return SDL_SwapBE32(x);
    }


    [[nodiscard]]
    constexpr
    Uint32
    from_be(Uint32 x)
        noexcept
    {
        return SDL_SwapBE32(x);
    }


    [[nodiscard]]
    constexpr
    Sint64
    from_be(Sint64 x)
        noexcept
    {
        return SDL_SwapBE64(x);
    }


    [[nodiscard]]
    constexpr
    Uint64
    from_be(Uint64 x)
        noexcept
    {
        return SDL_SwapBE64(x);
    }


    [[nodiscard]]
    inline
    float
    from_be(float x)
        noexcept
    {
        return SDL_SwapFloatBE(x);
    }


    [[nodiscard]]
    constexpr
    Sint8
    from_le(Sint8 x)
        noexcept
    {
        return x;
    }


    [[nodiscard]]
    constexpr
    Uint8
    from_le(Uint8 x)
        noexcept
    {
        return x;
    }


    [[nodiscard]]
    constexpr
    Sint16
    from_le(Sint16 x)
        noexcept
    {
        return SDL_SwapLE16(x);
    }


    [[nodiscard]]
    constexpr
    Uint16
    from_le(Uint16 x)
        noexcept
    {
        return SDL_SwapLE16(x);
    }


    [[nodiscard]]
    constexpr
    Sint32
    from_le(Sint32 x)
        noexcept
    {
        return SDL_SwapLE32(x);
    }


    [[nodiscard]]
    constexpr
    Uint32
    from_le(Uint32 x)
        noexcept
    {
        return SDL_SwapLE32(x);
    }


    [[nodiscard]]
    constexpr
    Sint64
    from_le(Sint64 x)
        noexcept
    {
        return SDL_SwapLE64(x);
    }


    [[nodiscard]]
    constexpr
    Uint64
    from_le(Uint64 x)
        noexcept
    {
        return SDL_SwapLE64(x);
    }


    [[nodiscard]]
    inline
    float
    from_le(float x)
        noexcept
    {
        return SDL_SwapFloatLE(x);
    }


    // Storing data.


    [[nodiscard]]
    constexpr
    Sint8
    to_be(Sint8 x)
        noexcept
    {
        return x;
    }


    [[nodiscard]]
    constexpr
    Uint8
    to_be(Uint8 x)
        noexcept
    {
        return x;
    }


    [[nodiscard]]
    constexpr
    Sint16
    to_be(Sint16 x)
        noexcept
    {
        return SDL_SwapBE16(x);
    }


    [[nodiscard]]
    constexpr
    Uint16
    to_be(Uint16 x)
        noexcept
    {
        return SDL_SwapBE16(x);
    }


    [[nodiscard]]
    constexpr
    Sint32
    to_be(Sint32 x)
        noexcept
    {
        return SDL_SwapBE32(x);
    }


    [[nodiscard]]
    constexpr
    Uint32
    to_be(Uint32 x)
        noexcept
    {
        return SDL_SwapBE32(x);
    }


    [[nodiscard]]
    constexpr
    Sint64
    to_be(Sint64 x)
        noexcept
    {
        return SDL_SwapBE64(x);
    }


    [[nodiscard]]
    constexpr
    Uint64
    to_be(Uint64 x)
        noexcept
    {
        return SDL_SwapBE64(x);
    }


    [[nodiscard]]
    inline
    float
    to_be(float x)
        noexcept
    {
        return SDL_SwapFloatBE(x);
    }


    [[nodiscard]]
    constexpr
    Sint8
    to_le(Sint8 x)
        noexcept
    {
        return x;
    }


    [[nodiscard]]
    constexpr
    Uint8
    to_le(Uint8 x)
        noexcept
    {
        return x;
    }


    [[nodiscard]]
    constexpr
    Sint16
    to_le(Sint16 x)
        noexcept
    {
        return SDL_SwapLE16(x);
    }


    [[nodiscard]]
    constexpr
    Uint16
    to_le(Uint16 x)
        noexcept
    {
        return SDL_SwapLE16(x);
    }


    [[nodiscard]]
    constexpr
    Sint32
    to_le(Sint32 x)
        noexcept
    {
        return SDL_SwapLE32(x);
    }


    [[nodiscard]]
    constexpr
    Uint32
    to_le(Uint32 x)
        noexcept
    {
        return SDL_SwapLE32(x);
    }


    [[nodiscard]]
    constexpr
    Sint64
    to_le(Sint64 x)
        noexcept
    {
        return SDL_SwapLE64(x);
    }


    [[nodiscard]]
    constexpr
    Uint64
    to_le(Uint64 x)
        noexcept
    {
        return SDL_SwapLE64(x);
    }


    [[nodiscard]]
    inline
    float
    to_le(float x)
        noexcept
    {
        return SDL_SwapFloatLE(x);
    }

} // namespace sdl::endian

#endif
