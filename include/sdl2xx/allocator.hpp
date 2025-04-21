/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_ALLOCATOR_HPP
#define SDL2XX_ALLOCATOR_HPP

#include <cstdlib>
#include <type_traits>


/**
 * A custom allocator to use with C++ standard library containers.
 *
 * All allocations are directed to SDL_malloc() and SDL_free().
 */
namespace sdl {

    namespace malloc_allocator {

        /// Allocate memory using SDL_malloc()
        void*
        allocate(std::size_t size);


        template<typename T>
        T
        allocate_as(std::size_t size)
        {
            return reinterpret_cast<T>(allocate(size));
        }


        /// Free memory using SDL_free()
        void
        deallocate(void* ptr)
            noexcept;

    } // namespace malloc_allocator


    template <typename T>
    struct allocator {

        using value_type      = T;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;

        using propagate_on_container_move_assignment = std::true_type;


        T*
        allocate(std::size_t count)
        {
            return malloc_allocator::allocate_as<T*>(count * sizeof(T));
        }


        void
        deallocate(T* ptr, std::size_t /*count*/)
        {
            malloc_allocator::deallocate(ptr);
        }


        constexpr bool
        operator==(const allocator&) noexcept
        {
            return true;
        }

    };

} // namespace sdl


#endif
