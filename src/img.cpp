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


    surface
    load(rwops& src,
         const char* type)
    {
        return load(src.data(), false, type);
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


    expected<surface, error>
    try_load(rwops& src,
             const char* type)
        noexcept
    {
        return try_load(src.data(), false, type);
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


    surface
    load(rwops& src)
    {
        return load(src.data(), false);
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


    expected<surface, error>
    try_load(rwops& src)
        noexcept
    {
        return try_load(src.data(), false);
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


    texture
    load_texture(renderer& ren,
                 rwops& src)
    {
        return load_texture(ren, src.data(), false);
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


    expected<texture, error>
    try_load_texture(renderer& ren,
                     rwops& src)
        noexcept
    {
        return try_load_texture(ren, src.data(), false);
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


    texture
    load_texture(renderer& ren,
                 rwops& src,
                 const char* type)
    {
        return load_texture(ren, src.data(), false, type);
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


    expected<texture, error>
    try_load_texture(renderer& ren,
                     rwops& src,
                     const char* type)
        noexcept
    {
        return try_load_texture(ren, src.data(), false, type);
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    bool
    is_avif(SDL_RWops* src)
        noexcept
    {
        return IMG_isAVIF(src);
    }


    bool
    is_avif(rwops& src)
        noexcept
    {
        return is_avif(src.data());
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    bool
    is_bmp(SDL_RWops* src)
        noexcept
    {
        return IMG_isBMP(src);
    }


    bool
    is_bmp(rwops& src)
        noexcept
    {
        return is_bmp(src.data());
    }


    bool
    is_cur(SDL_RWops* src)
        noexcept
    {
        return IMG_isCUR(src);
    }


    bool
    is_cur(rwops& src)
        noexcept
    {
        return is_cur(src.data());
    }


    bool
    is_gif(SDL_RWops* src)
        noexcept
    {
        return IMG_isGIF(src);
    }


    bool
    is_gif(rwops& src)
        noexcept
    {
        return is_gif(src.data());
    }


    bool
    is_ico(SDL_RWops* src)
        noexcept
    {
        return IMG_isICO(src);
    }


    bool
    is_ico(rwops& src)
        noexcept
    {
        return is_ico(src.data());
    }


    bool
    is_jpg(SDL_RWops* src)
        noexcept
    {
        return IMG_isJPG(src);
    }


    bool
    is_jpg(rwops& src)
        noexcept
    {
        return is_jpg(src.data());
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    bool
    is_jxl(SDL_RWops* src)
        noexcept
    {
        return IMG_isJXL(src);
    }


    bool
    is_jxl(rwops& src)
        noexcept
    {
        return is_jxl(src.data());
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    bool
    is_lbm(SDL_RWops* src)
        noexcept
    {
        return IMG_isLBM(src);
    }


    bool
    is_lbm(rwops& src)
        noexcept
    {
        return is_lbm(src.data());
    }


    bool
    is_pcx(SDL_RWops* src)
        noexcept
    {
        return IMG_isPCX(src);
    }


    bool
    is_pcx(rwops& src)
        noexcept
    {
        return is_pcx(src.data());
    }


    bool
    is_png(SDL_RWops* src)
        noexcept
    {
        return IMG_isPNG(src);
    }


    bool
    is_png(rwops& src)
        noexcept
    {
        return is_png(src.data());
    }


    bool
    is_pnm(SDL_RWops* src)
        noexcept
    {
        return IMG_isPNM(src);
    }


    bool
    is_pnm(rwops& src)
        noexcept
    {
        return is_pnm(src.data());
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)

    bool
    is_svg(SDL_RWops* src)
        noexcept
    {
        return IMG_isSVG(src);
    }


    bool
    is_svg(rwops& src)
            noexcept
    {
        return is_svg(src.data());
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    bool
    is_qoi(SDL_RWops* src)
        noexcept
    {
        return IMG_isQOI(src);
    }


    bool
    is_qoi(rwops& src)
        noexcept
    {
        return is_qoi(src.data());
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    bool
    is_tif(SDL_RWops* src)
        noexcept
    {
        return IMG_isTIF(src);
    }


    bool
    is_tif(rwops& src)
        noexcept
    {
        return is_tif(src.data());
    }


    bool
    is_xcf(SDL_RWops* src)
        noexcept
    {
        return IMG_isXCF(src);
    }


    bool
    is_xcf(rwops& src)
        noexcept
    {
        return is_xcf(src.data());
    }


    bool
    is_xpm(SDL_RWops* src)
        noexcept
    {
        return IMG_isXPM(src);
    }


    bool
    is_xpm(rwops& src)
        noexcept
    {
        return is_xpm(src.data());
    }


    bool
    is_xv(SDL_RWops* src)
        noexcept
    {
        return IMG_isXV(src);
    }


    bool
    is_xv(rwops& src)
        noexcept
    {
        return is_xv(src.data());
    }


    bool
    is_webp(SDL_RWops* src)
        noexcept
    {
        return IMG_isWEBP(src);
    }


    bool
    is_webp(rwops& src)
        noexcept
    {
        return is_webp(src.data());
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
    load_avif(rwops& src)
    {
        return load_avif(src.data());
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
    try_load_avif(rwops& src)
        noexcept
    {
        return try_load_avif(src.data());
    }


    expected<surface, error>
    try_load_avif(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_avif(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_bmp(rwops& src)
    {
        return load_bmp(src.data());
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
    try_load_bmp(rwops& src)
        noexcept
    {
        return try_load_bmp(src.data());
    }


    expected<surface, error>
    try_load_bmp(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_bmp(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_cur(rwops& src)
    {
        return load_cur(src.data());
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
    try_load_cur(rwops& src)
        noexcept
    {
        return try_load_cur(src.data());
    }


    expected<surface, error>
    try_load_cur(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_cur(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_gif(rwops& src)
    {
        return load_gif(src.data());
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
    try_load_gif(rwops& src)
        noexcept
    {
        return try_load_gif(src.data());
    }


    expected<surface, error>
    try_load_gif(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_gif(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_ico(rwops& src)
    {
        return load_ico(src.data());
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
    try_load_ico(rwops& src)
        noexcept
    {
        return try_load_ico(src.data());
    }


    expected<surface, error>
    try_load_ico(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_ico(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_jpg(rwops& src)
    {
        return load_jpg(src.data());
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
    try_load_jpg(rwops& src)
        noexcept
    {
        return try_load_jpg(src.data());
    }


    expected<surface, error>
    try_load_jpg(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_jpg(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_jxl(rwops& src)
    {
        return load_jxl(src.data());
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
    try_load_jxl(rwops& src)
        noexcept
    {
        return try_load_jxl(src.data());
    }


    expected<surface, error>
    try_load_jxl(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_jxl(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_lbm(rwops& src)
    {
        return load_lbm(src.data());
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
    try_load_lbm(rwops& src)
        noexcept
    {
        return try_load_lbm(src.data());
    }


    expected<surface, error>
    try_load_lbm(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_lbm(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_pcx(rwops& src)
    {
        return load_pcx(src.data());
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
    try_load_pcx(rwops& src)
        noexcept
    {
        return try_load_pcx(src.data());
    }


    expected<surface, error>
    try_load_pcx(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_pcx(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_png(rwops& src)
    {
        return load_png(src.data());
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
    try_load_png(rwops& src)
        noexcept
    {
        return try_load_png(src.data());
    }


    expected<surface, error>
    try_load_png(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_png(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_pnm(rwops& src)
    {
        return load_pnm(src.data());
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
    try_load_pnm(rwops& src)
        noexcept
    {
        return try_load_pnm(src.data());
    }


    expected<surface, error>
    try_load_pnm(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_pnm(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_svg(rwops& src)
    {
        return load_svg(src.data());
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
    try_load_svg(rwops& src)
        noexcept
    {
        return try_load_svg(src.data());
    }


    expected<surface, error>
    try_load_svg(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_svg(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_svg(rwops& src,
             int width,
             int height)
    {
        return load_svg(src.data(), width, height);
    }


    surface
    load_svg(SDL_RWops* src,
             vec2 size)
    {
        return load_svg(src, size.x, size.y);
    }


    surface
    load_svg(rwops& src,
             vec2 size)
    {
        return load_svg(src.data(), size);
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
    try_load_svg(rwops& src,
                 int width,
                 int height)
        noexcept
    {
        return try_load_svg(src.data(), width, height);
    }


    expected<surface, error>
    try_load_svg(SDL_RWops* src,
                  vec2 size)
        noexcept
    {
        return try_load_svg(src, size.x, size.y);
    }


    expected<surface, error>
    try_load_svg(rwops& src,
                 vec2 size)
        noexcept
    {
        return try_load_svg(src.data(), size);
    }


    expected<surface, error>
    try_load_svg(const path& filename,
                 int width,
                 int height)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_svg(src, width, height);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_qoi(rwops& src)
    {
        return load_qoi(src.data());
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
    try_load_qoi(rwops& src)
        noexcept
    {
        return try_load_qoi(src.data());
    }


    expected<surface, error>
    try_load_qoi(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_qoi(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_tif(rwops& src)
    {
        return load_tif(src.data());
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
    try_load_tif(rwops& src)
        noexcept
    {
        return try_load_tif(src.data());
    }


    expected<surface, error>
    try_load_tif(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_tif(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_webp(rwops& src)
    {
        return load_webp(src.data());
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
    try_load_webp(rwops& src)
        noexcept
    {
        return try_load_webp(src.data());
    }


    expected<surface, error>
    try_load_webp(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_webp(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_xcf(rwops& src)
    {
        return load_xcf(src.data());
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
    try_load_xcf(rwops& src)
        noexcept
    {
        return try_load_xcf(src.data());
    }


    expected<surface, error>
    try_load_xcf(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_xcf(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_xpm(rwops& src)
    {
        return load_xpm(src.data());
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
    try_load_xpm(rwops& src)
        noexcept
    {
        return try_load_xpm(src.data());
    }


    expected<surface, error>
    try_load_xpm(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_xpm(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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
    load_xv(rwops& src)
    {
        return load_xv(src.data());
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
    try_load_xv(rwops& src)
        noexcept
    {
        return try_load_xv(src.data());
    }


    expected<surface, error>
    try_load_xv(const path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_xv(src);
        }
        catch (error& e) {
            return unexpected{e};
        }
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


    void
    save_jpg(const surface& src,
             rwops& dst,
             int quality)
    {
        save_jpg(src, dst.data(), false, quality);
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


    expected<void, error>
    try_save_jpg(const surface& src,
                 rwops& dst,
                 int quality)
        noexcept
    {
        return try_save_jpg(src, dst.data(), false, quality);
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


    void
    save_png(const surface& src,
             rwops& dst)
    {
        save_png(src, dst.data(), false);
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


    expected<void, error>
    try_save_png(const surface& src,
                 rwops& dst)
        noexcept
    {
        return try_save_png(src, dst.data(), false);
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
    load_animation(rwops& src)
    {
        return load_animation(src.data(), false);
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


    animation
    load_animation(rwops& src,
                   const char* type)
    {
        return load_animation(src.data(), false, type);
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
    try_load_animation(rwops& src)
        noexcept
    {
        return try_load_animation(src.data(), false);
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


    expected<animation, error>
    try_load_animation(rwops& src,
                       const char* type)
        noexcept
    {
        return try_load_animation(src.data(), false, type);
    }


    animation
    load_gif_animation(SDL_RWops* src)
    {
        auto anim = IMG_LoadGIFAnimation_RW(src);
        if (!anim)
            throw error{};
        return convert_and_free(anim);
    }


    animation
    load_gif_animation(rwops& src)
    {
        return load_gif_animation(src.data());
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


    expected<animation, error>
    try_load_gif_animation(rwops& src)
        noexcept
    {
        return try_load_gif_animation(src.data());
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

} // namespace sdl::img
