/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <SDL2/SDL_stdinc.h>

#include "rect.hpp"

namespace sdl {

    [[nodiscard]]
    bool
    rect::intersects(const rect& other)
        noexcept
    {
        return SDL_HasIntersection(this, &other);
    }


    [[nodiscard]]
    rect
    intersect(const rect& a,
              const rect& b)
        noexcept
    {
        rect result;
        SDL_IntersectRect(&a, &b, &result);
        return result;
    }


    bool
    rect::clip(vec2& a,
               vec2& b)
        const noexcept
    {
        return SDL_IntersectRectAndLine(this,
                                        &a.x, &a.y,
                                        &b.x, &b.y);
    }


    [[nodiscard]]
    rect
    operator &(const rect& a,
               const rect& b)
        noexcept
    {
        return intersect(a, b);
    }


    [[nodiscard]]
    rect
    merge(const rect& a,
          const rect& b)
        noexcept
    {
        rect result;
        SDL_UnionRect(&a, & b, &result);
        return result;
    }


    [[nodiscard]]
    rect
    operator |(const rect& a,
               const rect& b)
        noexcept
    {
        return merge(a, b);
    }


    [[nodiscard]]
    std::pair<rect, bool>
    enclose(std::span<const vec2> points)
        noexcept
    {
        rect result;
        bool not_empty = SDL_EnclosePoints(points.data(),
                                           points.size(),
                                           nullptr,
                                           &result);
        return {result, not_empty};
    }


    [[nodiscard]]
    std::pair<rect, bool>
    enclose(std::span<const vec2> points,
            const rect& clip)
        noexcept
    {
        rect result;
        bool not_empty = SDL_EnclosePoints(points.data(),
                                           points.size(),
                                           &clip,
                                           &result);
        return {result, not_empty};
    }


    string
    to_string(const rect& r)
    {
        char buf[64];
        SDL_snprintf(buf, sizeof buf,
                     "rect{ (%d, %d), (%d x %d) }",
                     r.x, r.y, r.w, r.h);
        return buf;
    }


    std::ostream&
    operator <<(std::ostream& out,
                const rect& r)
    {
        return out << to_string(r);
    }



    // rectf

    bool
    rectf::intersects(const rectf& other)
        noexcept
    {
        return SDL_HasIntersectionF(this, &other);
    }


    bool
    rectf::clip(vec2f& a,
                vec2f& b)
        const noexcept
    {
        return SDL_IntersectFRectAndLine(this,
                                         &a.x, &a.y,
                                         &b.x, &b.y);
    }


    bool
    similar(const rectf& a,
            const rectf& b,
            float epsilon)
        noexcept
    {
        return SDL_FRectEqualsEpsilon(&a, &b, epsilon);
    }


    rectf
    intersect(const rectf& a,
              const rectf& b)
        noexcept
    {
        rectf result;
        SDL_IntersectFRect(&a, &b, &result);
        return result;
    }


    rectf
    operator &(const rectf& a,
               const rectf& b)
        noexcept
    {
        return intersect(a, b);
    }


    rectf
    merge(const rectf& a,
          const rectf& b)
        noexcept
    {
        rectf result;
        SDL_UnionFRect(&a, &b, &result);
        return result;
    }


    rectf
    operator |(const rectf& a,
               const rectf& b)
        noexcept
    {
        return merge(a, b);
    }


    std::pair<rectf, bool>
    enclose(std::span<const vec2f> points)
        noexcept
    {
        rectf result;
        bool not_empty = SDL_EncloseFPoints(points.data(),
                                            points.size(),
                                            nullptr,
                                            &result);
        return {result, not_empty};
    }


    std::pair<rectf, bool>
    enclose(std::span<const vec2f> points,
            const rectf& clip)
        noexcept
    {
        rectf result;
        bool not_empty = SDL_EncloseFPoints(points.data(),
                                            points.size(),
                                            &clip,
                                            &result);
        return {result, not_empty};
    }


    string
    to_string(const rectf& r)
    {
        char buf[128];
        SDL_snprintf(buf, sizeof buf,
                     "rectf{ (%f, %f), (%f x %f) }",
                     r.x, r.y, r.w, r.h);
        return buf;
    }


    std::ostream&
    operator <<(std::ostream& out,
                const rectf& r)
    {
        return out << to_string(r);
    }

} // namespace sdl
