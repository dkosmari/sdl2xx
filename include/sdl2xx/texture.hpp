/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_TEXTURE_HPP
#define SDL2XX_TEXTURE_HPP

#include <tuple>
#include <utility>

#include <SDL_render.h>

#include "basic_wrapper.hpp"
#include "color.hpp"
#include "pixels.hpp"
#include "unique_ptr.hpp"
#include "vec2.hpp"


namespace sdl {

    class rect;
    class renderer;
    class surface;


    class texture : public basic_wrapper<SDL_Texture*> {

        void* user_data = nullptr;
        unique_ptr<surface> locked_surface;


        void
        link_this()
            noexcept;

    public:

        using parent_t = basic_wrapper<SDL_Texture*>;
        using state_t = std::tuple<raw_type,
                                   void*,
                                   unique_ptr<surface>>;


        // Inherit constructors.
        using basic_wrapper::basic_wrapper;


        texture(SDL_Texture* tex)
            noexcept;

        texture(renderer& ren,
                pixels::format_enum fmt,
                SDL_TextureAccess access,
                int width,
                int height);

        texture(renderer& ren,
                surface& surf);

        /// Move constructor.
        texture(texture&& other)
            noexcept;


        ~texture()
            noexcept;


        /// Move assignment.
        texture&
        operator =(texture&& other)
            noexcept;


        void
        create(renderer& ren,
               pixels::format_enum format,
               SDL_TextureAccess access,
               int width,
               int height);

        void
        create(renderer& ren,
               surface& surf);


        void
        destroy()
            noexcept;


        void
        acquire(state_t state)
            noexcept;

        void
        acquire(raw_type new_raw,
                void* new_user_data = nullptr,
                unique_ptr<surface> new_locked_surface = {})
            noexcept;


        state_t
        release()
            noexcept;


        struct info_t {
            pixels::format_enum format;
            SDL_TextureAccess access;
            int width;
            int height;
        };

        [[nodiscard]]
        info_t
        query()
            const;

        [[nodiscard]]
        pixels::format_enum
        get_format()
            const;

        [[nodiscard]]
        SDL_TextureAccess
        get_access()
            const;

        [[nodiscard]]
        vec2
        get_size()
            const;

        [[nodiscard]]
        int
        get_width()
            const;

        [[nodiscard]]
        int
        get_height()
            const;


        void
        set_color_mod(Uint8 r,
                      Uint8 g,
                      Uint8 b);

        void
        set_color_mod(color c);

        [[nodiscard]]
        color
        get_color_mod()
            const;


        void
        set_alpha_mod(Uint8 a);

        [[nodiscard]]
        Uint8
        get_alpha_mod()
            const;


        void
        set_blend_mode(SDL_BlendMode mode);

        [[nodiscard]]
        SDL_BlendMode
        get_blend_mode()
            const;


        void
        set_scale_mode(SDL_ScaleMode mode);

        [[nodiscard]]
        SDL_ScaleMode
        get_scale_mode()
            const;


        void*
        set_user_data(void* new_user_data)
            noexcept;

        [[nodiscard]]
        void*
        get_user_data()
            const noexcept;


        void
        update(const rect* area,
               const void* pixels,
               int pitch);

        void
        update_yuv(const rect* area,
                   const Uint8* y, int y_pitch,
                   const Uint8* u, int u_pitch,
                   const Uint8* v, int v_pitch);

        void
        update_nv(const rect* area,
                  const Uint8* y, int y_pitch,
                  const Uint8* uv, int uv_pitch);


        [[nodiscard]]
        std::pair<void*, int>
        lock(const rect* area = nullptr);

        [[nodiscard]]
        surface*
        lock_surface(const rect* area = nullptr);

        void
        unlock()
            noexcept;


        vec2f
        gl_bind()
            const;

        void
        gl_unbind()
            const;


        [[nodiscard]]
        static
        texture*
        get_wrapper(SDL_Texture* tex)
            noexcept;

    }; // class texture

} // namespace sdl

#endif
