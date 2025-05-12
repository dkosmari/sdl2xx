/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include "renderer.hpp"

#include "error.hpp"
#include "surface.hpp"
#include "texture.hpp"
#include "window.hpp"


namespace sdl {

    namespace {

        namespace detail {

            // This implements a table to map raw pointers to their C++ wrapper.
            // TODO: these should all be protected by a mutex.


            using element_t = std::pair<const SDL_Renderer*,
                                        const renderer*>;

            vector<element_t> renderer_map;


            void
            update(const SDL_Renderer* raw,
                   const renderer* wrap)
            {
                for (auto& [m_raw, m_wrap] : renderer_map) {
                    if (m_raw == raw) {
                        m_wrap = wrap;
                        return;
                    }
                }
                renderer_map.emplace_back(raw, wrap);
            }


            void
            remove(const SDL_Renderer* raw)
            {
                std::erase_if(renderer_map,
                              [raw](const element_t& elem) -> bool
                              {
                                  return elem.first == raw;
                              });
            }


            const renderer*
            find(const SDL_Renderer* raw)
            {
                for (auto [m_raw, m_wrap] : renderer_map)
                    if (m_raw == raw)
                        return m_wrap;
                return nullptr;
            }


            renderer*
            find(SDL_Renderer* raw)
            {
                for (auto [m_raw, m_wrap] : renderer_map)
                    if (m_raw == raw)
                        return const_cast<renderer*>(m_wrap);
                return nullptr;
            }


            renderer::info_t
            convert(const SDL_RendererInfo& src)
            {
                vector<pixels::format_enum> formats(src.num_texture_formats);
                for (Uint32 i = 0; i < src.num_texture_formats; ++i)
                    formats[i] = static_cast<pixels::format_enum>(src.texture_formats[i]);
                return {
                    .name               = src.name,
                    .flags              = src.flags,
                    .texture_formats    = std::move(formats),
                    .max_texture_width  = src.max_texture_width,
                    .max_texture_height = src.max_texture_height
                };
            }

        } // namespace detail

    } // namespace


    int
    renderer::get_num_drivers()
    {
        int result = SDL_GetNumRenderDrivers();
        if (result < 0)
            throw error{};
        return result;
    }


    renderer::info_t
    renderer::get_driver_info(int index)
    {
        SDL_RendererInfo info;
        if (SDL_GetRenderDriverInfo(index, &info) < 0)
            throw error{};
        return detail::convert(info);
    }


    renderer::renderer(SDL_Renderer* ren)
        noexcept
    {
        acquire(ren);
    }


    renderer::renderer(window& win,
                       int index,
                       Uint32 flags)
    {
        create(win, index, flags);
    }


    renderer::renderer(surface& surf)
    {
        create(surf);
    }


    renderer::renderer(renderer&& other)
        noexcept
    {
        acquire(other.release());
    }


    renderer::~renderer()
        noexcept
    {
        destroy();
    }


    renderer&
    renderer::operator =(renderer&& other)
        noexcept
    {
        if (this != &other) {
            destroy();
            acquire(other.release());
        }
        return *this;
    }


