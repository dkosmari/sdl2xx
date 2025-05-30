/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_WINDOW_HPP
#define SDL2XX_WINDOW_HPP

#include <concepts>
#include <functional>
#include <optional>
#include <span>
#include <tuple>

#include <SDL_video.h>

#include "basic_wrapper.hpp"
#include "blob.hpp"
#include "display.hpp"
#include "string.hpp"
#include "rect.hpp"
#include "vec2.hpp"
#include "unique_ptr.hpp"


namespace sdl {

    class renderer;

    class surface;


    class window : public basic_wrapper<SDL_Window*> {

        unique_ptr<surface> surf;


        void
        link_this()
            noexcept;

    public:

        using base_type = basic_wrapper<SDL_Window*>;

        using state_type = std::tuple<raw_type, unique_ptr<surface>>;


        enum flag : Uint32 {
            allow_high_dpi     = SDL_WINDOW_ALLOW_HIGHDPI,
            always_on_top      = SDL_WINDOW_ALWAYS_ON_TOP,
            borderless         = SDL_WINDOW_BORDERLESS,
            foreign            = SDL_WINDOW_FOREIGN,
            fullscreen         = SDL_WINDOW_FULLSCREEN,
            fullscreen_desktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
            hidden             = SDL_WINDOW_HIDDEN,
            input_focus        = SDL_WINDOW_INPUT_FOCUS,
            keyboard_grabbed   = SDL_WINDOW_KEYBOARD_GRABBED,
            maximized          = SDL_WINDOW_MAXIMIZED,
            metal              = SDL_WINDOW_METAL,
            minimized          = SDL_WINDOW_MINIMIZED,
            mouse_capture      = SDL_WINDOW_MOUSE_CAPTURE,
            mouse_focus        = SDL_WINDOW_MOUSE_FOCUS,
            mouse_grabbed      = SDL_WINDOW_MOUSE_GRABBED,
            opengl             = SDL_WINDOW_OPENGL,
            popup_menu         = SDL_WINDOW_POPUP_MENU,
            resizable          = SDL_WINDOW_RESIZABLE,
            shown              = SDL_WINDOW_SHOWN,
            skip_taskbar       = SDL_WINDOW_SKIP_TASKBAR,
            tooltip            = SDL_WINDOW_TOOLTIP,
            utility            = SDL_WINDOW_UTILITY,
            vulkan             = SDL_WINDOW_VULKAN,
        };

        static constexpr int coord_centered  = SDL_WINDOWPOS_CENTERED;
        static constexpr int coord_undefined = SDL_WINDOWPOS_UNDEFINED;

        static constexpr vec2 pos_centered  = vec2{coord_centered, coord_centered};
        static constexpr vec2 pos_undefined = vec2{coord_undefined, coord_undefined};


        // Inherit constructors.
        using base_type::base_type;


        explicit
        window(SDL_Window* win)
            noexcept;

        window(const char* title,
               int x, int y,
               int w, int h,
               Uint32 flags = 0);

        // stringy title
        window(const concepts::string auto& title,
               int x, int y,
               int w, int h,
               Uint32 flags = 0);

        // vec2 args
        window(const char* title,
               vec2 pos,
               vec2 size,
               Uint32 flags = 0);

        // stringy title + vec2 args
        window(const concepts::string auto& title,
               vec2 pos,
               vec2 size,
               Uint32 flags = 0);

        // flag list
        template<std::same_as<flag>... Flags>
        requires(sizeof...(Flags) > 0)
        window(const char* title,
               int x, int y,
               int w, int h,
               Flags... flags);

        // stringy title + flag list
        template<concepts::string Str,
                 std::same_as<flag>... Flags>
        requires(sizeof...(Flags) > 0)
        window(const Str& title,
               int x, int y,
               int w, int h,
               Flags... flags);

        // vec2 args + flag list
        template<std::same_as<flag>... Flags>
        requires(sizeof...(Flags) > 0)
        window(const char* title,
               vec2 pos,
               vec2 size,
               Flags... flags);

        // stringy title + vec2 args + flag list
        template<concepts::string Str,
                 std::same_as<flag>... Flags>
        window(const Str& title,
               vec2 pos,
               vec2 size,
               Flags... flags);


        /// Move constructor.
        window(window&& other)
            noexcept;


        ~window()
            noexcept;


        /// Move assignment.
        window&
        operator =(window&& other)
            noexcept;


        void
        create(const char* title,
               int x, int y,
               int w, int h,
               Uint32 flags = 0);

        // stringy title
        inline
        void
        create(const concepts::string auto& title,
               int x, int y,
               int w, int h,
               Uint32 flags = 0);

        // vec2 args
        void
        create(const char* title,
               vec2 pos,
               vec2 size,
               Uint32 flags = 0);

        // stringy title + vec2 args
        inline
        void
        create(const concepts::string auto& title,
               vec2 pos,
               vec2 size,
               Uint32 flags = 0);

        // flag list
        template<std::same_as<flag>... Flags>
        requires(sizeof...(Flags) > 0)
        void
        create(const char* title,
               int x, int y,
               int w, int h,
               Flags... flags);

        // stringy title + flag list
        template<concepts::string Str,
                 std::same_as<flag>... Flags>
        requires(sizeof...(Flags) > 0)
        void
        create(const Str& title,
               int x, int y,
               int w, int h,
               Flags... flags);

