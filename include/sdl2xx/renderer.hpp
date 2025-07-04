/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_RENDERER_HPP
#define SDL2XX_RENDERER_HPP

#include <concepts>
#include <optional>
#include <span>
#include <tuple>
#include <utility>

#include <SDL_render.h>

#include "angle.hpp"
#include "basic_wrapper.hpp"
#include "color.hpp"
#include "pixels.hpp"
#include "rect.hpp"
#include "string.hpp"
#include "vec2.hpp"
#include "vector.hpp"


namespace sdl {

    class window;
    class surface;
    class texture;

    using vertex = SDL_Vertex;


    struct renderer : basic_wrapper<SDL_Renderer*> {

        using base_type = basic_wrapper<SDL_Renderer*>;


        enum class flag : Uint32 {
            software       = SDL_RENDERER_SOFTWARE,
            accelerated    = SDL_RENDERER_ACCELERATED,
            present_vsync  = SDL_RENDERER_PRESENTVSYNC,
            target_texture = SDL_RENDERER_TARGETTEXTURE,
        };


        [[nodiscard]]
        static
        int
        get_num_drivers();


        struct info_t {
            string name;
            Uint32 flags;
            vector<pixels::format_enum> texture_formats;
            int max_texture_width;
            int max_texture_height;
        };

        [[nodiscard]]
        static
        info_t
        get_driver_info(int index);


        // Inherit constructors.
        using base_type::base_type;


        explicit
        renderer(SDL_Renderer* ren)
            noexcept;

        renderer(window& win,
                 int index,
                 Uint32 flags = 0);

        template<std::same_as<flag>... Flags>
        requires(sizeof...(Flags) > 0)
        renderer(window& win,
                 int index,
                 Flags... flags);

        explicit
        renderer(surface& surf);


        /// Move constructor.
        renderer(renderer&& other)
            noexcept;


        ~renderer()
            noexcept;


        /// Move assignment.
        renderer&
        operator =(renderer&& other)
            noexcept;


        void
        create(window& win,
               int index,
               Uint32 flags = 0);

        template<std::same_as<flag>... Flags>
        requires(sizeof...(Flags) > 0)
        void
        create(window& win,
               int index,
               Flags... flags);

        void
        create(surface& surf);


        void
        destroy()
            noexcept;


        void
        acquire(state_type state)
            noexcept;


        state_type
        release()
            noexcept;


        [[nodiscard]]
        window*
        get_window();


        [[nodiscard]]
        info_t
        get_info()
            const;


        [[nodiscard]]
        vec2
        get_output_size()
            const;


        [[nodiscard]]
        bool
        renter_target_supported()
            const noexcept;

        void
        set_render_target();

        void
        set_render_target(texture& tex);

        [[nodiscard]]
        texture*
        get_render_target()
            const noexcept;


        void
        set_logical_size(int width, int height);

        void
        set_logical_size(vec2 size);

        [[nodiscard]]
        vec2
        get_logical_size()
            const noexcept;

        [[nodiscard]]
        int
        get_logical_width()
            const noexcept;

        [[nodiscard]]
        int
        get_logical_height()
            const noexcept;


        void
        set_integer_scale(bool enable);

        [[nodiscard]]
        bool
        get_integer_scale()
            const noexcept;


        void
        reset_viewport();

        void
        set_viewport(const rect* vp);

        void
        set_viewport(const rect& vp);

        [[nodiscard]]
        rect
        get_viewport()
            const noexcept;


        void
        reset_clip();

        void
        set_clip(const rect* clip);

        void
        set_clip(const rect& clip);

        [[nodiscard]]
        rect
        get_clip()
            const noexcept;

        [[nodiscard]]
        bool
        is_clip_enabled()
            const noexcept;


        void
        set_scale(float scale_x,
                  float scale_y);

        void
        set_scale(vec2f scale);

        [[nodiscard]]
        vec2f
        get_scale()
            const noexcept;


        [[nodiscard]]
        vec2f
        window_to_logical(int win_x,
                          int win_y)
            const noexcept;

        [[nodiscard]]
        vec2f
        window_to_logical(vec2 win_pt)
            const noexcept;


        [[nodiscard]]
        vec2
        logical_to_window(float log_x,
                          float log_y)
            const noexcept;

        [[nodiscard]]
        vec2
        logical_to_window(vec2f log_pt)
            const noexcept;


        void
        set_color(Uint8 r,
                  Uint8 g,
                  Uint8 b,
                  Uint8 a = 0xff);

