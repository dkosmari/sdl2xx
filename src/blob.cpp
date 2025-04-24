/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include "blob.hpp"

namespace sdl {

    blob::blob(Uint8* raw_data,
               std::size_t size)
        noexcept :
        ptr{raw_data, size}
    {}


    bool
    blob::empty()
        const noexcept
    {
        return !ptr;
    }


    std::span<Uint8>
    blob::data()
        noexcept
    {
        if (!ptr)
            return {};
        return {ptr.get(), ptr.get_deleter().size};
    }


    std::span<const Uint8>
    blob::data()
        const noexcept
    {
        if (!ptr)
            return {};
        return {ptr.get(), ptr.get_deleter().size};
    }

} // namespace sdl
