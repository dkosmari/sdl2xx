/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_RECT_HPP
#define SDL2XX_RECT_HPP

#include <algorithm> // minmax()
#include <iosfwd>
#include <span>
#include <utility> // pair<>

#include <SDL_rect.h>

#include "string.hpp"
#include "vec2.hpp"


namespace sdl {

    struct rect : SDL_Rect {

        constexpr
        rect()
            noexcept :
            SDL_Rect{0, 0, 0, 0}
        {}


        constexpr
        rect(const SDL_Rect& r)
            noexcept :
            SDL_Rect{r.x, r.y, r.w, r.h}
        {}


        constexpr
        rect(int x, int y,
             int w, int h)
            noexcept :
            SDL_Rect{x, y, w, h}
        {}


        [[nodiscard]]
        static
        inline
        constexpr
        rect
        from_corners(vec2 a,
                     vec2 b)
            noexcept
        {
            auto [min_x, max_x] = std::minmax(a.x, b.x);
            auto [min_y, max_y] = std::minmax(a.y, b.y);
            return rect{
                min_x,
                min_y,
                max_x - min_x,
                max_y - min_y
            };
        }


        [[nodiscard]]
        static
        inline
        constexpr
        rect
        from_corner_size(vec2 corner,
                         vec2 size)
            noexcept
        {
            return rect{
                corner.x,
                corner.y,
                size.x,
                size.y
            };
        }


        [[nodiscard]]
        static
        inline
        constexpr
        rect
        from_center_size(vec2 center,
                         vec2 size)
            noexcept
        {
            return rect{
                center.x - size.x / 2,
                center.y - size.y / 2,
                size.x,
                size.y
            };
        }


        constexpr
        bool
        operator ==(const rect& other)
            const noexcept = default;


        [[nodiscard]]
        constexpr
        bool
        empty()
            const noexcept
        {
            return w <= 0 && h <= 0;
        }


        [[nodiscard]]
        constexpr
        bool
        contains(vec2 p)
            const noexcept
        {
            return p.x >= x
                && p.y >= y
                && p.x < x + w
                && p.y < y + h;
        }


        [[nodiscard]]
        bool
        intersects(const rect& other)
            noexcept;


        bool
        clip(vec2& a,
             vec2& b)
            const noexcept;


        constexpr
        void
        set_min_corner(vec2 corner)
            noexcept
        {
            w = x + w - corner.x;
            h = y + h - corner.y;
            x = corner.x;
            y = corner.y;
        }


        constexpr
        void
        set_max_corner(vec2 corner)
            noexcept
        {
            w = corner.x - x;
            h = corner.y - y;
        }


        constexpr
        void
        set_corners(vec2 a,
                    vec2 b)
            noexcept
        {
            auto [min_x, max_x] = std::minmax(a.x, b.x);
            auto [min_y, max_y] = std::minmax(a.y, b.y);
            x = min_x;
            y = min_y;
            w = max_x - min_x;
            h = max_y - min_y;
        }


        constexpr
        void
        set_center(vec2 center)
            noexcept
        {
            x = center.x - w / 2;
            y = center.y - h / 2;
        }


        constexpr
        void
        set_size(vec2 size)
            noexcept
        {
            w = size.x;
            h = size.y;
        }


        constexpr
        void
        set_size(int new_w,
                 int new_h)
            noexcept
        {
            w = new_w;
            h = new_h;
        }


        [[nodiscard]]
        constexpr
        std::pair<vec2, vec2>
        get_corners()
            const noexcept
        {
            return {
                vec2{x, y},
                vec2{x + w, y + h}
            };
        }


        [[nodiscard]]
        constexpr
        vec2
        get_min_corner()
            const noexcept
        {
            return vec2{x, y};
        }


        [[nodiscard]]
        constexpr
        vec2
        get_max_corner()
            const noexcept
        {
            return vec2{x + w, y + h};
        }


        [[nodiscard]]
        constexpr
        vec2
        get_center()
            const noexcept
        {
            return vec2{
                x + w / 2,
                y + h / 2
            };
        }


        [[nodiscard]]
        constexpr
        vec2
        get_size()
            const noexcept
        {
            return vec2{w, h};
        }

    };

    static_assert(sizeof(rect) == sizeof(SDL_Rect));


    [[nodiscard]]
    rect
    intersect(const rect& a,
              const rect& b)
        noexcept;


    /// Same as intersect()
    [[nodiscard]]
    rect
    operator &(const rect& a,
               const rect& b)
        noexcept;


    [[nodiscard]]
    rect
    merge(const rect& a,
          const rect& b)
        noexcept;


    /// Same as merge()
    [[nodiscard]]
    rect
    operator |(const rect& a,
               const rect& b)
        noexcept;


    [[nodiscard]]
    std::pair<rect, bool>
    enclose(std::span<const vec2> points)
        noexcept;


    [[nodiscard]]
    std::pair<rect, bool>
    enclose(std::span<const vec2> points,
            const rect& clip)
        noexcept;


    string
    to_string(const rect& r);