    void
    renderer::create(window& win,
                     int index,
                     Uint32 flags)
    {
        auto ptr = SDL_CreateRenderer(win.data(), index, flags);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    renderer::create(surface& surf)
    {
        auto ptr = SDL_CreateSoftwareRenderer(surf.data());
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    renderer::destroy()
        noexcept
    {
        if (is_valid())
            SDL_DestroyRenderer(release());
    }


    void
    renderer::acquire(state_type state)
        noexcept
    {
        parent_type::acquire(state);
        if (raw)
            detail::update(raw, this);
    }


    renderer::state_type
    renderer::release()
        noexcept
    {
        detail::remove(raw);
        return parent_type::release();
    }


    window*
    renderer::get_window()
    {
        SDL_Window* win = SDL_RenderGetWindow(raw);
        if (!win)
            throw error{};
        return window::get_wrapper(win);
    }


    renderer::info_t
    renderer::get_info()
        const
    {
        SDL_RendererInfo info;
        if (SDL_GetRendererInfo(raw, &info) < 0)
            throw error{};
        return detail::convert(info);
    }


    vec2
    renderer::get_output_size()
        const
    {
        vec2 result;
        if (SDL_GetRendererOutputSize(raw, &result.x, &result.y) < 0)
            throw error{};
        return result;
    }


    bool
    renderer::renter_target_supported()
        const noexcept
    {
        return SDL_RenderTargetSupported(raw);
    }


    void
    renderer::set_render_target()
    {
        if (SDL_SetRenderTarget(raw, nullptr) < 0)
            throw error{};
    }


    void
    renderer::set_render_target(texture& tex)
    {
        if (SDL_SetRenderTarget(raw, tex.data()) < 0)
            throw error{};
    }


    texture*
    renderer::get_render_target()
        const noexcept
    {
        return texture::get_wrapper(SDL_GetRenderTarget(raw));
    }


    void
    renderer::set_logical_size(int width,
                               int height)
    {
        if (SDL_RenderSetLogicalSize(raw, width, height) < 0)
            throw error{};
    }


    void
    renderer::set_logical_size(vec2 size)
    {
        set_logical_size(size.x, size.y);
    }


    vec2
    renderer::get_logical_size()
        const noexcept
    {
        vec2 size;
        SDL_RenderGetLogicalSize(raw, &size.x, &size.y);
        return size;
    }


    int
    renderer::get_logical_width()
        const noexcept
    {
        int width;
        SDL_RenderGetLogicalSize(raw, &width, nullptr);
        return width;
    }


    int
    renderer::get_logical_height()
        const noexcept
    {
        int height;
        SDL_RenderGetLogicalSize(raw, nullptr, &height);
        return height;
    }


    void
    renderer::set_integer_scale(bool enable)
    {
        if (SDL_RenderSetIntegerScale(raw, enable ? SDL_TRUE : SDL_FALSE) < 0)
            throw error{};
    }


    bool
    renderer::get_integer_scale()
        const noexcept
    {
        return SDL_RenderGetIntegerScale(raw);
    }


    void
    renderer::reset_viewport()
    {
        set_viewport(nullptr);
    }


    void
    renderer::set_viewport(const rect* vp)
    {
        if (SDL_RenderSetViewport(raw, vp) < 0)
            throw error{};
    }


    void
    renderer::set_viewport(const rect& vp)
    {
        set_viewport(&vp);
    }


    rect
    renderer::get_viewport()
        const noexcept
    {
        rect vp;
        SDL_RenderGetViewport(raw, &vp);
        return vp;
    }


    void
    renderer::reset_clip()
    {
        set_clip(nullptr);
    }


    void
    renderer::set_clip(const rect* clip)
    {
        if (SDL_RenderSetClipRect(raw, clip) < 0)
            throw error{};
    }


    void
    renderer::set_clip(const rect& clip)
    {
        set_clip(&clip);
    }


    rect
    renderer::get_clip()
        const noexcept
    {
        rect clip;
        SDL_RenderGetClipRect(raw, &clip);
        return clip;
    }


    bool
    renderer::is_clip_enabled()
        const noexcept
    {
        return SDL_RenderIsClipEnabled(raw);
    }


    void
    renderer::set_scale(float scale_x,
                        float scale_y)
    {
        if (SDL_RenderSetScale(raw, scale_x, scale_y) < 0)
            throw error{};
    }


    void
    renderer::set_scale(vec2f scale)
    {
        set_scale(scale.x, scale.y);
    }


    vec2f
    renderer::get_scale()
        const noexcept
    {
        vec2f scale;
        SDL_RenderGetScale(raw, &scale.x, &scale.y);
        return scale;
    }


    vec2f
    renderer::window_to_logical(int win_x,
                                int win_y)
        const noexcept
    {
        vec2f result;
        SDL_RenderWindowToLogical(raw, win_x, win_y, &result.x, &result.y);
        return result;
    }


    vec2f
    renderer::window_to_logical(vec2 win_pt)
        const noexcept
    {
        return window_to_logical(win_pt.x, win_pt.y);
    }


    vec2
    renderer::logical_to_window(float log_x,
                                float log_y)
        const noexcept
    {
        vec2 result;
        SDL_RenderLogicalToWindow(raw, log_x, log_y, &result.x, &result.y);
        return result;
    }


    vec2
    renderer::logical_to_window(vec2f log_pt)
        const noexcept
    {
        return logical_to_window(log_pt.x, log_pt.y);
    }


    void
    renderer::set_color(Uint8 r,
                        Uint8 g,
                        Uint8 b,
                        Uint8 a)
    {
        if (SDL_SetRenderDrawColor(raw, r, g, b, a) < 0)
            throw error{};
    }


    void
    renderer::set_color(color c)
    {
        set_color(c.r, c.g, c.b, c.a);
    }


    color
    renderer::get_color()
        const
    {
        color c;
        if (SDL_GetRenderDrawColor(raw, &c.r, &c.g, &c.b, &c.a) < 0)
            throw error{};
        return c;
    }


    void
    renderer::set_blend_mode(SDL_BlendMode mode)
    {
        if (SDL_SetRenderDrawBlendMode(raw, mode) < 0)
            throw error{};
    }


    SDL_BlendMode
    renderer::get_blend_mode()
        const
    {
        SDL_BlendMode mode;
        if (SDL_GetRenderDrawBlendMode(raw, &mode) < 0)
            throw error{};
        return mode;
    }


    void
    renderer::clear()
    {
        if (SDL_RenderClear(raw) < 0)
            throw error{};

    }


    void
    renderer::draw_point(int x,
                         int y)
    {
        if (SDL_RenderDrawPoint(raw, x, y) < 0)
            throw error{};
    }


    void
    renderer::draw_point(float x,
                         float y)
    {
        if (SDL_RenderDrawPointF(raw, x, y) < 0)
            throw error{};
    }


    void
    renderer::draw_point(vec2 pt)
    {
        draw_point(pt.x, pt.y);
    }


    void
    renderer::draw_point(vec2f pt)
    {
        draw_point(pt.x, pt.y);
    }


    void
    renderer::draw_points(const vec2* pts,
                          std::size_t count)
    {
        if (SDL_RenderDrawPoints(raw, pts, count) < 0)
            throw error{};
    }


    void
    renderer::draw_points(std::span<const vec2> pts)
    {
        draw_points(pts.data(), pts.size());
    }


    void
    renderer::draw_points(const vec2f* pts,
                          std::size_t count)
    {
        if (SDL_RenderDrawPointsF(raw, pts, count) < 0)
            throw error{};
    }


    void
    renderer::draw_points(std::span<const vec2f> pts)
    {
        draw_points(pts.data(), pts.size());
    }


    void
    renderer::draw_line(int a_x,
                        int a_y,
                        int b_x,
                        int b_y)
    {
        if (SDL_RenderDrawLine(raw,
                               a_x, a_y,
                               b_x, b_y) < 0)
            throw error{};
    }


    void
    renderer::draw_line(float a_x,
                        float a_y,
                        float b_x,
                        float b_y)
    {
        if (SDL_RenderDrawLineF(raw,
                                a_x, a_y,
                                b_x, b_y) < 0)
            throw error{};
    }


    void
    renderer::draw_line(vec2 a,
                        vec2 b)
    {
        draw_line(a.x, a.y, b.x, b.y);
    }


    void
    renderer::draw_line(vec2f a,
                        vec2f b)
    {
        draw_line(a.x, a.y, b.x, b.y);
    }


    void
    renderer::draw_lines(std::span<const vec2> pts)
    {
        if (SDL_RenderDrawLines(raw, pts.data(), pts.size()) < 0)
            throw error{};
    }


    void
    renderer::draw_lines(std::span<const vec2f> pts)
    {
        if (SDL_RenderDrawLinesF(raw, pts.data(), pts.size()) < 0)
            throw error{};
    }


    void
    renderer::draw_box(const rect* box)
    {
        if (SDL_RenderDrawRect(raw, box) < 0)
            throw error{};
    }


    void
    renderer::draw_box(const rectf* box)
    {
        if (SDL_RenderDrawRectF(raw, box) < 0)
            throw error{};
    }


    void
    renderer::draw_box()
    {
        if (SDL_RenderDrawRect(raw, nullptr) < 0)
            throw error{};
    }


    void
    renderer::draw_box(const rect& box)
    {
        draw_box(&box);
    }


    void
    renderer::draw_box(const rectf& box)
    {
        draw_box(&box);
    }


    void
    renderer::draw_boxes(std::span<const rect> boxes)
    {
        if (SDL_RenderDrawRects(raw, boxes.data(), boxes.size()) < 0)
            throw error{};
    }


    void
    renderer::draw_boxes(std::span<const rectf> boxes)
    {
        if (SDL_RenderDrawRectsF(raw, boxes.data(), boxes.size()) < 0)
            throw error{};
    }


    void
    renderer::fill_box(const rect* box)
    {
        if (SDL_RenderFillRect(raw, box) < 0)
            throw error{};
    }


    void
    renderer::fill_box(const rectf* box)
    {
        if (SDL_RenderFillRectF(raw, box) < 0)
            throw error{};
    }


    void
    renderer::fill_box()
    {
        if (SDL_RenderFillRect(raw, nullptr) < 0)
            throw error{};
    }


    void
    renderer::fill_box(const rect& box)
    {
        fill_box(&box);
    }


    void
    renderer::fill_box(const rectf& box)
    {
        fill_box(&box);
    }


    void
    renderer::fill_boxes(std::span<const rect> boxes)
    {
        if (SDL_RenderFillRects(raw, boxes.data(), boxes.size()) < 0)
            throw error{};
    }


    void
    renderer::fill_boxes(std::span<const rectf> boxes)
    {
        if (SDL_RenderFillRectsF(raw, boxes.data(), boxes.size()) < 0)
            throw error{};
    }


    void
    renderer::copy(const texture& tex)
    {
        if (SDL_RenderCopy(raw,
                           const_cast<SDL_Texture*>(tex.data()),
                           nullptr,
                           nullptr) < 0)
            throw error{};
    }


    void
    renderer::copy(const texture& tex,
                   const rect* src_area)
    {
        if (SDL_RenderCopy(raw,
                           const_cast<SDL_Texture*>(tex.data()),
                           src_area,
                           nullptr) < 0)
            throw error{};
    }


    void
    renderer::copy(const texture& tex,
                   const rect* src_area,
                   const rect* dst_area)
    {
        if (SDL_RenderCopy(raw,
                           const_cast<SDL_Texture*>(tex.data()),
                           src_area,
                           dst_area) < 0)
            throw error{};
    }


    void
    renderer::copy(const texture& tex,
                   const rect* src_area,
                   const rectf* dst_area)
    {
        if (SDL_RenderCopyF(raw,
                            const_cast<SDL_Texture*>(tex.data()),
                            src_area,
                            dst_area) < 0)
            throw error{};
    }


    void
    renderer::copy(const texture& tex,
                   const rect& src_area)
    {
        copy(tex, &src_area);
    }


    void
    renderer::copy(const texture& tex,
                   const std::optional<rect>& src_area,
                   const rect& dst_area)
    {
        const rect* src_ptr = src_area ? &src_area.value() : nullptr;
        copy(tex, src_ptr, &dst_area);
    }


    void
    renderer::copy(const texture& tex,
                   const std::optional<rect>& src_area,
                   const rectf& dst_area)
    {
        const rect* src_ptr = src_area ? &src_area.value() : nullptr;
        copy(tex, src_ptr, &dst_area);
    }


    void
    renderer::copy_ex(const texture& tex,
                      const rect* src_area,
                      const rect* dst_area,
                      degrees rot,
                      const vec2* center,
                      SDL_RendererFlip flip)
    {
        if (SDL_RenderCopyEx(raw,
                             const_cast<SDL_Texture*>(tex.data()),
                             src_area,
                             dst_area,
                             rot.value(),
                             center,
                             flip) < 0)
            throw error{};
    }


    void
    renderer::copy_ex(const texture& tex,
                      const rect* src_area,
                      const rectf* dst_area,
                      degrees rot,
                      const vec2f* center,
                      SDL_RendererFlip flip)
    {
        if (SDL_RenderCopyExF(raw,
                              const_cast<SDL_Texture*>(tex.data()),
                              src_area,
                              dst_area,
                              rot.value(),
                              center,
                              flip) < 0)
            throw error{};
    }


    void
    renderer::geometry(const std::optional<texture>& tex,
                       std::span<const vertex> vertices)
    {
        const SDL_Texture* tex_ptr = tex ? tex->data() : nullptr;
        if (SDL_RenderGeometry(raw,
                               const_cast<SDL_Texture*>(tex_ptr),
                               vertices.data(),
                               vertices.size(),
                               nullptr,
                               0) < 0)
            throw error{};
    }


    void
    renderer::geometry(const std::optional<texture>& tex,
                       std::span<const SDL_Vertex> vertices,
                       std::span<const int> indices)
    {
        const SDL_Texture* tex_ptr = tex ? tex->data() : nullptr;
        if (SDL_RenderGeometry(raw,
                               const_cast<SDL_Texture*>(tex_ptr),
                               vertices.data(),
                               vertices.size(),
                               indices.data(),
                               indices.size()) < 0)
            throw error{};
    }


    void
    renderer::geometry_raw(const std::optional<texture>& tex,
                           const float* xy, int xy_stride,
                           const SDL_Color* col, int col_stride,
                           const float* uv, int uv_stride,
                           int num_vertices,
                           const void* indices,
                           int num_indices,
                           int index_size)
    {
        const SDL_Texture* tex_ptr = tex ? tex->data() : nullptr;
        if (SDL_RenderGeometryRaw(raw,
                                  const_cast<SDL_Texture*>(tex_ptr),
                                  xy, xy_stride,
                                  col, col_stride,
                                  uv, uv_stride,
                                  num_vertices,
                                  indices, num_indices,
                                  index_size) < 0)
            throw error{};
    }


    void
    renderer::read_pixels(const std::optional<rect>& area,
                          pixels::format_enum format,
                          void* pixels,
                          int pitch)
    {
        const rect* area_ptr = area ? &area.value() : nullptr;
        if (SDL_RenderReadPixels(raw,
                                 area_ptr,
                                 static_cast<SDL_PixelFormatEnum>(format),
                                 pixels,
                                 pitch) < 0)
            throw error{};
    }


    void
    renderer::present()
        noexcept
    {
        SDL_RenderPresent(raw);
    }


    void
    renderer::flush()
    {
        if (SDL_RenderFlush(raw) < 0)
            throw error{};
    }


    void*
    renderer::get_metal_layer()
        noexcept
    {
        return SDL_RenderGetMetalLayer(raw);
    }


    void*
    renderer::get_metal_command_encoder()
        noexcept
    {
        return SDL_RenderGetMetalCommandEncoder(raw);
    }


    void
    renderer::set_vsync(bool enabled)
    {
        if (SDL_RenderSetVSync(raw, enabled ? 1 : 0) < 0)
            throw error{};
    }


    renderer*
    renderer::get_wrapper(SDL_Renderer* ren)
        noexcept
    {
        return detail::find(ren);
    }


    const renderer*
    renderer::get_wrapper(const SDL_Renderer* ren)
        noexcept
    {
        return detail::find(ren);
    }

} // namespace sdl
