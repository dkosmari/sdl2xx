/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <new>

#include <SDL_stdinc.h>

#include "allocator.hpp"


namespace sdl::malloc_allocator {

    void*
    allocate(std::size_t size)
    {
        void* ptr = SDL_malloc(size);
        if (!ptr)
            throw std::bad_alloc{};
        return ptr;
    }


    void
    deallocate(void* ptr)
        noexcept
    {
        SDL_free(ptr);
    }

} // namespace sdl::malloc_allocator
