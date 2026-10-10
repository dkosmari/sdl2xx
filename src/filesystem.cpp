/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2026  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <SDL_filesystem.h>

#include "sdl2xx/filesystem.hpp"

#include "impl/value_or_throw.hpp"
#include "sdl2xx/unique_ptr.hpp"


using sdl::impl::value_or_throw;


namespace sdl {

    std::filesystem::path
    get_base_path()
    {
        return value_or_throw(try_get_base_path());
    }


    std::expected<std::filesystem::path, error>
    try_get_base_path()
        noexcept
    {
        unique_ptr<char> str{SDL_GetBasePath()};
        if (!str)
            return std::unexpected{error{}};
        try {
            return std::filesystem::path{str.get()};
        }
        catch (std::exception& e) {
            return std::unexpected{error{e}};
        }
    }


    std::filesystem::path
    get_pref_path(const string& org,
                  const string& app)
    {
        return value_or_throw(try_get_pref_path(org, app));
    }


    std::expected<std::filesystem::path, error>
    try_get_pref_path(const string& org,
                      const string& app)
        noexcept
    {
        unique_ptr<char> str{SDL_GetPrefPath(org.data(), app.data())};
        if (!str)
            return std::unexpected{error{}};
        try {
            return std::filesystem::path{str.get()};
        }
        catch (std::exception& e) {
            return std::unexpected{error{e}};
        }
    }

} // namespace sdl
