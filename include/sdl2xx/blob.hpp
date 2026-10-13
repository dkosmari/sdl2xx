/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025-2026  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_BLOB_HPP
#define SDL2XX_BLOB_HPP

#include <concepts>
#include <cstddef>
#include <span>
#include <type_traits>

#include <SDL_types.h>

#include "unique_ptr.hpp"


namespace sdl {

    /**
     * Manage a blob of bytes using SDL_free().
     */

    class blob {

        using storage_type = unique_ptr<std::byte[]>;

        storage_type ptr;


    public:

        constexpr
        blob()
            noexcept = default;


        explicit
        blob(std::size_t size_);


        blob(std::byte* data_,
             std::size_t size_)
            noexcept;


        template<std::size_t E>
        explicit
        blob(std::span<std::byte, E> src)
            noexcept :
            blob{src.data(), src.size()}
        {}


        template<typename T,
                 std::size_t E>
        requires (!std::same_as<std::remove_volatile_t<T>, std::byte>)
        explicit
        blob(std::span<T, E> src)
            noexcept :
            blob{as_writable_bytes(src)}
        {}


        bool
        empty()
            const noexcept;


        std::size_t
        size()
            const noexcept;


        std::byte*
        data()
            noexcept;

        const std::byte*
        data()
            const noexcept;


        template<typename T>
        T*
        data_as()
            noexcept
        {
            return reinterpret_cast<T*>(data());
        }


        template<typename T>
        const T*
        data_as()
            const noexcept
        {
            return reinterpret_cast<const T*>(data());
        }


        explicit
        operator std::span<std::byte>()
            noexcept;


        explicit
        operator std::span<const std::byte>()
            const noexcept;

    }; // class blob

} // namespace sdl

#endif
