/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_COLOR_HPP
#define SDL2XX_COLOR_HPP

#include <iosfwd>

#include <SDL_pixels.h>

#include "angle.hpp"
#include "string.hpp"


namespace sdl {

    struct rgb {
        float r = 0; ///< in [0, 1]
        float g = 0; ///< in [0, 1]
        float b = 0; ///< in [0, 1]

        constexpr
        bool
        operator ==(const rgb& other)
            const noexcept = default;
    };


    struct rgba : rgb {
        float a = 0; ///< in [0, 1]

        constexpr
        bool
        operator ==(const rgba& other)
            const noexcept = default;
    };


    struct hsl {
        degreesf h = 0_degf;
        float s = 0; ///< in [0, 1]
        float l = 0; ///< in [0, 1]

        constexpr
        bool
        operator ==(const hsl& other)
            const noexcept = default;
    };


    struct hsv {
        degreesf h = 0_degf;
        float s = 0; ///< in [0, 1]
        float v = 0; ///< in [0, 1]

        constexpr
        bool
        operator ==(const hsv& other)
            const noexcept = default;
    };


    struct color : SDL_Color {

        constexpr
        color()
            noexcept :
            SDL_Color{0, 0, 0, 0xff}
        {}


        constexpr
        color(Uint8 r_,
              Uint8 g_,
              Uint8 b_,
              Uint8 a_ = 0xff)
            noexcept :
            SDL_Color{r_, g_, b_, a_}
        {}


        static
        color
        from_rgb(float r,
                 float g,
                 float b)
            noexcept;

        static
        inline
        color
        from_rgb(const rgb& src)
            noexcept
        {
            return from_rgb(src.r, src.g, src.b);
        }


        static
        color
        from_rgba(float r,
                  float g,
                  float b,
                  float a)
            noexcept;

        static
        inline
        color
        from_rgba(const rgba& src)
            noexcept
        {
            return from_rgba(src.r, src.g, src.b, src.a);
        }


        static
        color
        from_hsl(degreesf h,
                 float s,
                 float l)
            noexcept;

        static
        color
        from_hsl(const hsl& src)
            noexcept
        {
            return from_hsl(src.h, src.s, src.l);
        }

        static
        color
        from_hsv(degreesf h,
                 float s,
                 float v)
            noexcept;

        static
        color
        from_hsv(const hsv& src)
            noexcept
        {
            return from_hsv(src.h, src.s, src.v);
        }


        constexpr
        rgb
        to_rgb()
            const noexcept
        {
            return {
                r / 255.0f,
                g / 255.0f,
                b / 255.0f
            };
        }


        constexpr
        rgba
        to_rgba()
            const noexcept
        {
            return {
                r / 255.0f,
                g / 255.0f,
                b / 255.0f,
                a / 255.0f
            };
        }


        hsl
        to_hsl()
            const noexcept;


        hsv
        to_hsv()
            const noexcept;


        constexpr
        bool
        operator ==(const color& other)
            const noexcept
        {
            return r == other.r
                && g == other.g
                && b == other.b
                && a == other.a;
        }


        static const color black;
        static const color white;

        static const color transparent;

        // Primary colors.
        static const color red;
        static const color green;
        static const color blue;

        // Secondary colors.
        static const color cyan;
        static const color magenta;
        static const color yellow;

        // Dark colors.
        static const color maroon; // aka dark red
        static const color ao;     // aka dark green
        static const color navy;   // aka dark blue
        static const color teal;   // aka dark cyan
        static const color purple; // aka dark magenta
        static const color olive;  // aka dark yellow

    };

    static_assert(sizeof(color) == sizeof(SDL_Color));


    inline
    namespace literals {

        inline
        namespace color_literals {

            constexpr
            color
            operator ""_rgb(unsigned long long rgb)
                noexcept
            {
                return {
                    static_cast<Uint8>(rgb >> 16u),
                    static_cast<Uint8>(rgb >>  8u),
                    static_cast<Uint8>(rgb >>  0u),
                    0xff
                };
            }

            constexpr
            color
            operator ""_rgba(unsigned long long rgba)
                noexcept
            {
                return {
                    static_cast<Uint8>(rgba >> 24u),
                    static_cast<Uint8>(rgba >> 16u),
                    static_cast<Uint8>(rgba >>  8u),
                    static_cast<Uint8>(rgba >>  0u)
                };
            }

        } // namespace color_literals

    } // namespace literals


    // TODO: implement arithmetic for rgb, rgba, hsl, hsv


    // serialization

    string
    to_string(const hsl& c);

    string
    to_string(const hsv& c);

    string
    to_string(color c);


    std::ostream&
    operator <<(std::ostream& out,
                const hsl& c);

    std::ostream&
    operator <<(std::ostream& out,
                const hsv& c);

    std::ostream&
    operator <<(std::ostream& out,
                color c);




    inline const color color::black{0, 0, 0};
    inline const color color::white{0xff, 0xff, 0xff};

    inline const color color::transparent{0, 0, 0, 0};

    // Primary colors
    inline const color color::red   = 0xff0000_rgb;
    inline const color color::green = 0x00ff00_rgb;
    inline const color color::blue  = 0x0000ff_rgb;

    // Secondary colors
    inline const color color::cyan    = 0x00ffff_rgb;
    inline const color color::magenta = 0xff00ff_rgb;
    inline const color color::yellow  = 0xffff00_rgb;

    // Dark colors.
    inline const color color::maroon = 0x800000_rgb;
    inline const color color::ao     = 0x008000_rgb;
    inline const color color::navy   = 0x000080_rgb;
    inline const color color::teal   = 0x008080_rgb;
    inline const color color::purple = 0x800080_rgb;
    inline const color color::olive  = 0x808000_rgb;

} // namespace sdl

#endif
