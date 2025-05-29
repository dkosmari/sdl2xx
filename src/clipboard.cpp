/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <utility>

#include "clipboard.hpp"
#include "unique_ptr.hpp"


using std::expected;
using std::unexpected;


namespace sdl::clipboard {

    void
    set_text(const char* text)
    {
        auto result = try_set_text(text);
        if (!result)
            throw result.error();
    }


    expected<void, error>
    try_set_text(const char* text)
        noexcept
    {
        if (SDL_SetClipboardText(text) < 0)
            return unexpected{error{}};
        return {};
    }


    string
    get_text()
    {
        auto result = try_get_text();
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<string, error>
    try_get_text()
        noexcept
    {
        try {
            unique_ptr<char> text{SDL_GetClipboardText()};
            if (!text)
                return unexpected{error{}};
            string result = text.get();
            if (result.empty())
                return unexpected{error{}};
            return result;
        }
        catch (std::exception& e) {
            return unexpected{error{e}};
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
        auto result = try_set_primary_selection(text);
        if (!result)
            throw result.error();
    }


    expected<void, error>
    try_set_primary_selection(const char* text)
        noexcept
    {
        if (SDL_SetPrimarySelectionText(text) < 0)
            return unexpected{error{}};
        return {};
    }


    string
    get_primary_selection()
    {
        auto result = try_get_primary_selection();
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<string, error>
    try_get_primary_selection()
        noexcept
    {
        try {
            unique_ptr<char> text{SDL_GetPrimarySelectionText()};
            if (!text)
                return unexpected{error{}};
            string result = text.get();
            if (result.empty())
                return unexpected{error{}};
            return result;
        }
        catch (std::exception& e){
            return unexpected{error{e}};
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
