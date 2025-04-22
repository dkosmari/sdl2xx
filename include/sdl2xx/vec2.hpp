/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_VEC2_HPP
#define SDL2XX_VEC2_HPP

#include <cmath>
#include <iosfwd>

#include <SDL_rect.h>

#include "string.hpp"


namespace sdl {

    struct vec2f;


    struct vec2 : SDL_Point {

        constexpr
        vec2()
            noexcept :
            SDL_Point{0, 0}
        {}


        constexpr
        vec2(const SDL_Point& pt)
            noexcept :
            SDL_Point{pt}
        {}


        constexpr
        vec2(int x, int y)
            noexcept :
            SDL_Point{x, y}
        {}


        explicit
        constexpr
        vec2(const vec2f& other)
            noexcept;


        [[nodiscard]]
        constexpr
        bool
        operator ==(const vec2& other)
            const noexcept = default;

    };

    static_assert(sizeof(vec2) == sizeof(SDL_Point));

    using point = vec2;


    [[nodiscard]]
    float
    length(vec2 v)
        noexcept;


    [[nodiscard]]
    inline
    constexpr
    int
    length2(vec2 v)
        noexcept
    {
        return v.x * v.x + v.y * v.y;
    }


    // arithmetic operators

    [[nodiscard]]
    inline
    constexpr
    vec2
    operator +(vec2 a)
        noexcept
    {
        return a;
    }


