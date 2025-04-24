/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include "surface.hpp"

#include "error.hpp"


namespace sdl {

    void
    surface::link_this()
        noexcept
    {
        if (raw)
            raw->userdata = this;
    }


    surface::surface(Uint32 flags,
                     int width,
                     int height,
                     int depth,
                     Uint32 r_mask,
                     Uint32 g_mask,
                     Uint32 b_mask,
                     Uint32 a_mask)
    {
        create(flags,
               width,
               height,
               depth,
               r_mask,
               g_mask,
               b_mask,
               a_mask);
    }


    surface::surface(Uint32 flags,
                     int width,
                     int height,
                     int depth,
                     SDL_PixelFormatEnum format)
    {
        create(flags,
               width,
               height,
               depth,
               format);
    }


    surface::surface(void* pixels,
                     int width,
                     int height,
                     int depth,
                     int pitch,
                     Uint32 r_mask,
                     Uint32 g_mask,
                     Uint32 b_mask,
                     Uint32 a_mask)
    {
        create(pixels,
               width,
               height,
               depth,
               pitch,
               r_mask,
               g_mask,
               b_mask,
               a_mask);
    }


    surface::surface(void* pixels,
                int width,
                int height,
                int depth,
                int pitch,
                     SDL_PixelFormatEnum format)
    {
        create(pixels,
               width,
               height,
               depth,
               pitch,
               format);
    }


    surface::surface(const surface& other,
                     const SDL_PixelFormat* format,
                     Uint32 flags)
    {
        create(other, format, flags);
    }


    surface::surface(const surface& other,
                     SDL_PixelFormatEnum format,
                     Uint32 flags)
    {
        create(other, format, flags);
    }


    surface::surface(SDL_Surface* surf,
                     dont_destroy_t)
        noexcept
    {
        acquire({surf, nullptr, false});
    }


    surface::surface(surface&& other)
        noexcept
    {
        acquire(other.release());
    }


    surface::surface(const surface& other) :
        basic_wrapper{}
    {
        create(other);
    }


    surface::~surface()
        noexcept
    {
        destroy();
    }


    surface&
    surface::operator =(surface&& other)
        noexcept
    {
        if (this != &other) {
            destroy();
            acquire(other.release());
        }
        return *this;
    }


    surface&
    surface::operator =(const surface& other)
    {
        if (this != &other)
            create(other);
        return *this;
    }


