/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <utility>
#include <ranges>

#include "gl.hpp"

#include "error.hpp"
#include "vector.hpp"
#include "window.hpp"


namespace sdl::gl {

    void
    load_library(const char* path)
    {
        if (SDL_GL_LoadLibrary(path))
            throw error{};
    }


    void*
    get_proc_address(const char* proc)
        noexcept
    {
        return SDL_GL_GetProcAddress(proc);
    }


    void
    unload_library()
        noexcept
    {
        SDL_GL_UnloadLibrary();
    }


    bool
    extension_supported(const char* extension)
        noexcept
    {
        return SDL_GL_ExtensionSupported(extension);
    }


    void
    reset_attributes()
        noexcept
    {
        SDL_GL_ResetAttributes();
    }


    void
    set_attribute(SDL_GLattr attr, int value)
    {
        if (SDL_GL_SetAttribute(attr, value) < 0)
            throw error{};
    }


    int
    get_attribute(SDL_GLattr attr)
    {
        int result;
        if (SDL_GL_GetAttribute(attr, &result) < 0)
            throw error{};
        return result;
    }


    window*
    get_current_window()
    {
        auto win = SDL_GL_GetCurrentWindow();
        if (!win)
            throw error{};
        return window::get_wrapper(win);
    }


    SDL_GLContext
    get_current_context()
    {
        auto ctx = SDL_GL_GetCurrentContext();
        if (!ctx)
            throw error{};
        return ctx;
    }


    void
    destroy(SDL_GLContext context)
        noexcept
    {
        SDL_GL_DeleteContext(context);
    }


    void
    set_swap_interval(swap_interval interval)
    {
        if (SDL_GL_SetSwapInterval(static_cast<int>(interval)) < 0)
            throw error{};
    }


    swap_interval
    get_swap_interval()
        noexcept
    {
        return static_cast<swap_interval>(SDL_GL_GetSwapInterval());
    }

} // namespace sdl::gl
