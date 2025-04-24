/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_INIT_HPP
#define SDL2XX_INIT_HPP

#include <SDL.h>


namespace sdl {

    struct init {

        enum flag : Uint32 {
            timer           = SDL_INIT_TIMER,
            audio           = SDL_INIT_AUDIO,
            video           = SDL_INIT_VIDEO,
            joystick        = SDL_INIT_JOYSTICK,
            haptic          = SDL_INIT_HAPTIC,
            game_controller = SDL_INIT_GAMECONTROLLER,
            events          = SDL_INIT_EVENTS,
            sensor          = SDL_INIT_SENSOR,
            everything      = SDL_INIT_EVERYTHING,
        };


        init(Uint32 flags = 0);

        // Disallow copies.
        init(const init&) = delete;

        ~init()
            noexcept;

    };


    struct sub_init {

        const Uint32 flags;

        sub_init(Uint32 flags);

        ~sub_init()
            noexcept;

    };


    /// Same as SDL_init(), but throws sdl::error
    void
    initialize(Uint32 flags = 0);


    /// Same as SDL_Quit()
    void
    finalize()
        noexcept;


    /// Same as SDL_WasInit()
    Uint32
    was_init(Uint32 flags = 0)
        noexcept;

} // namespace sdl

#endif
