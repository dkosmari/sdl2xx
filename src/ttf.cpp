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


    // class font

    void
    font::release()
        noexcept
    {
        ptr = nullptr;
    }


    font::font(TTF_Font* raw)
        noexcept :
        ptr{raw}
    {}


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


    font::font(font&& other)
        noexcept
    {
        ptr = other.ptr;
        other.release();
    }


    font::~font()
    {
        destroy();
    }


    font&
    font::operator =(font&& other)
        noexcept
    {
        if  (this != &other) {
            destroy();
            ptr = other.ptr;
            other.release();
        }
        return *this;
    }


    void
    font::create(const path& filename,
                 int pt_size)
    {
        destroy();
        ptr = TTF_OpenFont(filename.c_str(), pt_size);
        if (!ptr)
            throw error{};
    }


    void
    font::create(const path& filename,
                 int pt_size,
                 const options& opt)
    {
        destroy();

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
    }


    void
    font::create(SDL_RWops* src,
                 bool free_src,
                 int pt_size)
    {
        destroy();
        ptr = TTF_OpenFontRW(src, free_src, pt_size);
        if (!ptr)
            throw error{};
    }


    void
    font::create(SDL_RWops* src,
                 bool free_src,
                 int pt_size,
                 const options& opt)
    {
        destroy();

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
    }


    void
    font::destroy()
        noexcept
    {
        if (ptr) {
            TTF_CloseFont(ptr);
            ptr = nullptr;
        }
    }


    bool
    font::is_valid()
        const noexcept
    {
        return ptr;
    }


    font::operator bool()
        const noexcept
    {
        return ptr;
    }


    TTF_Font*
    font::data()
        noexcept
    {
        return ptr;
    }


    const TTF_Font*
    font::data()
        const noexcept
    {
        return ptr;
    }


    unsigned
    font::get_style()
        const noexcept
    {
        return TTF_GetFontStyle(ptr);
    }


    void
    font::set_style(unsigned s)
        noexcept
    {
        TTF_SetFontStyle(ptr, s);
    }


    int
    font::get_outline()
        const noexcept
    {
        return TTF_GetFontOutline(ptr);
    }


    void
    font::set_outline(int outline)
        noexcept
    {
        TTF_SetFontOutline(ptr, outline);
    }


    font::hinting
    font::get_hinting()
        const noexcept
    {
        return static_cast<hinting>(TTF_GetFontHinting(ptr));
    }


    void
    font::set_hinting(hinting h)
        noexcept
    {
        TTF_SetFontHinting(ptr, static_cast<int>(h));
    }


#if SDL_TTF_VERSION_ATLEAST(2, 20, 0)

    font::align
    font::get_align()
        const noexcept
    {
        return static_cast<align>(TTF_GetFontWrappedAlign(ptr));
    }


    void
    font::set_align(align a)
        noexcept
    {
        TTF_SetFontWrappedAlign(ptr, static_cast<int>(a));
    }

#endif // SDL_TTF_VERSION_ATLEAST(2, 20, 0)


    int
    font::get_height()
        const noexcept
    {
        return TTF_FontHeight(ptr);
    }


    int
    font::get_ascent()
        const noexcept
    {
        return TTF_FontAscent(ptr);
    }


    int
    font::get_descent()
        const noexcept
    {
        return TTF_FontDescent(ptr);
    }


    int
    font::get_line_skip()
        const noexcept
    {
        return TTF_FontLineSkip(ptr);
    }


    bool
    font::get_kerning()
        const noexcept
    {
        return TTF_GetFontKerning(ptr);
    }


    void
    font::set_kerning(bool allowed)
        noexcept
    {
        TTF_SetFontKerning(ptr, allowed);
    }


    long
    font::get_num_faces()
        const noexcept
    {
        return TTF_FontFaces(ptr);
    }


    bool
    font::is_fixed_width()
        const noexcept
    {
        return TTF_FontFaceIsFixedWidth(ptr);
    }


    const char*
    font::get_family_name()
        const
    {
        return TTF_FontFaceFamilyName(ptr);
    }


    const char*
    font::get_style_name()
        const
    {
        return TTF_FontFaceStyleName(ptr);
    }


    bool
    font::has_glyph(char32_t codepoint)
        const noexcept
    {
        return TTF_GlyphIsProvided32(ptr, codepoint);
    }


    font::metrics
    font::get_metrics(char32_t codepoint)
        const
    {
        metrics result;
        // TODO: check if it can actually return -1
        if (TTF_GlyphMetrics32(ptr,
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
        if (TTF_SizeText(ptr,
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
        if (TTF_SizeUTF8(ptr,
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
        if (TTF_MeasureText(ptr,
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
        if (TTF_MeasureUTF8(ptr,
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
        auto surf = TTF_RenderText_Solid(ptr, text, fg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_solid(const char8_t* text,
                       color fg)
        const
    {
        auto surf = TTF_RenderUTF8_Solid(ptr,
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
        auto surf = TTF_RenderText_Solid_Wrapped(ptr, text, fg, max_width);
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
        auto surf = TTF_RenderUTF8_Solid_Wrapped(ptr,
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
        auto surf = TTF_RenderGlyph32_Solid(ptr, codepoint, fg);
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
        auto surf = TTF_RenderText_Shaded(ptr, text, fg, bg);
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
        auto surf = TTF_RenderUTF8_Shaded(ptr,
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
        auto surf = TTF_RenderText_Shaded_Wrapped(ptr,
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
        auto surf = TTF_RenderUTF8_Shaded_Wrapped(ptr,
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
        auto surf = TTF_RenderGlyph32_Shaded(ptr, codepoint, fg, bg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_blended(const char* text,
                         color fg)
        const
    {
        auto surf = TTF_RenderText_Blended(ptr, text, fg);
        if (!surf)
            throw error{};
        return surface{surf};
    }


    surface
    font::render_blended(const char8_t* text,
                         color fg)
        const
    {
        auto surf = TTF_RenderUTF8_Blended(ptr,
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
        auto surf = TTF_RenderText_Blended_Wrapped(ptr, text, fg, max_width);
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
        auto surf = TTF_RenderUTF8_Blended_Wrapped(ptr,
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
        auto surf = TTF_RenderGlyph32_Blended(ptr, codepoint, fg);
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
        auto surf = TTF_RenderText_LCD(ptr, text, fg, bg);
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
        auto surf = TTF_RenderUTF8_LCD(ptr,
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
        auto surf = TTF_RenderText_LCD_Wrapped(ptr, text, fg, bg, max_width);
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
        auto surf = TTF_RenderUTF8_LCD_Wrapped(ptr,
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
        auto surf = TTF_RenderGlyph32_LCD(ptr, codepoint, fg, bg);
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
        int result = TTF_GetFontKerningSizeGlyphs32(ptr, prev_codepoint, codepoint);
        if (result < 0)
            throw error{};
        return result;
    }


    void
    font::set_sdf(bool enable)
    {
        if (TTF_SetFontSDF(ptr, enable ? SDL_TRUE : SDL_FALSE) < 0)
            throw error{};
    }


    bool
    font::get_sdf()
        const noexcept
    {
        return TTF_GetFontSDF(ptr);
    }


#if SDL_TTF_VERSION_ATLEAST(2, 20, 0)

    void
    font::set_direction(direction dir)
    {
        if (TTF_SetFontDirection(ptr, static_cast<TTF_Direction>(dir)) < 0)
            throw error{};
    }


    void
    font::set_script(const char* script)
    {
        if (TTF_SetFontScriptName(ptr, script) < 0)
            throw error{};
    }

#endif


} // namespace sdl::ttf
