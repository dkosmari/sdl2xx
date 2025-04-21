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
        if (ptr)
            SDL_SetTextureUserData(ptr, this);
    }


    void
    texture::release()
        noexcept
    {
        ptr = nullptr;
        user_data = nullptr;
    }


    texture::texture(SDL_Texture* tex)
        noexcept :
        ptr{tex}
    {}


    texture::texture(renderer& ren,
                     SDL_PixelFormatEnum format,
                     SDL_TextureAccess access,
                     int width,
                     int height)
    {
        create(ren, format, access, width, height);
    }



    texture::texture(renderer& ren,
                     surface& surf)
    {
        create(ren, surf);
    }


    /// Move constructor.
    texture::texture(texture&& other)
        noexcept :
        ptr{other.ptr},
        user_data{other.user_data},
        locked_surface{std::move(other.locked_surface)}
    {
        other.release();
        link_this();
    }


    texture::~texture()
        noexcept
    {
        destroy();
    }


    /// Move assignment.
    texture&
    texture::operator =(texture&& other)
        noexcept
    {
        if (this != &other) {
            destroy();
            ptr = other.ptr;
            user_data = other.user_data;
            locked_surface = std::move(other.locked_surface);
            other.release();
            link_this();
        }
        return *this;
    }


    void
    texture::create(renderer& ren,
                    SDL_PixelFormatEnum format,
                    SDL_TextureAccess access,
                    int width,
                    int height)
    {
        destroy();
        ptr = SDL_CreateTexture(ren.data(), format, access, width, height);
        if (!ptr)
            throw error{};
        link_this();
    }


    void
    texture::create(renderer& ren,
                    surface& surf)
    {
        destroy();
        ptr = SDL_CreateTextureFromSurface(ren.data(), surf.data());
        if (!ptr)
            throw error{};
        link_this();
    }


    void
    texture::destroy()
        noexcept
    {
        if (ptr) {
            locked_surface.reset();
            SDL_DestroyTexture(ptr);
            release();
        }
    }


    bool
    texture::is_valid()
        const noexcept
    {
        return ptr;
    }


    texture::operator bool()
        const noexcept
    {
        return ptr;
    }


    SDL_Texture*
    texture::data()
        noexcept
    {
        return ptr;
    }


    const SDL_Texture*
    texture::data()
        const noexcept
    {
        return ptr;
    }


    texture::info_t
    texture::query()
        const
    {
        Uint32 format;
        int access;
        int width;
        int height;
        if (SDL_QueryTexture(ptr, &format, &access, &width, &height) < 0)
            throw error{};
        return info_t{
            .format = static_cast<SDL_PixelFormatEnum>(format),
            .access = static_cast<SDL_TextureAccess>(access),
            .width = width,
            .height = height
        };
    }


    SDL_PixelFormatEnum
    texture::get_format()
        const
    {
        Uint32 format;
        if (SDL_QueryTexture(ptr, &format, nullptr, nullptr, nullptr) < 0)
            throw error{};
        return static_cast<SDL_PixelFormatEnum>(format);
    }


    SDL_TextureAccess
    texture::get_access()
        const
    {
        int access;
        if (SDL_QueryTexture(ptr, nullptr, &access, nullptr, nullptr) < 0)
            throw error{};
        return static_cast<SDL_TextureAccess>(access);
    }


    vec2
    texture::get_size()
        const
    {
        vec2 size;
        if (SDL_QueryTexture(ptr, nullptr, nullptr, &size.x, &size.y) < 0)
            throw error{};
        return size;
    }


    int
    texture::get_width()
        const
    {
        int width;
        if (SDL_QueryTexture(ptr, nullptr, nullptr, &width, nullptr) < 0)
            throw error{};
        return width;
    }


    int
    texture::get_height()
        const
    {
        int height;
        if (SDL_QueryTexture(ptr, nullptr, nullptr, nullptr, &height) < 0)
            throw error{};
        return height;
    }


    void
    texture::set_color_mod(Uint8 r,
                           Uint8 g,
                           Uint8 b)
    {
        if (SDL_SetTextureColorMod(ptr, r, g, b) < 0)
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
        if (SDL_GetTextureColorMod(ptr, &c.r, &c.g, &c.b) < 0)
            throw error{};
        return c;
    }


    void
    texture::set_alpha_mod(Uint8 a)
    {
        if (SDL_SetTextureAlphaMod(ptr, a) < 0)
            throw error{};
    }


    Uint8
    texture::get_alpha_mod()
        const
    {
        Uint8 a;
        if (SDL_GetTextureAlphaMod(ptr, &a) < 0)
            throw error{};
        return a;
    }


    void
    texture::set_blend_mode(SDL_BlendMode mode)
    {
        if (SDL_SetTextureBlendMode(ptr, mode) < 0)
            throw error{};
    }


    SDL_BlendMode
    texture::get_blend_mode()
        const
    {
        SDL_BlendMode mode;
        if (SDL_GetTextureBlendMode(ptr, &mode) < 0)
            throw error{};
        return mode;
    }


    void
    texture::set_scale_mode(SDL_ScaleMode mode)
    {
        if (SDL_SetTextureScaleMode(ptr, mode) < 0)
            throw error{};
    }


    SDL_ScaleMode
    texture::get_scale_mode()
        const
    {
        SDL_ScaleMode mode;
        if (SDL_GetTextureScaleMode(ptr, &mode) < 0)
            throw error{};
        return mode;
    }


    void
    texture::set_user_data(void* d)
        noexcept
    {
        user_data = d;
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
        if (SDL_UpdateTexture(ptr, area, pixels, pitch) < 0)
            throw error{};
    }


    void
    texture::update_yuv(const rect* area,
                        const Uint8* y, int y_pitch,
                        const Uint8* u, int u_pitch,
                        const Uint8* v, int v_pitch)
    {
        if (SDL_UpdateYUVTexture(ptr, area,
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
        if (SDL_UpdateNVTexture(ptr, area,
                                y, y_pitch,
                                uv, uv_pitch) < 0)
            throw error{};
    }


    std::pair<void*, int>
    texture::lock(const rect* area)
    {
        void* pixels;
        int pitch;
        if (SDL_LockTexture(ptr, area, &pixels, &pitch) < 0)
            throw error{};
        return {pixels, pitch};
    }


    surface*
    texture::lock_surface(const rect* area)
    {
        SDL_Surface* surf;
        if (SDL_LockTextureToSurface(ptr, area, &surf) < 0)
            throw error{};
        locked_surface = make_unique<surface>(surf, surface::dont_destroy);
        return locked_surface.get();
    }


    void
    texture::unlock()
        noexcept
    {
        locked_surface.reset();
        SDL_UnlockTexture(ptr);
    }


    vec2f
    texture::gl_bind()
        const
    {
        vec2f size;
        if (SDL_GL_BindTexture(ptr, &size.x, &size.y) < 0)
            throw error{};
        return size;
    }


    void
    texture::gl_unbind()
        const
    {
        if (SDL_GL_UnbindTexture(ptr) < 0)
            throw error{};
    }


    texture*
    texture::get_wrapper(SDL_Texture* tex)
        noexcept
    {
        return reinterpret_cast<texture*>(SDL_GetTextureUserData(tex));
    }

} // namespace sdl
