/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_WINDOW_HPP
#define SDL2XX_WINDOW_HPP

#include <functional>
#include <optional>
#include <span>

#include <SDL_video.h>

#include "blob.hpp"
#include "string.hpp"
#include "rect.hpp"
#include "vec2.hpp"
#include "unique_ptr.hpp"


namespace sdl {

    class renderer;
    class surface;


    class window {

        SDL_Window* ptr = nullptr;
        unique_ptr<surface> surf;


        void
        link_this()
            noexcept;


        explicit
        window(SDL_Window* win)
            noexcept;


    public:

        enum pos : int {
            centered  = SDL_WINDOWPOS_CENTERED,
            undefined = SDL_WINDOWPOS_UNDEFINED
        };


        constexpr
        window()
            noexcept = default;

        window(const string& title,
               int x, int y,
               int w, int h,
               Uint32 flags);

        ~window()
            noexcept;


        /// Move constructor.
        window(window&& other)
            noexcept;

        /// Move assignment.
        window&
        operator =(window&& other)
            noexcept;


        void
        create(const string& title,
               int x, int y,
               int w, int h,
               Uint32 flags);


        [[nodiscard]]
        static
        window
        create_from(const void* data);


        void
        destroy()
            noexcept;


        [[nodiscard]]
        bool
        is_valid()
            const noexcept;

        [[nodiscard]]
        explicit
        operator bool()
            const noexcept;


        [[nodiscard]]
        SDL_Window*
        data()
            noexcept;


        [[nodiscard]]
        const SDL_Window*
        data()
            const noexcept;


        [[nodiscard]]
        int
        get_display_index()
            const;


        // TODO: wrap SDL_SetWindowDisplayMode()
        // TODO: wrap SDL_GetWindowDisplayMode()


        [[nodiscard]]
        blob
        get_icc_profile()
            const;


        [[nodiscard]]
        Uint32
        get_pixel_format()
            const;


        [[nodiscard]]
        Uint32
        get_id()
            const noexcept;

        [[nodiscard]]
        static
        window*
        get_from_id(Uint32 id);


        [[nodiscard]]
        Uint32
        get_flags()
            const noexcept;


        void
        set_title(const string& title)
            noexcept;

        [[nodiscard]]
        string
        get_title()
            const;


        // TODO: wrap SDLCALL SDL_SetWindowIcon()


        void*
        set_user_data(const string& key, void* value)
            noexcept;

        [[nodiscard]]
        void*
        get_user_data(const string& key)
            const noexcept;

        template<typename T>
        [[nodiscard]]
        T
        get_user_data_as(const string& key)
            const noexcept
        {
            return reinterpret_cast<T*>(get_user_data(key));
        }


        void
        set_position(int x, int y)
            noexcept;

        void
        set_position(vec2 pos)
            noexcept;


        [[nodiscard]]
        vec2
        get_position()
            const noexcept;


        void
        set_size(int w, int h)
            noexcept;

        void
        set_size(vec2 size)
            noexcept;

        [[nodiscard]]
        vec2
        get_size()
            const noexcept;

        [[nodiscard]]
        int
        width()
            const noexcept;

        [[nodiscard]]
        int
        height()
            const noexcept;


        struct borders {
            int top;
            int left;
            int bottom;
            int right;
        };

        [[nodiscard]]
        borders
        get_borders()
            const;


        [[nodiscard]]
        vec2
        get_size_in_pixels()
            const noexcept;


        void
        set_min_size(int w, int h)
            noexcept;

        void
        set_min_size(vec2 size)
            noexcept;

        [[nodiscard]]
        vec2
        get_min_size()
            const noexcept;


        void
        set_max_size(int w, int h)
            noexcept;

        void
        set_max_size(vec2 size)
            noexcept;

        [[nodiscard]]
        vec2
        get_max_size()
            const noexcept;


        void
        set_bordered(bool bordered)
            noexcept;


        void
        set_resizable(bool resizable)
            noexcept;


        void
        set_always_on_top(bool on_top)
            noexcept;


        void
        show()
            noexcept;

        void
        hide()
            noexcept;


        void
        raise()
            noexcept;


        void
        maximize()
            noexcept;

        void
        minimize()
            noexcept;

        void
        restore()
            noexcept;


        void
        set_fullscreen(Uint32 flags);


        surface*
        get_surface();


        void
        update_surface();


        void
        update_surface(std::span<const rect> rects);


        void
        set_grab(bool grabbed)
            noexcept;

        [[nodiscard]]
        bool
        get_grab()
            const noexcept;


        void
        set_keyboard_grab(bool grabbed)
            noexcept;

        [[nodiscard]]
        bool
        get_keyboard_grab()
            const noexcept;

        void
        set_mouse_grab(bool grabbed)
            noexcept;

        [[nodiscard]]
        bool
        get_mouse_grab()
            const noexcept;

        static
        window*
        get_grabbed_window()
            noexcept;


        void
        set_mouse_rect(const rect& r);


        [[nodiscard]]
        std::optional<rect>
        get_mouse_rect()
            const noexcept;


        void
        set_brightness(float brightness);

        [[nodiscard]]
        float
        get_brightness()
            const noexcept;


        void
        set_opacity(float opacity);

        [[nodiscard]]
        float
        get_opacity()
            const;


        void
        set_modal_for(window& parent);


        void
        set_focus();


        void
        set_gamma_ramp(const Uint16* red,
                       const Uint16* green,
                       const Uint16* blue);

        bool
        try_set_gamma_ramp(const Uint16* red,
                           const Uint16* green,
                           const Uint16* blue)
            noexcept;


        void
        get_gamma_ramp(Uint16* red,
                       Uint16* green,
                       Uint16* blue)
            const;

        bool
        try_get_gamma_ramp(Uint16* red,
                           Uint16* green,
                           Uint16* blue)
            const noexcept;


#if 0
        // TODO: wrap SDL_SetWindowHitTest()
        using hit_test_func_t = std::function<SDL_HitTestResult (window&, vec2)>;

        void
        set_hit_test(hit_test_func_t func);
#endif


        void
        flash(SDL_FlashOperation op);


        [[nodiscard]]
        SDL_GLContext
        create_context();


        void
        make_current(SDL_GLContext context);


        [[nodiscard]]
        vec2
        get_gl_drawable_size()
            const noexcept;


        void
        gl_swap()
            noexcept;



        [[nodiscard]]
        bool
        is_screen_keyboard_shown()
            noexcept;


        renderer*
        get_renderer();


        [[nodiscard]]
        static
        window*
        get_wrapper(SDL_Window* win)
            noexcept;

    };

} // namespace sdl

#endif
