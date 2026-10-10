/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2026  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_FILESYSTEM_HPP
#define SDL2XX_FILESYSTEM_HPP

#include <expected>
#include <filesystem>
#include <string>

#include "error.hpp"
#include "string.hpp"


namespace sdl {

    [[nodiscard]]
    std::filesystem::path
    get_base_path();


    std::expected<std::filesystem::path, error>
    try_get_base_path()
        noexcept;


    [[nodiscard]]
    std::filesystem::path
    get_pref_path(const string& org,
                  const string& app);


    std::expected<std::filesystem::path, error>
    try_get_pref_path(const string& org,
                      const string& app)
        noexcept;

} // namespace sdl

#endif
