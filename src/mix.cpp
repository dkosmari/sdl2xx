/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include "mix.hpp"

#include "error.hpp"


namespace sdl::mix {

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
    open_audio(int frequency,
               format_t format,
               int channels,
               int chunk_size)
    {
        if (Mix_OpenAudio(frequency, format, channels, chunk_size) < 0)
            throw error{};
    }


    void
    open(int frequency,
         format_t format,
         int channels,
         int chunk_size,
         const char* device,
         bool allowed_changes)
    {
        if (Mix_OpenAudioDevice(frequency, format, channels,
                                chunk_size, device, allowed_changes) < 0)
            throw error{};
    }


    std::optional<spec_t>
    query()
    {
        spec_t spec;
        if (!Mix_QuerySpec(&spec.frequency, &spec.format, &spec.channels))
            return {};
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


    chunk::chunk(chunk&& other)
        noexcept
    {
        acquire(other.release());
    }


    chunk::~chunk()
    {
        destroy();
    }


    chunk&
    chunk::operator =(chunk&& other)
        noexcept
    {
        if (this != &other) {
            destroy();
            acquire(other.release());
        }
        return *this;
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


    music::music(music&& other)
        noexcept
    {
        acquire(other.release());
    }


    music::~music()
        noexcept
    {
        destroy();
    }


    music&
    music::operator =(music&& other)
        noexcept
    {
        if (this != &other) {
            destroy();
            acquire(other.release());
        }
        return *this;
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


} // namespace sdl::mix
