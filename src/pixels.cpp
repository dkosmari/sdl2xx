/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <ostream>

#include "pixels.hpp"

#include "error.hpp"


namespace sdl::pixels {


    string
    to_string(pixel_type t)
    {
        switch (t) {
            case pixel_type::array_f16:
                return "array_f16";
            case pixel_type::array_f32:
                return "array_f32";
            case pixel_type::array_u16:
                return "array_u16";
            case pixel_type::array_u32:
                return "array_u32";
            case pixel_type::array_u8:
                return "array_u8";
            case pixel_type::index_1:
                return "index_1";
            case pixel_type::index_4:
                return "index_4";
            case pixel_type::index_8:
                return "index_8";
            case pixel_type::packed_16:
                return "packed_16";
            case pixel_type::packed_32:
                return "packed_32";
            case pixel_type::packed_8:
                return "packed_8";
            case pixel_type::unknown:
                return "unknown";
            default:
                return "invalid";
        }
    }


    std::ostream&
    operator <<(std::ostream& out,
                pixel_type t)
    {
        return out << to_string(t);
    }



    string
    to_string(bitmap_order bo)
    {
        switch (bo) {
            case bitmap_order::lsb:
                return "lsb";
            case bitmap_order::msb:
                return "msb";
            case bitmap_order::none:
                return "none";
            default:
                return "invalid";
        }
    }


    std::ostream&
    operator <<(std::ostream& out,
                bitmap_order bo)
    {
        return out << to_string(bo);
    }


    string
    to_string(packed_order po)
    {
        switch (po) {
            case packed_order::abgr:
                return "abgr";
            case packed_order::argb:
                return "argb";
            case packed_order::bgra:
                return "bgra";
            case packed_order::bgrx:
                return "bgrx";
            case packed_order::none:
                return "none";
            case packed_order::rgba:
                return "rgba";
            case packed_order::rgbx:
                return "rgbx";
            case packed_order::xbgr:
                return "xbgr";
            case packed_order::xrgb:
                return "xrgb";
            default:
                return "invalid";
        }
    }


    std::ostream&
    operator <<(std::ostream& out,
                packed_order po)
    {
        return out << to_string(po);
    }


    string
    to_string(array_order ao)
    {
        switch (ao) {
            case array_order::abgr:
                return "abgr";
            case array_order::argb:
                return "argb";
            case array_order::bgr:
                return "bgr";
            case array_order::bgra:
                return "bgra";
            case array_order::none:
                return "none";
            case array_order::rgb:
                return "rgb";
            case array_order::rgba:
                return "rgba";
            default:
                return "invalid";
            }
    }


    std::ostream&
    operator <<(std::ostream& out,
                array_order ao)
    {
        return out << to_string(ao);
    }


    string
    to_string(packed_layout pl)
    {
        switch (pl) {
            case packed_layout::layout_1010102:
                return "1010102";
            case packed_layout::layout_1555:
                return "1555";
            case packed_layout::layout_2101010:
                return "2101010";
            case packed_layout::layout_332:
                return "332";
            case packed_layout::layout_4444:
                return "4444";
            case packed_layout::layout_5551:
                return "5551";
            case packed_layout::layout_565:
                return "565";
            case packed_layout::layout_8888:
                return "8888";
            case packed_layout::none:
                return "none";
            default:
                return "invalid";
        }
    }


    std::ostream&
    operator <<(std::ostream& out,
                packed_layout pl)
    {
        return out << to_string(pl);
    }


#if 0

    string
    to_string(format_enum fmt)
    {
        switch (fmt) {
            case format_enum::abgr_1555:
                return "abgr_1555";
            case format_enum::abgr_4444:
                return "abgr_4444";
            case format_enum::abgr_8888:
                return "abgr_8888";
            case format_enum::argb_1555:
                return "argb_1555";
            case format_enum::argb_2101010:
                return "argb_2101010";
            case format_enum::argb_4444:
                return "argb_4444";
            case format_enum::argb_8888:
                return "argb_8888";
            case format_enum::bgr_24:
                return "bgr_24";
            case format_enum::bgr_565:
                return "bgr_565";
            case format_enum::bgra_4444:
                return "bgra_4444";
            case format_enum::bgra_5551:
                return "bgra_5551";
            case format_enum::bgra_8888:
                return "bgra_8888";
            case format_enum::bgrx_8888:
                return "bgrx_8888";
            case format_enum::external_oes:
                return "external_oes";
            case format_enum::index_1_lsb:
                return "index_1_lsb";
            case format_enum::index_1_msb:
                return "index_1_msb";
            case format_enum::index_4_lsb:
                return "index_4_lsb";
            case format_enum::index_4_msb:
                return "index_4_msb";
            case format_enum::index_8:
                return "index_8";
            case format_enum::iyuv:
                return "iyuv";
            case format_enum::nv_12:
                return "nv_12";
            case format_enum::nv_21:
                return "nv_21";
            case format_enum::rgb_24:
                return "rgb_24";
            case format_enum::rgb_332:
                return "rgb_332";
            case format_enum::rgb_565:
                return "rgb_565";
            case format_enum::rgba_4444:
                return "rgba_4444";
            case format_enum::rgba_5551:
                return "rgba_5551";
            case format_enum::rgba_8888:
                return "rgba_8888";
            case format_enum::rgbx_8888:
                return "rgbx_8888";
            case format_enum::unknown:
                return "unknown";
            case format_enum::uyvy:
                return "uyvy";
            case format_enum::xbgr_1555:
                return "xbgr_1555";
            case format_enum::xbgr_4444:
                return "xbgr_4444";
            case format_enum::xbgr_8888:
                return "xbgr_8888";
            case format_enum::xrgb_1555:
                return "xrgb_1555";
            case format_enum::xrgb_4444:
                return "xrgb_4444";
            case format_enum::xrgb_8888:
                return "xrgb_8888";
            case format_enum::yuy_2:
                return "yuy_2";
            case format_enum::yv_12:
                return "yv_12";
            case format_enum::yvyu:
                return "yvyu";
            default:
                return "invalid";
        }
    }

#else