    [[nodiscard]]
    inline
    constexpr
    vec2
    operator -(vec2 a)
        noexcept
    {
        return vec2{-a.x, -a.y};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2
    operator +(vec2 a,
               vec2 b)
        noexcept
    {
        return {a.x + b.x, a.y + b.y};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2
    operator -(vec2 a,
               vec2 b)
        noexcept
    {
        return {a.x - b.x, a.y - b.y};
    }


    // component-wise multiplication
    [[nodiscard]]
    constexpr
    vec2
    scaled(vec2 a,
           vec2 b)
    {
        return {a.x * b.x, a.y * b.y};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2
    operator *(int a,
               vec2 b)
        noexcept
    {
        return {a * b.x, a * b.y};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2
    operator *(vec2 a,
               int b)
        noexcept
    {
        return {a.x * b, a.y * b};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2
    operator /(vec2 a,
               int b)
        noexcept
    {
        return {a.x / b, a.y / b};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2
    operator %(vec2 a,
               int b)
        noexcept
    {
        return {a.x % b, a.y % b};
    }


    [[nodiscard]]
    inline
    constexpr
    std::pair<vec2, vec2>
    div(vec2 a,
        int b)
        noexcept
    {
        return {a / b, a % b};
    }


    [[nodiscard]]
    inline
    constexpr
    int
    dot(vec2 a,
        vec2 b)
        noexcept
    {
        return a.x * b.x + a.y * b.y;
    }


    // assignment operators

    inline
    constexpr
    vec2&
    operator +=(vec2& a,
                const vec2 b)
        noexcept
    {
        a.x += b.x;
        a.y += b.y;
        return a;
    }


    inline
    constexpr
    vec2&
    operator -=(vec2& a,
                const vec2 b)
        noexcept
    {
        a.x -= b.x;
        a.y -= b.y;
        return a;
    }


    inline
    constexpr
    vec2&
    scale(vec2& a,
          vec2 b)
        noexcept
    {
        a.x *= b.x;
        a.y *= b.y;
        return a;
    }


    inline
    constexpr
    vec2&
    operator *=(vec2& a,
                int b)
    {
        a.x *= b;
        a.y *= b;
        return a;
    }


    inline
    constexpr
    vec2&
    operator /=(vec2& a,
                int b)
    {
        a.x /= b;
        a.y /= b;
        return a;
    }


    inline
    constexpr
    vec2&
    operator %=(vec2& a,
                int b)
    {
        a.x %= b;
        a.y %= b;
        return a;
    }


    // serialization

    [[nodiscard]]
    string
    to_string(vec2 v);


    std::ostream&
    operator <<(std::ostream& out,
                vec2 v);



    struct vec2f : SDL_FPoint {

        constexpr
        vec2f()
            noexcept :
            SDL_FPoint{0, 0}
        {}


        constexpr
        vec2f(const SDL_FPoint& pt)
            noexcept :
            SDL_FPoint{pt}
        {}


        constexpr
        vec2f(float x, float y)
            noexcept :
            SDL_FPoint{x, y}
        {}


        explicit
        constexpr
        vec2f(const vec2& other)
            noexcept;


        [[nodiscard]]
        constexpr
        bool
        operator ==(const vec2f& other)
            const noexcept = default;

    };

    static_assert(sizeof(vec2f) == sizeof(SDL_FPoint));

    using pointf = vec2f;


    [[nodiscard]]
    float
    length(vec2f v)
        noexcept;


    [[nodiscard]]
    inline
    constexpr
    float
    length2(vec2f v)
        noexcept
    {
        return v.x * v.x + v.y * v.y;
    }


    [[nodiscard]]
    vec2f
    normalized(vec2f v)
        noexcept;


    // arithmetic operators

    [[nodiscard]]
    inline
    constexpr
    vec2f
    operator +(vec2f a)
        noexcept
    {
        return a;
    }


    [[nodiscard]]
    inline
    constexpr
    vec2f
    operator -(vec2f a)
        noexcept
    {
        return vec2f{-a.x, -a.y};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2f
    operator +(const vec2f& a,
               const vec2f& b)
        noexcept
    {
        return {a.x + b.x, a.y + b.y};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2f
    operator -(vec2f a,
               vec2f b)
        noexcept
    {
        return {a.x - b.x, a.y - b.y};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2f
    scaled(vec2f a,
           vec2f b)
        noexcept
    {
        return {a.x * b.x, a.y * b.y};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2f
    operator *(float a,
               vec2f b)
        noexcept
    {
        return {a * b.x, a * b.y};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2f
    operator *(vec2f a,
               float b)
        noexcept
    {
        return {a.x * b, a.y * b};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2f
    operator /(vec2f a,
               float b)
        noexcept
    {
        return {a.x / b, a.y / b};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2f
    ceil(vec2f a)
        noexcept
    {
        return {std::ceil(a.x), std::ceil(a.x)};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2f
    floor(vec2f a)
        noexcept
    {
        return {std::floor(a.x), std::floor(a.x)};
    }


    [[nodiscard]]
    inline
    constexpr
    vec2f
    trunc(vec2f a)
        noexcept
    {
        return {std::trunc(a.x), std::trunc(a.x)};
    }


    [[nodiscard]]
    inline
    //constexpr
    std::pair<vec2f, vec2f>
    modf(vec2f a)
        noexcept
    {
        float ix;
        float fx = std::modf(a.x, &ix);
        float fy;
        float iy = std::modf(a.y, &iy);
        return {
            vec2f{ix, fx},
            vec2f{iy, fy}
        };
    }


    [[nodiscard]]
    inline
    constexpr
    float
    dot(vec2f a,
        vec2f b)
        noexcept
    {
        return a.x * b.x + a.y * b.y;
    }


    // assignment operators

    inline
    constexpr
    vec2f&
    operator +=(vec2f& a,
                const vec2f b)
        noexcept
    {
        a.x += b.x;
        a.y += b.y;
        return a;
    }


    inline
    constexpr
    vec2f&
    operator -=(vec2f& a,
                const vec2f b)
        noexcept
    {
        a.x -= b.x;
        a.y -= b.y;
        return a;
    }


    inline
    constexpr
    vec2f&
    scale(vec2f& a,
          vec2f b)
        noexcept
    {
        a.x *= b.x;
        a.y *= b.y;
        return a;
    }


    inline
    constexpr
    vec2f&
    operator *=(vec2f& a,
                float b)
    {
        a.x *= b;
        a.y *= b;
        return a;
    }


    inline
    constexpr
    vec2f&
    operator /=(vec2f& a,
                float b)
    {
        a.x /= b;
        a.y /= b;
        return a;
    }


    [[nodiscard]]
    inline
    vec2f
    with_length(vec2f v,
                float len)
        noexcept
    {
        return len * normalized(v);
    }


    // serialization

    string
    to_string(vec2f v);


    std::ostream&
    operator <<(std::ostream& out,
                vec2f v);


    // explicit conversions

    inline
    constexpr
    vec2::vec2(const vec2f& other)
        noexcept :
        SDL_Point(static_cast<int>(other.x),
                  static_cast<int>(other.y))
    {}


    inline
    constexpr
    vec2f::vec2f(const vec2& other)
        noexcept :
        SDL_FPoint(static_cast<float>(other.x),
                   static_cast<float>(other.y))
    {}

} // namespace sdl

#endif
