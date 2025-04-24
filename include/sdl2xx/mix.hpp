/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_MIX_HPP
#define SDL2XX_MIX_HPP

#include <filesystem>
#include <optional>
#include <span>

#include <SDL_mixer.h>

#include "audio.hpp"
#include "basic_wrapper.hpp"
#include "string.hpp"


namespace sdl::mix {

    using std::filesystem::path;

    using audio::format_t;


    [[nodiscard]]
    SDL_version
    linked_version()
        noexcept;


    struct init {

        enum flag : Uint32 {
            flac = MIX_INIT_FLAC,
            mid  = MIX_INIT_MID,
            mod  = MIX_INIT_MOD,
            mp3  = MIX_INIT_MP3,
            ogg  = MIX_INIT_OGG,
            opus = MIX_INIT_OPUS,

            all = flac | mid | mod | mp3 | ogg | opus
        };


        // Disallow copies.
        init(const init&) = delete;

        init(Uint32 flags = flag::all)
            noexcept;

        ~init()
            noexcept;

    };


    Uint32
    initialize(Uint32 flags = init::flag::all)
        noexcept;


    [[nodiscard]]
    Uint32
    is_initialized()
        noexcept;


    void
    finalize()
        noexcept;


    void
    open(int frequency,
         format_t format,
         int channels,
         int chunk_size);


    void
    open(int frequency,
         format_t format,
         int channels,
         int chunk_size,
         const char* device,
         bool allowed_changes);

    inline
    void
    open(int frequency,
         format_t format,
         int channels,
         int chunk_size,
         const concepts::string auto& device,
         bool allowed_changes)
    {
        open(frequency, format, channels, chunk_size, device.data(), allowed_changes);
    }


    struct spec_t {
        int frequency;
        format_t format;
        int channels;
    };


    [[nodiscard]]
    std::optional<spec_t>
    query();


    int
    allocate_channels(int num);


    struct chunk : basic_wrapper<Mix_Chunk*> {

        // Inherit constructors.
        using basic_wrapper::basic_wrapper;

        chunk(const path& filename);

        chunk(SDL_RWops* src,
              bool close_src);

        explicit
        chunk(Uint8* ms_wav);

        chunk(const void* samples,
              std::size_t size);

        template<typename T>
        inline
        chunk(std::span<T> samples) :
            chunk{samples.data(), samples.size_bytes()}
        {}


        /// Move constructor.
        chunk(chunk&& other)
            noexcept;


        ~chunk()
            noexcept;


        /// Move assignment.
        chunk&
        operator =(chunk&& other)
            noexcept;


        void
        create(const path& filename);

        void
        create(SDL_RWops* src,
               bool close_src);

        void
        create(Uint8* ms_wav);

        void
        create(const void* samples,
               std::size_t size);

        inline
        void
        create(std::span<auto> samples)
        {
            create(samples.data(), samples.size_bytes());
        }


        void
        destroy()
            noexcept;


        [[nodiscard]]
        static
        unsigned
        get_num_decoders()
            noexcept;


        [[nodiscard]]
        static
        const char*
        get_decoder(unsigned index);


        [[nodiscard]]
        static
        bool
        has_decoder(const char* name)
            noexcept;

        [[nodiscard]]
        static
        bool
        has_decoder(const concepts::string auto& name)
            noexcept
        {
            return has_decoder(name.data());
        }

    }; // struct chunk



    struct music : basic_wrapper<Mix_Music*> {

        // Inherit constructors.
        using basic_wrapper::basic_wrapper;


        enum class type {
            cmd     = MUS_CMD,
            flac    = MUS_FLAC,
            mid     = MUS_MID,
            mod     = MUS_MOD,
            modplug = MUS_MODPLUG_UNUSED,
            mp3     = MUS_MP3,
            mus     = MUS_MP3_MAD_UNUSED,
            none    = MUS_NONE,
            ogg     = MUS_OGG,
            opus    = MUS_OPUS,
            wav     = MUS_WAV,
        };


        music(const path& filename);

        music(SDL_RWops* src,
              bool close_src);

        music(SDL_RWops* src,
              bool close_src,
              type t);

        /// Move constructor.
        music(music&& other)
            noexcept;

        ~music()
            noexcept;


        /// Move assignment.
        music&
        operator =(music&& other)
            noexcept;


        void
        create(const path& filename);

        void
        create(SDL_RWops* src,
               bool close_src);

        void
        create(SDL_RWops* src,
               bool close_src,
               type t);

        void
        destroy()
            noexcept;


        [[nodiscard]]
        static
        unsigned
        get_num_decoders()
            noexcept;


        [[nodiscard]]
        static
        const char*
        get_decoder(unsigned index);


        [[nodiscard]]
        static
        bool
        has_decoder(const char* name)
            noexcept;

        [[nodiscard]]
        static
        bool
        has_decoder(const concepts::string auto& name)
            noexcept
        {
            return has_decoder(name.data());
        }


        [[nodiscard]]
        type
        get_type()
            const noexcept;


        [[nodiscard]]
        const char*
        get_title()
            const noexcept;

        [[nodiscard]]
        static
        const char*
        get_current_title()
            noexcept;


        [[nodiscard]]
        const char*
        get_title_tag()
            const noexcept;

        [[nodiscard]]
        static
        const char*
        get_current_title_tag()
            noexcept;


        [[nodiscard]]
        const char*
        get_artist_tag()
            const noexcept;

        [[nodiscard]]
        static
        const char*
        get_current_artist_tag()
            noexcept;


        [[nodiscard]]
        const char*
        get_album_tag()
            const noexcept;

        [[nodiscard]]
        static
        const char*
        get_current_album_tag()
            noexcept;


        [[nodiscard]]
        const char*
        get_copyright_tag()
            const noexcept;

        [[nodiscard]]
        static
        const char*
        get_current_copyright_tag()
            noexcept;

    }; // struct music



    using mix_function = void (SDLCALL *)(void* ctx,
                                          Uint8* samples,
                                          int len);

    void
    set_post_mix(mix_function func,
                 void* ctx)
        noexcept;


    void
    hook_music(mix_function func,
               void* ctx)
        noexcept;


    void*
    get_music_hook_data()
        noexcept;



    using music_finished_function = void (SDLCALL *)(void);


    void
    hook_music_finished(music_finished_function func)
        noexcept;


    using channel_finished_function = void (SDLCALL *)(int channel);


    void
    hook_channel_finished(channel_finished_function func)
        noexcept;





} // namespace sdl::mix

#endif