        // vec2 args + flag list
        template<std::same_as<flag>... Flags>
        requires(sizeof...(Flags) > 0)
        void
        create(const char* title,
               vec2 pos,
               vec2 size,
               Flags... flags);

        // stringy title + vec2 args + flag list
        template<concepts::string Str,
                 std::same_as<flag>... Flags>
        void
        create(const Str& title,
               vec2 pos,
               vec2 size,
               Flags... flags);


        [[nodiscard]]
        static
        window
        create_from(const void* data);


        void
        destroy()
            noexcept;


        void
        acquire(state_type state)
            noexcept;

        void
        acquire(SDL_Window* new_raw,
                unique_ptr<surface> new_surf = {})
            noexcept;


        state_type
        release()
            noexcept;


        [[nodiscard]]
        unsigned
        get_display_index()
            const;


        void
        set_display_mode(const display::mode& mode);


        [[nodiscard]]
        display::mode
        get_display_mode()
            const;


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
        set_title(const char* title)
            noexcept;

        inline
        void
        set_title(const concepts::string auto& title)
            noexcept
        {
            set_title(title.data());
        }


        [[nodiscard]]
        const char*
        get_title()
            const;


        void
        set_icon(const surface& icon)
            noexcept;


        void*
        set_user_data(const char* key,
                      void* value)
            noexcept;

        inline
        void*
        set_user_data(const concepts::string auto& key,
                      void* value)
            noexcept
        {
            return set_user_data(key.data(), value);
        }


        [[nodiscard]]
        void*
        get_user_data(const char* key)
            const noexcept;

        [[nodiscard]]
        inline
        void*
        get_user_data(const concepts::string auto& key)
            const noexcept
        {
            return get_user_data(key.data());
        }


        template<typename T>
        [[nodiscard]]
        T
        get_user_data_as(const char* key)
            const noexcept
        {
            return reinterpret_cast<T*>(get_user_data(key));
        }

        template<typename T>
        [[nodiscard]]
        T
        get_user_data_as(const concepts::string auto& key)
            const noexcept
        {
            return get_user_data_as<T>(key.data());
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
        get_width()
            const noexcept;

        [[nodiscard]]
        int
        get_height()
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


        void
        warp_mouse(int x,
                   int y)
            noexcept;

        void
        warp_mouse(vec2 pos)
            noexcept;

    }; // class window


    template<std::same_as<window::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    [[nodiscard]]
    constexpr
    Uint32
    convert(Flags... flags)
        noexcept
    {
        return (static_cast<Uint32>(flags) | ...);
    }


    // Implementation of templated methods.

    // stringy title
    inline
    window::window(const concepts::string auto& title,
                   int x, int y,
                   int w, int h,
                   Uint32 flags) :
        window{title.data(), x, y, w, h, flags}
    {}


    // stringy title + vec2 args
    inline
    window::window(const concepts::string auto& title,
                   vec2 pos,
                   vec2 size,
                   Uint32 flags) :
        window{title.data(), pos, size, flags}
    {}


    // flag list
    template<std::same_as<window::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    window::window(const char* title,
                   int x, int y,
                   int w, int h,
                   Flags... flags) :
        window{title, x, y, w, h, convert(flags...)}
    {}


    // stringy title + flag list
    template<concepts::string Str,
             std::same_as<window::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    window::window(const Str& title,
                   int x, int y,
                   int w, int h,
                   Flags... flags) :
        window{title.data(), x, y, w, h, flags...}
    {}


    // vec2 args + flag list
    template<std::same_as<window::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    window::window(const char* title,
                   vec2 pos,
                   vec2 size,
                   Flags... flags) :
        window{title, pos, size, convert(flags...)}
    {}


    // stringy title + vec2 args + flag list
    template<concepts::string Str,
             std::same_as<window::flag>... Flags>
    window::window(const Str& title,
                   vec2 pos,
                   vec2 size,
                   Flags... flags) :
        window{title.data(), pos, size, flags...}
    {}


    // stringy title
    inline
    void
    window::create(const concepts::string auto& title,
                   int x, int y,
                   int w, int h,
                   Uint32 flags)
    {
        create(title.data(), x, y, w, h, flags);
    }


    // stringy title + vec2 args
    inline
    void
    window::create(const concepts::string auto& title,
                   vec2 pos,
                   vec2 size,
                   Uint32 flags)
    {
        create(title.data(), pos, size, flags);
    }


    // flag list
    template<std::same_as<window::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    void
    window::create(const char* title,
                   int x, int y,
                   int w, int h,
                   Flags... flags)
    {
        create(title, x, y, w, h, convert(flags...));
    }


    // stringy title + flag list
    template<concepts::string Str,
             std::same_as<window::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    void
    window::create(const Str& title,
                   int x, int y,
                   int w, int h,
                   Flags... flags)
    {
        create(title.data(), x, y, w, h, flags...);
    }


    // vec2 args + flag list
    template<std::same_as<window::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    void
    window::create(const char* title,
                   vec2 pos,
                   vec2 size,
                   Flags... flags)
    {
        create(title, pos, size, convert(flags...));
    }


    // stringy title + vec2 args + flag list
    template<concepts::string Str,
             std::same_as<window::flag>... Flags>
    void
    window::create(const Str& title,
                   vec2 pos,
                   vec2 size,
                   Flags... flags)
    {
        create(title.data(), pos, size, flags...);
    }

} // namespace sdl

#endif
