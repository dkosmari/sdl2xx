/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_PIXELS_HPP
#define SDL2XX_PIXELS_HPP

#include <iosfwd>
#include <memory>
#include <span>

#include <SDL_pixels.h>

#include "basic_wrapper.hpp"
#include "color.hpp"
#include "string.hpp"


namespace sdl::pixels {

    enum class pixel_type {
        array_f16 = SDL_PIXELTYPE_ARRAYF16,
        array_f32 = SDL_PIXELTYPE_ARRAYF32,
        array_u16 = SDL_PIXELTYPE_ARRAYU16,
        array_u32 = SDL_PIXELTYPE_ARRAYU32,
        array_u8  = SDL_PIXELTYPE_ARRAYU8,
        index_1   = SDL_PIXELTYPE_INDEX1,
        index_4   = SDL_PIXELTYPE_INDEX4,
        index_8   = SDL_PIXELTYPE_INDEX8,
        packed_16 = SDL_PIXELTYPE_PACKED16,
        packed_32  = SDL_PIXELTYPE_PACKED32,
        packed_8  = SDL_PIXELTYPE_PACKED8,
        unknown   = SDL_PIXELTYPE_UNKNOWN,
    };


    string
    to_string(pixel_type t);


    std::ostream&
    operator <<(std::ostream& out,
                pixel_type t);


    enum class bitmap_order {
        lsb  = SDL_BITMAPORDER_4321,
        msb  = SDL_BITMAPORDER_1234,
        none = SDL_BITMAPORDER_NONE,
    };


    string
    to_string(bitmap_order bo);


    std::ostream&
    operator <<(std::ostream& out,
                bitmap_order bo);



    enum class packed_order {
        abgr = SDL_PACKEDORDER_ABGR,
        argb = SDL_PACKEDORDER_ARGB,
        bgra = SDL_PACKEDORDER_BGRA,
        bgrx = SDL_PACKEDORDER_BGRX,
        none = SDL_PACKEDORDER_NONE,
        rgba = SDL_PACKEDORDER_RGBA,
        rgbx = SDL_PACKEDORDER_RGBX,
        xbgr = SDL_PACKEDORDER_XBGR,
        xrgb = SDL_PACKEDORDER_XRGB,
    };


    string
    to_string(packed_order po);


    std::ostream&
    operator <<(std::ostream& out,
                packed_order po);


    enum class array_order {
        abgr = SDL_ARRAYORDER_ABGR,
        argb = SDL_ARRAYORDER_ARGB,
        bgr  = SDL_ARRAYORDER_BGR,
        bgra = SDL_ARRAYORDER_BGRA,
        none = SDL_ARRAYORDER_NONE,
        rgb  = SDL_ARRAYORDER_RGB,
        rgba = SDL_ARRAYORDER_RGBA,
    };


    string
    to_string(array_order ao);


    std::ostream&
    operator <<(std::ostream& out,
                array_order ao);


    enum class packed_layout {
        layout_1010102 = SDL_PACKEDLAYOUT_1010102,
        layout_1555    = SDL_PACKEDLAYOUT_1555,
        layout_2101010 = SDL_PACKEDLAYOUT_2101010,
        layout_332     = SDL_PACKEDLAYOUT_332,
        layout_4444    = SDL_PACKEDLAYOUT_4444,
        layout_5551    = SDL_PACKEDLAYOUT_5551,
        layout_565     = SDL_PACKEDLAYOUT_565,
        layout_8888    = SDL_PACKEDLAYOUT_8888,
        none           = SDL_PACKEDLAYOUT_NONE,
    };


    string
    to_string(packed_layout pl);


    std::ostream&
    operator <<(std::ostream& out,
                packed_layout pl);


