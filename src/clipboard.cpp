/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025-2026  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <utility>

#include "sdl2xx/clipboard.hpp"

#include "impl/value_or_throw.hpp"
#include "sdl2xx/unique_ptr.hpp"


using sdl::impl::value_or_throw;


namespace sdl::clipboard {

    void
    set_text(const char* text)
    {
        return value_or_throw(try_set_text(text));
    }


    std::expected<void, error>
    try_set_text(const char* text)
        noexcept
    {
        if (SDL_SetClipboardText(text) < 0)
            return std::unexpected{error{}};
        return {};
    }


    string
    get_text()
    {
        return value_or_throw(try_get_text());
    }


    std::expected<string, error>
    try_get_text()
        noexcept
    {
        try {
            unique_ptr<char> text{SDL_GetClipboardText()};
            if (!text)
                return std::unexpected{error{}};
            string result = text.get();
            if (result.empty())
                return std::unexpected{error{}};
            return result;
        }
        catch (std::exception& e) {
            return std::unexpected{error{e}};
        }
    }


    bool
    has_text()
        noexcept
    {
        return SDL_HasClipboardText();
    }


#if SDL_VERSION_ATLEAST(2, 26, 0)

    void
    set_primary_selection(const char* text)
    {
        return value_or_throw(try_set_primary_selection(text));
    }


    std::expected<void, error>
    try_set_primary_selection(const char* text)
        noexcept
    {
        if (SDL_SetPrimarySelectionText(text) < 0)
            return std::unexpected{error{}};
        return {};
    }


    string
    get_primary_selection()
    {
        return value_or_throw(try_get_primary_selection());
    }


    std::expected<string, error>
    try_get_primary_selection()
        noexcept
    {
        try {
            unique_ptr<char> text{SDL_GetPrimarySelectionText()};
            if (!text)
                return std::unexpected{error{}};
            string result = text.get();
            if (result.empty())
                return std::unexpected{error{}};
            return result;
        }
        catch (std::exception& e){
            return std::unexpected{error{e}};
        }
    }


    bool
    has_primary_selection()
        noexcept
    {
        return SDL_HasPrimarySelectionText();
    }

#endif // SDL_VERSION_ATLEAST(2, 26, 0)

} // namespace sdl::clipboard
