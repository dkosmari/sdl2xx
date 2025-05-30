/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_TTF_HPP
#define SDL2XX_TTF_HPP

#include <expected>
#include <filesystem>
#include <optional>
#include <string>

#include <SDL_ttf.h>

#include "basic_wrapper.hpp"
#include "color.hpp"
#include "error.hpp"
#include "rwops.hpp"
#include "string.hpp"
#include "surface.hpp"
#include "vec2.hpp"


namespace sdl::ttf {

    using std::filesystem::path;


    [[nodiscard]]
    SDL_version
    linked_version()
        noexcept;


    struct ft_version {
        int major;
        int minor;
        int patch;
    };


    [[nodiscard]]
    ft_version
    freetype_version()
        noexcept;


#if SDL_TTF_VERSION_ATLEAST(2, 20, 0)

    struct hb_version {
        int major;
        int minor;
        int patch;
    };


    [[nodiscard]]
    hb_version
    harfbuzz_version()
        noexcept;

#endif // SDL_TTF_VERSION_ATLEAST(2, 20, 0)


    struct init {

        init();

        // Disallow copies.
        init(const init&) = delete;

        ~init()
            noexcept;


        [[nodiscard]]
        static
        unsigned
        count()
            noexcept;

    }; // struct init


    struct font : basic_wrapper<TTF_Font*> {

        using base_type = basic_wrapper<TTF_Font*>;


        struct dpi_opt {
            unsigned h;
            unsigned v;
        };


        struct options {
            std::optional<long> index = {};
            std::optional<dpi_opt> dpi = {};
        };


        // Inherit constructors.
        using base_type::base_type;


        font(const path& filename,
             int pt_size);

        font(const path& filename,
             int pt_size,
             const options& opt);

        font(SDL_RWops* src,
             bool close_src,
             int pt_size);

        font(SDL_RWops* src,
             bool close_src,
             int pt_size,
             const options& opt);

        font(rwops& src,
             int pt_size);

        font(rwops& src,
             int pt_size,
             const options& opt);


        /// Move constructor.
        font(font&& other)
            noexcept = default;


        ~font()
            noexcept;


        /// Move assignment.
        font&
        operator =(font&& other)
            noexcept = default;


        void
        create(const path& filename,
               int pt_size);

        void
        create(const path& filename,
               int pt_size,
               const options& opt);

        void
        create(SDL_RWops* src,
               bool free_src,
               int pt_size);

        void
        create(SDL_RWops* src,
               bool free_src,
               int pt_size,
               const options& opt);


        void
        create(rwops& src,
               int pt_size);

        void
        create(rwops& src,
               int pt_size,
               const options& opt);

        void
        destroy()
            noexcept;


        enum style : unsigned {
            normal         = TTF_STYLE_NORMAL,
            bold           = TTF_STYLE_BOLD,
            italic         = TTF_STYLE_ITALIC,
            underline      = TTF_STYLE_UNDERLINE,
            strike_through = TTF_STYLE_STRIKETHROUGH,
        };


        [[nodiscard]]
        unsigned
        get_style()
            const noexcept;

        void
        set_style(unsigned s)
            noexcept;


        [[nodiscard]]
        int
        get_outline()
            const noexcept;

        void
        set_outline(int outline)
            noexcept;


        enum class hinting {
            normal         = TTF_HINTING_NORMAL,
            light          = TTF_HINTING_LIGHT,
            mono           = TTF_HINTING_MONO,
            none           = TTF_HINTING_NONE,
            light_subpixel = TTF_HINTING_LIGHT_SUBPIXEL,
        };

        [[nodiscard]]
        hinting
        get_hinting()
            const noexcept;

        void
        set_hinting(hinting h)
            noexcept;


#if SDL_TTF_VERSION_ATLEAST(2, 20, 0)

        enum class align {
            left   = TTF_WRAPPED_ALIGN_LEFT,
            center = TTF_WRAPPED_ALIGN_CENTER,
            right  = TTF_WRAPPED_ALIGN_RIGHT,
        };

        [[nodiscard]]
        align
        get_align()
            const noexcept;

        void
        set_align(align a)
            noexcept;

#endif // SDL_TTF_VERSION_ATLEAST(2, 20, 0)


        [[nodiscard]]
        int
        get_height()
            const noexcept;