    enum class format_enum : Uint32 {
        abgr_1555    = SDL_PIXELFORMAT_ABGR1555,
        abgr_32      = SDL_PIXELFORMAT_ABGR32,
        abgr_4444    = SDL_PIXELFORMAT_ABGR4444,
        abgr_8888    = SDL_PIXELFORMAT_ABGR8888,
        argb_1555    = SDL_PIXELFORMAT_ARGB1555,
        argb_2101010 = SDL_PIXELFORMAT_ARGB2101010,
        argb_32      = SDL_PIXELFORMAT_ARGB32,
        argb_4444    = SDL_PIXELFORMAT_ARGB4444,
        argb_8888    = SDL_PIXELFORMAT_ARGB8888,
        bgr_24       = SDL_PIXELFORMAT_BGR24,
        bgr_444      = SDL_PIXELFORMAT_BGR444,
        bgr_555      = SDL_PIXELFORMAT_BGR555,
        bgr_565      = SDL_PIXELFORMAT_BGR565,
        bgr_888      = SDL_PIXELFORMAT_BGR888,
        bgra_32      = SDL_PIXELFORMAT_BGRA32,
        bgra_4444    = SDL_PIXELFORMAT_BGRA4444,
        bgra_5551    = SDL_PIXELFORMAT_BGRA5551,
        bgra_8888    = SDL_PIXELFORMAT_BGRA8888,
        bgrx_8888    = SDL_PIXELFORMAT_BGRX8888,
        external_oes = SDL_PIXELFORMAT_EXTERNAL_OES,
        index_1_lsb  = SDL_PIXELFORMAT_INDEX1LSB,
        index_1_msb  = SDL_PIXELFORMAT_INDEX1MSB,
        index_4_lsb  = SDL_PIXELFORMAT_INDEX4LSB,
        index_4_msb  = SDL_PIXELFORMAT_INDEX4MSB,
        index_8      = SDL_PIXELFORMAT_INDEX8,
        iyuv         = SDL_PIXELFORMAT_IYUV,
        nv_12        = SDL_PIXELFORMAT_NV12,
        nv_21        = SDL_PIXELFORMAT_NV21,
        rgb_24       = SDL_PIXELFORMAT_RGB24,
        rgb_332      = SDL_PIXELFORMAT_RGB332,
        rgb_444      = SDL_PIXELFORMAT_RGB444,
        rgb_555      = SDL_PIXELFORMAT_RGB555,
        rgb_565      = SDL_PIXELFORMAT_RGB565,
        rgb_888      = SDL_PIXELFORMAT_RGB888,
        rgba_32      = SDL_PIXELFORMAT_RGBA32,
        rgba_4444    = SDL_PIXELFORMAT_RGBA4444,
        rgba_5551    = SDL_PIXELFORMAT_RGBA5551,
        rgba_8888    = SDL_PIXELFORMAT_RGBA8888,
        rgbx_8888    = SDL_PIXELFORMAT_RGBX8888,
        unknown      = SDL_PIXELFORMAT_UNKNOWN,
        uyvy         = SDL_PIXELFORMAT_UYVY,
        xbgr_1555    = SDL_PIXELFORMAT_XBGR1555,
        xbgr_4444    = SDL_PIXELFORMAT_XBGR4444,
        xbgr_8888    = SDL_PIXELFORMAT_XBGR8888,
        xrgb_1555    = SDL_PIXELFORMAT_XRGB1555,
        xrgb_4444    = SDL_PIXELFORMAT_XRGB4444,
        xrgb_8888    = SDL_PIXELFORMAT_XRGB8888,
        yuy_2        = SDL_PIXELFORMAT_YUY2,
        yv_12        = SDL_PIXELFORMAT_YV12,
        yvyu         = SDL_PIXELFORMAT_YVYU,
    };


    string
    to_string(format_enum fmt);


    std::ostream&
    operator <<(std::ostream& out,
                format_enum fmt);


    struct masks {
        int bpp;
        Uint32 red;
        Uint32 green;
        Uint32 blue;
        Uint32 alpha;
    };


    [[nodiscard]]
    masks
    to_masks(format_enum fmt);


    [[nodiscard]]
    format_enum
    to_format_enum(int bpp,
              Uint32 red,
              Uint32 green,
              Uint32 blue,
              Uint32 alpha)
        noexcept;

    [[nodiscard]]
    format_enum
    to_format_enum(const masks& m)
        noexcept;


    struct palette : basic_wrapper<SDL_Palette*> {

        using parent_type = basic_wrapper<SDL_Palette*>;


        // Inherit constructors.
        using parent_type::parent_type;


        explicit
        palette(unsigned n);


        // Named constructor: from a palette, increase ref
        [[nodiscard]]
        static
        palette
        ref_up(SDL_Palette* p)
            noexcept;


        /// Move constructor.
        palette(palette&& other)
            noexcept = default;


        ~palette()
            noexcept;


        /// Move assignment.
        palette&
        operator =(palette&& other)
            noexcept = default;


        void
        create(unsigned n);


        void
        destroy()
            noexcept;


        void
        set_colors(const color* colors,
                   unsigned first,
                   unsigned n);


        void
        set_colors(std::span<const color> colors,
                   unsigned first);


    }; // struct palette


    struct format : basic_wrapper<SDL_PixelFormat*> {

        using parent_type = basic_wrapper<SDL_PixelFormat*>;


        // Inherit constructors.
        using parent_type::parent_type;


        explicit
        format(format_enum fmt);


        /// Move constructor.
        format(format&& other)
            noexcept = default;


        // Named constructor: from a format, increase ref
        [[nodiscard]]
        static
        format
        ref_up(SDL_PixelFormat* src)
            noexcept;


        ~format()
            noexcept;


        /// Move assignment.
        format&
        operator =(format&& other)
            noexcept = default;


        void
        create(format_enum fmt);


        void
        destroy()
            noexcept;


        // TODO: accessor to all the fields


        [[nodiscard]]
        bool
        has_palette()
            const noexcept;


        [[nodiscard]]
        palette
        get_palette();


        void
        set_palette(palette& p);


        [[nodiscard]]
        Uint32
        map_rgb(Uint8 r,
                Uint8 g,
                Uint8 b)
            const noexcept;

        [[nodiscard]]
        Uint32
        map_rgb(color c)
            const noexcept;


        [[nodiscard]]
        Uint32
        map_rgba(Uint8 r,
                 Uint8 g,
                 Uint8 b,
                 Uint8 a)
            const noexcept;

        [[nodiscard]]
        Uint32
        map_rgba(color c)
            const noexcept;


        [[nodiscard]]
        color
        get_rgb(Uint32 pixel)
            const noexcept;


        [[nodiscard]]
        color
        get_rgba(Uint32 pixel)
            const noexcept;


    }; // struct format


    void
    calculate_gamma_ramp(float gamma,
                         std::span<Uint16, 256> ramp)
        noexcept;

} // namespace sdl::pixels

#endif