    string
    to_string(format_enum fmt)
    {
        return SDL_GetPixelFormatName(static_cast<SDL_PixelFormatEnum>(fmt));
    }

#endif


    std::ostream&
    operator <<(std::ostream& out,
                format_enum fmt)
    {
        return out << to_string(fmt);
    }


    masks
    to_masks(format_enum fmt)
    {
        int bpp;
        Uint32 red;
        Uint32 green;
        Uint32 blue;
        Uint32 alpha;
        if (!SDL_PixelFormatEnumToMasks(static_cast<Uint32>(fmt),
                                        &bpp,
                                        &red,
                                        &green,
                                        &blue,
                                        &alpha))
            throw error{};
        return {bpp, red, green, blue, alpha};
    }


    format_enum
    to_format_enum(int bpp,
              Uint32 red,
              Uint32 green,
              Uint32 blue,
              Uint32 alpha)
        noexcept
    {
        return static_cast<format_enum>(SDL_MasksToPixelFormatEnum(bpp,
                                                              red,
                                                              green,
                                                              blue,
                                                              alpha));
    }


    format_enum
    to_format_enum(const masks& m)
        noexcept
    {
        return to_format_enum(m.bpp, m.red, m.green, m.blue, m.alpha);
    }


    palette::palette(unsigned n)
    {
        create(n);
    }


    palette
    palette::ref_up(SDL_Palette* p)
        noexcept
    {
        ++p->refcount;
        return palette{p};
    }


    palette::~palette()
        noexcept
    {
        destroy();
    }


    void
    palette::create(unsigned n)
    {
        auto p = SDL_AllocPalette(n);
        if (!p)
            throw error{};
        destroy();
        acquire(p);
    }


    void
    palette::destroy()
        noexcept
    {
        if (is_valid())
            SDL_FreePalette(release());
    }


    void
    palette::set_colors(const color* colors,
                        unsigned first,
                        unsigned n)
    {
        if (SDL_SetPaletteColors(raw, colors, first, n) < 0)
            throw error{};
    }


    void
    palette::set_colors(std::span<const color> colors,
                        unsigned first)
    {
        set_colors(colors.data(), first, colors.size());
    }


    format::format(format_enum fmt)
    {
        create(fmt);
    }


    format
    format::ref_up(SDL_PixelFormat* src)
        noexcept
    {
        ++src->refcount;
        return format{src};
    }


    format::~format()
        noexcept
    {
        destroy();
    }


    void
    format::create(format_enum fmt)
    {
        auto ptr = SDL_AllocFormat(static_cast<SDL_PixelFormatEnum>(fmt));
        if (!ptr)
            throw error{};
        acquire(ptr);
    }


    void
    format::destroy()
        noexcept
    {
        if (is_valid())
            SDL_FreeFormat(release());
    }


    bool
    format::has_palette()
        const noexcept
    {
        return raw->palette;
    }


    palette
    format::get_palette()
    {
        if (!raw->palette)
            throw error{"format has no palette"};
        return palette::ref_up(raw->palette);
    }


    void
    format::set_palette(palette& p)
    {
        if (SDL_SetPixelFormatPalette(raw, p.data()) < 0)
            throw error{};
    }


    Uint32
    format::map_rgb(Uint8 r,
                    Uint8 g,
                    Uint8 b)
        const noexcept
    {
        return SDL_MapRGB(raw, r, g, b);
    }


    Uint32
    format::map_rgb(color c)
        const noexcept
    {
        return map_rgb(c.r, c.g, c.b);
    }


    Uint32
    format::map_rgba(Uint8 r,
                     Uint8 g,
                     Uint8 b,
                     Uint8 a)
        const noexcept
    {
        return SDL_MapRGBA(raw, r, g, b, a);
    }


    Uint32
    format::map_rgba(color c)
        const noexcept
    {
        return map_rgba(c.r, c.g, c.b, c.a);
    }


    color
    format::get_rgb(Uint32 pixel)
        const noexcept
    {
        color c;
        SDL_GetRGB(pixel, raw, &c.r, &c.g, &c.b);
        return c;
    }


    color
    format::get_rgba(Uint32 pixel)
        const noexcept
    {
        color c;
        SDL_GetRGBA(pixel, raw, &c.r, &c.g, &c.b, &c.a);
        return c;
    }


    void
    calculate_gamma_ramp(float gamma,
                         std::span<Uint16, 256> ramp)
        noexcept
    {
        SDL_CalculateGammaRamp(gamma, ramp.data());
    }


    void
    calculate_gamma_ramp(float gamma,
                         std::span<Uint16> ramp)
    {
        if (ramp.size() < 256)
            throw error{"ramp length must be >= 256"};
        calculate_gamma_ramp(gamma, std::span<Uint16, 256>(ramp));
    }

} // namespace sdl::pixels
