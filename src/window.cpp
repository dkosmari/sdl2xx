/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <utility>

#include <SDL_keyboard.h>

#include "window.hpp"

#include "error.hpp"
#include "renderer.hpp"
#include "surface.hpp"


namespace sdl {

    static const char* wrapper_key = "wrapper";


    void
    window::link_this()
        noexcept
    {
        if (ptr)
            set_user_data(wrapper_key, this);
    }


    window::window(SDL_Window* win)
        noexcept :
        ptr{win}
    {}


    window::window(const char* title,
                   int x, int y,
                   int w, int h,
                   Uint32 flags)
    {
        create(title, x, y, w, h, flags);
    }


    window::window(const char* title,
                   vec2 pos,
                   vec2 size,
                   Uint32 flags)
    {
        create(title, pos, size, flags);
    }


    window::window(window&& other)
        noexcept :
        ptr{other.ptr},
        surf{std::move(other.surf)}
    {
        other.ptr = nullptr;
        link_this();
    }


    window::~window()
        noexcept
    {
        destroy();
    }


    window&
    window::operator =(window&& other)
        noexcept
    {
        if (this != &other) {
            destroy();
            ptr = other.ptr;
            other.ptr = nullptr;
            surf = std::move(other.surf);
            link_this();
        }
        return *this;
    }


    void
    window::create(const char* title,
                   int x, int y,
                   int w, int h,
                   Uint32 flags)
    {
        destroy();
        ptr = SDL_CreateWindow(title, x, y, w, h, flags);
        if (!ptr)
            throw error{};
        link_this();
    }


    void
    window::create(const char* title,
                   vec2 pos,
                   vec2 size,
                   Uint32 flags)
    {
        create(title, pos.x, pos.y, size.x, size.y, flags);
    }


    window
    window::create_from(const void* data)
    {
        return window{SDL_CreateWindowFrom(data)};
    }


    void
    window::destroy()
        noexcept
    {
        if (ptr) {
            SDL_DestroyWindow(ptr);
            ptr = nullptr;
            surf.reset();
        }
    }


    bool
    window::is_valid()
        const noexcept
    {
        return ptr;
    }


    window::operator bool()
        const noexcept
    {
        return ptr;
    }


    SDL_Window*
    window::data()
        noexcept
    {
        return ptr;
    }


    const SDL_Window*
    window::data()
        const noexcept
    {
        return ptr;
    }


    unsigned
    window::get_display_index()
        const
    {
        int result = SDL_GetWindowDisplayIndex(ptr);
        if (result < 0)
            throw error{};
        return result;
    }


    void
    window::set_display_mode(const display::mode& mode)
    {
        if (SDL_SetWindowDisplayMode(ptr, &mode) < 0)
            throw error{};
    }


    display::mode
    window::get_display_mode()
        const
    {
        display::mode result;
        if (SDL_GetWindowDisplayMode(ptr, &result) < 0)
            throw error{};
        return result;
    }


    blob
    window::get_icc_profile()
        const
    {
        std::size_t icc_size;
        Uint8* icc_data = reinterpret_cast<Uint8*>(SDL_GetWindowICCProfile(ptr,
                                                                           &icc_size));
        if (!icc_data)
            throw error{};

        return blob{icc_data, icc_size};
    }


    Uint32
    window::get_pixel_format()
        const
    {
        Uint32 result = SDL_GetWindowPixelFormat(ptr);
        if (result == SDL_PIXELFORMAT_UNKNOWN)
            throw error{};
        return result;
    }


    Uint32
    window::get_id()
        const noexcept
    {
        return SDL_GetWindowID(ptr);
    }


    window*
    window::get_from_id(Uint32 id)
    {
        SDL_Window* win = SDL_GetWindowFromID(id);
        if (!win)
            throw error{};
        return get_wrapper(win);
    }


    Uint32
    window::get_flags()
        const noexcept
    {
        return SDL_GetWindowFlags(ptr);
    }


    void
    window::set_title(const char* title)
        noexcept
    {
        SDL_SetWindowTitle(ptr, title);
    }


    const char*
    window::get_title()
        const
    {
        return SDL_GetWindowTitle(ptr);
    }


    void
    window::set_icon(const surface& icon)
        noexcept
    {
        SDL_SetWindowIcon(ptr, const_cast<SDL_Surface*>(icon.data()));
    }


    void*
    window::set_user_data(const char* key,
                          void* value)
        noexcept
    {
        return SDL_SetWindowData(ptr, key, value);
    }


    void*
    window::get_user_data(const char* key)
        const noexcept
    {
        return SDL_GetWindowData(ptr, key);
    }


