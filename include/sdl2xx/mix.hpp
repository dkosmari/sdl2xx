/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_MIX_HPP
#define SDL2XX_MIX_HPP

#include <chrono>
#include <concepts>
#include <filesystem>
#include <optional>
#include <span>

#include <SDL_mixer.h>

#include "angle.hpp"
#include "audio.hpp"
#include "owner_wrapper.hpp"
#include "rwops.hpp"
#include "string.hpp"
#include "vector.hpp"


namespace sdl::mix {

    using std::chrono::milliseconds;

    using std::filesystem::path;

    using audio::format;


    using dbl_seconds = std::chrono::duration<double>;


    constexpr auto default_frequency = MIX_DEFAULT_FREQUENCY;
    constexpr auto default_format    = MIX_DEFAULT_FORMAT;
    constexpr auto default_channels  = MIX_DEFAULT_CHANNELS;

    constexpr auto max_volume = MIX_MAX_VOLUME;


    enum class fading_status {
        in   = MIX_FADING_IN,
        none = MIX_NO_FADING,
        out  = MIX_FADING_OUT,
    };


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

        template<std::same_as<flag>... Args>
        requires(sizeof...(Args) > 0)
        init(Args... args)
            noexcept :
            init{static_cast<Uint32>((args | ...))}
        {}


