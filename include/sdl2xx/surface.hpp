/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_SURFACE_HPP
#define SDL2XX_SURFACE_HPP

#include <filesystem>
#include <span>
#include <tuple>

#include <SDL_surface.h>

#include "owner_wrapper.hpp"
#include "color.hpp"
#include "pixels.hpp"
#include "rect.hpp"


namespace sdl {

    using std::filesystem::path;


    class surface : public owner_wrapper<SDL_Surface*> {

        void* user_data = nullptr;


        void
        link_this()
            noexcept;

    public:

        using base_type = owner_wrapper<SDL_Surface*>;

        using state_type = std::tuple<
            std::tuple_element_t<0, base_type::state_type>,
            std::tuple_element_t<1, base_type::state_type>,
            void*
            >;


        struct dont_destroy_t{};
        static constexpr dont_destroy_t dont_destroy{};


        // Inherit constructors.
        using base_type::base_type;


        surface(Uint32 flags,
                int width,
                int height,
                int depth,
                Uint32 r_mask,
                Uint32 g_mask,
                Uint32 b_mask,
                Uint32 a_mask);

        surface(Uint32 flags,
                int width,
                int height,
                int depth,
                pixels::format_enum fmt);

        surface(void* pixels,
                int width,
                int height,
                int depth,
                int pitch,
                Uint32 r_mask,
                Uint32 g_mask,
                Uint32 b_mask,
                Uint32 a_mask);

        surface(void* pixels,
                int width,
                int height,
                int depth,
                int pitch,
                pixels::format_enum fmt);

        surface(const surface& other,
                const pixels::format& fmt,
                Uint32 flags);

        surface(const surface& other,
                pixels::format_enum fmt,
                Uint32 flags);

        surface(SDL_Surface* surf, dont_destroy_t)
            noexcept;

        /// Move constructor.
        surface(surface&& other)
            noexcept;

        /// Copy constructor.
        explicit
        surface(const surface& other);


        ~surface()
            noexcept;


        /// Copy assignment.
        surface&
        operator =(const surface& other);

        /// Move assignment.
        surface&
        operator =(surface&& other)
            noexcept;


        void
        create(Uint32 flags,
               int width,
               int height,
               int depth,
               Uint32 r_mask,
               Uint32 g_mask,
               Uint32 b_mask,
               Uint32 a_mask);

        void
        create(Uint32 flags,
               int width,
               int height,
               int depth,
               pixels::format_enum fmt);

        void
        create(void* pixels,
               int width,
               int height,
               int depth,
               int pitch,
               Uint32 r_mask,
               Uint32 g_mask,
               Uint32 b_mask,
               Uint32 a_mask);

        void
        create(void* pixels,
               int width,
               int height,
               int depth,
               int pitch,
               pixels::format_enum fmt);

        void
        create(const surface& other);

        void
        create(const surface& other,
               const pixels::format& fmt,
               Uint32 flags);

        void
        create(const surface& other,
               pixels::format_enum fmt,
               Uint32 flags);


        void
        destroy()
            noexcept override;


        void
        acquire(state_type state)
            noexcept;

        void
        acquire(SDL_Surface* new_raw,
                bool new_owner = true,
                void* new_user_data = nullptr)
            noexcept;


        state_type
        release()
            noexcept;


        // accessors to the public SDL_Surface fields
        [[nodiscard]]
        Uint32
        get_flags()
            const noexcept;


        [[nodiscard]]
        pixels::format
        get_format()
            const noexcept;


        [[nodiscard]]
        int
        get_width()
            const noexcept;

        [[nodiscard]]
        int
        get_height()
            const noexcept;

        [[nodiscard]]
        vec2
        get_size()
            const noexcept;


        [[nodiscard]]
        void*
        get_pixels()
            noexcept;

        [[nodiscard]]
        const void*
        get_pixels()
            const noexcept;

        template<typename T>
        [[nodiscard]]
        T*
        get_pixels_as()
            noexcept
        {
            return reinterpret_cast<T*>(get_pixels());
        }

        template<typename T>
        [[nodiscard]]
        const T*
        get_pixels_as()
            const noexcept
        {
            return reinterpret_cast<const T*>(get_pixels());
        }


