/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025-2026  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include "sdl2xx/blob.hpp"

namespace sdl {

    blob::blob(std::size_t size_) :
        ptr{size_ ? make_unique<std::byte[]>(size_) : storage_type{}}
    {}


    blob::blob(std::byte* data_,
               std::size_t size_)
        noexcept :
        ptr{data_, size_}
    {}


    bool
    blob::empty()
        const noexcept
    {
        return !ptr;
    }


    std::size_t
    blob::size()
        const noexcept
    {
        return ptr ? ptr.get_deleter().size : 0;
    }


    std::byte*
    blob::data()
        noexcept
    {
        if (!ptr)
            return nullptr;
        return ptr.get();
    }


    const std::byte*
    blob::data()
        const noexcept
    {
        if (!ptr)
            return nullptr;
        return ptr.get();
    }


    blob::operator std::span<std::byte>()
        noexcept
    {
        if (!ptr)
            return {};
        return {data(), size()};
    }


    blob::operator std::span<const std::byte>()
        const noexcept
    {
        if (!ptr)
            return {};
        return {data(), size()};
    }

} // namespace sdl