        [[nodiscard]]
        int
        get_ascent()
            const noexcept;

        [[nodiscard]]
        int
        get_descent()
            const noexcept;


        [[nodiscard]]
        int
        get_line_skip()
            const noexcept;


        [[nodiscard]]
        bool
        get_kerning()
            const noexcept;

        void
        set_kerning(bool allowed)
            noexcept;


        [[nodiscard]]
        long
        get_num_faces()
            const noexcept;


        [[nodiscard]]
        bool
        is_fixed_width()
            const noexcept;


        [[nodiscard]]
        const char*
        get_family_name()
            const noexcept;


        [[nodiscard]]
        const char*
        get_style_name()
            const noexcept;


        [[nodiscard]]
        bool
        has_glyph(char32_t codepoint)
            const noexcept;


        struct metrics {
            vec2 min;
            vec2 max;
            int advance;
        };


        [[nodiscard]]
        metrics
        get_metrics(char32_t codepoint)
            const;

        [[nodiscard]]
        std::expected<metrics, error>
        try_get_metrics(char32_t codepoint)
            const noexcept;


        [[nodiscard]]
        vec2
        get_size(const char* text)
            const;

        [[nodiscard]]
        std::expected<vec2, error>
        try_get_size(const char* text)
            const noexcept;


        [[nodiscard]]
        vec2
        get_size(const char8_t* text)
            const;

        [[nodiscard]]
        std::expected<vec2, error>
        try_get_size(const char8_t* text)
            const noexcept;


        [[nodiscard]]
        inline
        vec2
        get_size(const concepts::string auto& text)
            const
        {
            return get_size(text.data());
        }


        [[nodiscard]]
        inline
        std::expected<vec2, error>
        try_get_size(const concepts::string auto& text)
            const noexcept
        {
            return try_get_size(text.data());
        }


        [[nodiscard]]
        vec2
        get_size_latin1(const char* text)
            const;


        [[nodiscard]]
        std::expected<vec2, error>
        try_get_size_latin1(const char* text)
            const noexcept;


        [[nodiscard]]
        inline
        vec2
        get_size_latin1(const concepts::string auto& text)
            const
        {
            return get_size_latin1(text.data());
        }


        [[nodiscard]]
        inline
        std::expected<vec2, error>
        try_get_size_latin1(const concepts::string auto& text)
            const noexcept
        {
            return try_get_size_latin1(text.data());
        }


        struct measure {
            int width;
            int count;
        };


        [[nodiscard]]
        measure
        get_measure(const char* text,
                    int max_width)
            const;

        [[nodiscard]]
        std::expected<measure, error>
        try_get_measure(const char* text,
                        int max_width)
            const noexcept;

        [[nodiscard]]
        measure
        get_measure(const char8_t* text,
                    int max_width)
            const;

        [[nodiscard]]
        std::expected<measure, error>
        try_get_measure(const char8_t* text,
                        int max_width)
            const noexcept;


        [[nodiscard]]
        inline
        measure
        get_measure(const concepts::string auto& text,
                    int max_width)
            const
        {
            return get_measure(text.data(), max_width);
        }


        [[nodiscard]]
        inline
        std::expected<measure, error>
        try_get_measure(const concepts::string auto& text,
                        int max_width)
            const noexcept
        {
            return try_get_measure(text.data(), max_width);
        }


        [[nodiscard]]
        measure
        get_measure_latin1(const char* text,
                           int max_width)
            const;

        [[nodiscard]]
        std::expected<measure, error>
        try_get_measure_latin1(const char* text,
                               int max_width)
            const noexcept;


        [[nodiscard]]
        inline
        measure
        get_measure_latin1(const concepts::string auto& text,
                           int max_width)
            const
        {
            return get_measure_latin1(text.data(), max_width);
        }


        [[nodiscard]]
        inline
        std::expected<measure, error>
        try_get_measure_latin1(const concepts::string auto& text,
                               int max_width)
            const noexcept
        {
            return try_get_measure_latin1(text.data(), max_width);
        }


        // ----- //
        // Solid //
        // ----- //