        [[nodiscard]]
        void*
        get_user_data()
            const noexcept;

        void*
        set_user_data(void* new_user_data)
            noexcept;


        [[nodiscard]]
        int
        get_ref_count()
            const noexcept;


        void
        set_palette(pixels::palette& pal);


        bool
        must_lock()
            const noexcept
        {
            return SDL_MUSTLOCK(raw);
        }


        void
        lock();

        bool
        try_lock()
            noexcept;

        void
        unlock()
            noexcept;


        class lock_guard {

            surface& surf;

        public:

            struct adopt_lock_t {};
            static constexpr adopt_lock_t adopt_lock{};


            lock_guard(surface& s);

            lock_guard(surface& s,
                       adopt_lock_t adopt)
                noexcept;

            lock_guard(const lock_guard& other) = delete;

            ~lock_guard()
                noexcept;

        };


        [[nodiscard]]
        static
        surface
        load_bmp(SDL_RWops* src,
                 bool close_src);

        [[nodiscard]]
        static
        surface
        load_bmp(const path& filename);


        void
        save_bmp(SDL_RWops* dst,
                 bool close);

        void
        save_bmp(const path& filename);


        void
        set_rle(bool rle);

        [[nodiscard]]
        bool
        has_rle()
            const noexcept;


        void
        set_color_key(Uint32 key);

        void
        set_color_key(color key);

        void
        unset_color_key();


        [[nodiscard]]
        bool
        has_color_key()
            const noexcept;

        [[nodiscard]]
        Uint32
        get_color_key()
            const;


        void
        set_color_mod(Uint8 r,
                      Uint8 g,
                      Uint8 b);

        void
        set_color_mod(color c);

        [[nodiscard]]
        color
        get_color_mod()
            const;


        void
        set_alpha_mod(Uint8 alpha);

        [[nodiscard]]
        Uint8
        get_alpha_mod()
            const;


        void
        set_blend_mode(SDL_BlendMode mode);

        SDL_BlendMode
        get_blend_mode()
            const;


        bool
        set_clip(const rect& clip)
            noexcept;

        bool
        unset_clip()
            noexcept;

        [[nodiscard]]
        rect
        get_clip()
            const noexcept;


        void
        fill(const rect& r,
             Uint32 pixel);

        void
        fill(const rect& r,
             color c);

        void
        fill(vec2 a,
             vec2 b,
             Uint32 pixel);

        void
        fill(vec2 a,
             vec2 b,
             color c);

        void
        fill(std::span<const rect> rs,
             Uint32 pixel);

        void
        fill(std::span<const rect> rs,
             color c);

    }; // class surface


    void
    convert_pixels(int width,
                   int height,
                   pixels::format_enum src_format,
                   const void* src,
                   int src_pitch,
                   pixels::format_enum dst_format,
                   void* dst,
                   int dst_pitch);


    void
    premultiply_alpha(int width,
                      int height,
                      pixels::format_enum src_format,
                      const void* src,
                      int src_pitch,
                      pixels::format_enum dst_format,
                      void* dst,
                      int dst_pitch);


    void
    blit(const surface& src, rect* src_rect,
               surface& dst, rect* dst_rect);

    void
    lower_blit(const surface& src, rect* src_rect,
                     surface& dst, rect* dst_rect);


    void
    soft_stretch(const surface& src, const rect* src_rect,
                       surface& dst, const rect* dst_rect);

    void
    soft_stretch_linear(const surface& src, const rect* src_rect,
                              surface& dst, const rect* dst_rect);


    void
    blit_scaled(const surface& src, const rect* src_rect,
                      surface& dst, rect* dst_rect);

    void
    lower_blit_scaled(const surface& src, rect* src_rect,
                            surface& dst, rect* dst_rect);


    void
    set_yuv_conversion_mode(SDL_YUV_CONVERSION_MODE mode)
        noexcept;

    SDL_YUV_CONVERSION_MODE
    get_yuv_conversion_mode()
        noexcept;

    SDL_YUV_CONVERSION_MODE
    get_yuv_conversion_mode(int width, int height)
        noexcept;


} // namespace sdl

#endif
