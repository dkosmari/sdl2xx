/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <algorithm>
#include <cmath>

#include "mix.hpp"

#include "error.hpp"
#include "impl/utils.hpp"


namespace sdl::mix {

    using impl::utils::map_to_uint8;

    namespace {

        unsigned
        map_to_volume(float v)
        {
            return impl::utils::map_to_uint(v, max_volume);
        }

    } // namespace


    SDL_version
    linked_version()
        noexcept
    {
        return *Mix_Linked_Version();
    }


    init::init(Uint32 flags)
        noexcept
    {
        initialize(flags);
    }


    init::~init()
        noexcept
    {}


    Uint32
    initialize(Uint32 flags)
        noexcept
    {
        return Mix_Init(flags);
    }


    Uint32
    is_initialized()
        noexcept
    {
        return Mix_Init(0);
    }


    void
    finalize()
        noexcept
    {
        Mix_Quit();
    }


    void
    open()
    {
        auto [name, spec] = audio::get_default_info(false);
        open(spec.freq, spec.format, spec.channels, 2048, name, true);
    }


    void
    open(int frequency,
         format_t format,
         unsigned channels,
         int chunk_size)
    {
        if (Mix_OpenAudio(frequency, format, channels, chunk_size) < 0)
            throw error{};
    }


    void
    open(int frequency,
         format_t format,
         unsigned channels,
         int chunk_size,
         const char* name,
         bool allowed_changes)
    {
        if (Mix_OpenAudioDevice(frequency, format, channels,
                                chunk_size, name, allowed_changes) < 0)
            throw error{};
    }


    void
    close()
        noexcept
    {
        Mix_CloseAudio();
    }


    device::device()
    {
        open();
    }


    device::device(int frequency,
                   format_t format,
                   unsigned channels,
                   int chunk_size)
    {
        open(frequency, format, channels, chunk_size);
    }


    device::device(int frequency,
                   format_t format,
                   unsigned channels,
                   int chunk_size,
                   const char* name,
                   bool allowed_changes)
    {
        open(frequency, format, channels, chunk_size, name, allowed_changes);
    }


    device::~device()
        noexcept
    {
        close();
    }


    void
    device::reopen()
    {
        close();
        open();
    }


    void
    device::reopen(int frequency,
                   format_t format,
                   unsigned channels,
                   int chunk_size)
    {
        close();
        open(frequency, format, channels, chunk_size);
    }


    void
    device::reopen(int frequency,
                   format_t format,
                   unsigned channels,
                   int chunk_size,
                   const char* name,
                   bool allowed_changes)
    {
        close();
        open(frequency, format, channels, chunk_size, name, allowed_changes);
    }


    std::optional<spec_t>
    query()
    {
        int channels;
        spec_t spec;
        if (!Mix_QuerySpec(&spec.frequency, &spec.format, &channels))
            return {};
        spec.channels = channels;
        return spec;
    }


    int
    allocate_channels(int num)
    {
        return Mix_AllocateChannels(num);
    }


    chunk::chunk(const path& filename)
    {
        create(filename);
    }


    chunk::chunk(SDL_RWops* src,
                 bool close_src)
    {
        create(src, close_src);
    }


    chunk::chunk(Uint8* ms_wav)
    {
        create(ms_wav);
    }


    chunk::chunk(const void* samples,
                 std::size_t size)
    {
        create(samples, size);
    }


    chunk::~chunk()
    {
        destroy();
    }


