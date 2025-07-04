/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_BLOB_HPP
#define SDL2XX_BLOB_HPP

#include <cstddef>
#include <span>

#include <SDL_types.h>

#include "unique_ptr.hpp"


namespace sdl {

    struct blob {

        unique_ptr<Uint8[]> ptr;


        constexpr
        blob()
            noexcept = default;

        explicit
        blob(std::size_t size);

        blob(Uint8* raw_data,
             std::size_t size)
            noexcept;


        bool
        empty()
            const noexcept;


        std::span<Uint8>
        data()
            noexcept;

        std::span<const Uint8>
        data()
            const noexcept;


        std::span<std::byte>
        bytes()
            noexcept;

        std::span<const std::byte>
        bytes()
            const noexcept;


    };

} // namespace sdl

#endif