        [[nodiscard]]
        surface
        render_glyph_solid(char32_t codepoint,
                           color fg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_glyph_solid(char32_t codepoint,
                               color fg)
            const noexcept;


        [[nodiscard]]
        surface
        render_solid(const char* text,
                     color fg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_solid(const char* text,
                         color fg)
            const noexcept;

        [[nodiscard]]
        surface
        render_solid(const char8_t* text,
                     color fg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_solid(const char8_t* text,
                         color fg)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_solid(const concepts::string auto& text,
                     color fg)
            const
        {
            return render_solid(text.data(), fg);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_solid(const concepts::string auto& text,
                         color fg)
            const noexcept
        {
            return try_render_solid(text.data(), fg);
        }


        [[nodiscard]]
        surface
        render_solid_latin1(const char* text,
                            color fg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_solid_latin1(const char* text,
                                color fg)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_solid_latin1(const concepts::string auto& text,
                            color fg)
            const
        {
            return render_solid_latin1(text.data(), fg);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_solid_latin1(const concepts::string auto& text,
                                color fg)
            const noexcept
        {
            return try_render_solid_latin1(text.data(), fg);
        }


        // --------------- //
        // Solid + Wrapped //
        // --------------- //


        [[nodiscard]]
        surface
        render_solid(const char* text,
                     color fg,
                     Uint32 max_width)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_solid(const char* text,
                         color fg,
                         Uint32 max_width)
            const noexcept;

        [[nodiscard]]
        surface
        render_solid(const char8_t* text,
                     color fg,
                     Uint32 max_width)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_solid(const char8_t* text,
                         color fg,
                         Uint32 max_width)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_solid(const concepts::string auto& text,
                     color fg,
                     Uint32 max_width)
            const
        {
            return render_solid(text.data(), fg, max_width);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_solid(const concepts::string auto& text,
                         color fg,
                         Uint32 max_width)
            const noexcept
        {
            return try_render_solid(text.data(), fg, max_width);
        }


        [[nodiscard]]
        surface
        render_solid_latin1(const char* text,
                            color fg,
                            Uint32 max_width)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_solid_latin1(const char* text,
                                color fg,
                                Uint32 max_width)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_solid_latin1(const concepts::string auto& text,
                            color fg,
                            Uint32 max_width)
            const
        {
            return render_solid_latin1(text.data(), fg, max_width);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_solid_latin1(const concepts::string auto& text,
                                color fg,
                                Uint32 max_width)
            const noexcept
        {
            return try_render_solid_latin1(text.data(), fg, max_width);
        }


        // ------ //
        // Shaded //
        // ------ //


        [[nodiscard]]
        surface
        render_glyph_shaded(char32_t codepoint,
                            color fg,
                            color bg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_glyph_shaded(char32_t codepoint,
                                color fg,
                                color bg)
            const noexcept;

        [[nodiscard]]
        surface
        render_shaded(const char* text,
                      color fg,
                      color bg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_shaded(const char* text,
                          color fg,
                          color bg)
            const noexcept;

        [[nodiscard]]
        surface
        render_shaded(const char8_t* text,
                      color fg,
                      color bg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_shaded(const char8_t* text,
                          color fg,
                          color bg)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_shaded(const concepts::string auto& text,
                      color fg,
                      color bg)
            const
        {
            return render_shaded(text.data(), fg, bg);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_shaded(const concepts::string auto& text,
                          color fg,
                          color bg)
            const noexcept
        {
            return try_render_shaded(text.data(), fg, bg);
        }


        [[nodiscard]]
        surface
        render_shaded_latin1(const char* text,
                             color fg,
                             color bg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_shaded_latin1(const char* text,
                                 color fg,
                                 color bg)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_shaded_latin1(const concepts::string auto& text,
                             color fg,
                             color bg)
            const
        {
            return render_shaded_latin1(text.data(), fg, bg);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_shaded_latin1(const concepts::string auto& text,
                                 color fg,
                                 color bg)
            const noexcept
        {
            return try_render_shaded_latin1(text.data(), fg, bg);
        }


        // ---------------- //
        // Shaded + Wrapped //
        // ---------------- //


        [[nodiscard]]
        surface
        render_shaded(const char* text,
                      color fg,
                      color bg,
                      Uint32 max_width)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_shaded(const char* text,
                          color fg,
                          color bg,
                          Uint32 max_width)
            const noexcept;

        [[nodiscard]]
        surface
        render_shaded(const char8_t* text,
                      color fg,
                      color bg,
                      Uint32 max_width)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_shaded(const char8_t* text,
                          color fg,
                          color bg,
                          Uint32 max_width)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_shaded(const concepts::string auto& text,
                      color fg,
                      color bg,
                      Uint32 max_width)
            const
        {
            return render_shaded(text.data(), fg, bg, max_width);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_shaded(const concepts::string auto& text,
                          color fg,
                          color bg,
                          Uint32 max_width)
            const noexcept
        {
            return try_render_shaded(text.data(), fg, bg, max_width);
        }


        [[nodiscard]]
        surface
        render_shaded_latin1(const char* text,
                             color fg,
                             color bg,
                             Uint32 max_width)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_shaded_latin1(const char* text,
                                 color fg,
                                 color bg,
                                 Uint32 max_width)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_shaded_latin1(const concepts::string auto& text,
                             color fg,
                             color bg,
                             Uint32 max_width)
            const
        {
            return render_shaded_latin1(text.data(), fg, bg, max_width);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_shaded_latin1(const concepts::string auto& text,
                                 color fg,
                                 color bg,
                                 Uint32 max_width)
            const noexcept
        {
            return try_render_shaded_latin1(text.data(), fg, bg, max_width);
        }


        // ------- //
        // Blended //
        // ------- //


        [[nodiscard]]
        surface
        render_glyph_blended(char32_t codepoint,
                             color fg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_glyph_blended(char32_t codepoint,
                                 color fg)
            const noexcept;


        [[nodiscard]]
        surface
        render_blended(const char* text,
                       color fg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_blended(const char* text,
                           color fg)
            const noexcept;

        [[nodiscard]]
        surface
        render_blended(const char8_t* text,
                       color fg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_blended(const char8_t* text,
                           color fg)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_blended(const concepts::string auto& text,
                       color fg)
            const
        {
            return render_blended(text.data(), fg);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_blended(const concepts::string auto& text,
                           color fg)
            const noexcept
        {
            return try_render_blended(text.data(), fg);
        }


        [[nodiscard]]
        surface
        render_blended_latin1(const char* text,
                              color fg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_blended_latin1(const char* text,
                                  color fg)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_blended_latin1(const concepts::string auto& text,
                              color fg)
            const
        {
            return render_blended_latin1(text.data(), fg);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_blended_latin1(const concepts::string auto& text,
                                  color fg)
            const noexcept
        {
            return try_render_blended_latin1(text.data(), fg);
        }


        // ----------------- //
        // Blended + Wrapped //
        // ----------------- //


        [[nodiscard]]
        surface
        render_blended(const char* text,
                       color fg,
                       Uint32 max_width)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_blended(const char* text,
                           color fg,
                           Uint32 max_width)
            const noexcept;

        [[nodiscard]]
        surface
        render_blended(const char8_t* text,
                       color fg,
                       Uint32 max_width)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_blended(const char8_t* text,
                           color fg,
                           Uint32 max_width)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_blended(const concepts::string auto& text,
                       color fg,
                       Uint32 max_width)
            const
        {
            return render_blended(text.data(), fg, max_width);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_blended(const concepts::string auto& text,
                           color fg,
                           Uint32 max_width)
            const noexcept
        {
            return try_render_blended(text.data(), fg, max_width);
        }


        [[nodiscard]]
        surface
        render_blended_latin1(const char* text,
                              color fg,
                              Uint32 max_width)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_blended_latin1(const char* text,
                                  color fg,
                                  Uint32 max_width)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_blended_latin1(const concepts::string auto& text,
                              color fg,
                              Uint32 max_width)
            const
        {
            return render_blended_latin1(text.data(), fg, max_width);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_blended_latin1(const concepts::string auto& text,
                                  color fg,
                                  Uint32 max_width)
            const noexcept
        {
            return try_render_blended_latin1(text.data(), fg, max_width);
        }


#if SDL_TTF_VERSION_ATLEAST(2, 20, 0)

        // --- //
        // LCD //
        // --- //


        [[nodiscard]]
        surface
        render_glyph_lcd(char32_t codepoint,
                         color fg,
                         color bg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_glyph_lcd(char32_t codepoint,
                             color fg,
                             color bg)
            const noexcept;


        [[nodiscard]]
        surface
        render_lcd(const char* text,
                   color fg,
                   color bg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_lcd(const char* text,
                       color fg,
                       color bg)
            const noexcept;

        [[nodiscard]]
        surface
        render_lcd(const char8_t* text,
                   color fg,
                   color bg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_lcd(const char8_t* text,
                       color fg,
                       color bg)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_lcd(const concepts::string auto& text,
                   color fg,
                   color bg)
            const
        {
            return render_lcd(text.data(), fg, bg);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_lcd(const concepts::string auto& text,
                       color fg,
                       color bg)
            const noexcept
        {
            return try_render_lcd(text.data(), fg, bg);
        }


        [[nodiscard]]
        surface
        render_lcd_latin1(const char* text,
                          color fg,
                          color bg)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_lcd_latin1(const char* text,
                              color fg,
                              color bg)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_lcd_latin1(const concepts::string auto& text,
                          color fg,
                          color bg)
            const
        {
            return render_lcd_latin1(text.data(), fg, bg);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_lcd_latin1(const concepts::string auto& text,
                              color fg,
                              color bg)
            const noexcept
        {
            return try_render_lcd_latin1(text.data(), fg, bg);
        }


        // ------------- //
        // LCD + Wrapped //
        // ------------- //


        [[nodiscard]]
        surface
        render_lcd(const char* text,
                   color fg,
                   color bg,
                   Uint32 max_width)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_lcd(const char* text,
                       color fg,
                       color bg,
                       Uint32 max_width)
            const noexcept;

        [[nodiscard]]
        surface
        render_lcd(const char8_t* text,
                   color fg,
                   color bg,
                   Uint32 max_width)
            const;

        [[nodiscard]]
        std::expected<surface, error>
        try_render_lcd(const char8_t* text,
                       color fg,
                       color bg,
                       Uint32 max_width)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_lcd(const concepts::string auto& text,
                   color fg,
                   color bg,
                   Uint32 max_width)
            const
        {
            return render_lcd(text.data(), fg, bg, max_width);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_lcd(const concepts::string auto& text,
                       color fg,
                       color bg,
                       Uint32 max_width)
            const noexcept
        {
            return try_render_lcd(text.data(), fg, bg, max_width);
        }


        [[nodiscard]]
        surface
        render_lcd_latin1(const char* text,
                          color fg,
                          color bg,
                          Uint32 max_width)
            const;


        [[nodiscard]]
        std::expected<surface, error>
        try_render_lcd_latin1(const char* text,
                              color fg,
                              color bg,
                              Uint32 max_width)
            const noexcept;


        [[nodiscard]]
        inline
        surface
        render_lcd_latin1(const concepts::string auto& text,
                          color fg,
                          color bg,
                          Uint32 max_width)
            const
        {
            return render_lcd_latin1(text.data(), fg, bg, max_width);
        }


        [[nodiscard]]
        inline
        std::expected<surface, error>
        try_render_lcd_latin1(const concepts::string auto& text,
                              color fg,
                              color bg,
                              Uint32 max_width)
            const noexcept
        {
            return try_render_lcd_latin1(text.data(), fg, bg, max_width);
        }

#endif // SDL_TTF_VERSION_ATLEAST(2, 20, 0)


        [[nodiscard]]
        int
        get_kerning_size(char32_t prev_codepoint,
                         char32_t codepoint)
            const;

        [[nodiscard]]
        std::expected<int, error>
        try_get_kerning_size(char32_t prev_codepoint,
                             char32_t codepoint)
            const noexcept;


        void
        set_sdf(bool enable);

        std::expected<void, error>
        try_set_sdf(bool enable)
            noexcept;


        [[nodiscard]]
        bool
        get_sdf()
            const noexcept;


#if SDL_TTF_VERSION_ATLEAST(2, 20, 0)

        enum class direction {
            ltr = TTF_DIRECTION_LTR,
            rtl = TTF_DIRECTION_RTL,
            ttb = TTF_DIRECTION_TTB,
            btt = TTF_DIRECTION_BTT,
        };


        void
        set_direction(direction dir);

        std::expected<void, error>
        try_set_direction(direction dir)
            noexcept;


        void
        set_script(const char* script);

        std::expected<void, error>
        try_set_script(const char* script)
            noexcept;

#endif // SDL_TTF_VERSION_ATLEAST(2, 20, 0)

    }; // class font

} // namespace sdl::ttf

#endif
