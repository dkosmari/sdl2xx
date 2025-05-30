/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_INIT_HPP
#define SDL2XX_INIT_HPP

#include <concepts>

#include <SDL.h>


namespace sdl {

    struct init {

        enum class flag : Uint32 {
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


        init(Uint32 flags);

        template<std::same_as<flag>... Flags>
        requires(sizeof...(Flags) > 0)
        init(Flags... flags);


        // Disallow copies.
        init(const init&) = delete;


        ~init()
            noexcept;

    }; // class init


    template<std::same_as<init::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    [[nodiscard]]
    constexpr
    Uint32
    convert(Flags... flags)
        noexcept
    {
        return (static_cast<Uint32>(flags) | ...);
    }


    struct sub_init {

        const Uint32 flags;

        sub_init(Uint32 flags);

        template<std::same_as<init::flag>... Flags>
        requires(sizeof...(Flags) > 0)
        sub_init(Flags... flags);


        // Disallow copies.
        sub_init(const sub_init&) = delete;


        ~sub_init()
            noexcept;

    };


    /// Same as SDL_init(), but throws sdl::error
    void
    initialize(Uint32 flags);


    template<std::same_as<init::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    void
    initialize(Flags... flags)
    {
        initialize(convert(flags...));
    }



    /// Same as SDL_Quit()
    void
    finalize()
        noexcept;


    /// Same as SDL_WasInit()
    [[nodiscard]]
    Uint32
    was_init(Uint32 flags = 0)
        noexcept;

    template<std::same_as<init::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    [[nodiscard]]
    Uint32
    was_init(Flags... flags)
        noexcept
    {
        return was_init(convert(flags...));
    }


    // Implementation of templated methods.

    template<std::same_as<init::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    init::init(Flags... flags) :
        init{convert(flags...)}
    {}


    template<std::same_as<init::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    sub_init::sub_init(Flags... flags) :
        sub_init{convert(flags...)}
    {}

} // namespace sdl

#endif
