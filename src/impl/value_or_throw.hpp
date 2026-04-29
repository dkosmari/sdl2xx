/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2026  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef VALUE_OR_THROW_HPP
#define VALUE_OR_THROW_HPP

#include <expected>
#include <type_traits>
#include <utility>


namespace sdl::impl {

    template<typename T,
             typename E>
    T&
    ref_or_throw(std::expected<T, E>& e)
    {
        if (!e)
            throw e.error();
        return *e;
    }


    template<typename T,
             typename E>
    const T&
    ref_or_throw(const std::expected<T, E>& e)
    {
        if (!e)
            throw e.error();
        return *e;
    }


    template<typename T,
             typename E>
    T
    value_or_throw(std::expected<T, E>&& e)
    {
        if (!e)
            throw e.error();
        if constexpr (!std::is_same_v<T, void>)
            return std::move(*e);
    }


    template<typename T,
             typename E>
    T
    value_or_throw(const std::expected<T, E>& e)
    {
        if (!e)
            throw e.error();
        if constexpr (!std::is_same_v<T, void>)
            return *e;
    }

} // namespace sdl::impl

#endif
