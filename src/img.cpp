/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025-2026  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <stdexcept>
#include <utility>

#include "sdl2xx/img.hpp"

#include "impl/value_or_throw.hpp"
#include "sdl2xx/renderer.hpp"


using sdl::impl::value_or_throw;


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
        return value_or_throw(try_load(src, close_src, type));
    }


    surface
    load(rwops& src,
         const char* type)
    {
        return value_or_throw(try_load(src, type));
    }


    std::expected<surface, error>
    try_load(SDL_RWops* src,
             bool close_src,
             const char* type)
        noexcept
    {
        auto surf = IMG_LoadTyped_RW(src, close_src, type);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load(rwops& src,
             const char* type)
        noexcept
    {
        return try_load(src.data(), false, type);
    }


    surface
    load(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load(filename));
    }


    std::expected<surface, error>
    try_load(const std::filesystem::path& filename)
        noexcept
    {
        auto surf = IMG_Load(filename.c_str());
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    surface
    load(SDL_RWops* src,
         bool close_src)
    {
        return value_or_throw(try_load(src, close_src));
    }


    surface
    load(rwops& src)
    {
        return value_or_throw(try_load(src));
    }


    std::expected<surface, error>
    try_load(SDL_RWops* src,
             bool close_src)
        noexcept
    {
        auto surf = IMG_Load_RW(src, close_src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};

    }


    std::expected<surface, error>
    try_load(rwops& src)
        noexcept
    {
        return try_load(src.data(), false);
    }


    texture
    load_texture(renderer& ren,
                 const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_texture(ren, filename));
    }


    std::expected<texture, error>
    try_load_texture(renderer& ren,
                     const std::filesystem::path& filename)
        noexcept
    {
        auto tex = IMG_LoadTexture(ren.data(), filename.c_str());
        if (!tex)
            return std::unexpected{error{}};
        return texture{tex};
    }


    texture
    load_texture(renderer& ren,
                 SDL_RWops* src,
                 bool close_src)
    {
        return value_or_throw(try_load_texture(ren, src, close_src));
    }


    texture
    load_texture(renderer& ren,
                 rwops& src)
    {
        return value_or_throw(try_load_texture(ren, src));
    }


    std::expected<texture, error>
    try_load_texture(renderer& ren,
                     SDL_RWops* src,
                     bool close_src)
        noexcept
    {

        auto tex = IMG_LoadTexture_RW(ren.data(), src, close_src);
        if (!tex)
            return std::unexpected{error{}};
        return texture{tex};
    }


    std::expected<texture, error>
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
        return value_or_throw(try_load_texture(ren, src, close_src, type));
    }


    texture
    load_texture(renderer& ren,
                 rwops& src,
                 const char* type)
    {
        return value_or_throw(try_load_texture(ren, src, type));
    }


    std::expected<texture, error>
    try_load_texture(renderer& ren,
                     SDL_RWops* src,
                     bool close_src,
                     const char* type)
        noexcept
    {
        auto tex = IMG_LoadTextureTyped_RW(ren.data(), src, close_src, type);
        if (!tex)
            return std::unexpected{error{}};
        return texture{tex};
    }


    std::expected<texture, error>
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
        return value_or_throw(try_load_avif(src));
    }


    surface
    load_avif(rwops& src)
    {
        return value_or_throw(try_load_avif(src));
    }


    surface
    load_avif(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_avif(filename));
    }


    std::expected<surface, error>
    try_load_avif(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadAVIF_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_avif(rwops& src)
        noexcept
    {
        return try_load_avif(src.data());
    }


    std::expected<surface, error>
    try_load_avif(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_avif(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    surface
    load_bmp(SDL_RWops* src)
    {
        return value_or_throw(try_load_bmp(src));
    }


    surface
    load_bmp(rwops& src)
    {
        return value_or_throw(try_load_bmp(src));
    }


    surface
    load_bmp(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_bmp(filename));
    }


    std::expected<surface, error>
    try_load_bmp(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadBMP_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_bmp(rwops& src)
        noexcept
    {
        return try_load_bmp(src.data());
    }


    std::expected<surface, error>
    try_load_bmp(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_bmp(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


    surface
    load_cur(SDL_RWops* src)
    {
        return value_or_throw(try_load_cur(src));
    }


    surface
    load_cur(rwops& src)
    {
        return value_or_throw(try_load_cur(src));
    }


    surface
    load_cur(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_cur(filename));
    }


    std::expected<surface, error>
    try_load_cur(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadCUR_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_cur(rwops& src)
        noexcept
    {
        return try_load_cur(src.data());
    }


    std::expected<surface, error>
    try_load_cur(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_cur(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


    surface
    load_gif(SDL_RWops* src)
    {
        return value_or_throw(try_load_gif(src));
    }


    surface
    load_gif(rwops& src)
    {
        return value_or_throw(try_load_gif(src));
    }


    surface
    load_gif(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_gif(filename));
    }


    std::expected<surface, error>
    try_load_gif(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadGIF_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_gif(rwops& src)
        noexcept
    {
        return try_load_gif(src.data());
    }


    std::expected<surface, error>
    try_load_gif(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_gif(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


    surface
    load_ico(SDL_RWops* src)
    {
        return value_or_throw(try_load_ico(src));
    }


    surface
    load_ico(rwops& src)
    {
        return value_or_throw(try_load_ico(src));
    }


    surface
    load_ico(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_ico(filename));
    }


    std::expected<surface, error>
    try_load_ico(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadICO_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_ico(rwops& src)
        noexcept
    {
        return try_load_ico(src.data());
    }


    std::expected<surface, error>
    try_load_ico(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_ico(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


    surface
    load_jpg(SDL_RWops* src)
    {
        return value_or_throw(try_load_jpg(src));
    }


    surface
    load_jpg(rwops& src)
    {
        return value_or_throw(try_load_jpg(src));
    }


    surface
    load_jpg(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_jpg(filename));
    }


    std::expected<surface, error>
    try_load_jpg(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadJPG_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_jpg(rwops& src)
        noexcept
    {
        return try_load_jpg(src.data());
    }


    std::expected<surface, error>
    try_load_jpg(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_jpg(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    surface
    load_jxl(SDL_RWops* src)
    {
        return value_or_throw(try_load_jxl(src));
    }


    surface
    load_jxl(rwops& src)
    {
        return value_or_throw(try_load_jxl(src));
    }


    surface
    load_jxl(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_jxl(filename));
    }


    std::expected<surface, error>
    try_load_jxl(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadJXL_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_jxl(rwops& src)
        noexcept
    {
        return try_load_jxl(src.data());
    }


    std::expected<surface, error>
    try_load_jxl(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_jxl(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    surface
    load_lbm(SDL_RWops* src)
    {
        return value_or_throw(try_load_lbm(src));
    }


    surface
    load_lbm(rwops& src)
    {
        return value_or_throw(try_load_lbm(src));
    }


    surface
    load_lbm(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_lbm(filename));
    }


    std::expected<surface, error>
    try_load_lbm(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadLBM_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_lbm(rwops& src)
        noexcept
    {
        return try_load_lbm(src.data());
    }


    std::expected<surface, error>
    try_load_lbm(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_lbm(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


    surface
    load_pcx(SDL_RWops* src)
    {
        return value_or_throw(try_load_pcx(src));
    }


    surface
    load_pcx(rwops& src)
    {
        return value_or_throw(try_load_pcx(src));
    }


    surface
    load_pcx(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_pcx(filename));
    }


    std::expected<surface, error>
    try_load_pcx(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadPCX_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_pcx(rwops& src)
        noexcept
    {
        return try_load_pcx(src.data());
    }


    std::expected<surface, error>
    try_load_pcx(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_pcx(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


    surface
    load_png(SDL_RWops* src)
    {
        return value_or_throw(try_load_png(src));
    }


    surface
    load_png(rwops& src)
    {
        return value_or_throw(try_load_png(src));
    }


    surface
    load_png(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_png(filename));
    }


    std::expected<surface, error>
    try_load_png(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadPNG_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_png(rwops& src)
        noexcept
    {
        return try_load_png(src.data());
    }


    std::expected<surface, error>
    try_load_png(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_png(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


    surface
    load_pnm(SDL_RWops* src)
    {
        return value_or_throw(try_load_pnm(src));
    }


    surface
    load_pnm(rwops& src)
    {
        return value_or_throw(try_load_pnm(src));
    }


    surface
    load_pnm(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_pnm(filename));
    }


    std::expected<surface, error>
    try_load_pnm(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadPNM_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_pnm(rwops& src)
        noexcept
    {
        return try_load_pnm(src.data());
    }


    std::expected<surface, error>
    try_load_pnm(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_pnm(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)

    surface
    load_svg(SDL_RWops* src)
    {
        return value_or_throw(try_load_svg(src));
    }


    surface
    load_svg(rwops& src)
    {
        return value_or_throw(try_load_svg(src));
    }


    surface
    load_svg(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_svg(filename));
    }


    std::expected<surface, error>
    try_load_svg(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadSVG_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_svg(rwops& src)
        noexcept
    {
        return try_load_svg(src.data());
    }


    std::expected<surface, error>
    try_load_svg(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_svg(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    surface
    load_svg(SDL_RWops* src,
             int width,
             int height)
    {
        return value_or_throw(try_load_svg(src, width, height));
    }


    surface
    load_svg(rwops& src,
             int width,
             int height)
    {
        return value_or_throw(try_load_svg(src, width, height));
    }


    surface
    load_svg(SDL_RWops* src,
             vec2 size)
    {
        return value_or_throw(try_load_svg(src, size));
    }


    surface
    load_svg(rwops& src,
             vec2 size)
    {
        return value_or_throw(try_load_svg(src, size));
    }


    surface
    load_svg(const std::filesystem::path& filename,
             int width,
             int height)
    {
        return value_or_throw(try_load_svg(filename, width, height));
    }


    surface
    load_svg(const std::filesystem::path& filename,
             vec2 size)
    {
        return value_or_throw(try_load_svg(filename, size));
    }


    std::expected<surface, error>
    try_load_svg(SDL_RWops* src,
                 int width,
                 int height)
        noexcept
    {
        auto surf = IMG_LoadSizedSVG_RW(src, width, height);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_svg(rwops& src,
                 int width,
                 int height)
        noexcept
    {
        return try_load_svg(src.data(), width, height);
    }


    std::expected<surface, error>
    try_load_svg(SDL_RWops* src,
                  vec2 size)
        noexcept
    {
        return try_load_svg(src, size.x, size.y);
    }


    std::expected<surface, error>
    try_load_svg(rwops& src,
                 vec2 size)
        noexcept
    {
        return try_load_svg(src.data(), size);
    }


    std::expected<surface, error>
    try_load_svg(const std::filesystem::path& filename,
                 int width,
                 int height)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_svg(src, width, height);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


    std::expected<surface, error>
    try_load_svg(const std::filesystem::path& filename,
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
        return value_or_throw(try_load_qoi(src));
    }


    surface
    load_qoi(rwops& src)
    {
        return value_or_throw(try_load_qoi(src));
    }


    surface
    load_qoi(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_qoi(filename));
    }


    std::expected<surface, error>
    try_load_qoi(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadQOI_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_qoi(rwops& src)
        noexcept
    {
        return try_load_qoi(src.data());
    }


    std::expected<surface, error>
    try_load_qoi(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_qoi(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    surface
    load_tif(SDL_RWops* src)
    {
        return value_or_throw(try_load_tif(src));
    }


    surface
    load_tif(rwops& src)
    {
        return value_or_throw(try_load_tif(src));
    }


    surface
    load_tif(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_tif(filename));
    }


    std::expected<surface, error>
    try_load_tif(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadTIF_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_tif(rwops& src)
        noexcept
    {
        return try_load_tif(src.data());
    }


    std::expected<surface, error>
    try_load_tif(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_tif(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


    surface
    load_webp(SDL_RWops* src)
    {
        return value_or_throw(try_load_webp(src));
    }


    surface
    load_webp(rwops& src)
    {
        return value_or_throw(try_load_webp(src));
    }


    surface
    load_webp(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_webp(filename));
    }


    std::expected<surface, error>
    try_load_webp(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadWEBP_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_webp(rwops& src)
        noexcept
    {
        return try_load_webp(src.data());
    }


    std::expected<surface, error>
    try_load_webp(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_webp(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


    surface
    load_xcf(SDL_RWops* src)
    {
        return value_or_throw(try_load_xcf(src));
    }


    surface
    load_xcf(rwops& src)
    {
        return value_or_throw(try_load_xcf(src));
    }


    surface
    load_xcf(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_xcf(filename));
    }


    std::expected<surface, error>
    try_load_xcf(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadXCF_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_xcf(rwops& src)
        noexcept
    {
        return try_load_xcf(src.data());
    }


    std::expected<surface, error>
    try_load_xcf(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_xcf(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


    surface
    load_xpm(SDL_RWops* src)
    {
        return value_or_throw(try_load_xpm(src));
    }


    surface
    load_xpm(rwops& src)
    {
        return value_or_throw(try_load_xpm(src));
    }


    surface
    load_xpm(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_xpm(filename));
    }


    surface
    load_xpm(char* xpm[])
    {
        return value_or_throw(try_load_xpm(xpm));
    }


    std::expected<surface, error>
    try_load_xpm(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadXPM_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_xpm(rwops& src)
        noexcept
    {
        return try_load_xpm(src.data());
    }


    std::expected<surface, error>
    try_load_xpm(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_xpm(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


    std::expected<surface, error>
    try_load_xpm(char* xpm[])
        noexcept
    {
        auto surf = IMG_ReadXPMFromArray(xpm);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    surface
    load_xpm_to_rgb888(char* xpm[])
    {
        return value_or_throw(try_load_xpm_to_rgb888(xpm));
    }


    std::expected<surface, error>
    try_load_xpm_to_rgb888(char* xpm[])
        noexcept
    {
        auto surf = IMG_ReadXPMFromArrayToRGB888(xpm);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)


    surface
    load_xv(SDL_RWops* src)
    {
        return value_or_throw(try_load_xv(src));
    }


    surface
    load_xv(rwops& src)
    {
        return value_or_throw(try_load_xv(src));
    }


    surface
    load_xv(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_xv(filename));
    }


    std::expected<surface, error>
    try_load_xv(SDL_RWops* src)
        noexcept
    {
        auto surf = IMG_LoadXV_RW(src);
        if (!surf)
            return std::unexpected{error{}};
        return surface{surf};
    }


    std::expected<surface, error>
    try_load_xv(rwops& src)
        noexcept
    {
        return try_load_xv(src.data());
    }


    std::expected<surface, error>
    try_load_xv(const std::filesystem::path& filename)
        noexcept
    {
        try {
            rwops src{filename, "rb"};
            return try_load_xv(src);
        }
        catch (error& e) {
            return std::unexpected{e};
        }
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)

    void
    save_jpg(const surface& src,
             const std::filesystem::path& filename,
             int quality)
    {
        return value_or_throw(try_save_jpg(src, filename, quality));
    }


    void
    save_jpg(const surface& src,
             SDL_RWops* dst,
             bool close_dst,
             int quality)
    {
        return value_or_throw(try_save_jpg(src, dst, close_dst, quality));
    }


    void
    save_jpg(const surface& src,
             rwops& dst,
             int quality)
    {
        return value_or_throw(try_save_jpg(src, dst, quality));
    }


    std::expected<void, error>
    try_save_jpg(const surface& src,
                 const std::filesystem::path& filename,
                 int quality)
        noexcept
    {
        if (IMG_SaveJPG(const_cast<SDL_Surface*>(src.data()),
                        filename.c_str(),
                        quality) < 0)
            return std::unexpected{error{}};
        return {};
    }


    std::expected<void, error>
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
            return std::unexpected{error{}};
        return {};
    }


    std::expected<void, error>
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
             const std::filesystem::path& filename)
    {
        return value_or_throw(try_save_png(src, filename));
    }


    void
    save_png(const surface& src,
             SDL_RWops* dst,
             bool close_dst)
    {
        return value_or_throw(try_save_png(src, dst, close_dst));
    }


    void
    save_png(const surface& src,
             rwops& dst)
    {
        return value_or_throw(try_save_png(src, dst));
    }


    std::expected<void, error>
    try_save_png(const surface& src,
                 const std::filesystem::path& filename)
        noexcept
    {
        if (IMG_SavePNG(const_cast<SDL_Surface*>(src.data()),
                        filename.c_str()) < 0)
            return std::unexpected{error{}};
        return {};
    }


    std::expected<void, error>
    try_save_png(const surface& src,
                 SDL_RWops* dst,
                 bool close_dst)
        noexcept
    {
        if (IMG_SavePNG_RW(const_cast<SDL_Surface*>(src.data()),
                           dst,
                           close_dst) < 0)
            return std::unexpected{error{}};
        return {};
    }


    std::expected<void, error>
    try_save_png(const surface& src,
                 rwops& dst)
        noexcept
    {
        return try_save_png(src, dst.data(), false);
    }


#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

    animation
    load_animation(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_animation(filename));
    }


    animation
    load_animation(SDL_RWops* src,
                   bool close_src)
    {
        return value_or_throw(try_load_animation(src, close_src));
    }


    animation
    load_animation(rwops& src)
    {
        return value_or_throw(try_load_animation(src));
    }


    animation
    load_animation(SDL_RWops* src,
                   bool close_src,
                   const char* type)
    {
        return value_or_throw(try_load_animation(src, close_src, type));
    }


    animation
    load_animation(rwops& src,
                   const char* type)
    {
        return value_or_throw(try_load_animation(src, type));
    }


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


    std::expected<animation, error>
    try_load_animation(const std::filesystem::path& filename)
        noexcept
    {
        auto anim = IMG_LoadAnimation(filename.c_str());
        if (!anim)
            return std::unexpected{error{}};
        try {
            return convert_and_free(anim);
        }
        catch (std::exception& e) {
            return std::unexpected{error{e}};
        }
    }


    std::expected<animation, error>
    try_load_animation(SDL_RWops* src,
                       bool close_src)
        noexcept
    {
        auto anim = IMG_LoadAnimation_RW(src, close_src);
        if (!anim)
            return std::unexpected{error{}};
        try {
            return convert_and_free(anim);
        }
        catch (std::exception& e) {
            return std::unexpected{error{e}};
        }
    }


    std::expected<animation, error>
    try_load_animation(rwops& src)
        noexcept
    {
        return try_load_animation(src.data(), false);
    }


    std::expected<animation, error>
    try_load_animation(SDL_RWops* src,
                       bool close_src,
                       const char* type)
        noexcept
    {
        auto anim = IMG_LoadAnimationTyped_RW(src, close_src, type);
        if (!anim)
            return std::unexpected{error{}};
        try {
            return convert_and_free(anim);
        }
        catch (std::exception& e) {
            return std::unexpected{error{e}};
        }
    }


    std::expected<animation, error>
    try_load_animation(rwops& src,
                       const char* type)
        noexcept
    {
        return try_load_animation(src.data(), false, type);
    }


    animation
    load_gif_animation(SDL_RWops* src)
    {
        return value_or_throw(try_load_gif_animation(src));
    }


    animation
    load_gif_animation(rwops& src)
    {
        return value_or_throw(try_load_gif_animation(src));
    }


    std::expected<animation, error>
    try_load_gif_animation(SDL_RWops* src)
        noexcept
    {
        auto anim = IMG_LoadGIFAnimation_RW(src);
        if (!anim)
            return std::unexpected{error{}};
        try {
            return convert_and_free(anim);
        }
        catch (std::exception& e) {
            return std::unexpected{error{e}};
        }
    }


    std::expected<animation, error>
    try_load_gif_animation(rwops& src)
        noexcept
    {
        return try_load_gif_animation(src.data());
    }

#endif // SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)

} // namespace sdl::img
