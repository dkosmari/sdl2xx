/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <SDL_events.h>         // SDL_ENABLE, SDL_DISABLE

#include "mouse.hpp"

#include "surface.hpp"
#include "window.hpp"


using std::expected;
using std::unexpected;


namespace sdl::mouse {

    window*
    get_focus()
        noexcept
    {
        return window::get_wrapper(SDL_GetMouseFocus());
    }


    state_t
    get_state()
        noexcept
    {
        state_t result;
        result.buttons = SDL_GetMouseState(&result.pos.x, &result.pos.y);
        return result;
    }


#if SDL_VERSION_ATLEAST(2, 0, 4)

    state_t
    get_global_state()
        noexcept
    {
        state_t result;
        result.buttons = SDL_GetGlobalMouseState(&result.pos.x, &result.pos.y);
        return result;
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 4)


    state_t
    get_relative_state()
        noexcept
    {
        state_t result;
        result.buttons = SDL_GetRelativeMouseState(&result.pos.x, &result.pos.y);
        return result;
    }


    void
    warp(int x,
         int y)
        noexcept
    {
        SDL_WarpMouseInWindow(nullptr, x, y);
    }


    void
    warp(vec2 pos)
        noexcept
    {
        warp(pos.x, pos.y);
    }


#if SDL_VERSION_ATLEAST(2, 0, 4)

    void
    warp_global(int x,
                int y)
    {
        auto result = try_warp_global(x, y);
        if (!result)
            throw error{};
    }


    void
    warp_global(vec2 pos)
    {
        warp_global(pos.x, pos.y);
    }


    expected<void, error>
    try_warp_global(int x,
                    int y)
        noexcept
    {
        if (SDL_WarpMouseGlobal(x, y) < 0)
            return unexpected{error{}};
        return {};
    }


    expected<void, error>
    try_warp_global(vec2 pos)
        noexcept
    {
        return try_warp_global(pos.x, pos.y);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 4)


    void
    set_relative_mode(bool enabled)
    {
        auto result = try_set_relative_mode(enabled);
        if (!result)
            throw result.error();
    }


    expected<void, error>
    try_set_relative_mode(bool enabled)
        noexcept
    {
        if (SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE) < 0)
            return unexpected{error{}};
        return {};
    }


    bool
    get_relative_mode()
        noexcept
    {
        return SDL_GetRelativeMouseMode();
    }


#if SDL_VERSION_ATLEAST(2, 0, 4)

    void
    capture_mouse(bool enabled)
    {
        auto result = try_capture_mouse(enabled);
        if (!result)
            throw error();
    }


    expected<void, error>
    try_capture_mouse(bool enabled)
        noexcept
    {
        if (SDL_CaptureMouse(enabled ? SDL_TRUE : SDL_FALSE) < 0)
            return unexpected{error{}};
        return {};
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 4)


    cursor::cursor(const Uint8* data,
                   const Uint8* mask,
                   int w,
                   int h,
                   int hot_x,
                   int hot_y)
    {
        create(data, mask, w, h, hot_x, hot_y);
    }


    cursor::cursor(const Uint8* data,
                   const Uint8* mask,
                   vec2 size,
                   vec2 hot)
    {
        create(data, mask, size, hot);
    }



    cursor::cursor(const surface& surf,
                   int hot_x,
                   int hot_y)
    {
        create(surf, hot_x, hot_y);
    }


    cursor::cursor(const surface& surf,
                   vec2 hot)
    {
        create(surf, hot);
    }


    cursor::cursor(system_cursor c)
    {
        create(c);
    }


    cursor::~cursor()
        noexcept
    {
        destroy();
    }


    void
    cursor::create(const Uint8* data,
                   const Uint8* mask,
                   int w,
                   int h,
                   int hot_x,
                   int hot_y)
    {
        auto ptr = SDL_CreateCursor(data, mask, w, h,  hot_x, hot_y);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    cursor::create(const Uint8* data,
                   const Uint8* mask,
                   vec2 size,
                   vec2 hot)
    {
        create(data, mask, size.x, size.y, hot.x, hot.y);
    }


    void
    cursor::create(const surface& surf,
                   int hot_x,
                   int hot_y)
    {
        auto ptr = SDL_CreateColorCursor(const_cast<SDL_Surface*>(surf.data()),
                                         hot_x,
                                         hot_y);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    cursor::create(const surface& surf,
                   vec2 hot)
    {
        create(surf, hot.x, hot.y);
    }


    void
    cursor::create(system_cursor c)
    {
        auto ptr = SDL_CreateSystemCursor(convert(c));
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    cursor::destroy()
        noexcept
    {
        if (is_valid())
            SDL_FreeCursor(release());
    }


    void
    cursor::activate()
        noexcept
    {
        SDL_SetCursor(raw);
    }


    void
    redraw()
        noexcept
    {
        SDL_SetCursor(nullptr);
    }


    bool
    show(bool enable)
    {
        auto result = try_show(enable);
        if (!result)
            throw result.error();
        return *result;
    }


    expected<bool, error>
    try_show(bool enable)
        noexcept
    {
        int result = SDL_ShowCursor(enable ? SDL_ENABLE : SDL_DISABLE);
        if (result < 0)
            return unexpected{error{}};
        return result == SDL_ENABLE;
    }


    bool
    is_shown()
    {
        auto result = try_is_shown();
        if (!result)
            throw result.error();
        return *result;
    }


    expected<bool, error>
    try_is_shown()
        noexcept
    {
        int result = SDL_ShowCursor(SDL_QUERY);
        if (result < 0)
            return unexpected{error{}};
        return result == SDL_ENABLE;
    }

} // namespace sdl::mouse
