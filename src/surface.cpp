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
                     pixels::format_enum fmt)
    {
        create(flags,
               width,
               height,
               depth,
               fmt);
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
                     pixels::format_enum fmt)
    {
        create(pixels,
               width,
               height,
               depth,
               pitch,
               fmt);
    }


    surface::surface(const surface& other,
                     const pixels::format& fmt,
                     Uint32 flags)
    {
        create(other, fmt, flags);
    }


    surface::surface(const surface& other,
                     pixels::format_enum fmt,
                     Uint32 flags)
    {
        create(other, fmt, flags);
    }


    surface::surface(SDL_Surface* surf,
                     dont_destroy_t)
        noexcept
    {
        acquire(surf, false, nullptr);
    }


    surface::surface(surface&& other)
        noexcept
    {
        acquire(other.release());
    }


    surface::surface(const surface& other) :
        parent_t{}
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
                    pixels::format_enum fmt)
    {
        auto ptr = SDL_CreateRGBSurfaceWithFormat(flags,
                                                  width,
                                                  height,
                                                  depth,
                                                  static_cast<SDL_PixelFormatEnum>(fmt));
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
                    pixels::format_enum fmt)
    {
        auto ptr = SDL_CreateRGBSurfaceWithFormatFrom(pixels,
                                                      width,
                                                      height,
                                                      depth,
                                                      pitch,
                                                      static_cast<SDL_PixelFormatEnum>(fmt));
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    surface::create(const surface& other)
    {
        if (other.raw) {
            auto ptr = SDL_DuplicateSurface(other.raw);
            if (!ptr)
                throw error{};
            destroy();
            acquire(ptr);
        } else
            destroy();
    }


    void
    surface::create(const surface& other,
                    const pixels::format& fmt,
                    Uint32 flags)
    {
        auto ptr = SDL_ConvertSurface(other.raw, fmt.data(), flags);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    surface::create(const surface& other,
                    pixels::format_enum fmt,
                    Uint32 flags)
    {
        auto ptr = SDL_ConvertSurfaceFormat(other.raw,
                                            static_cast<SDL_PixelFormatEnum>(fmt),
                                            flags);
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
            auto [old_raw, old_owner, old_user_data] = release();
            if (old_owner)
                SDL_FreeSurface(old_raw);
        }

    }


    void
    surface::acquire(state_t state)
        noexcept
    {
        parent_t::acquire({
                std::move(get<0>(state)),
                std::move(get<1>(state))
            });
        user_data = get<2>(state);
        link_this();
    }


    void
    surface::acquire(SDL_Surface* new_raw,
                     bool new_owner,
                     void* new_user_data)
        noexcept
    {
        acquire({new_raw, new_owner, new_user_data});
    }


    surface::state_t
    surface::release()
        noexcept
    {
        return std::tuple_cat(parent_t::release(),
                              std::tuple(set_user_data(nullptr)));
    }


    Uint32
    surface::get_flags()
        const noexcept
    {
        return raw->flags;
    }


    pixels::format
    surface::get_format()
        const noexcept
    {
        return pixels::format::ref_up(raw->format);
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
    surface::set_palette(pixels::palette& pal)
    {
        if (SDL_SetSurfacePalette(raw, pal.data()) < 0)
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
    surface::set_color_key(color key)
    {
        auto fmt = get_format();
        Uint32 pixel = fmt.map_rgba(key);
        set_color_key(pixel);
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
                  Uint32 pixel)
    {
        if (SDL_FillRect(raw, &r, pixel) < 0)
            throw error{};
    }


    void
    surface::fill(const rect& r,
                  color c)
    {
        auto fmt = get_format();
        Uint32 pixel = fmt.map_rgba(c);
        fill(r, pixel);
    }


    void
    surface::fill(vec2 a,
                  vec2 b,
                  Uint32 pixel)
    {
        fill(rect::from_corners(a, b), pixel);
    }


    void
    surface::fill(vec2 a,
                  vec2 b,
                  color c)
    {
        auto fmt = get_format();
        Uint32 pixel = fmt.map_rgba(c);
        fill(a, b, pixel);
    }


    void
    surface::fill(std::span<const rect> rs,
                  Uint32 c)
    {
        if (SDL_FillRects(raw, rs.data(), rs.size(), c) < 0)
            throw error{};
    }


    void
    surface::fill(std::span<const rect> rs,
                  color c)
    {
        auto fmt = get_format();
        Uint32 pixel = fmt.map_rgba(c);
        fill(rs, pixel);
    }


    void
    convert_pixels(int width,
                   int height,
                   pixels::format_enum src_format,
                   const void* src,
                   int src_pitch,
                   pixels::format_enum dst_format,
                   void* dst,
                   int dst_pitch)
    {
        if (SDL_ConvertPixels(width,
                              height,
                              static_cast<SDL_PixelFormatEnum>(src_format),
                              src,
                              src_pitch,
                              static_cast<SDL_PixelFormatEnum>(dst_format),
                              dst,
                              dst_pitch) < 0)
            throw error{};
    }


    void
    premultiply_alpha(int width,
                      int height,
                      pixels::format_enum src_format,
                      const void* src,
                      int src_pitch,
                      pixels::format_enum dst_format,
                      void* dst,
                      int dst_pitch)
    {
        if (SDL_PremultiplyAlpha(width,
                                 height,
                                 static_cast<SDL_PixelFormatEnum>(src_format),
                                 src,
                                 src_pitch,
                                 static_cast<SDL_PixelFormatEnum>(dst_format),
                                 dst,
                                 dst_pitch) < 0)
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