        ~init()
            noexcept;

    };


    Uint32
    initialize(Uint32 flags = init::flag::all)
        noexcept;


    template<std::same_as<init::flag>... Args>
    requires(sizeof...(Args) > 0)
    Uint32
    initialize(Args... args)
        noexcept
    {
        return initialize(static_cast<Uint32>((args | ...)));
    }


    [[nodiscard]]
    Uint32
    is_initialized()
        noexcept;


    void
    finalize()
        noexcept;


    void
    open();

    void
    open(int frequency,
         format fmt,
         unsigned channels,
         int chunk_size);

    void
    open(int frequency,
         format fmt,
         unsigned channels,
         int chunk_size,
         const char* name,
         Uint32 allowed_changes = audio::allow::change_any);

    inline
    void
    open(int frequency,
         format fmt,
         unsigned channels,
         int chunk_size,
         const concepts::string auto& name,
         Uint32 allowed_changes = audio::allow::change_any)
    {
        open(frequency, fmt, channels, chunk_size, name.data(), allowed_changes);
    }


    void
    close()
        noexcept;


    struct spec {
        int frequency;
        format fmt;
        unsigned channels;
    };


    // Call open/close on constructor and destructor.
    struct device {

        // Forbid copying.
        device(const device&) = delete;


        device();

        device(int frequency,
               format fmt,
               unsigned channels,
               int chunk_size);

        device(int frequency,
               format fmt,
               unsigned channels,
               int chunk_size,
               const char* name,
               Uint32 allowed_changes = audio::allow::change_any);

        inline
        device(int frequency,
               format fmt,
               unsigned channels,
               int chunk_size,
               const concepts::string auto& name,
               Uint32 allowed_changes = audio::allow::change_any) :
            device{frequency, fmt, channels, chunk_size, name.data(), allowed_changes}
        {}


        ~device()
            noexcept;


        void
        reopen();

        void
        reopen(int frequency,
               format fmt,
               unsigned channels,
               int chunk_size);

        void
        reopen(int frequency,
               format fmt,
               unsigned channels,
               int chunk_size,
               const char* name,
               Uint32 allowed_changes = audio::allow::change_any);

        inline
        void
        reopen(int frequency,
               format fmt,
               unsigned channels,
               int chunk_size,
               const concepts::string auto& name,
               Uint32 allowed_changes = audio::allow::change_any)
        {
            reopen(frequency, fmt, channels, chunk_size, name.data(), allowed_changes);
        }

    }; // struct device


    [[nodiscard]]
    std::optional<spec>
    query();


    int
    allocate_channels(int num);


    struct chunk : owner_wrapper<Mix_Chunk*> {

        using base_type = owner_wrapper<Mix_Chunk*>;


        // Inherit constructors.
        using base_type::base_type;


        chunk(const path& filename);

        chunk(SDL_RWops* src,
              bool close_src);

        explicit
        chunk(rwops& src);

        explicit
        chunk(Uint8* ms_wav);

        chunk(const void* samples,
              std::size_t size);

        template<typename T,
                 std::size_t E>
        inline
        chunk(std::span<const T, E> samples) :
            chunk{samples.data(), samples.size_bytes()}
        {}


        /// Move constructor.
        chunk(chunk&& other)
            noexcept = default;


        ~chunk()
            noexcept;


        /// Move assignment.
        chunk&
        operator =(chunk&& other)
            noexcept = default;


        void
        create(const path& filename);

        void
        create(SDL_RWops* src,
               bool close_src);

        void
        create(rwops& src);

        void
        create(Uint8* ms_wav);

        void
        create(const void* samples,
               std::size_t size);

        template<typename T,
                 std::size_t E>
        void
        create(std::span<const T, E> samples)
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


        unsigned
        play_on(unsigned channel,
                int loops = 0)
            noexcept;

        std::optional<unsigned>
        play(int loops = 0)
            noexcept;


        unsigned
        play_on(unsigned channel,
                milliseconds max_duration,
                int loops = 0)
            noexcept;

        std::optional<unsigned>
        play(milliseconds max_duration,
             int loops = 0)
            noexcept;


        unsigned
        fade_in_on(unsigned channel,
                   milliseconds fade_duration,
                   int loops = 0)
            noexcept;

        std::optional<unsigned>
        fade_in(milliseconds fade_duration,
                int loops = 0)
            noexcept;


        unsigned
        fade_in_on(unsigned channel,
                   milliseconds fade_duration,
                   milliseconds max_duration,
                   int loops = 0)
            noexcept;

        std::optional<unsigned>
        fade_in(milliseconds fade_duration,
                milliseconds max_duration,
                int loops = 0)
            noexcept;


        [[nodiscard]]
        float
        get_volume()
            const noexcept;

        float
        set_volume(float new_volume)
            const noexcept;

    }; // struct chunk


    // Note: return observer chunk.
    [[nodiscard]]
    chunk
    get_chunk(unsigned channel)
        noexcept;


    struct music : basic_wrapper<Mix_Music*> {

        using base_type = basic_wrapper<Mix_Music*>;


        // Inherit constructors.
        using base_type::base_type;


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

        music(rwops& src);

        music(rwops& src,
              type t);

        /// Move constructor.
        music(music&& other)
            noexcept = default;


        ~music()
            noexcept;


        /// Move assignment.
        music&
        operator =(music&& other)
            noexcept = default;


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
        create(rwops& src);

        void
        create(rwops& src,
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


        void
        play(int loops = 0);


        void
        fade_in(milliseconds duration,
                int loops = 0);


        void
        fade_in(milliseconds duration,
                dbl_seconds position,
                int loops = 0);


        [[nodiscard]]
        float
        get_volume()
            const noexcept;

        static
        float
        set_volume(float new_volume)
            noexcept;


        static
        void
        halt()
            noexcept;


        bool
        fade_out(milliseconds fade_duration)
            noexcept;


        [[nodiscard]]
        static
        fading_status
        get_fading_status()
            noexcept;


        static
        void
        pause()
            noexcept;


        static
        void
        resume()
            noexcept;


        [[nodiscard]]
        static
        bool
        is_paused()
            noexcept;


        static
        void
        rewind()
            noexcept;


        static
        bool
        mod_jump_to(int order)
            noexcept;


        static
        bool
        set_position(dbl_seconds position)
            noexcept;


        [[nodiscard]]
        std::optional<dbl_seconds>
        get_position()
            const noexcept;


        [[nodiscard]]
        std::optional<dbl_seconds>
        get_duration()
            const noexcept;

        [[nodiscard]]
        static
        std::optional<dbl_seconds>
        get_current_duration()
            noexcept;


        [[nodiscard]]
        std::optional<dbl_seconds>
        get_loop_start()
            const noexcept;

        [[nodiscard]]
        static
        std::optional<dbl_seconds>
        get_current_loop_start()
            noexcept;


        [[nodiscard]]
        std::optional<dbl_seconds>
        get_loop_end()
            const noexcept;

        [[nodiscard]]
        static
        std::optional<dbl_seconds>
        get_current_loop_end()
            noexcept;


        [[nodiscard]]
        std::optional<dbl_seconds>
        get_loop_length()
            const noexcept;

        [[nodiscard]]
        static
        std::optional<dbl_seconds>
        get_current_loop_length()
            noexcept;


        [[nodiscard]]
        static
        bool
        is_playing()
            noexcept;


        static
        bool
        set_cmd(const char* cmd)
            noexcept;

        static
        inline
        bool
        set_cmd(const concepts::string auto& cmd)
            noexcept
        {
            return set_cmd(cmd.data());
        }


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


    using effect_function = Mix_EffectFunc_t;

    using effect_done_function = Mix_EffectDone_t;


    void
    register_effect(unsigned channel,
                    effect_function effect_func,
                    effect_done_function effect_done_func,
                    void* ctx);


    void
    unregister_effect(unsigned channel,
                      effect_function func);


    void
    unregister_all_effects(unsigned channel);


    void
    set_panning(unsigned channel,
                float left,
                float right);

    void
    reset_panning(unsigned channel);


    void
    set_position(unsigned channel,
                 degreesf angle,
                 float distance);

    void
    reset_position(unsigned channel);


    void
    set_distance(unsigned channel,
                 float distance);

    void
    reset_distance(unsigned channel);


    void
    set_reverse_stereo(unsigned channel,
                       bool reverse);


    [[nodiscard]]
    unsigned
    reserve_channels(unsigned n)
        noexcept;


    void
    set_group(unsigned channel,
              int tag);

    void
    set_group(unsigned first,
              unsigned last,
              int tag);


    [[nodiscard]]
    std::optional<unsigned>
    get_first_available(int tag)
        noexcept;


    [[nodiscard]]
    unsigned
    group_size(int tag)
        noexcept;

    [[nodiscard]]
    unsigned
    size()
        noexcept;


    [[nodiscard]]
    std::optional<unsigned>
    get_newest(int tag)
        noexcept;


    [[nodiscard]]
    std::optional<unsigned>
    get_oldest(int tag)
        noexcept;


    [[nodiscard]]
    float
    get_volume(unsigned channel)
        noexcept;

    [[nodiscard]]
    float
    get_volume()
        noexcept;


    float
    set_volume(unsigned channel,
               float new_volume)
        noexcept;

    float
    set_volume(float new_volume)
        noexcept;


    [[nodiscard]]
    float
    get_master_volume()
        noexcept;


    float
    set_master_volume(float new_volume)
        noexcept;


    void
    halt(unsigned channel);

    void
    halt();


    void
    halt_group(int tag)
        noexcept;


    unsigned
    expire(unsigned channel,
           milliseconds duration)
        noexcept;

    unsigned
    expire(milliseconds duration)
        noexcept;


    unsigned
    no_expire(unsigned channel)
        noexcept;

    unsigned
    no_expire()
        noexcept;


    void
    fade_out(unsigned channel,
             milliseconds fade_duration);


    unsigned
    fade_out_group(int tag,
                   milliseconds fade_duration)
        noexcept;


    fading_status
    get_fading_status(unsigned channel)
        noexcept;


    void
    pause(unsigned channel)
        noexcept;

    void
    pause()
        noexcept;


    void
    resume(unsigned channel)
        noexcept;

    void
    resume()
        noexcept;


    [[nodiscard]]
    bool
    is_paused(unsigned channel)
        noexcept;

    [[nodiscard]]
    bool
    is_paused()
        noexcept;


    [[nodiscard]]
    bool
    is_playing(unsigned channel)
        noexcept;

    [[nodiscard]]
    unsigned
    is_playing()
        noexcept;


    bool
    set_sound_fonts(const char* paths)
        noexcept;


    [[nodiscard]]
    const char*
    get_sound_fonts();


    using sound_font_function = int (SDLCALL *)(const char*, void*);

    bool
    for_eac_sound_font(sound_font_function func,
                       void* ctx)
        noexcept;


    void
    set_timidity_cfg(const path& cfg_filename);


    const char*
    get_timidity_cfg()
        noexcept;

} // namespace sdl::mix

#endif
