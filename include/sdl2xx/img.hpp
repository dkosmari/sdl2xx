/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_IMG_HPP
#define SDL2XX_IMG_HPP

#include <concepts>
#include <expected>
#include <filesystem>

#include <SDL_image.h>

#include "error.hpp"
#include "surface.hpp"
#include "texture.hpp"
#include "vector.hpp"


namespace sdl::img {

    using std::filesystem::path;


    [[nodiscard]]
    SDL_version
    linked_version()
        noexcept;


    struct init {

        enum flag : Uint32 {
#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
            avif = IMG_INIT_AVIF,
#endif
            jpg  = IMG_INIT_JPG,
#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
            jxl  = IMG_INIT_JXL,
#endif
            png  = IMG_INIT_PNG,
            tif  = IMG_INIT_TIF,
            webp = IMG_INIT_WEBP,

            all =
#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
                avif |
#endif
                jpg  |
#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
                jxl  |
#endif
                png  |
                tif  |
                webp
        };


        // Disallow copies.
        init(const init&) = delete;


        init(Uint32 flags = flag::all)
            noexcept;


        template<std::same_as<flag>... Args>
        requires(sizeof...(Args) > 0)
        init(Args... args)
            noexcept :
            init{static_cast<Uint32>((args | ...))}
        {}


        ~init()
            noexcept;

    };


    Uint32
    initialize(Uint32 flags = init::all)
        noexcept;


    template<std::same_as<init::flag>... Args>
    requires(sizeof...(Args) > 0)
    Uint32
    initialize(Args... args)
        noexcept
    {
        return initialize(static_cast<Uint32>((args | ...)));
    }


    [[nodiscard]]
    Uint32
    is_initialized()
        noexcept;


    void
    finalize()
        noexcept;


    [[nodiscard]]
    surface
    load(SDL_RWops* src,
         bool close_src,
         const char* type);

    [[nodiscard]]
    std::expected<surface, error>
    try_load(SDL_RWops* src,
             bool close_src,
             const char* type)
        noexcept;

    [[nodiscard]]
    inline
    surface
    load(SDL_RWops* src,
         bool close_src,
         const concepts::string auto& type)
    {
        return load(src, close_src, type.data());
    }

    [[nodiscard]]
    inline
    std::expected<surface, error>
    try_load(SDL_RWops* src,
             bool close_src,
             const concepts::string auto& type)
        noexcept
    {
        return try_load(src, close_src, type.data());
    }


    [[nodiscard]]
    surface
    load(const path& filename);

    [[nodiscard]]
    std::expected<surface, error>
    try_load(const path& filename)
        noexcept;


