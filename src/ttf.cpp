/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include "ttf.hpp"

#include "error.hpp"


namespace sdl::ttf {

    SDL_version
    linked_version()
        noexcept
    {
        return *TTF_Linked_Version();
    }


    ft_version
    freetype_version()
        noexcept
    {
        ft_version ver;
        TTF_GetFreeTypeVersion(&ver.major, &ver.minor, &ver.patch);
        return ver;
    }


#if SDL_TTF_VERSION_ATLEAST(2, 20, 0)

    hb_version
    harfbuzz_version()
        noexcept
    {
        hb_version ver;
        TTF_GetHarfBuzzVersion(&ver.major, &ver.minor, &ver.patch);
        return ver;
    }

#endif // SDL_TTF_VERSION_ATLEAST(2, 20, 0)


    // class init

    init::init()
    {
        if (TTF_Init())
            throw error{};
    }


    init::~init()
        noexcept
    {
        TTF_Quit();
    }


    unsigned
    init::count()
        noexcept
    {
        return TTF_WasInit();
    }



    font::font(const path& filename,
               int pt_size)
    {
        create(filename, pt_size);
    }


    font::font(const path& filename,
               int pt_size,
               const options& opt)
    {
        create(filename, pt_size, opt);
    }


    font::font(SDL_RWops* src,
               bool close_src,
               int pt_size)
    {
        create(src, close_src, pt_size);
    }


    font::font(SDL_RWops* src,
               bool close_src,
               int pt_size,
               const options& opt)
    {
        create(src, close_src, pt_size, opt);
    }


    font::~font()
    {
        destroy();
    }