        void
        set_color(color c);

        [[nodiscard]]
        color
        get_color()
            const;


        void
        set_blend_mode(SDL_BlendMode mode);

        [[nodiscard]]
        SDL_BlendMode
        get_blend_mode()
            const;


        void
        clear();


        void
        draw_point(int x,
                   int y);

        void
        draw_point(float x,
                   float y);

        void
        draw_point(vec2 pt);

        void
        draw_point(vec2f pt);


        void
        draw_points(const vec2* pts,
                    std::size_t count);

        void
        draw_points(std::span<const vec2> pts);


        void
        draw_points(const vec2f* pts,
                    std::size_t count);

        void
        draw_points(std::span<const vec2f> pts);


        void
        draw_line(int a_x,
                  int a_y,
                  int b_x,
                  int b_y);

        void
        draw_line(float a_x,
                  float a_y,
                  float b_x,
                  float b_y);

        void
        draw_line(vec2 a,
                  vec2 b);

        void
        draw_line(vec2f a,
                  vec2f b);

        void
        draw_lines(std::span<const vec2> pts);

        void
        draw_lines(std::span<const vec2f> pts);


        void
        draw_box(const rect* box);

        void
        draw_box(const rectf* box);

        void
        draw_box();

        void
        draw_box(const rect& box);

        void
        draw_box(const rectf& box);

        void
        draw_boxes(std::span<const rect> boxes);

        void
        draw_boxes(std::span<const rectf> boxes);


        void
        fill_box(const rect* box);

        void
        fill_box(const rectf* box);

        void
        fill_box();

        void
        fill_box(const rect& box);

        void
        fill_box(const rectf& box);

        void
        fill_boxes(std::span<const rect> boxes);

        void
        fill_boxes(std::span<const rectf> boxes);


        void
        copy(const texture& tex);

        void
        copy(const texture& tex,
             const rect* src_area);

        void
        copy(const texture& tex,
             const rect* src_area,
             const rect* dst_area);

        void
        copy(const texture& tex,
             const rect* src_area,
             const rectf* dst_area);


        void
        copy(const texture& tex,
             const rect& src_area);

        void
        copy(const texture& tex,
             const std::optional<rect>& src_area,
             const rect& dst_area);

        void
        copy(const texture& tex,
             const std::optional<rect>& src_area,
             const rectf& dst_area);


        void
        copy_ex(const texture& tex,
                const rect* src_area,
                const rect* dst_area,
                degrees rot,
                const vec2* center,
                SDL_RendererFlip flip);

        void
        copy_ex(const texture& tex,
                const rect* src_area,
                const rectf* dst_area,
                degrees rot,
                const vec2f* center,
                SDL_RendererFlip flip);


        void
        geometry(const std::optional<texture>& tex,
                 std::span<const vertex> vertices);

        void
        geometry(const std::optional<texture>& tex,
                 std::span<const vertex> vertices,
                 std::span<const int> indices);


        void
        geometry_raw(const std::optional<texture>& tex,
                     const float* xy, int xy_stride,
                     const SDL_Color* col, int col_stride,
                     const float* uv, int uv_stride,
                     int num_vertices,
                     const void* indices, int num_indices,
                     int index_size);


        void
        read_pixels(const std::optional<rect>& area,
                    pixels::format_enum format,
                    void* pixels,
                    int pitch);


        void
        present()
            noexcept;


        void
        flush();


        void*
        get_metal_layer()
            noexcept;


        void*
        get_metal_command_encoder()
            noexcept;


        void
        set_vsync(bool enabled);


        [[nodiscard]]
        static
        renderer*
        get_wrapper(SDL_Renderer* ren)
            noexcept;


        [[nodiscard]]
        const renderer*
        get_wrapper(const SDL_Renderer* ren)
            noexcept;

    }; // struct renderer


    template<std::same_as<renderer::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    [[nodiscard]]
    constexpr
    Uint32
    convert(Flags... flags)
        noexcept
    {
        return (static_cast<Uint32>(flags) | ...);
    }


    // Implementation of templated methods.

    template<std::same_as<renderer::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    renderer::renderer(window& win,
                       int index,
                       Flags... flags) :
        renderer{win, index, convert(flags...)}
    {}


    template<std::same_as<renderer::flag>... Flags>
    requires(sizeof...(Flags) > 0)
    void
    renderer::create(window& win,
               int index,
               Flags... flags)
    {
        create(win, index, convert(flags...));
    }

} // namespace sdl

#endif
