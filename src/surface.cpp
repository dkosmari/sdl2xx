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


    surface::surface(int width,
                     int height,
                     int depth,
                     Uint32 r_mask,
                     Uint32 g_mask,
                     Uint32 b_mask,
                     Uint32 a_mask)
    {
        create(width, height, depth, r_mask, g_mask, b_mask, a_mask);
    }


    surface::surface(int width,
                     int height,
                     int depth,
                     pixels::format_enum fmt)
    {
        create(width,
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
                     const pixels::format& fmt)
    {
        create(other, fmt);
    }


    surface::surface(const surface& other,
                     pixels::format_enum fmt)
    {
        create(other, fmt);
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
        base_type{}
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
    surface::create(int width,
                    int height,
                    int depth,
                    Uint32 r_mask,
                    Uint32 g_mask,
                    Uint32 b_mask,
                    Uint32 a_mask)
    {
        auto ptr = SDL_CreateRGBSurface(0,
                                        width, height,
                                        depth,
                                        r_mask, g_mask, b_mask, a_mask);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    surface::create(int width,
                    int height,
                    int depth,
                    pixels::format_enum fmt)
    {
        auto ptr = SDL_CreateRGBSurfaceWithFormat(0,
                                                  width, height,
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
        auto ptr = SDL_CreateRGBSurfaceFrom(pixels, width, height,
                                            depth, pitch,
                                            r_mask, g_mask, b_mask, a_mask);
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
                                                      width, height,
                                                      depth, pitch,
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
                    const pixels::format& fmt)
    {
        auto ptr = SDL_ConvertSurface(other.raw, fmt.data(), 0);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    surface::create(const surface& other,
                    pixels::format_enum fmt)
    {
        auto ptr = SDL_ConvertSurfaceFormat(other.raw,
                                            static_cast<SDL_PixelFormatEnum>(fmt),
                                            0);
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
    surface::acquire(state_type state)
        noexcept
    {
        base_type::acquire({
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


    surface::state_type
    surface::release()
        noexcept
    {
        return std::tuple_cat(base_type::release(),
                              std::tuple(set_user_data(nullptr)));
    }


    pixels::format
    surface::get_format()
        const noexcept
    {
        return pixels::format::ref_up(raw->format);
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
        const
    {
        if (!try_lock())
            throw error{};
    }


    bool
    surface::try_lock()
        const noexcept
    {
        return !SDL_LockSurface(raw);
    }


    void
    surface::unlock()
        const noexcept
    {
        SDL_UnlockSurface(raw);
    }


    surface::locker::locker(const surface* s) :
        base_type{s}
    {
        lock();
    }


    surface::locker::locker(const surface& s) :
        locker{&s}
    {}


    surface::locker::locker(const surface* s,
                            std::adopt_lock_t)
        noexcept :
        base_type{s}
    {
        if (is_guarded() && guarded->is_locked())
            locked = true;
    }


    surface::locker::locker(const surface& s,
                            std::adopt_lock_t adopt)
        noexcept :
        locker{&s, adopt}
    {}


    surface::locker::locker(const surface* s,
                            std::try_to_lock_t)
        noexcept :
        base_type{s}
    {
        try_lock();
    }


    surface::locker::locker(const surface& s,
                            std::try_to_lock_t attempt)
        noexcept :
        locker{&s, attempt}
    {}


    surface::locker::~locker()
        noexcept
    {
        unlock();
    }


    void
    surface::locker::lock()
    {
        if (!is_guarded())
            return;
        guarded->lock();
        locked = true;
    }


    bool
    surface::locker::try_lock()
        noexcept
    {
        if (!is_guarded())
            return false;
        locked = guarded->try_lock();
        return locked;
    }


    void
    surface::locker::unlock()
        noexcept
    {
        if (!is_guarded())
            return;
        if (!is_locked())
            return;
        guarded->unlock();
        locked = false;
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
    surface::load_bmp(rwops& src)
    {
        return load_bmp(src.data(), false);
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
                      bool close_dst)
    {
        if (SDL_SaveBMP_RW(raw, dst, close_dst) < 0)
            throw error{};
    }


    void
    surface::save_bmp(rwops& dst)
    {
        save_bmp(dst.data(), false);
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
    surface::fill(Uint32 pixel)
    {
        if (SDL_FillRect(raw, nullptr, pixel) < 0)
            throw error{};
    }


    void
    surface::fill(color c)
    {
        auto fmt = get_format();
        Uint32 pixel = fmt.map_rgba(c);
        fill(pixel);
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


    color
    surface::read_pixel(int x, int y)
        const
    {
        color result;
        if (x < 0 || x >= get_width())
            throw error{"invalid argument: x"};
        if (y < 0 || y >= get_height())
            throw error{"invalid argument: y"};

        auto fmt = get_format();
        Uint8 pixel_size = fmt.get_pixel_size_bytes();

        locker guard{this, std::defer_lock};
        if (must_lock())
            guard.lock();

        auto ptr = get_pixels_as<Uint8>() + y * get_pitch() + x * pixel_size;

        if (!SDL_ISPIXELFORMAT_FOURCC(fmt.get_enum_raw())) {

            // Note: all pixel formats in SDL2 are 4 bytes or less.
            // Note: SDL2 doesn't handle pixel formats that don't fall on pixel boundaries
            // very well, so we don't try to handle it here either.
            Uint32 pixel;
            SDL_memcpy(&pixel, ptr, pixel_size);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            // SDL expects pixel bits to be the lower bits, so we need to shift it down
            // on big endian systems.
            pixel >>= 8 * (sizeof pixel) - fmt.get_pixel_size_bits();
#endif
            return fmt.get_rgba(pixel);

        } else {

            // Slow version: convert to argb_8888
            surface converted{*this, pixels::format_enum::argb_8888};
            return converted.read_pixel(x, y);

        }
    }


    color
    surface::read_pixel(vec2 pos)
        const
    {
        return read_pixel(pos.x, pos.y);
    }


    void
    surface::write_pixel(int x, int y,
                         color c)
    {
        rect r{x, y, 1, 1};
        fill(r, c);
    }


    void
    surface::write_pixel(vec2 pos,
                         color c)
    {
        write_pixel(pos.x, pos.y, c);
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


    SDL_YUV_CONVERSION_MODE
    get_yuv_conversion_mode(vec2 size)
        noexcept
    {
        return get_yuv_conversion_mode(size.x, size.y);
    }

} // namespace sdl