    void
    surface::create(Uint32 flags,
                    int width,
                    int height,
                    int depth,
                    Uint32 r_mask,
                    Uint32 g_mask,
                    Uint32 b_mask,
                    Uint32 a_mask)
    {
        auto ptr = SDL_CreateRGBSurface(flags,
                                        width,
                                        height,
                                        depth,
                                        r_mask,
                                        g_mask,
                                        b_mask,
                                        a_mask);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    surface::create(Uint32 flags,
                    int width,
                    int height,
                    int depth,
                    SDL_PixelFormatEnum format)
    {
        auto ptr = SDL_CreateRGBSurfaceWithFormat(flags,
                                                  width,
                                                  height,
                                                  depth,
                                                  format);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    surface::create(void* pixels,
                    int width,
                    int height,
                    int depth,
                    int pitch,
                    Uint32 r_mask,
                    Uint32 g_mask,
                    Uint32 b_mask,
                    Uint32 a_mask)
    {
        auto ptr = SDL_CreateRGBSurfaceFrom(pixels,
                                            width,
                                            height,
                                            depth,
                                            pitch,
                                            r_mask,
                                            g_mask,
                                            b_mask,
                                            a_mask);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    surface::create(void* pixels,
                    int width,
                    int height,
                    int depth,
                    int pitch,
                    SDL_PixelFormatEnum format)
    {
        auto ptr = SDL_CreateRGBSurfaceWithFormatFrom(pixels,
                                                      width,
                                                      height,
                                                      depth,
                                                      pitch,
                                                      format);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    surface::create(const surface& other)
    {
        auto ptr = SDL_DuplicateSurface(other.raw);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    surface::create(const surface& other,
                    const SDL_PixelFormat* format,
                    Uint32 flags)
    {
        auto ptr = SDL_ConvertSurface(other.raw, format, flags);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    surface::create(const surface& other,
                    SDL_PixelFormatEnum format,
                    Uint32 flags)
    {
        auto ptr = SDL_ConvertSurfaceFormat(other.raw, format, flags);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    surface::destroy()
        noexcept
    {
        if (raw) {
            auto [old_surf, old_user_data, old_owner] = release();
            if (old_owner)
                SDL_FreeSurface(old_surf);
        }

    }


    void
    surface::acquire(const std::tuple<SDL_Surface*, void*, bool>& state)
        noexcept
    {
        basic_wrapper::acquire(get<0>(state));
        user_data = get<1>(state);
        owner = get<2>(state);
        link_this();
    }


    void
    surface::acquire(SDL_Surface* new_raw)
        noexcept
    {
        basic_wrapper::acquire(new_raw);
        user_data = nullptr;
        owner = true;
        link_this();
    }


    std::tuple<SDL_Surface*, void*, bool>
    surface::release()
        noexcept
    {
        auto old_owner = owner;
        user_data = nullptr;
        owner = true;
        return {
            basic_wrapper::release(),
            set_user_data(nullptr),
            old_owner
        };
    }


    Uint32
    surface::get_flags()
        const noexcept
    {
        return raw->flags;
    }


    SDL_PixelFormat*
    surface::get_format()
        const noexcept
    {
        return raw->format;
    }


    int
    surface::get_width()
        const noexcept
    {
        return raw->w;
    }


    int
    surface::get_height()
        const noexcept
    {
        return raw->h;
    }


    vec2
    surface::get_size()
        const noexcept
    {
        return { raw->w, raw->h };
    }


    void*
    surface::get_pixels()
        noexcept
    {
        return raw->pixels;
    }


    const void*
    surface::get_pixels()
        const noexcept
    {
        return raw->pixels;
    }


    void*
    surface::get_user_data()
        const noexcept
    {
        return user_data;
    }


    void*
    surface::set_user_data(void* new_user_data)
        noexcept
    {
        void* old_user_data = user_data;
        user_data = new_user_data;
        return old_user_data;
    }


    int
    surface::get_ref_count()
        const noexcept
    {
        return raw->refcount;
    }


    void
    surface::set_palette(SDL_Palette* palette)
    {
        if (SDL_SetSurfacePalette(raw, palette) < 0)
            throw error{};
    }


    void
    surface::lock()
    {
        if (!try_lock())
            throw error{};
    }


    bool
    surface::try_lock()
        noexcept
    {
        return SDL_LockSurface(raw) >= 0;
    }


    void
    surface::unlock()
        noexcept
    {
        SDL_UnlockSurface(raw);
    }


    surface::lock_guard::lock_guard(surface& s) :
        surf(s)
    {
        surf.lock();
    }


    surface::lock_guard::lock_guard(surface& s,
                                    surface::lock_guard::adopt_lock_t)
        noexcept :
        surf(s)
    {}


    surface::lock_guard::~lock_guard()
        noexcept
    {
        surf.unlock();
    }


    surface
    surface::load_bmp(SDL_RWops* src,
                      bool close_src)
    {
        SDL_Surface* surf = SDL_LoadBMP_RW(src, close_src);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    surface::load_bmp(const path& filename)
    {
        SDL_Surface* surf = SDL_LoadBMP(filename.c_str());
        if (!surf)
            throw error{};
        return surface{surf};
    }


    void
    surface::save_bmp(SDL_RWops* dst,
                      bool close)
    {
        if (SDL_SaveBMP_RW(raw, dst, close) < 0)
            throw error{};
    }


    void
    surface::save_bmp(const path& filename)
    {
        if (SDL_SaveBMP(raw, filename.c_str()) < 0)
            throw error{};
    }


    void
    surface::set_rle(bool rle)
    {
        if (SDL_SetSurfaceRLE(raw, rle) < 0)
            throw error{};
    }


    bool
    surface::has_rle()
        const noexcept
    {
        return SDL_HasSurfaceRLE(raw);
    }


    void
    surface::set_color_key(Uint32 key)
    {
        if (SDL_SetColorKey(raw, SDL_TRUE, key) < 0)
            throw error{};
    }


    void
    surface::unset_color_key()
    {
        if (SDL_SetColorKey(raw, SDL_FALSE, 0) < 0)
            throw error{};
    }


    bool
    surface::has_color_key()
        const noexcept
    {
        return SDL_HasColorKey(raw);
    }


    Uint32
    surface::get_color_key()
        const
    {
        Uint32 result;
        if (SDL_GetColorKey(raw, &result) < 0)
            throw error{};
        return result;
    }


    void
    surface::set_color_mod(Uint8 r,
                           Uint8 g,
                           Uint8 b)
    {
        if (SDL_SetSurfaceColorMod(raw, r, g, b) < 0)
            throw error{};
    }


    void
    surface::set_color_mod(color c)
    {
        set_color_mod(c.r, c.g, c.b);
    }


    color
    surface::get_color_mod()
        const
    {
        color result;
        if (SDL_GetSurfaceColorMod(raw, &result.r, &result.g, &result.b) < 0)
            throw error{};
        return result;
    }



    void
    surface::set_alpha_mod(Uint8 alpha)
    {
        if (SDL_SetSurfaceAlphaMod(raw, alpha) < 0)
            throw error{};
    }


    Uint8
    surface::get_alpha_mod()
        const
    {
        Uint8 result;
        if (SDL_GetSurfaceAlphaMod(raw, &result) < 0)
            throw error{};
        return result;
    }


    void
    surface::set_blend_mode(SDL_BlendMode mode)
    {
        if (SDL_SetSurfaceBlendMode(raw, mode) < 0)
            throw error{};
    }


    SDL_BlendMode
    surface::get_blend_mode()
        const
    {
        SDL_BlendMode result;
        if (SDL_GetSurfaceBlendMode(raw, &result) < 0)
            throw error{};
        return result;
    }


    bool
    surface::set_clip(const rect& clip)
        noexcept
    {
        return SDL_SetClipRect(raw, &clip);
    }


    bool
    surface::unset_clip()
        noexcept
    {
        return SDL_SetClipRect(raw, nullptr);
    }


    rect
    surface::get_clip()
        const noexcept
    {
        return raw->clip_rect;
    }


    void
    surface::fill(const rect& r,
                  Uint32 c)
    {
        if (SDL_FillRect(raw, &r, c) < 0)
            throw error{};
    }


    void
    surface::fill(std::span<const rect> rs,
                  Uint32 c)
    {
        if (SDL_FillRects(raw, rs.data(), rs.size(), c) < 0)
            throw error{};
    }



    void
    convert_pixels(int width,
                   int height,
                   SDL_PixelFormatEnum src_format,
                   const void* src,
                   int src_pitch,
                   SDL_PixelFormatEnum dst_format,
                   void* dst,
                   int dst_pitch)
    {
        if (SDL_ConvertPixels(width, height,
                              src_format, src, src_pitch,
                              dst_format, dst, dst_pitch) < 0)
            throw error{};
    }


    void
    premultiply_alpha(int width,
                      int height,
                      SDL_PixelFormatEnum src_format,
                      const void* src,
                      int src_pitch,
                      SDL_PixelFormatEnum dst_format,
                      void* dst,
                      int dst_pitch)
    {
        if (SDL_PremultiplyAlpha(width, height,
                                 src_format, src, src_pitch,
                                 dst_format, dst, dst_pitch) < 0)
            throw error{};
    }


    void
    upper_blit(const surface& src, rect* src_rect,
                     surface& dst, rect* dst_rect)
    {
        if (SDL_BlitSurface(const_cast<SDL_Surface*>(src.data()),
                            src_rect,
                            dst.data(),
                            dst_rect) < 0)
            throw error{};
    }


    void
    lower_blit(const surface& src, rect* src_rect,
                     surface& dst, rect* dst_rect)
    {
        if (SDL_LowerBlit(const_cast<SDL_Surface*>(src.data()),
                          src_rect,
                          dst.data(),
                          dst_rect) < 0)
            throw error{};
    }


    void
    soft_stretch(const surface& src, const rect* src_rect,
                       surface& dst, const rect* dst_rect)
    {
        if (SDL_SoftStretch(const_cast<SDL_Surface*>(src.data()),
                            src_rect,
                            dst.data(),
                            dst_rect) < 0)
            throw error{};
    }


    void
    soft_stretch_linear(const surface& src, const rect* src_rect,
                              surface& dst, const rect* dst_rect)
    {
        if (SDL_SoftStretchLinear(const_cast<SDL_Surface*>(src.data()),
                                  src_rect,
                                  dst.data(),
                                  dst_rect) < 0)
            throw error{};
    }


    void
    blit_scaled(const surface& src, const rect* src_rect,
                      surface& dst,       rect* dst_rect)
    {
        if (SDL_BlitScaled(const_cast<SDL_Surface*>(src.data()),
                           src_rect,
                           dst.data(),
                           dst_rect) < 0)
            throw error{};
    }


    void
    lower_blit_scaled(const surface& src, rect* src_rect,
                            surface& dst, rect* dst_rect)
    {
        if (SDL_LowerBlitScaled(const_cast<SDL_Surface*>(src.data()),
                                src_rect,
                                dst.data(),
                                dst_rect) < 0)
            throw error{};
    }


    void
    set_yuv_conversion_mode(SDL_YUV_CONVERSION_MODE mode)
        noexcept
    {
        SDL_SetYUVConversionMode(mode);
    }


    SDL_YUV_CONVERSION_MODE
    get_yuv_conversion_mode()
        noexcept
    {
        return SDL_GetYUVConversionMode();
    }


    SDL_YUV_CONVERSION_MODE
    get_yuv_conversion_mode(int width, int height)
        noexcept
    {
        return SDL_GetYUVConversionModeForResolution(width, height);
    }

} // namespace sdl