    void
    font::create(const path& filename,
                 int pt_size)
    {
        auto ptr = TTF_OpenFont(filename.c_str(), pt_size);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    font::create(const path& filename,
                 int pt_size,
                 const options& opt)
    {
        TTF_Font* ptr = nullptr;
        if (opt.index) {
            if (opt.dpi) {
                // index, dpi
                ptr = TTF_OpenFontIndexDPI(filename.c_str(),
                                           pt_size,
                                           *opt.index,
                                           opt.dpi->h,
                                           opt.dpi->v);
            } else {
                // index, no dpi
                ptr = TTF_OpenFontIndex(filename.c_str(),
                                        pt_size,
                                        *opt.index);
            }
        } else {
            if (opt.dpi) {
                // no index, dpi
                ptr = TTF_OpenFontDPI(filename.c_str(),
                                      pt_size,
                                      opt.dpi->h,
                                      opt.dpi->v);
            } else {
                // no index, no dpi
                ptr = TTF_OpenFont(filename.c_str(),
                                   pt_size);
            }
        }

        if (!ptr)
            throw error{};

        destroy();
        acquire(ptr);
    }


    void
    font::create(SDL_RWops* src,
                 bool free_src,
                 int pt_size)
    {
        auto ptr = TTF_OpenFontRW(src, free_src, pt_size);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    font::create(SDL_RWops* src,
                 bool free_src,
                 int pt_size,
                 const options& opt)
    {
        TTF_Font* ptr = nullptr;
        if (opt.index) {
            if (opt.dpi) {
                // index, dpi
                ptr = TTF_OpenFontIndexDPIRW(src,
                                             free_src,
                                             pt_size,
                                             *opt.index,
                                             opt.dpi->h,
                                             opt.dpi->v);
            } else {
                // index, no dpi
                ptr = TTF_OpenFontIndexRW(src,
                                          free_src,
                                          pt_size,
                                          *opt.index);
            }
        } else {
            if (opt.dpi) {
                // no index, dpi
                ptr = TTF_OpenFontDPIRW(src,
                                        free_src,
                                        pt_size,
                                        opt.dpi->h,
                                        opt.dpi->v);
            } else {
                // no index, no dpi
                ptr = TTF_OpenFontRW(src, free_src, pt_size);
            }
        }

        if (!ptr)
            throw error{};

        destroy();
        acquire(ptr);
    }


    void
    font::destroy()
        noexcept
    {
        if (is_valid())
            TTF_CloseFont(release());
    }


    unsigned
    font::get_style()
        const noexcept
    {
        return TTF_GetFontStyle(raw);
    }


    void
    font::set_style(unsigned s)
        noexcept
    {
        TTF_SetFontStyle(raw, s);
    }


    int
    font::get_outline()
        const noexcept
    {
        return TTF_GetFontOutline(raw);
    }


    void
    font::set_outline(int outline)
        noexcept
    {
        TTF_SetFontOutline(raw, outline);
    }


    font::hinting
    font::get_hinting()
        const noexcept
    {
        return static_cast<hinting>(TTF_GetFontHinting(raw));
    }


    void
    font::set_hinting(hinting h)
        noexcept
    {
        TTF_SetFontHinting(raw, static_cast<int>(h));
    }


#if SDL_TTF_VERSION_ATLEAST(2, 20, 0)

    font::align
    font::get_align()
        const noexcept
    {
        return static_cast<align>(TTF_GetFontWrappedAlign(raw));
    }


    void
    font::set_align(align a)
        noexcept
    {
        TTF_SetFontWrappedAlign(raw, static_cast<int>(a));
    }

#endif // SDL_TTF_VERSION_ATLEAST(2, 20, 0)


    int
    font::get_height()
        const noexcept
    {
        return TTF_FontHeight(raw);
    }


    int
    font::get_ascent()
        const noexcept
    {
        return TTF_FontAscent(raw);
    }


    int
    font::get_descent()
        const noexcept
    {
        return TTF_FontDescent(raw);
    }


    int
    font::get_line_skip()
        const noexcept
    {
        return TTF_FontLineSkip(raw);
    }


    bool
    font::get_kerning()
        const noexcept
    {
        return TTF_GetFontKerning(raw);
    }


    void
    font::set_kerning(bool allowed)
        noexcept
    {
        TTF_SetFontKerning(raw, allowed);
    }


    long
    font::get_num_faces()
        const noexcept
    {
        return TTF_FontFaces(raw);
    }


    bool
    font::is_fixed_width()
        const noexcept
    {
        return TTF_FontFaceIsFixedWidth(raw);
    }


    const char*
    font::get_family_name()
        const
    {
        return TTF_FontFaceFamilyName(raw);
    }


    const char*
    font::get_style_name()
        const
    {
        return TTF_FontFaceStyleName(raw);
    }


    bool
    font::has_glyph(char32_t codepoint)
        const noexcept
    {
        return TTF_GlyphIsProvided32(raw, codepoint);
    }


    font::metrics
    font::get_metrics(char32_t codepoint)
        const
    {
        metrics result;
        // TODO: check if it can actually return -1
        if (TTF_GlyphMetrics32(raw,
                               codepoint,
                               &result.min.x,
                               &result.max.x,
                               &result.min.y,
                               &result.max.y,
                               &result.advance) < 0)
            throw error{};
        return result;
    }


    vec2
    font::get_size(const char* text)
        const
    {
        vec2 size;
        if (TTF_SizeText(raw,
                         text,
                         &size.x,
                         &size.y) < 0)
            throw error{};
        return size;
    }

    vec2
    font::get_size(const char8_t* text)
        const
    {
        vec2 size;
        if (TTF_SizeUTF8(raw,
                         reinterpret_cast<const char*>(text),
                         &size.x,
                         &size.y) < 0)
            throw error{};
        return size;
    }


    vec2
    font::get_size_utf8(const char* text)
        const
    {
        return get_size(reinterpret_cast<const char8_t*>(text));
    }


    font::measure
    font::get_measure(const char* text,
                      int max_width)
        const
    {
        measure result;
        if (TTF_MeasureText(raw,
                            text,
                            max_width,
                            &result.width,
                            &result.count) < 0)
            throw error{};
        return result;
    }


    font::measure
    font::get_measure(const char8_t* text,
                      int max_width)
        const
    {
        measure result;
        if (TTF_MeasureUTF8(raw,
                            reinterpret_cast<const char*>(text),
                            max_width,
                            &result.width,
                            &result.count) < 0)
            throw error{};
        return result;
    }


    font::measure
    font::get_measure_utf8(const char* text,
                           int max_width)
        const
    {
        return get_measure(reinterpret_cast<const char8_t*>(text), max_width);
    }


    surface
    font::render_solid(const char* text,
                       color fg)
        const
    {
        auto surf = TTF_RenderText_Solid(raw, text, fg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_solid(const char8_t* text,
                       color fg)
        const
    {
        auto surf = TTF_RenderUTF8_Solid(raw,
                                         reinterpret_cast<const char*>(text),
                                         fg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_solid_utf8(const char* text,
                            color fg)
        const
    {
        return render_solid(reinterpret_cast<const char8_t*>(text), fg);
    }


    surface
    font::render_solid(const char* text,
                               color fg,
                               Uint32 max_width)
        const
    {
        auto surf = TTF_RenderText_Solid_Wrapped(raw, text, fg, max_width);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_solid(const char8_t* text,
                               color fg,
                               Uint32 max_width)
        const
    {
        auto surf = TTF_RenderUTF8_Solid_Wrapped(raw,
                                                 reinterpret_cast<const char*>(text),
                                                 fg,
                                                 max_width);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_solid_utf8(const char* text,
                            color fg,
                            Uint32 max_width)
        const
    {
        return render_solid(reinterpret_cast<const char8_t*>(text),
                            fg,
                            max_width);
    }


    surface
    font::render_glyph_solid(char32_t codepoint,
                             color fg)
        const
    {
        auto surf = TTF_RenderGlyph32_Solid(raw, codepoint, fg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_shaded(const char* text,
                        color fg,
                        color bg)
        const
    {
        auto surf = TTF_RenderText_Shaded(raw, text, fg, bg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_shaded(const char8_t* text,
                        color fg,
                        color bg)
        const
    {
        auto surf = TTF_RenderUTF8_Shaded(raw,
                                          reinterpret_cast<const char*>(text),
                                          fg,
                                          bg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_shaded_utf8(const char* text,
                             color fg,
                             color bg)
        const
    {
        return render_shaded(reinterpret_cast<const char8_t*>(text),
                             fg,
                             bg);
    }


    surface
    font::render_shaded(const char* text,
                        color fg,
                        color bg,
                        Uint32 max_width)
        const
    {
        auto surf = TTF_RenderText_Shaded_Wrapped(raw,
                                                  text,
                                                  fg,
                                                  bg,
                                                  max_width);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_shaded(const char8_t* text,
                        color fg,
                        color bg,
                        Uint32 max_width)
        const
    {
        auto surf = TTF_RenderUTF8_Shaded_Wrapped(raw,
                                                  reinterpret_cast<const char*>(text),
                                                  fg,
                                                  bg,
                                                  max_width);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_shaded_utf8(const char* text,
                             color fg,
                             color bg,
                             Uint32 max_width)
        const
    {
        return render_shaded(reinterpret_cast<const char8_t*>(text),
                             fg,
                             bg,
                             max_width);
    }


    surface
    font::render_glyph_shaded(char32_t codepoint,
                              color fg,
                              color bg)
        const
    {
        auto surf = TTF_RenderGlyph32_Shaded(raw, codepoint, fg, bg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_blended(const char* text,
                         color fg)
        const
    {
        auto surf = TTF_RenderText_Blended(raw, text, fg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_blended(const char8_t* text,
                         color fg)
        const
    {
        auto surf = TTF_RenderUTF8_Blended(raw,
                                           reinterpret_cast<const char*>(text),
                                           fg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_blended_utf8(const char* text,
                              color fg)
        const
    {
        return render_blended(reinterpret_cast<const char8_t*>(text),
                              fg);
    }


    surface
    font::render_blended(const char* text,
                         color fg,
                         Uint32 max_width)
        const
    {
        auto surf = TTF_RenderText_Blended_Wrapped(raw, text, fg, max_width);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_blended(const char8_t* text,
                         color fg,
                         Uint32 max_width)
        const
    {
        auto surf = TTF_RenderUTF8_Blended_Wrapped(raw,
                                                   reinterpret_cast<const char*>(text),
                                                   fg,
                                                   max_width);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_blended_utf8(const char* text,
                              color fg,
                              Uint32 max_width)
        const
    {
        return render_blended(reinterpret_cast<const char8_t*>(text),
                              fg,
                              max_width);
    }


    surface
    font::render_glyph_blended(char32_t codepoint,
                               color fg)
        const
    {
        auto surf = TTF_RenderGlyph32_Blended(raw, codepoint, fg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


#if SDL_TTF_VERSION_ATLEAST(2, 20, 0)

    surface
    font::render_lcd(const char* text,
                     color fg,
                     color bg)
        const
    {
        auto surf = TTF_RenderText_LCD(raw, text, fg, bg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_lcd(const char8_t* text,
                     color fg,
                     color bg)
        const
    {
        auto surf = TTF_RenderUTF8_LCD(raw,
                                       reinterpret_cast<const char*>(text),
                                       fg,
                                       bg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_lcd_utf8(const char* text,
                          color fg,
                          color bg)
        const
    {
        return render_lcd(reinterpret_cast<const char8_t*>(text),
                          fg,
                          bg);
    }


    surface
    font::render_lcd(const char* text,
                     color fg,
                     color bg,
                     Uint32 max_width)
        const
    {
        auto surf = TTF_RenderText_LCD_Wrapped(raw, text, fg, bg, max_width);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_lcd(const char8_t* text,
                     color fg,
                     color bg,
                     Uint32 max_width)
        const
    {
        auto surf = TTF_RenderUTF8_LCD_Wrapped(raw,
                                               reinterpret_cast<const char*>(text),
                                               fg,
                                               bg,
                                               max_width);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_lcd_utf8(const char* text,
                          color fg,
                          color bg,
                          Uint32 max_width)
        const
    {
        return render_lcd(reinterpret_cast<const char8_t*>(text),
                          fg,
                          bg,
                          max_width);
    }


    surface
    font::render_glyph_lcd(char32_t codepoint,
                           color fg,
                           color bg)
        const
    {
        auto surf = TTF_RenderGlyph32_LCD(raw, codepoint, fg, bg);
        if (!surf)
            throw error{};
        return surface{surf};
    }

#endif // SDL_TTF_VERSION_ATLEAST(2, 20, 0)


    int
    font::get_kerning_size(char32_t prev_codepoint,
                           char32_t codepoint)
        const
    {
        int result = TTF_GetFontKerningSizeGlyphs32(raw, prev_codepoint, codepoint);
        if (result < 0)
            throw error{};
        return result;
    }


    void
    font::set_sdf(bool enable)
    {
        if (TTF_SetFontSDF(raw, enable ? SDL_TRUE : SDL_FALSE) < 0)
            throw error{};
    }


    bool
    font::get_sdf()
        const noexcept
    {
        return TTF_GetFontSDF(raw);
    }


#if SDL_TTF_VERSION_ATLEAST(2, 20, 0)

    void
    font::set_direction(direction dir)
    {
        if (TTF_SetFontDirection(raw, static_cast<TTF_Direction>(dir)) < 0)
            throw error{};
    }


    void
    font::set_script(const char* script)
    {
        if (TTF_SetFontScriptName(raw, script) < 0)
            throw error{};
    }

#endif


} // namespace sdl::ttf
