/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <SDL.h>

#include "init.hpp"

#include "error.hpp"

namespace sdl {

    init::init(Uint32 flags)
    {
        initialize(flags);
    }


    init::~init()
        noexcept
    {
        finalize();
    }


    sub_init::sub_init(Uint32 flags) :
        flags{flags}
    {
        if (SDL_InitSubSystem(flags) < 0)
            throw error{};
    }


    sub_init::~sub_init()
        noexcept
    {
        SDL_QuitSubSystem(flags);
    }


    void
    initialize(Uint32 flags)
    {
        if (SDL_Init(flags) < 0)
            throw error{};
    }


    void
    finalize()
        noexcept
    {
        SDL_Quit();
    }


    Uint32
    was_init(Uint32 flags)
        noexcept
    {
        return SDL_WasInit(flags);
    }

} // namespace sdl
