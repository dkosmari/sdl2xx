/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include "display.hpp"

#include "error.hpp"
#include "window.hpp"


namespace sdl::display {

    unsigned
    get_num_displays()
    {
        int result = SDL_GetNumVideoDisplays();
        if (result < 0)
            throw error{};
        return result;
    }


    const char*
    get_name(unsigned index)
    {
        const char* result = SDL_GetDisplayName(index);
        if (!result)
            throw error{};
        return result;
    }


    rect
    get_bounds(unsigned index)
    {
        rect result;
        if (SDL_GetDisplayBounds(index, &result) < 0)
            throw error{};
        return result;
    }


    rect
    get_usable_bounds(unsigned index)
    {
        rect result;
        if (SDL_GetDisplayUsableBounds(index, &result) < 0)
            throw error{};
        return result;
    }


    dpi
    get_dpi(unsigned index)
    {
        dpi result;
        if (SDL_GetDisplayDPI(index,
                              &result.diagonal,
                              &result.horizontal,
                              &result.vertical) < 0)
            throw error{};
        return result;
    }


    orientation
    get_orientation(unsigned index)
        noexcept
    {
        return SDL_GetDisplayOrientation(index);
    }


    unsigned
    get_num_modes(unsigned index)
    {
        int result = SDL_GetNumDisplayModes(index);
        if (result < 0)
            throw error{};
        return result;
    }


    mode
    get_mode(unsigned disp_index,
             unsigned mode_index)
    {
        mode result;
        if (SDL_GetDisplayMode(disp_index, mode_index, &result) < 0)
            throw error{};
        return result;
    }


    mode
    get_desktop_mode(unsigned index)
    {
        mode result;
        if (SDL_GetDesktopDisplayMode(index, &result) < 0)
            throw error{};
        return result;
    }


    mode
    get_current_mode(unsigned index)
    {
        mode result;
        if (SDL_GetCurrentDisplayMode(index, &result) < 0)
            throw error{};
        return result;
    }


    mode
    get_closest_mode(unsigned index,
                     const mode& desired)
    {
        mode result;
        if (!SDL_GetClosestDisplayMode(index, &desired, &result))
            throw error{};
        return result;
    }


    unsigned
    get_index(point pt)
    {
        int result = SDL_GetPointDisplayIndex(&pt);
        if (result < 0)
            throw error{};
        return result;
    }


    unsigned
    get_index(const rect& box)
    {
        int result = SDL_GetRectDisplayIndex(&box);
        if (result < 0)
            throw error{};
        return result;
    }


    unsigned
    get_index(const window& win)
    {
        int result = SDL_GetWindowDisplayIndex(const_cast<SDL_Window*>(win.data()));
        if (result < 0)
            throw error{};
        return result;
    }

} // namespace sdl::display