    void
    window::set_position(int x, int y)
        noexcept
    {
        SDL_SetWindowPosition(ptr, x, y);
    }


    void
    window::set_position(vec2 pos)
        noexcept
    {
        set_position(pos.x, pos.y);
    }


    vec2
    window::get_position()
        const noexcept
    {
        vec2 result;
        SDL_GetWindowPosition(ptr, &result.x, &result.y);
        return result;
    }


    void
    window::set_size(int w, int h)
        noexcept
    {
        SDL_SetWindowSize(ptr, w, h);
    }


    void
    window::set_size(vec2 size)
        noexcept
    {
        set_size(size.x, size.y);
    }


    vec2
    window::get_size()
        const noexcept
    {
        vec2 result;
        SDL_GetWindowSize(ptr, &result.x, &result.y);
        return result;
    }


    int
    window::get_width()
        const noexcept
    {
        int width;
        SDL_GetWindowSize(ptr, &width, nullptr);
        return width;
    }


    int
    window::get_height()
        const noexcept
    {
        int height;
        SDL_GetWindowSize(ptr, nullptr, &height);
        return height;
    }


    window::borders
    window::get_borders()
        const
    {
        borders result;
        if (SDL_GetWindowBordersSize(ptr,
                                     &result.top,
                                     &result.left,
                                     &result.bottom,
                                     &result.right) < 0)
            throw error{};
        return result;
    }


    vec2
    window::get_size_in_pixels()
        const noexcept
    {
        vec2 result;
        SDL_GetWindowSizeInPixels(ptr, &result.x, &result.y);
        return result;
    }


    void
    window::set_min_size(int w, int h)
        noexcept
    {
        SDL_SetWindowMinimumSize(ptr, w, h);
    }


    void
    window::set_min_size(vec2 size)
        noexcept
    {
        set_min_size(size.x, size.y);
    }


    vec2
    window::get_min_size()
        const noexcept
    {
        vec2 result;
        SDL_GetWindowMinimumSize(ptr, &result.x, &result.y);
        return result;
    }



    void
    window::set_max_size(int w, int h)
        noexcept
    {
        SDL_SetWindowMaximumSize(ptr, w, h);
    }


    void
    window::set_max_size(vec2 size)
        noexcept
    {
        set_max_size(size.x, size.y);
    }


    vec2
    window::get_max_size()
        const noexcept
    {
        vec2 result;
        SDL_GetWindowMaximumSize(ptr, &result.x, &result.y);
        return result;
    }


    void
    window::set_bordered(bool bordered)
        noexcept
    {
        SDL_SetWindowBordered(ptr, bordered ? SDL_TRUE : SDL_FALSE);
    }


    void
    window::set_resizable(bool resizable)
        noexcept
    {
        SDL_SetWindowResizable(ptr, resizable ? SDL_TRUE : SDL_FALSE);
    }


    void
    window::set_always_on_top(bool on_top)
        noexcept
    {
        SDL_SetWindowAlwaysOnTop(ptr, on_top ? SDL_TRUE : SDL_FALSE);
    }


    void
    window::show()
        noexcept
    {
        SDL_ShowWindow(ptr);
    }


    void
    window::hide()
        noexcept
    {
        SDL_HideWindow(ptr);
    }


    void
    window::raise()
        noexcept
    {
        SDL_RaiseWindow(ptr);
    }


    void
    window::maximize()
        noexcept
    {
        SDL_MaximizeWindow(ptr);
    }


    void
    window::minimize()
        noexcept
    {
        SDL_MinimizeWindow(ptr);
    }


    void
    window::restore()
        noexcept
    {
        SDL_RestoreWindow(ptr);
    }


    void
    window::set_fullscreen(Uint32 flags)
    {
        if (SDL_SetWindowFullscreen(ptr, flags) < 0)
            throw error{};
    }


    surface*
    window::get_surface()
    {
        SDL_Surface* s = SDL_GetWindowSurface(ptr);
        if (!s)
            throw error{};
        surf = make_unique<surface>(s, surface::dont_destroy);
        return surf.get();
    }


    void
    window::update_surface()
    {
        if (SDL_UpdateWindowSurface(ptr) < 0)
            throw error{};
    }


    void
    window::update_surface(std::span<const rect> rects)
    {
        if (SDL_UpdateWindowSurfaceRects(ptr, rects.data(), rects.size()) < 0)
            throw error{};
    }


    void
    window::set_grab(bool grabbed)
        noexcept
    {
        SDL_SetWindowGrab(ptr, grabbed ? SDL_TRUE : SDL_FALSE);
    }


    bool
    window::get_grab()
        const noexcept
    {
        return SDL_GetWindowGrab(ptr);
    }



