/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <SDL_video.h>

#include "video.hpp"


namespace sdl {

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



} // namespace sdl
