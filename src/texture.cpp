/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include "texture.hpp"

#include "error.hpp"
#include "renderer.hpp"
#include "surface.hpp"


namespace sdl {

    void
    texture::link_this()
        noexcept
    {
        if (raw)
            SDL_SetTextureUserData(raw, this);
    }


    texture::texture()
        noexcept = default;


    texture::texture(SDL_Texture* tex)
        noexcept
    {
        acquire(tex);
    }


    texture::texture(renderer& ren,
                     pixels::format_enum fmt,
                     SDL_TextureAccess access,
                     int width,
                     int height)
    {
        create(ren, fmt, access, width, height);
    }



    texture::texture(renderer& ren,
                     surface& surf)
    {
        create(ren, surf);
    }


    texture::texture(texture&& other)
        noexcept
    {
        acquire(other.release());
    }


    texture::~texture()
        noexcept
    {
        destroy();
    }


    texture&
    texture::operator =(texture&& other)
        noexcept
    {
        if (this != &other) {
            destroy();
            acquire(other.release());
        }
        return *this;
    }


    void
    texture::create(renderer& ren,
                    pixels::format_enum fmt,
                    SDL_TextureAccess access,
                    int width,
                    int height)
    {
        auto ptr = SDL_CreateTexture(ren.data(),
                                     static_cast<SDL_PixelFormatEnum>(fmt),
                                     access,
                                     width,
                                     height);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    texture::create(renderer& ren,
                    surface& surf)
    {
        auto ptr = SDL_CreateTextureFromSurface(ren.data(), surf.data());
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    texture::destroy()
        noexcept
    {
        if (is_valid()) {
            auto [old_raw, old_user_data, old_locked_surface] = release();
            SDL_DestroyTexture(old_raw);
        }
    }


    void
    texture::acquire(state_type state)
        noexcept
    {
        base_type::acquire(get<0>(state));
        user_data = get<1>(state);
        locked_surface = std::move(get<2>(state));
        link_this();
    }


    void
    texture::acquire(raw_type new_raw,
                     void* new_user_data,
                     unique_ptr<surface> new_locked_surface)
        noexcept
    {
        acquire({ new_raw, new_user_data, std::move(new_locked_surface) });
    }


    texture::state_type
    texture::release()
        noexcept
    {
        return {
            base_type::release(),
            set_user_data(nullptr),
            std::move(locked_surface)
        };
    }


    texture::info_t
    texture::query()
        const
    {
        Uint32 format;
        int access;
        int width;
        int height;
        if (SDL_QueryTexture(raw, &format, &access, &width, &height) < 0)
            throw error{};
        return info_t{
            .format = static_cast<pixels::format_enum>(format),
            .access = static_cast<SDL_TextureAccess>(access),
            .width = width,
            .height = height
        };
    }


    pixels::format_enum
    texture::get_format()
        const
    {
        Uint32 format;
        if (SDL_QueryTexture(raw, &format, nullptr, nullptr, nullptr) < 0)
            throw error{};
        return static_cast<pixels::format_enum>(format);
    }


    SDL_TextureAccess
    texture::get_access()
        const
    {
        int access;
        if (SDL_QueryTexture(raw, nullptr, &access, nullptr, nullptr) < 0)
            throw error{};
        return static_cast<SDL_TextureAccess>(access);
    }


    vec2
    texture::get_size()
        const
    {
        vec2 size;
        if (SDL_QueryTexture(raw, nullptr, nullptr, &size.x, &size.y) < 0)
            throw error{};
        return size;
    }


    int
    texture::get_width()
        const
    {
        int width;
        if (SDL_QueryTexture(raw, nullptr, nullptr, &width, nullptr) < 0)
            throw error{};
        return width;
    }


    int
    texture::get_height()
        const
    {
        int height;
        if (SDL_QueryTexture(raw, nullptr, nullptr, nullptr, &height) < 0)
            throw error{};
        return height;
    }


    void
    texture::set_color_mod(Uint8 r,
                           Uint8 g,
                           Uint8 b)
    {
        if (SDL_SetTextureColorMod(raw, r, g, b) < 0)
            throw error{};
    }


    void
    texture::set_color_mod(color c)
    {
        set_color_mod(c.r, c.g, c.b);
    }


    color
    texture::get_color_mod()
        const
    {
        color c;
        if (SDL_GetTextureColorMod(raw, &c.r, &c.g, &c.b) < 0)
            throw error{};
        return c;
    }


    void
    texture::set_alpha_mod(Uint8 a)
    {
        if (SDL_SetTextureAlphaMod(raw, a) < 0)
            throw error{};
    }


    Uint8
    texture::get_alpha_mod()
        const
    {
        Uint8 a;
        if (SDL_GetTextureAlphaMod(raw, &a) < 0)
            throw error{};
        return a;
    }


    void
    texture::set_blend_mode(SDL_BlendMode mode)
    {
        if (SDL_SetTextureBlendMode(raw, mode) < 0)
            throw error{};
    }


    SDL_BlendMode
    texture::get_blend_mode()
        const
    {
        SDL_BlendMode mode;
        if (SDL_GetTextureBlendMode(raw, &mode) < 0)
            throw error{};
        return mode;
    }


    void
    texture::set_scale_mode(SDL_ScaleMode mode)
    {
        if (SDL_SetTextureScaleMode(raw, mode) < 0)
            throw error{};
    }


    SDL_ScaleMode
    texture::get_scale_mode()
        const
    {
        SDL_ScaleMode mode;
        if (SDL_GetTextureScaleMode(raw, &mode) < 0)
            throw error{};
        return mode;
    }


    void*
    texture::set_user_data(void* new_user_data)
        noexcept
    {
        void* old_user_data = user_data;
        user_data = new_user_data;
        return old_user_data;
    }


    void*
    texture::get_user_data()
        const noexcept
    {
        return user_data;
    }


    void
    texture::update(const rect* area,
                    const void* pixels,
                    int pitch)
    {
        if (SDL_UpdateTexture(raw, area, pixels, pitch) < 0)
            throw error{};
    }


    void
    texture::update_yuv(const rect* area,
                        const Uint8* y, int y_pitch,
                        const Uint8* u, int u_pitch,
                        const Uint8* v, int v_pitch)
    {
        if (SDL_UpdateYUVTexture(raw, area,
                                 y, y_pitch,
                                 u, u_pitch,
                                 v, v_pitch) < 0)
            throw error{};
    }


    void
    texture::update_nv(const rect* area,
                       const Uint8* y, int y_pitch,
                       const Uint8* uv, int uv_pitch)
    {
        if (SDL_UpdateNVTexture(raw, area,
                                y, y_pitch,
                                uv, uv_pitch) < 0)
            throw error{};
    }


    std::pair<void*, int>
    texture::lock(const rect* area)
    {
        void* pixels;
        int pitch;
        if (SDL_LockTexture(raw, area, &pixels, &pitch) < 0)
            throw error{};
        return {pixels, pitch};
    }


    surface*
    texture::lock_surface(const rect* area)
    {
        SDL_Surface* surf;
        if (SDL_LockTextureToSurface(raw, area, &surf) < 0)
            throw error{};
        locked_surface = make_unique<surface>(surf, surface::dont_destroy);
        return locked_surface.get();
    }


    void
    texture::unlock()
        noexcept
    {
        locked_surface.reset();
        SDL_UnlockTexture(raw);
    }


    vec2f
    texture::gl_bind()
        const
    {
        vec2f size;
        if (SDL_GL_BindTexture(raw, &size.x, &size.y) < 0)
            throw error{};
        return size;
    }


    void
    texture::gl_unbind()
        const
    {
        if (SDL_GL_UnbindTexture(raw) < 0)
            throw error{};
    }


    texture*
    texture::get_wrapper(SDL_Texture* tex)
        noexcept
    {
        return reinterpret_cast<texture*>(SDL_GetTextureUserData(tex));
    }

} // namespace sdl
