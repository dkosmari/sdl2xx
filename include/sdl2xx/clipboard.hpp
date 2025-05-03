/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_CLIPBOARD_HPP
#define SDL2XX_CLIPBOARD_HPP

#include <expected>

#include <SDL_clipboard.h>
#include <SDL_version.h>

#include "error.hpp"
#include "string.hpp"


namespace sdl::clipboard {

    void
    set_text(const char* text);

    inline
    void
    set_text(const concepts::string auto& text)
    {
        set_text(text.data());
    }

    std::expected<void, error>
    try_set_text(const char* text)
        noexcept;

    inline
    std::expected<void, error>
    try_set_text(const concepts::string auto& text)
        noexcept
    {
        return try_set_text(text.data());
    }


    [[nodiscard]]
    string
    get_text();


    [[nodiscard]]
    std::expected<string, error>
    try_get_text()
        noexcept;


    [[nodiscard]]
    bool
    has_text()
        noexcept;


#if SDL_VERSION_ATLEAST(2, 26, 0)

    void
    set_primary_selection(const char* text);

    inline
    void
    set_primary_selection(const concepts::string auto& text)
    {
        set_primary_text(text.data());
    }

    std::expected<void, error>
    try_set_primary_selection(const char* text)
        noexcept;

    inline
    std::expected<void, error>
    try_set_primary_selection(const concepts::string auto& text)
        noexcept
    {
        return try_set_primary_selection(text.data());
    }


    [[nodiscard]]
    string
    get_primary_selection();

    std::expected<string, error>
    try_get_primary_selection()
        noexcept;


    [[nodiscard]]
    bool
    has_primary_selection()
        noexcept;


#endif // SDL_VERSION_ATLEAST(2, 26, 0)



} // namespace sdl::clipboard

#endif
