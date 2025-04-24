/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_VIDEO_HPP
#define SDL2XX_VIDEO_HPP

#include <SDL_video.h>


namespace sdl::video {

    [[nodiscard]]
    unsigned
    get_num_drivers();


    [[nodiscard]]
    const char*
    get_driver(unsigned index);


    [[nodiscard]]
    const char*
    get_current_driver();


    /// Same as sdl::display::get_num_displays()
    [[nodiscard]]
    unsigned
    get_num_displays();


    [[nodiscard]]
    bool
    is_screen_saver_enabled()
        noexcept;


    void
    enable_screen_saver()
        noexcept;


    void
    disable_screen_saver()
        noexcept;

} // namespace sdl::video

#endif
