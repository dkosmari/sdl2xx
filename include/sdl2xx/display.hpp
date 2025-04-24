/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_DISPLAY_HPP
#define SDL2XX_DISPLAY_HPP

#include <SDL_video.h>

#include "rect.hpp"
#include "string.hpp"
#include "vec2.hpp"


namespace sdl {

    class window;

} // namespace sdl


namespace sdl::display {

    using mode = SDL_DisplayMode;

    using orientation = SDL_DisplayOrientation;


    [[nodiscard]]
    unsigned
    get_num_displays();


    [[nodiscard]]
    const char*
    get_name(unsigned index);


    [[nodiscard]]
    rect
    get_bounds(unsigned index);


    [[nodiscard]]
    rect
    get_usable_bounds(unsigned index);


    struct dpi {
        float diagonal;
        float horizontal;
        float vertical;
    };


    [[nodiscard]]
    dpi
    get_dpi(unsigned index);


    [[nodiscard]]
    orientation
    get_orientation(unsigned index)
        noexcept;


    [[nodiscard]]
    unsigned
    get_num_modes(unsigned index);


    [[nodiscard]]
    mode
    get_mode(unsigned disp_index,
             unsigned mode_index);


    [[nodiscard]]
    mode
    get_desktop_mode(unsigned index);


    [[nodiscard]]
    mode
    get_current_mode(unsigned index);


    [[nodiscard]]
    mode
    get_closest_mode(unsigned index,
                     const mode& desired);


    [[nodiscard]]
    unsigned
    get_index(point pt);


    [[nodiscard]]
    unsigned
    get_index(const rect& box);


    /// Same as sdl::window::get_display_index()
    [[nodiscard]]
    unsigned
    get_index(const window& win);

} // namespace sdl

#endif