    [[nodiscard]]
    surface
    load(SDL_RWops* src,
         bool close_src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load(SDL_RWops* src,
             bool close_src)
        noexcept;


    [[nodiscard]]
    texture
    load_texture(renderer& ren,
                 const path& filename);

    [[nodiscard]]
    std::expected<texture, error>
    try_load_texture(renderer& ren,
                     const path& filename)
        noexcept;


    [[nodiscard]]
    texture
    load_texture(renderer& ren,
                 SDL_RWops* src,
                 bool close_src);

    [[nodiscard]]
    std::expected<texture, error>
    try_load_texture(renderer& ren,
                     SDL_RWops* src,
                     bool close_src)
        noexcept;


    [[nodiscard]]
    texture
    load_texture(renderer& ren,
                 SDL_RWops* src,
                 bool close_src,
                 const char* type);

    [[nodiscard]]
    std::expected<texture, error>
    try_load_texture(renderer& ren,
                     SDL_RWops* src,
                     bool close_src,
                     const char* type)
        noexcept;

    [[nodiscard]]
    inline
    texture
    load_texture(renderer& ren,
                 SDL_RWops* src,
                 bool close_src,
                 const concepts::string auto& type)
    {
        return load_texture(ren, src, close_src, type.data());
    }

    [[nodiscard]]
    inline
    std::expected<texture, error>
    try_load_texture(renderer& ren,
                     SDL_RWops* src,
                     bool close_src,
                     const concepts::string auto& type)
    {
        return try_load_texture(ren, src, close_src, type.data());
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
    [[nodiscard]]
    bool
    is_avif(SDL_RWops* src)
        noexcept;
#endif

    [[nodiscard]]
    bool
    is_bmp(SDL_RWops* src)
        noexcept;

    [[nodiscard]]
    bool
    is_cur(SDL_RWops* src)
        noexcept;

    [[nodiscard]]
    bool
    is_gif(SDL_RWops* src)
        noexcept;

    [[nodiscard]]
    bool
    is_ico(SDL_RWops* src)
        noexcept;

    [[nodiscard]]
    bool
    is_jpg(SDL_RWops* src)
        noexcept;

#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
    [[nodiscard]]
    bool
    is_jxl(SDL_RWops* src)
        noexcept;
#endif

    [[nodiscard]]
    bool
    is_lbm(SDL_RWops* src)
        noexcept;

    [[nodiscard]]
    bool
    is_pcx(SDL_RWops* src)
        noexcept;

    [[nodiscard]]
    bool
    is_png(SDL_RWops* src)
        noexcept;

    [[nodiscard]]
    bool
    is_pnm(SDL_RWops* src)
        noexcept;

#if SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)
    [[nodiscard]]
    bool
    is_svg(SDL_RWops* src)
        noexcept;
#endif

#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
    [[nodiscard]]
    bool
    is_qoi(SDL_RWops* src)
        noexcept;
#endif

    [[nodiscard]]
    bool
    is_tif(SDL_RWops* src)
        noexcept;

    [[nodiscard]]
    bool
    is_xcf(SDL_RWops* src)
        noexcept;

    [[nodiscard]]
    bool
    is_xpm(SDL_RWops* src)
        noexcept;

    [[nodiscard]]
    bool
    is_xv(SDL_RWops* src)
        noexcept;

    [[nodiscard]]
    bool
    is_webp(SDL_RWops* src)
        noexcept;


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    [[nodiscard]]
    surface
    load_avif(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_avif(SDL_RWops* src)
        noexcept;
#endif


    [[nodiscard]]
    surface
    load_bmp(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_bmp(SDL_RWops* src)
        noexcept;


    [[nodiscard]]
    surface
    load_cur(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_cur(SDL_RWops* src)
        noexcept;


    [[nodiscard]]
    surface
    load_gif(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_gif(SDL_RWops* src)
        noexcept;


    [[nodiscard]]
    surface
    load_ico(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_ico(SDL_RWops* src)
        noexcept;


    [[nodiscard]]
    surface
    load_jpg(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_jpg(SDL_RWops* src)
        noexcept;


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    [[nodiscard]]
    surface
    load_jxl(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_jxl(SDL_RWops* src)
        noexcept;

#endif


    [[nodiscard]]
    surface
    load_lbm(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_lbm(SDL_RWops* src)
        noexcept;


    [[nodiscard]]
    surface
    load_pcx(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_pcx(SDL_RWops* src)
        noexcept;


    [[nodiscard]]
    surface
    load_png(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_png(SDL_RWops* src)
        noexcept;


    [[nodiscard]]
    surface
    load_pnm(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_pnm(SDL_RWops* src)
        noexcept;


#if SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)

    [[nodiscard]]
    surface
    load_svg(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_svg(SDL_RWops* src)
        noexcept;

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    [[nodiscard]]
    surface
    load_svg(SDL_RWops* src,
             int width,
             int height);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_svg(SDL_RWops* src,
                 int width,
                 int height)
        noexcept;

    [[nodiscard]]
    inline
    surface
    load_svg(SDL_RWops* src,
             vec2 size)
    {
        return load_svg(src, size.x, size.y);
    }

    [[nodiscard]]
    inline
    std::expected<surface, error>
    try_load_svg(SDL_RWops* src,
                 vec2 size)
    {
        return try_load_svg(src, size.x, size.y);
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    [[nodiscard]]
    surface
    load_qoi(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_qoi(SDL_RWops* src)
        noexcept;

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    [[nodiscard]]
    surface
    load_tif(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_tif(SDL_RWops* src)
        noexcept;


    [[nodiscard]]
    surface
    load_webp(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_webp(SDL_RWops* src)
        noexcept;


    [[nodiscard]]
    surface
    load_xcf(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_xcf(SDL_RWops* src)
        noexcept;


    [[nodiscard]]
    surface
    load_xpm(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_xpm(SDL_RWops* src)
        noexcept;

    [[nodiscard]]
    surface
    load_xpm(char* xpm[]);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_xpm(char* xpm[])
        noexcept;

#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    [[nodiscard]]
    surface
    load_xpm_to_rgb888(char* xpm[]);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_xpm_to_rgb888(char* xpm[])
        noexcept;

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    [[nodiscard]]
    surface
    load_xv(SDL_RWops* src);

    [[nodiscard]]
    std::expected<surface, error>
    try_load_xv(SDL_RWops* src)
        noexcept;


#if SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)

    void
    save_jpg(const surface& src,
             const path& filename,
             int quality);

    [[nodiscard]]
    std::expected<void, error>
    try_save_jpg(const surface& src,
                 const path& filename,
                 int quality)
        noexcept;

    void
    save_jpg(const surface& src,
             SDL_RWops* dst,
             bool close_dst,
             int quality);

    [[nodiscard]]
    std::expected<void, error>
    try_save_jpg(const surface& src,
                 SDL_RWops* dst,
                 bool close_dst,
                 int quality)
        noexcept;

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)


    void
    save_png(const surface& src,
             const path& filename);

    [[nodiscard]]
    std::expected<void, error>
    try_save_png(const surface& src,
                 const path& filename)
        noexcept;


    void
    save_png(const surface& src,
             SDL_RWops* dst,
             bool close_dst);

    [[nodiscard]]
    std::expected<void, error>
    try_save_png(const surface& src,
                 SDL_RWops* dst,
                 bool close_dst)
        noexcept;


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    struct animation {
        int width = 0;
        int height = 0;
        int count = 0;
        vector<surface> frames{};
        vector<int> delays{};
    };


    [[nodiscard]]
    animation
    load_animation(const path& filename);

    [[nodiscard]]
    std::expected<animation, error>
    try_load_animation(const path& filename)
        noexcept;


    [[nodiscard]]
    animation
    load_animation(SDL_RWops* src,
                   bool close_src);

    [[nodiscard]]
    std::expected<animation, error>
    try_load_animation(SDL_RWops* src,
                       bool close_src)
        noexcept;


    [[nodiscard]]
    animation
    load_animation(SDL_RWops* src,
                   bool close_src,
                   const char* type);

    [[nodiscard]]
    std::expected<animation, error>
    try_load_animation(SDL_RWops* src,
                       bool close_src,
                       const char* type)
        noexcept;

    [[nodiscard]]
    inline
    animation
    load_animation(SDL_RWops* src,
                   bool close_src,
                   const concepts::string auto& type)
    {
        return load_animation(src, close_src, type.data());
    }

    [[nodiscard]]
    inline
    std::expected<animation, error>
    try_load_animation(SDL_RWops* src,
                       bool close_src,
                       const concepts::string auto& type)
        noexcept
    {
        return try_load_animation(src, close_src, type.data());
    }


    [[nodiscard]]
    animation
    load_gif_animation(SDL_RWops* src);

    [[nodiscard]]
    std::expected<animation, error>
    try_load_gif_animation(SDL_RWops* src)
        noexcept;

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

} // namespace sdl::img

#endif
