/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <SDL_video.h>

#include "video.hpp"

#include "error.hpp"


namespace sdl::video {

    unsigned
    get_num_drivers()
    {
        int result = SDL_GetNumVideoDrivers();
        if (result < 0)
            throw error{};
        return result;
    }


    const char*
    get_driver(unsigned index)
    {
        const char* driver = SDL_GetVideoDriver(index);
        if (!driver)
            throw error{};
        return driver;
    }


    void
    init(const char* driver)
    {
        if (SDL_VideoInit(driver) < 0)
            throw error{};
    }


    void
    quit()
        noexcept
    {
        SDL_VideoQuit();
    }


    const char*
    get_current_driver()
    {
        const char* driver = SDL_GetCurrentVideoDriver();
        if (!driver)
            throw error{};
        return driver;
    }


    unsigned
    get_num_displays()
    {
        int result = SDL_GetNumVideoDisplays();
        if (result < 0)
            throw error{};
        return result;
    }


    bool
    is_screen_saver_enabled()
        noexcept
    {
        return SDL_IsScreenSaverEnabled();
    }


    void
    enable_screen_saver()
        noexcept
    {
        SDL_EnableScreenSaver();
    }


    void
    disable_screen_saver()
        noexcept
    {
        SDL_DisableScreenSaver();
    }



} // namespace sdl::video
