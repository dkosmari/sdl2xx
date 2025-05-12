/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_MOUSE_HPP
#define SDL2XX_MOUSE_HPP

#include <expected>

#include <SDL_mouse.h>
#include <SDL_version.h>

#include "basic_wrapper.hpp"
#include "error.hpp"
#include "vec2.hpp"


namespace sdl {

    class surface;

    class window;

} // namespace sdl


namespace sdl::mouse {

    enum class system_cursor {
        arrow      = SDL_SYSTEM_CURSOR_ARROW,
        crosshair  = SDL_SYSTEM_CURSOR_CROSSHAIR,
        hand       = SDL_SYSTEM_CURSOR_HAND,
        i_beam     = SDL_SYSTEM_CURSOR_IBEAM,
        max        = SDL_NUM_SYSTEM_CURSORS,
        no         = SDL_SYSTEM_CURSOR_NO,
        size_all   = SDL_SYSTEM_CURSOR_SIZEALL,
        size_ne_sw = SDL_SYSTEM_CURSOR_SIZENESW,
        size_ns    = SDL_SYSTEM_CURSOR_SIZENS,
        size_nw_se = SDL_SYSTEM_CURSOR_SIZENWSE,
        size_we    = SDL_SYSTEM_CURSOR_SIZEWE,
        wait       = SDL_SYSTEM_CURSOR_WAIT,
        wait_arrow = SDL_SYSTEM_CURSOR_WAITARROW,
    };


    [[nodiscard]]
    constexpr
    system_cursor
    convert(SDL_SystemCursor c)
        noexcept
    {
        return static_cast<system_cursor>(c);
    }

    [[nodiscard]]
    constexpr
    SDL_SystemCursor
    convert(system_cursor c)
        noexcept
    {
        return static_cast<SDL_SystemCursor>(c);
    }


    enum class wheel_dir {
        flipped = SDL_MOUSEWHEEL_FLIPPED,
        normal  = SDL_MOUSEWHEEL_NORMAL,
    };


    [[nodiscard]]
    constexpr
    wheel_dir
    convert(SDL_MouseWheelDirection dir)
        noexcept
    {
        return static_cast<wheel_dir>(dir);
    }


    [[nodiscard]]
    constexpr
    SDL_MouseWheelDirection
    convert(wheel_dir dir)
        noexcept
    {
        return static_cast<SDL_MouseWheelDirection>(dir);
    }


    [[nodiscard]]
    window*
    get_focus()
        noexcept;


    enum class button {
        left   = SDL_BUTTON_LEFT,
        middle = SDL_BUTTON_MIDDLE,
        right  = SDL_BUTTON_RIGHT,
        x1     = SDL_BUTTON_X1,
        x2     = SDL_BUTTON_X2,
    };


    struct state_t {

        Uint32 buttons;
        vec2 pos;


        [[nodiscard]]
        constexpr
        bool
        is_pressed(button b)
            noexcept
        {
            return buttons & SDL_BUTTON(static_cast<Uint32>(b));
        }


        [[nodiscard]]
        constexpr
        bool
        is_pressed(Uint32 b)
            noexcept
        {
            return buttons & SDL_BUTTON(b);
        }


        template<typename... Args>
        requires(sizeof...(Args) > 0)
        [[nodiscard]]
        constexpr
        bool
        any_pressed(Args... args)
            noexcept
        {
            Uint32 mask = (SDL_BUTTON(static_cast<Uint32>(args)) | ...);
            return buttons & mask;
        }


        template<typename... Args>
        requires(sizeof...(Args) > 0)
        [[nodiscard]]
        constexpr
        bool
        all_pressed(Args... args)
            noexcept
        {
            Uint32 mask = (SDL_BUTTON(static_cast<Uint32>(args)) | ...);
            return (buttons & mask) == mask;
        }

    };


    [[nodiscard]]
    state_t
    get_state()
        noexcept;


#if SDL_VERSION_ATLEAST(2, 0, 4)

    [[nodiscard]]
    state_t
    get_global_state()
        noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 4)


    [[nodiscard]]
    state_t
    get_relative_state()
        noexcept;


    void
    warp(int x,
         int y)
        noexcept;

    void
    warp(vec2 pos)
        noexcept;


#if SDL_VERSION_ATLEAST(2, 0, 4)

    void
    warp_global(int x,
                int y);

    void
    warp_global(vec2 pos);

    std::expected<void, error>
    try_warp_global(int x,
                    int y)
        noexcept;

    std::expected<void, error>
    try_warp_global(vec2 pos)
        noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 4)


    void
    set_relative_mode(bool enabled);

    std::expected<void, error>
    try_set_relative_mode(bool enabled)
        noexcept;


    [[nodiscard]]
    bool
    get_relative_mode()
        noexcept;


#if SDL_VERSION_ATLEAST(2, 0, 4)

    void
    capture_mouse(bool enabled);

    std::expected<void, error>
    try_capture_mouse(bool enabled)
        noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 4)


    struct cursor : basic_wrapper<SDL_Cursor*> {

        using parent_type = basic_wrapper<SDL_Cursor*>;


        // Inherit constructors.
        using parent_type::parent_type;


        cursor(const Uint8* data,
               const Uint8* mask,
               int w,
               int h,
               int hot_x,
               int hot_y);

        cursor(const Uint8* data,
               const Uint8* mask,
               vec2 size,
               vec2 hot);


        cursor(const surface& surf,
               int hot_x,
               int hot_y);

        cursor(const surface& surf,
               vec2 hot);


        cursor(system_cursor c);


        /// Move constructor.
        cursor(cursor&& other)
            noexcept = default;


        ~cursor()
            noexcept;


        /// Move assignment.
        cursor&
        operator =(cursor&& other)
            noexcept = default;


        void
        create(const Uint8* data,
               const Uint8* mask,
               int w,
               int h,
               int hot_x,
               int hot_y);

        void
        create(const Uint8* data,
               const Uint8* mask,
               vec2 size,
               vec2 hot);

        void
        create(const surface& surf,
               int hot_x,
               int hot_y);

        void
        create(const surface& surf,
               vec2 hot);


        void
        create(system_cursor c);


        void
        destroy()
            noexcept override;


        void
        activate()
            noexcept;


    }; // struct cursor


    void
    redraw()
        noexcept;


    // TODO: wrap SDL_GetCursor()

    // TODO: wrap SDL_GetDefaultCursor()


    bool
    show(bool enable);

    std::expected<bool, error>
    try_show(bool enable)
        noexcept;


    [[nodiscard]]
    bool
    is_shown();

    [[nodiscard]]
    std::expected<bool, error>
    try_is_shown()
        noexcept;

} // namespace sdl::mouse

#endif