    void
    chunk::create(const path& filename)
    {
        auto ptr = Mix_LoadWAV(filename.c_str());
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    chunk::create(SDL_RWops* src,
                  bool close_src)
    {
        auto ptr = Mix_LoadWAV_RW(src, close_src);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    chunk::create(Uint8* ms_wav)
    {
        auto ptr = Mix_QuickLoad_WAV(ms_wav);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    chunk::create(const void* samples,
                  std::size_t size)
    {
        auto ptr = Mix_QuickLoad_RAW(reinterpret_cast<Uint8*>(const_cast<void*>(samples)),
                                     size);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    chunk::destroy()
        noexcept
    {
        if (raw)
            Mix_FreeChunk(release());
    }


    unsigned
    chunk::get_num_decoders()
        noexcept
    {
        return Mix_GetNumChunkDecoders();
    }


    const char*
    chunk::get_decoder(unsigned index)
    {
        const char* name = Mix_GetChunkDecoder(index);
        if (!name)
            throw error{};
        return name;
    }


    bool
    chunk::has_decoder(const char* name)
        noexcept
    {
        return Mix_HasChunkDecoder(name);
    }


    unsigned
    chunk::play_on(unsigned channel,
                int loops)
        noexcept
    {
        return Mix_PlayChannel(channel, raw, loops);
    }


    std::optional<unsigned>
    chunk::play(int loops)
        noexcept
    {
        int result = Mix_PlayChannel(-1, raw, loops);
        if (result == -1)
            return {};
        return result;
    }


    unsigned
    chunk::play_on(unsigned channel,
                   milliseconds max_duration,
                   int loops)
        noexcept
    {
        return Mix_PlayChannelTimed(channel, raw, loops, max_duration.count());
    }


    std::optional<unsigned>
    chunk::play(milliseconds max_duration,
                int loops)
        noexcept
    {
        int result = Mix_PlayChannelTimed(-1, raw, loops, max_duration.count());
        if (result == -1)
            return {};
        return result;
    }


    unsigned
    chunk::fade_in_on(unsigned channel,
               milliseconds max_duration,
               int loops)
        noexcept
    {
        return Mix_FadeInChannel(channel, raw, loops, max_duration.count());
    }


    std::optional<unsigned>
    chunk::fade_in(milliseconds max_duration,
            int loops)
        noexcept
    {
        int result = Mix_FadeInChannel(-1, raw, loops, max_duration.count());
        if (result == -1)
            return {};
        return result;
    }


    unsigned
    chunk::fade_in_on(unsigned channel,
                      milliseconds fade_duration,
                      milliseconds max_duration,
                      int loops)
        noexcept
    {
        return Mix_FadeInChannelTimed(channel,
                                      raw,
                                      loops,
                                      fade_duration.count(),
                                      max_duration.count());
    }


    std::optional<unsigned>
    chunk::fade_in(milliseconds fade_duration,
                   milliseconds max_duration,
                   int loops)
        noexcept
    {
        int result = Mix_FadeInChannelTimed(-1,
                                            raw,
                                            loops,
                                            fade_duration.count(),
                                            max_duration.count());
        if (result == -1)
            return {};
        return result;
    }


    float
    chunk::get_volume()
        const noexcept
    {
        int result = Mix_VolumeChunk(raw, -1);
        return result / float(max_volume);
    }


    float
    chunk::set_volume(float new_volume)
        const noexcept
    {
        int result = Mix_VolumeChunk(raw, map_to_volume(new_volume));
        return result / float(max_volume);
    }


    music::music(const path& filename)
    {
        create(filename);
    }


    music::music(SDL_RWops* src,
                 bool close_src)
    {
        create(src, close_src);
    }


    music::music(SDL_RWops* src,
                 bool close_src,
                 type t)
    {
        create(src, close_src, t);
    }


    music::~music()
        noexcept
    {
        destroy();
    }


    void
    music::create(const path& filename)
    {
        auto ptr = Mix_LoadMUS(filename.c_str());
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    music::create(SDL_RWops* src,
                  bool close_src)
    {
        auto ptr = Mix_LoadMUS_RW(src, close_src);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    music::create(SDL_RWops* src,
                  bool close_src,
                  type t)
    {
        auto ptr = Mix_LoadMUSType_RW(src,
                                      static_cast<Mix_MusicType>(t),
                                      close_src);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    music::destroy()
        noexcept
    {
        if (raw)
            Mix_FreeMusic(release());
    }


    unsigned
    music::get_num_decoders()
        noexcept
    {
        return Mix_GetNumMusicDecoders();
    }


    const char*
    music::get_decoder(unsigned index)
    {
        const char* name = Mix_GetMusicDecoder(index);
        if (!name)
            throw error{};
        return name;
    }


    bool
    music::has_decoder(const char* name)
        noexcept
    {
        return Mix_HasMusicDecoder(name);
    }


    music::type
    music::get_type()
        const noexcept
    {
        return static_cast<type>(Mix_GetMusicType(raw));
    }


    const char*
    music::get_title()
        const noexcept
    {
        return Mix_GetMusicTitle(raw);
    }


    const char*
    music::get_current_title()
        noexcept
    {
        return Mix_GetMusicTitle(nullptr);
    }


    const char*
    music::get_title_tag()
        const noexcept
    {
        return Mix_GetMusicTitleTag(raw);
    }


    const char*
    music::get_current_title_tag()
        noexcept
    {
        return Mix_GetMusicTitleTag(nullptr);
    }


    const char*
    music::get_artist_tag()
        const noexcept
    {
        return Mix_GetMusicArtistTag(raw);
    }


    const char*
    music::get_current_artist_tag()
        noexcept
    {
        return Mix_GetMusicArtistTag(nullptr);
    }


    const char*
    music::get_album_tag()
        const noexcept
    {
        return Mix_GetMusicAlbumTag(raw);
    }


    const char*
    music::get_current_album_tag()
        noexcept
    {
        return Mix_GetMusicAlbumTag(nullptr);
    }


    const char*
    music::get_copyright_tag()
        const noexcept
    {
        return Mix_GetMusicCopyrightTag(raw);
    }


    const char*
    music::get_current_copyright_tag()
        noexcept
    {
        return Mix_GetMusicCopyrightTag(nullptr);
    }


    void
    music::play(int loops)
    {
        if (Mix_PlayMusic(raw, loops) < 0)
            throw error{};
    }


    void
    music::fade_in(milliseconds duration,
                   int loops)
    {
        if (Mix_FadeInMusic(raw, loops, duration.count()) < 0)
            throw error{};
    }


    void
    music::fade_in(milliseconds duration,
                   dbl_seconds position,
                   int loops)
    {
        if (Mix_FadeInMusicPos(raw, loops, duration.count(), position.count()) < 0)
            throw error{};
    }


    float
    music::get_volume()
        const noexcept
    {
        int result = Mix_GetMusicVolume(raw);
        return result / float(max_volume);
    }


    float
    music::set_volume(float new_volume)
        noexcept
    {
        int result = Mix_VolumeMusic(map_to_volume(new_volume));
        return result / float(max_volume);
    }


    void
    music::halt()
        noexcept
    {
        Mix_HaltMusic();
    }


    bool
    music::fade_out(milliseconds fade_duration)
        noexcept
    {
        return Mix_FadeOutMusic(fade_duration.count());
    }


    fading_status
    music::get_fading_status()
        noexcept
    {
        return static_cast<fading_status>(Mix_FadingMusic());
    }


    void
    music::pause()
        noexcept
    {
        Mix_PauseMusic();
    }


    void
    music::resume()
        noexcept
    {
        Mix_ResumeMusic();
    }


    bool
    music::is_paused()
        noexcept
    {
        return Mix_PausedMusic();
    }


    void
    music::rewind()
        noexcept
    {
        Mix_RewindMusic();
    }


    bool
    music::mod_jump_to(int order)
        noexcept
    {
        return !Mix_ModMusicJumpToOrder(order);
    }


    bool
    music::set_position(dbl_seconds position)
        noexcept
    {
        return !Mix_SetMusicPosition(position.count());
    }


    std::optional<dbl_seconds>
    music::get_position()
        const noexcept
    {
        double result = Mix_GetMusicPosition(raw);
        if (result == -1.0)
            return {};
        return dbl_seconds{result};
    }


    std::optional<dbl_seconds>
    music::get_duration()
        const noexcept
    {
        double result = Mix_MusicDuration(raw);
        if (result == -1.0)
            return {};
        return dbl_seconds{result};
    }


    std::optional<dbl_seconds>
    get_current_duration()
        noexcept
    {
        double result = Mix_MusicDuration(nullptr);
        if (result == -1.0)
            return {};
        return dbl_seconds{result};
    }


    std::optional<dbl_seconds>
    music::get_loop_start()
        const noexcept
    {
        double result = Mix_GetMusicLoopStartTime(raw);
        if (result == -1.0)
            return {};
        return dbl_seconds{result};
    }


    std::optional<dbl_seconds>
    music::get_current_loop_start()
        noexcept
    {
        double result = Mix_GetMusicLoopStartTime(nullptr);
        if (result == -1.0)
            return {};
        return dbl_seconds{result};
    }


    std::optional<dbl_seconds>
    music::get_loop_end()
        const noexcept
    {
        double result = Mix_GetMusicLoopEndTime(raw);
        if (result == -1.0)
            return {};
        return dbl_seconds{result};
    }


    std::optional<dbl_seconds>
    music::get_current_loop_end()
        noexcept
    {
        double result = Mix_GetMusicLoopEndTime(nullptr);
        if (result == -1.0)
            return {};
        return dbl_seconds{result};
    }


    std::optional<dbl_seconds>
    music::get_loop_length()
        const noexcept
    {
        double result = Mix_GetMusicLoopLengthTime(raw);
        if (result == -1.0)
            return {};
        return dbl_seconds{result};
    }


    std::optional<dbl_seconds>
    music::get_current_loop_length()
        noexcept
    {
        double result = Mix_GetMusicLoopLengthTime(nullptr);
        if (result == -1.0)
            return {};
        return dbl_seconds{result};
    }


    bool
    music::is_playing()
        noexcept
    {
        return Mix_PlayingMusic();
    }


    bool
    music::set_cmd(const char* cmd)
        noexcept
    {
        return !Mix_SetMusicCMD(cmd);
    }


    void
    set_post_mix(mix_function func,
                 void* ctx)
        noexcept
    {
        Mix_SetPostMix(func, ctx);
    }


    void
    hook_music(mix_function func,
               void* ctx)
        noexcept
    {
        Mix_HookMusic(func, ctx);
    }


    void*
    get_music_hook_data()
        noexcept
    {
        return Mix_GetMusicHookData();
    }


    void
    hook_music_finished(music_finished_function func)
        noexcept
    {
        Mix_HookMusicFinished(func);
    }


    void
    hook_channel_finished(channel_finished_function func)
        noexcept
    {
        Mix_ChannelFinished(func);
    }


    void
    register_effect(unsigned channel,
                    effect_function effect_func,
                    effect_done_function effect_done_func,
                    void* ctx)
    {
        if (!Mix_RegisterEffect(channel, effect_func, effect_done_func, ctx))
            throw error{};
    }


    void
    unregister_effect(unsigned channel,
                      effect_function func)
    {
        if (!Mix_UnregisterEffect(channel, func))
            throw error{};
    }


    void
    unregister_all_effects(unsigned channel)
    {
        if (!Mix_UnregisterAllEffects(channel))
            throw error{};
    }


    void
    set_panning(unsigned channel,
                float left,
                float right)
    {
        if (!Mix_SetPanning(channel,
                            map_to_uint8(left),
                            map_to_uint8(right)))
            throw error{};
    }


    void
    reset_panning(unsigned channel)
    {
        set_panning(channel, 255, 255);
    }


    void
    set_position(unsigned channel,
                 degreesf angle,
                 float distance)
    {
        if (!Mix_SetPosition(channel,
                             static_cast<Sint16>(angle.value()),
                             map_to_uint8(distance)))
            throw error{};
    }


    void
    reset_position(unsigned channel)
    {
        if (!Mix_SetPosition(channel, 0, 0))
            throw error{};
    }


    void
    set_distance(unsigned channel,
                 float distance)
    {
        if (!Mix_SetDistance(channel, map_to_uint8(distance)))
            throw error{};
    }


    void
    reset_distance(unsigned channel)
    {
        if (!Mix_SetDistance(channel, 0))
            throw error{};
    }


    void
    set_reverse_stereo(unsigned channel,
                       bool reverse)
    {
        if (!Mix_SetReverseStereo(channel, reverse))
            throw error{};
    }


    unsigned
    reserve_channels(unsigned n)
        noexcept
    {
        return Mix_ReserveChannels(n);
    }


    void
    set_group(unsigned channel,
              int tag)
    {
        if (!Mix_GroupChannel(channel, tag))
            throw error{};
    }


    void
    set_group(unsigned first,
              unsigned last,
              int tag)
    {
        if (!Mix_GroupChannels(first, last, tag))
            throw error{};
    }


    std::optional<unsigned>
    get_first_available(int tag)
        noexcept
    {
        int result = Mix_GroupAvailable(tag);
        if (result == -1)
            return {};
        return result;
    }


    unsigned
    group_size(int tag)
        noexcept
    {
        return Mix_GroupCount(tag);
    }


    unsigned
    size()
        noexcept
    {
        return group_size(-1);
    }


    std::optional<unsigned>
    get_newest(int tag)
        noexcept
    {
        int result = Mix_GroupNewer(tag);
        if (result == -1)
            return {};
        return result;
    }


    std::optional<unsigned>
    get_oldest(int tag)
        noexcept
    {
        int result = Mix_GroupOldest(tag);
        if (result == -1)
            return {};
        return result;
    }


    float
    get_volume(unsigned channel)
        noexcept
    {
        int result = Mix_Volume(channel, -1);
        return result / float(max_volume);
    }


    float
    get_volume()
        noexcept
    {
        int result = Mix_Volume(-1, -1);
        return result / float(max_volume);
    }


    float
    set_volume(unsigned channel,
               float new_volume)
        noexcept
    {
        int result = Mix_Volume(channel, map_to_volume(new_volume));
        return result / float(max_volume);
    }


    float
    set_volume(float new_volume)
        noexcept
    {
        int result = Mix_Volume(-1, map_to_volume(new_volume));
        return result / float(max_volume);
    }


    float
    get_master_volume()
        noexcept
    {
        int result = Mix_MasterVolume(-1);
        return result / float(max_volume);
    }


    float
    set_master_volume(float new_volume)
        noexcept
    {
        int result = Mix_MasterVolume(map_to_volume(new_volume));
        return result / float(max_volume);
    }


    void
    halt(unsigned channel)
    {
        if (Mix_HaltChannel(channel) < 0)
            throw error{};
    }


    void
    halt()
    {
        if (Mix_HaltChannel(-1) < 0)
            throw error{};
    }


    void
    halt_group(int tag)
        noexcept
    {
        Mix_HaltGroup(tag);
    }



    unsigned
    expire(unsigned channel,
           milliseconds duration)
        noexcept
    {
        return Mix_ExpireChannel(channel, duration.count());
    }


    unsigned
    expire(milliseconds duration)
        noexcept
    {
        return Mix_ExpireChannel(-1, duration.count());
    }


    unsigned
    no_expire(unsigned channel)
        noexcept
    {
        return Mix_ExpireChannel(channel, -1);
    }


    unsigned
    no_expire()
        noexcept
    {
        return Mix_ExpireChannel(-1, -1);
    }


    void
    fade_out(unsigned channel,
             milliseconds fade_duration)
    {
        if (!Mix_FadeOutChannel(channel, fade_duration.count()))
            throw error{};
    }


    unsigned
    fade_out_group(int tag,
                   milliseconds fade_duration)
        noexcept
    {
        return Mix_FadeOutGroup(tag, fade_duration.count());
    }


    fading_status
    get_fading_status(unsigned channel)
        noexcept
    {
        return static_cast<fading_status>(Mix_FadingChannel(channel));
    }


    void
    pause(unsigned channel)
        noexcept
    {
        Mix_Pause(channel);
    }


    void
    pause()
        noexcept
    {
        Mix_Pause(-1);
    }


    void
    resume(unsigned channel)
        noexcept
    {
        Mix_Resume(channel);
    }


    void
    resume()
        noexcept
    {
        Mix_Resume(-1);
    }


    bool
    is_paused(unsigned channel)
        noexcept
    {
        return Mix_Paused(channel);
    }


    bool
    is_paused()
        noexcept
    {
        return Mix_Paused(-1);
    }


    bool
    is_playing(unsigned channel)
        noexcept
    {
        return Mix_Playing(channel);
    }


    unsigned
    is_playing()
        noexcept
    {
        return Mix_Playing(-1);
    }


    bool
    set_sound_fonts(const char* paths)
        noexcept
    {
        return Mix_SetSoundFonts(paths);
    }


    const char*
    get_sound_fonts()
    {
        const char* result = Mix_GetSoundFonts();
        if (!result)
            throw error{}; // TODO: check if there's any error to report
        return result;
    }


    bool
    for_eac_sound_font(sound_font_function func,
                       void* ctx)
        noexcept
    {
        return Mix_EachSoundFont(func, ctx);
    }


    void
    set_timidity_cfg(const path& cfg_filename)
    {
        if (!Mix_SetTimidityCfg(cfg_filename.c_str()))
            throw error{};
    }


    const char*
    get_timidity_cfg()
        noexcept
    {
        return Mix_GetTimidityCfg();
    }

} // namespace sdl::mix