    void
    window::set_keyboard_grab(bool grabbed)
        noexcept
    {
        SDL_SetWindowKeyboardGrab(ptr, grabbed ? SDL_TRUE : SDL_FALSE);
    }


    bool
    window::get_keyboard_grab()
        const noexcept
    {
        return SDL_GetWindowKeyboardGrab(ptr);
    }


    void
    window::set_mouse_grab(bool grabbed)
        noexcept
    {
        SDL_SetWindowMouseGrab(ptr, grabbed ? SDL_TRUE : SDL_FALSE);
    }


    bool
    window::get_mouse_grab()
        const noexcept
    {
        return SDL_GetWindowMouseGrab(ptr);
    }


    window*
    window::get_grabbed_window()
        noexcept
    {
        return get_wrapper(SDL_GetGrabbedWindow());
    }


    void
    window::set_mouse_rect(const rect& r)
    {
        if (SDL_SetWindowMouseRect(ptr, &r) < 0)
            throw error{};
    }


    std::optional<rect>
    window::get_mouse_rect()
        const noexcept
    {
        auto r = SDL_GetWindowMouseRect(ptr);
        if (!r)
            return {};
        return rect{*r};
    }


    void
    window::set_brightness(float brightness)
    {
        if  (SDL_SetWindowBrightness(ptr, brightness) < 0)
            throw error{};
    }


    float
    window::get_brightness()
        const noexcept
    {
        return SDL_GetWindowBrightness(ptr);
    }


    void
    window::set_opacity(float opacity)
    {
        if (SDL_SetWindowOpacity(ptr, opacity) < 0)
            throw error{};
    }


    float
    window::get_opacity()
        const
    {
        float result;
        if (SDL_GetWindowOpacity(ptr, &result))
            throw error{};
        return result;
    }


    void
    window::set_modal_for(window& parent)
    {
        if (SDL_SetWindowModalFor(ptr, parent.ptr) < 0)
            throw error{};
    }


    void
    window::set_focus()
    {
        if (SDL_SetWindowInputFocus(ptr) < 0)
            throw error{};
    }


    void
    window::set_gamma_ramp(const Uint16* red,
                           const Uint16* green,
                           const Uint16* blue)
    {
        if (!try_set_gamma_ramp(red, green, blue))
            throw error{};
    }


    bool
    window::try_set_gamma_ramp(const Uint16* red,
                               const Uint16* green,
                               const Uint16* blue)
        noexcept
    {
        return SDL_SetWindowGammaRamp(ptr, red, green, blue) >= 0;
    }


    void
    window::get_gamma_ramp(Uint16* red,
                           Uint16* green,
                           Uint16* blue)
        const
    {
        if (!try_get_gamma_ramp(red, green, blue))
            throw error{};
    }


    bool
    window::try_get_gamma_ramp(Uint16* red,
                               Uint16* green,
                               Uint16* blue)
        const noexcept
    {
        return SDL_GetWindowGammaRamp(ptr, red, green, blue) >= 0;
    }


#if 0
    namespace {

        SDL_HitTestResult
        hit_test_wrapper(SDL_Window* win,
                         const SDL_Point* pt,
                         void* data)
        {
            auto func = reinterpret_cast<window::hit_test_func_t*>(data);
        }

    }

    void
    window::set_hit_test(std::function<hit_test_func_t> func)
    {

    }
#endif


    void
    window::flash(SDL_FlashOperation op)
    {
        if (SDL_FlashWindow(ptr, op) < 0)
            throw error{};
    }


    SDL_GLContext
    window::create_context()
    {
        auto result = SDL_GL_CreateContext(ptr);
        if (!result)
            throw error{};
        return result;
    }


    void
    window::make_current(SDL_GLContext context)
    {
        if (SDL_GL_MakeCurrent(ptr, context) < 0)
            throw error{};
    }


    vec2
    window::get_gl_drawable_size()
        const noexcept
    {
        vec2 result;
        SDL_GL_GetDrawableSize(ptr, &result.x, &result.y);
        return result;
    }


    void
    window::gl_swap()
        noexcept
    {
        SDL_GL_SwapWindow(ptr);
    }


    bool
    window::is_screen_keyboard_shown()
        noexcept
    {
        return SDL_IsScreenKeyboardShown(ptr);
    }


    renderer*
    window::get_renderer()
    {
        SDL_Renderer* ren = SDL_GetRenderer(ptr);
        if (!ren)
            throw error{};
        return renderer::get_wrapper(ren);
    }


    window*
    window::get_wrapper(SDL_Window* win)
        noexcept
    {
        if (!win)
            return nullptr;
        return reinterpret_cast<window*>(SDL_GetWindowData(win, wrapper_key));
    }

} // namespace sdl
