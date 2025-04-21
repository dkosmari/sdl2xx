/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_GL_HPP
#define SDL2XX_GL_HPP

#include <SDL_video.h>


namespace sdl {

    class window;

} // namespace sdl


namespace sdl::gl {

    void
    load_library(const char* path);


    [[nodiscard]]
    void*
    get_proc_address(const char* proc)
        noexcept;

    template<typename T>
    [[nodiscard]]
    T
    get_proc_address_as(const char* proc)
        noexcept
    {
        return reinterpret_cast<T>(get_proc_address(proc));
    }


    void
    unload_library()
        noexcept;


    [[nodiscard]]
    bool
    extension_supported(const char* extension)
        noexcept;


    void
    reset_attributes()
        noexcept;


    void
    set_attribute(SDL_GLattr attr, int value);


    int
    get_attribute(SDL_GLattr attr);


    window*
    get_current_window();


    SDL_GLContext
    get_current_context();


    void
    destroy(SDL_GLContext context)
        noexcept;


    enum class swap_interval {
        immediate = 0,
        vsync     = 1,
        adaptive  = -1,
    };


    void
    set_gl_swap_interval(swap_interval interval);


    swap_interval
    get_gl_swap_interval()
        noexcept;


} // namespace sdl::gl

#endif
