/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_VIDEO_HPP
#define SDL2XX_VIDEO_HPP

namespace sdl {

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

} // namespace sdl

#endif