    std::ostream&
    operator <<(std::ostream& out,
                const rect& r);




    struct rectf : SDL_FRect {

        constexpr
        rectf()
            noexcept :
            SDL_FRect{0, 0, 0, 0}
        {}


        constexpr
        rectf(const SDL_FRect& r)
            noexcept :
            SDL_FRect{r.x, r.y, r.w, r.h}
        {}


        constexpr
        rectf(float x, float y,
              float w, float h)
            noexcept :
            SDL_FRect{x, y, w, h}
        {}


        [[nodiscard]]
        static
        inline
        constexpr
        rectf
        from_corners(vec2f a,
                     vec2f b)
            noexcept
        {
            auto [min_x, max_x] = std::minmax(a.x, b.x);
            auto [min_y, max_y] = std::minmax(a.y, b.y);
            return rectf{
                min_x,
                min_y,
                max_x - min_x,
                max_y - min_y
            };
        }

        [[nodiscard]]
        static
        inline
        constexpr
        rectf
        from_corner_size(vec2f corner,
                         vec2f size)
            noexcept
        {
            return rectf{
                corner.x,
                corner.y,
                size.x,
                size.y
            };
        }


        [[nodiscard]]
        static
        inline
        constexpr
        rectf
        from_center_size(vec2f center,
                         vec2f size)
            noexcept
        {
            return rectf{
                center.x - size.x / 2,
                center.y - size.y / 2,
                size.x,
                size.y
            };
        }


        constexpr
        bool
        operator ==(const rectf& other)
            const noexcept = default;


        [[nodiscard]]
        constexpr
        bool
        empty()
            const noexcept
        {
            return w <= 0 && h <= 0;
        }


        [[nodiscard]]
        constexpr
        bool
        contains(vec2f p)
            const noexcept
        {
            return p.x >= x
                && p.y >= y
                && p.x < x + w
                && p.y < y + h;
        }


        [[nodiscard]]
        bool
        intersects(const rectf& other)
            noexcept;


        bool
        clip(vec2f& a,
             vec2f& b)
            const noexcept;


        constexpr
        void
        set_min_corner(vec2f corner)
            noexcept
        {
            x = corner.x;
            y = corner.y;
        }


        constexpr
        void
        set_max_corner(vec2f corner)
            noexcept
        {
            w = corner.x - x;
            h = corner.y - y;
        }


        constexpr
        void
        set_corners(vec2f a,
                    vec2f b)
            noexcept
        {
            auto [min_x, max_x] = std::minmax(a.x, b.x);
            auto [min_y, max_y] = std::minmax(a.y, b.y);
            x = min_x;
            y = min_y;
            w = max_x - min_x;
            h = max_y - min_y;
        }


        constexpr
        void
        set_center(vec2f center)
            noexcept
        {
            x = center.x - w / 2;
            y = center.y - h / 2;
        }


        constexpr
        void
        set_size(float new_w,
                 float new_h)
            noexcept
        {
            w = new_w;
            h = new_h;
        }


        constexpr
        void
        set_size(vec2f size)
            noexcept
        {
            w = size.x;
            h = size.y;
        }


        [[nodiscard]]
        constexpr
        std::pair<vec2f, vec2f>
        get_corners()
            const noexcept
        {
            return {
                vec2f{x, y},
                vec2f{x + w, y + h}
            };
        }


        [[nodiscard]]
        constexpr
        vec2f
        get_min_corner()
            const noexcept
        {
            return vec2f{x, y};
        }


        [[nodiscard]]
        constexpr
        vec2f
        get_max_corner()
            const noexcept
        {
            return vec2f{x + w, y + h};
        }


        [[nodiscard]]
        constexpr
        vec2f
        get_center()
            const noexcept
        {
            return vec2f{
                x + w / 2,
                y + h / 2
            };
        }


        [[nodiscard]]
        constexpr
        vec2f
        get_size()
            const noexcept
        {
            return vec2f{w, h};
        }

    };

    static_assert(sizeof(rectf) == sizeof(SDL_FRect));


    [[nodiscard]]
    bool
    similar(const rectf& a,
            const rectf& b,
            float epsilon = SDL_FLT_EPSILON)
        noexcept;


    [[nodiscard]]
    rectf
    intersect(const rectf& a,
              const rectf& b)
        noexcept;


    /// Same as intersect()
    [[nodiscard]]
    rectf
    operator &(const rectf& a,
               const rectf& b)
        noexcept;


    [[nodiscard]]
    rectf
    merge(const rectf& a,
          const rectf& b)
        noexcept;


    /// Same as merge()
    [[nodiscard]]
    rectf
    operator |(const rectf& a,
               const rectf& b)
        noexcept;


    [[nodiscard]]
    std::pair<rect, bool>
    enclose(std::span<const vec2> points)
        noexcept;


    [[nodiscard]]
    std::pair<rectf, bool>
    enclose(std::span<const vec2f> points,
            const rectf& clip)
        noexcept;


    string
    to_string(const rectf& r);


    std::ostream&
    operator <<(std::ostream& out,
                const rectf& r);


} // namespace sdl

#endif
