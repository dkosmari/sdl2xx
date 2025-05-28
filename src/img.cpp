/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <stdexcept>
#include <utility>

#include "img.hpp"

#include "renderer.hpp"


using std::expected;
using std::unexpected;


namespace sdl::img {

    SDL_version
    linked_version()
        noexcept
    {
        return *IMG_Linked_Version();
    }


    init::init(Uint32 flags)
        noexcept
    {
        initialize(flags);
    }


    init::~init()
        noexcept
    {
        finalize();
    }


    Uint32
    initialize(Uint32 flags)
        noexcept
    {
        return IMG_Init(flags);
    }


    Uint32
    is_initialized()
        noexcept
    {
        return IMG_Init(0);
    }


    void
    finalize()
        noexcept
    {
        IMG_Quit();
    }


    surface
    load(SDL_RWops* src,
         bool close_src,
         const char* type)
    {
        auto result = try_load(src, close_src, type);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load(SDL_RWops* src,
             bool close_src,
             const char* type)
        noexcept
    {
        auto surf = IMG_LoadTyped_RW(src, close_src, type);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    surface
    load(const path& filename)
    {
        auto result = try_load(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load(const path& filename)
        noexcept
    {
        auto surf = IMG_Load(filename.c_str());
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    surface
    load(SDL_RWops* src,
         bool close_src)
    {
        auto result = try_load(src, close_src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load(SDL_RWops* src,
             bool close_src)
        noexcept
    {
        auto surf = IMG_Load_RW(src, close_src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};

    }


    texture
    load_texture(renderer& ren,
                 const path& filename)
    {
        auto result = try_load_texture(ren, filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<texture, error>
    try_load_texture(renderer& ren,
                     const path& filename)
        noexcept
    {
        auto tex = IMG_LoadTexture(ren.data(), filename.c_str());
        if (!tex)
            return unexpected{error{}};
        return texture{tex};
    }


    texture
    load_texture(renderer& ren,
                 SDL_RWops* src,
                 bool close_src)
    {
        auto result = try_load_texture(ren, src, close_src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<texture, error>
    try_load_texture(renderer& ren,
                     SDL_RWops* src,
                     bool close_src)
        noexcept
    {

        auto tex = IMG_LoadTexture_RW(ren.data(), src, close_src);
        if (!tex)
            return unexpected{error{}};
        return texture{tex};
    }


    texture
    load_texture(renderer& ren,
                 SDL_RWops* src,
                 bool close_src,
                 const char* type)
    {
        auto result = try_load_texture(ren, src, close_src, type);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<texture, error>
    try_load_texture(renderer& ren,
                     SDL_RWops* src,
                     bool close_src,
                     const char* type)
        noexcept
    {
        auto tex = IMG_LoadTextureTyped_RW(ren.data(), src, close_src, type);
        if (!tex)
            return unexpected{error{}};
        return texture{tex};
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    bool
    is_avif(SDL_RWops* src)
        noexcept
    {
        return IMG_isAVIF(src);
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    bool
    is_bmp(SDL_RWops* src)
        noexcept
    {
        return IMG_isBMP(src);
    }


    bool
    is_cur(SDL_RWops* src)
        noexcept
    {
        return IMG_isCUR(src);
    }


    bool
    is_gif(SDL_RWops* src)
        noexcept
    {
        return IMG_isGIF(src);
    }


    bool
    is_ico(SDL_RWops* src)
        noexcept
    {
        return IMG_isICO(src);
    }


    bool
    is_jpg(SDL_RWops* src)
        noexcept
    {
        return IMG_isJPG(src);
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    bool
    is_jxl(SDL_RWops* src)
        noexcept
    {
        return IMG_isJXL(src);
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    bool
    is_lbm(SDL_RWops* src)
        noexcept
    {
        return IMG_isLBM(src);
    }


    bool
    is_pcx(SDL_RWops* src)
        noexcept
    {
        return IMG_isPCX(src);
    }


    bool
    is_png(SDL_RWops* src)
        noexcept
    {
        return IMG_isPNG(src);
    }


    bool
    is_pnm(SDL_RWops* src)
        noexcept
    {
        return IMG_isPNM(src);
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)

    bool
    is_svg(SDL_RWops* src)
        noexcept
    {
        return IMG_isSVG(src);
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    bool
    is_qoi(SDL_RWops* src)
        noexcept
    {
        return IMG_isQOI(src);
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    bool
    is_tif(SDL_RWops* src)
        noexcept
    {
        return IMG_isTIF(src);
    }


    bool
    is_xcf(SDL_RWops* src)
        noexcept
    {
        return IMG_isXCF(src);
    }


    bool
    is_xpm(SDL_RWops* src)
        noexcept
    {
        return IMG_isXPM(src);
    }


    bool
    is_xv(SDL_RWops* src)
        noexcept
    {
        return IMG_isXV(src);
    }


    bool
    is_webp(SDL_RWops* src)
        noexcept
    {
        return IMG_isWEBP(src);
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    surface
    load_avif(SDL_RWops* src)
    {
        auto result = try_load_avif(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_avif(const path& filename)
    {
        auto result = try_load_avif(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_avif(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadAVIF_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_avif(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_avif(src);
        SDL_RWclose(src);
        return result;
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    surface
    load_bmp(SDL_RWops* src)
    {
        auto result = try_load_bmp(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_bmp(const path& filename)
    {
        auto result = try_load_bmp(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_bmp(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadBMP_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_bmp(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_bmp(src);
        SDL_RWclose(src);
        return result;
    }


    surface
    load_cur(SDL_RWops* src)
    {
        auto result = try_load_cur(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_cur(const path& filename)
    {
        auto result = try_load_cur(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_cur(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadCUR_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_cur(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_cur(src);
        SDL_RWclose(src);
        return result;
    }


    surface
    load_gif(SDL_RWops* src)
    {
        auto result = try_load_gif(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_gif(const path& filename)
    {
        auto result = try_load_gif(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_gif(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadGIF_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_gif(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_gif(src);
        SDL_RWclose(src);
        return result;
    }


    surface
    load_ico(SDL_RWops* src)
    {
        auto result = try_load_ico(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_ico(const path& filename)
    {
        auto result = try_load_ico(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_ico(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadICO_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_ico(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_ico(src);
        SDL_RWclose(src);
        return result;
    }


    surface
    load_jpg(SDL_RWops* src)
    {
        auto result = try_load_jpg(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_jpg(const path& filename)
    {
        auto result = try_load_jpg(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_jpg(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadJPG_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_jpg(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_jpg(src);
        SDL_RWclose(src);
        return result;
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    surface
    load_jxl(SDL_RWops* src)
    {
        auto result = try_load_jxl(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_jxl(const path& filename)
    {
        auto result = try_load_jxl(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_jxl(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadJXL_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_jxl(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_jxl(src);
        SDL_RWclose(src);
        return result;
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    surface
    load_lbm(SDL_RWops* src)
    {
        auto result = try_load_lbm(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_lbm(const path& filename)
    {
        auto result = try_load_lbm(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_lbm(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadLBM_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_lbm(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_lbm(src);
        SDL_RWclose(src);
        return result;
    }


    surface
    load_pcx(SDL_RWops* src)
    {
        auto result = try_load_pcx(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_pcx(const path& filename)
    {
        auto result = try_load_pcx(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_pcx(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadPCX_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_pcx(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_pcx(src);
        SDL_RWclose(src);
        return result;
    }


    surface
    load_png(SDL_RWops* src)
    {
        auto result = try_load_png(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_png(const path& filename)
    {
        auto result = try_load_png(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_png(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadPNG_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_png(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_png(src);
        SDL_RWclose(src);
        return result;
    }


    surface
    load_pnm(SDL_RWops* src)
    {
        auto result = try_load_pnm(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_pnm(const path& filename)
    {
        auto result = try_load_pnm(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_pnm(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadPNM_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_pnm(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_pnm(src);
        SDL_RWclose(src);
        return result;
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)

    surface
    load_svg(SDL_RWops* src)
    {
        auto result = try_load_svg(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_svg(const path& filename)
    {
        auto result = try_load_svg(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_svg(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadSVG_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_svg(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_svg(src);
        SDL_RWclose(src);
        return result;
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    surface
    load_svg(SDL_RWops* src,
             int width,
             int height)
    {
        auto result = try_load_svg(src, width, height);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_svg(SDL_RWops* src,
             vec2 size)
    {
        return load_svg(src, size.x, size.y);
    }


    surface
    load_svg(const path& filename,
             int width,
             int height)
    {
        auto result = try_load_svg(filename, width, height);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_svg(const path& filename,
             vec2 size)
    {
        return load_svg(filename, size.x, size.y);
    }


    expected<surface, error>
    try_load_svg(SDL_RWops* src,
                 int width,
                 int height)
        noexcept
    {
        auto surf = IMG_LoadSizedSVG_RW(src, width, height);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_svg(SDL_RWops* src,
                  vec2 size)
        noexcept
    {
        return try_load_svg(src, size.x, size.y);
    }


    expected<surface, error>
    try_load_svg(const path& filename,
                 int width,
                 int height)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_svg(src, width, height);
        SDL_RWclose(src);
        return result;
    }


    expected<surface, error>
    try_load_svg(const path& filename,
                  vec2 size)
        noexcept
    {
        return try_load_svg(filename, size.x, size.y);
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    surface
    load_qoi(SDL_RWops* src)
    {
        auto result = try_load_qoi(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_qoi(const path& filename)
    {
        auto result = try_load_qoi(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_qoi(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadQOI_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_qoi(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_qoi(src);
        SDL_RWclose(src);
        return result;
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    surface
    load_tif(SDL_RWops* src)
    {
        auto result = try_load_tif(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_tif(const path& filename)
    {
        auto result = try_load_tif(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_tif(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadTIF_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_tif(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_tif(src);
        SDL_RWclose(src);
        return result;
    }


    surface
    load_webp(SDL_RWops* src)
    {
        auto result = try_load_webp(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_webp(const path& filename)
    {
        auto result = try_load_webp(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_webp(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadWEBP_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_webp(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_webp(src);
        SDL_RWclose(src);
        return result;
    }


    surface
    load_xcf(SDL_RWops* src)
    {
        auto result = try_load_xcf(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_xcf(const path& filename)
    {
        auto result = try_load_xcf(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_xcf(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadXCF_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_xcf(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_xcf(src);
        SDL_RWclose(src);
        return result;
    }


    surface
    load_xpm(SDL_RWops* src)
    {
        auto result = try_load_xpm(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_xpm(const path& filename)
    {
        auto result = try_load_xpm(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_xpm(char* xpm[])
    {
        auto result = try_load_xpm(xpm);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_xpm(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadXPM_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_xpm(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_xpm(src);
        SDL_RWclose(src);
        return result;
    }


    expected<surface, error>
    try_load_xpm(char* xpm[])
        noexcept
    {
        auto surf = IMG_ReadXPMFromArray(xpm);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    surface
    load_xpm_to_rgb888(char* xpm[])
    {
        auto result = try_load_xpm_to_rgb888(xpm);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_xpm_to_rgb888(char* xpm[])
        noexcept
    {
        auto surf = IMG_ReadXPMFromArrayToRGB888(xpm);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    surface
    load_xv(SDL_RWops* src)
    {
        auto result = try_load_xv(src);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    surface
    load_xv(const path& filename)
    {
        auto result = try_load_xv(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<surface, error>
    try_load_xv(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadXV_RW(src);
        if (!surf)
            return unexpected{error{}};
        return surface{surf};
    }


    expected<surface, error>
    try_load_xv(const path& filename)
        noexcept
    {
        auto src = SDL_RWFromFile(filename.c_str(), "rb");
        if (!src)
            return unexpected{error{}};
        auto result = try_load_xv(src);
        SDL_RWclose(src);
        return result;
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)

    void
    save_jpg(const surface& src,
             const path& filename,
             int quality)
    {
        auto result = try_save_jpg(src, filename, quality);
        if (!result)
            throw result.error();
    }


    void
    save_jpg(const surface& src,
             SDL_RWops* dst,
             bool close_dst,
             int quality)
    {
        auto result = try_save_jpg(src, dst, close_dst, quality);
        if (!result)
            throw result.error();
    }


    expected<void, error>
    try_save_jpg(const surface& src,
                 const path& filename,
                 int quality)
        noexcept
    {
        if (IMG_SaveJPG(const_cast<SDL_Surface*>(src.data()),
                        filename.c_str(),
                        quality) < 0)
            return unexpected{error{}};
        return {};
    }


    expected<void, error>
    try_save_jpg(const surface& src,
                 SDL_RWops* dst,
                 bool close_dst,
                 int quality)
        noexcept
    {
        if (IMG_SaveJPG_RW(const_cast<SDL_Surface*>(src.data()),
                           dst,
                           close_dst,
                           quality) < 0)
            return unexpected{error{}};
        return {};
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)


    void
    save_png(const surface& src,
             const path& filename)
    {
        auto result = try_save_png(src, filename);
        if (!result)
            throw result.error();
    }


    void
    save_png(const surface& src,
             SDL_RWops* dst,
             bool close_dst)
    {
        auto result = try_save_png(src, dst, close_dst);
        if (!result)
            throw result.error();
    }


    expected<void, error>
    try_save_png(const surface& src,
                 const path& filename)
        noexcept
    {
        if (IMG_SavePNG(const_cast<SDL_Surface*>(src.data()),
                        filename.c_str()) < 0)
            return unexpected{error{}};
        return {};
    }


    expected<void, error>
    try_save_png(const surface& src,
                 SDL_RWops* dst,
                 bool close_dst)
        noexcept
    {
        if (IMG_SavePNG_RW(const_cast<SDL_Surface*>(src.data()),
                           dst,
                           close_dst) < 0)
            return unexpected{error{}};
        return {};
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    namespace {

        animation
        convert_and_free(IMG_Animation* anim)
        {
            try {
                animation result{
                    .width = anim->w,
                    .height = anim->h,
                    .count = anim->count,
                    .delays = {anim->delays, anim->delays + anim->count}
                };
                result.frames.reserve(anim->count);
                for (int i = 0; i < anim->count; ++i) {
                    result.frames.emplace_back(anim->frames[i]);
                    anim->frames[i] = nullptr;
                }
                IMG_FreeAnimation(anim);
                return result;
            }
            catch (...) {
                IMG_FreeAnimation(anim);
                throw;
            }
        }

    } // namespace


    animation
    load_animation(const path& filename)
    {
        auto anim = IMG_LoadAnimation(filename.c_str());
        if (!anim)
            throw unexpected{error{}};
        return convert_and_free(anim);
    }


    animation
    load_animation(SDL_RWops* src,
                   bool close_src)
    {
        auto anim = IMG_LoadAnimation_RW(src, close_src);
        if (!anim)
            throw error{};
        return convert_and_free(anim);
    }


    animation
    load_animation(SDL_RWops* src,
                   bool close_src,
                   const char* type)
    {
        auto anim = IMG_LoadAnimationTyped_RW(src, close_src, type);
        if (!anim)
            throw error{};
        return convert_and_free(anim);
    }


    expected<animation, error>
    try_load_animation(const path& filename)
        noexcept
    {
        auto anim = IMG_LoadAnimation(filename.c_str());
        if (!anim)
            return unexpected{error{}};
        try {
            return convert_and_free(anim);
        }
        catch (std::exception& e) {
            return unexpected{error{e}};
        }
    }


    expected<animation, error>
    try_load_animation(SDL_RWops* src,
                       bool close_src)
        noexcept
    {
        auto anim = IMG_LoadAnimation_RW(src, close_src);
        if (!anim)
            return unexpected{error{}};
        try {
            return convert_and_free(anim);
        }
        catch (std::exception& e) {
            return unexpected{error{e}};
        }
    }


    expected<animation, error>
    try_load_animation(SDL_RWops* src,
                       bool close_src,
                       const char* type)
        noexcept
    {
        auto anim = IMG_LoadAnimationTyped_RW(src, close_src, type);
        if (!anim)
            return unexpected{error{}};
        try {
            return convert_and_free(anim);
        }
        catch (std::exception& e) {
            return unexpected{error{e}};
        }
    }


    animation
    load_gif_animation(SDL_RWops* src)
    {
        auto anim = IMG_LoadGIFAnimation_RW(src);
        if (!anim)
            throw error{};
        return convert_and_free(anim);
    }


    expected<animation, error>
    try_load_gif_animation(SDL_RWops* src)
        noexcept
    {
        auto anim = IMG_LoadGIFAnimation_RW(src);
        if (!anim)
            return unexpected{error{}};
        try {
            return convert_and_free(anim);
        }
        catch (std::exception& e) {
            return unexpected{error{e}};
        }
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

} // namespace sdl::img
