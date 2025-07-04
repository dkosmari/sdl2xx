/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_UNIQUE_PTR_HPP
#define SDL2XX_UNIQUE_PTR_HPP

#include <memory>
#include <type_traits>
#include <utility>

#include "allocator.hpp"


namespace sdl {

    template<typename T>
    struct deleter {

        void
        operator ()(T* ptr)
            const noexcept
        {
            if (ptr) {
                std::destroy_at(ptr);
                malloc_allocator::deallocate(ptr);
            }
        }

    };


    template<typename T>
    struct deleter<T[]> {

        std::size_t size = 0;

        constexpr
        deleter(std::size_t size)
            noexcept :
            size{size}
        {}


        void
        operator ()(T* ptr)
            noexcept
        {
            if (ptr) {
                while (size > 0)
                    std::destroy_at(ptr + --size);
                malloc_allocator::deallocate(ptr);
            }
        }

    };


    template<typename T>
    using unique_ptr = std::unique_ptr<T, deleter<T>>;


    template<typename T,
             typename... Args>
    requires(!std::is_array_v<T>)
    unique_ptr<T>
    make_unique(Args&&... args)
    {
        T* ptr = nullptr;
        try {
            ptr = malloc_allocator::allocate_as<T*>(sizeof(T));
            if (!ptr)
                return {};
            std::construct_at(ptr, std::forward<Args>(args)...);
            return unique_ptr<T>{ptr};
        }
        catch (...) {
            malloc_allocator::deallocate(ptr);
            throw;
        }
    }


    template<typename A>
    requires(std::is_unbounded_array_v<A>)
    unique_ptr<A>
    make_unique(std::size_t count)
    {
        // Avoid trying to do zero-sized allocations.
        if (!count)
            return {nullptr, 0};

        using T = std::remove_extent_t<A>;

        T* ptr = nullptr;
        std::size_t constructed = 0;
        try {
            ptr = malloc_allocator::allocate_as<T*>(count * sizeof(T));
            if (!ptr)
                return {nullptr, count};
            for (std::size_t i = 0; i < count; ++i) {
                std::construct_at(ptr + i);
                ++constructed;
            }
            return {ptr, count};
        }
        catch (...) {
            // Destroy every constructed object in reverse order.
            while (constructed > 0)
                std::destroy_at(ptr + --constructed);
            malloc_allocator::deallocate(ptr);
            throw;
        }
    }

} // namespace sdl

#endif
