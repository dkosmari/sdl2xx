/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include "audio.hpp"

#include "error.hpp"


namespace sdl::audio {

    unsigned
    get_num_drivers()
    {
        int result = SDL_GetNumAudioDrivers();
        if (result < 0)
            throw error{};
        return result;
    }


    [[nodiscard]]
    const char*
    get_driver(unsigned index)
    {
        const char* driver = SDL_GetAudioDriver(index);
        if (!driver)
            throw error{};
        return driver;
    }


    void
    init(const char* driver)
    {
        if (SDL_AudioInit(driver) < 0)
            throw error{};
    }


    void
    quit()
        noexcept
    {
        SDL_AudioQuit();
    }


    const char*
    get_current_driver()
    {
        const char* driver = SDL_GetCurrentAudioDriver();
        if (!driver)
            throw error{};
        return driver;
    }


    int
    get_num_devices(bool is_capture)
    {
        return SDL_GetNumAudioDevices(is_capture);
    }


    const char*
    get_name(unsigned index,
             bool is_capture)
    {
        const char* name = SDL_GetAudioDeviceName(index, is_capture);
        if (!name)
            throw error{};
        return name;
    }


    spec_t
    get_spec(unsigned index,
             bool is_capture)
    {
        spec_t spec;
        if (SDL_GetAudioDeviceSpec(index, is_capture, &spec))
            throw error{};
        return spec;
    }


    std::pair<string, spec_t>
    get_default_info(bool is_capture)
    {
        spec_t spec;
        char* raw_name = nullptr;
        if (SDL_GetDefaultAudioInfo(&raw_name, &spec, is_capture))
            throw error{};
        try {
            string name;
            if (raw_name)
                name = raw_name;
            SDL_free(raw_name);
            return {std::move(name), std::move(spec)};
        }
        catch (...) {
            SDL_free(raw_name);
            throw;
        }
    }



    device::device(SDL_AudioDeviceID id_)
        noexcept :
        id{id_}
    {}


    device::device(const char* name,
                   bool is_capture,
                   const spec_t& desired,
                   bool allowed_changes)
    {
        create(name, is_capture, desired, allowed_changes);
    }


    device::device(device&& other)
        noexcept :
        id{other.id}
    {
        other.id = 0;
    }


    device::~device()
        noexcept
    {
        destroy();
    }


    device&
    device::operator =(device&& other)
        noexcept
    {
        if (this != &other) {
            destroy();
            id = other.id;
            other.id = 0;
        }
        return *this;
    }



    void
    device::create(const char* name,
                   bool is_capture,
                   const spec_t& desired,
                   bool allowed_changes)
    {
        destroy();
        id = SDL_OpenAudioDevice(name, is_capture, &desired, nullptr, allowed_changes);
        if (!id)
            throw error{};
    }


    void
    device::create(const char* name,
                   bool is_capture,
                   const spec_t& desired,
                   spec_t& obtained,
                   bool allowed_changes)
    {
        destroy();
        id = SDL_OpenAudioDevice(name, is_capture, &desired, &obtained, allowed_changes);
        if (!id)
            throw error{};
    }


    void
    device::destroy()
        noexcept
    {
        if (id) {
            SDL_CloseAudioDevice(id);
            id = 0;
        }
    }


    bool
    device::is_valid()
        const noexcept
    {
        return id;
    }


    device::operator bool()
        const noexcept
    {
        return id;
    }


    SDL_AudioDeviceID
    device::data()
            const noexcept
    {
        return id;
    }



    status
    device::get_status()
        const
    {
        return static_cast<status>(SDL_GetAudioDeviceStatus(id));
    }


    void
    device::set_pause(bool paused)
        noexcept
    {
        SDL_PauseAudioDevice(id, paused);
    }


    void
    device::pause()
        noexcept
    {
        set_pause(true);
    }


    void
    device::unpause()
        noexcept
    {
        set_pause(false);
    }


    void
    device::play(const void* samples,
                std::size_t size)
    {
        if (SDL_QueueAudio(id, samples, size) < 0)
            throw error{};
    }


    std::size_t
    device::capture(void* buf,
                    std::size_t size)
        noexcept
    {
        return SDL_DequeueAudio(id, buf, size);
    }


    std::size_t
    device::get_size()
        const noexcept
    {
        return SDL_GetQueuedAudioSize(id);
    }


    void
    device::clear()
        noexcept
    {
        SDL_ClearQueuedAudio(id);
    }


    void
    device::lock()
        noexcept
    {
        SDL_LockAudioDevice(id);
    }


    void
    device::unlock()
        noexcept
    {
        SDL_UnlockAudioDevice(id);
    }


    std::pair<blob, spec_t>
    load_wav(SDL_RWops* src,
             bool close_src)
    {
        spec_t spec;
        Uint8* buf;
        Uint32 size;
        if (!SDL_LoadWAV_RW(src, close_src, &spec, &buf, &size))
            throw error{};
        // Note: SDL_FreeWAV() is just SDL_free()
        return {blob{buf, size}, std::move(spec)};
    }


    std::pair<blob, spec_t>
    load_wav(const path& filename)
    {
        return load_wav(SDL_RWFromFile(filename.c_str(), "rb"), true);
    }


    converter::converter(format_t src_format,
                         Uint8 src_channels,
                         int src_rate,
                         format_t dst_format,
                         Uint8 dst_channels,
                         int dst_rate)
    {
        int result = SDL_BuildAudioCVT(this,
                                       src_format, src_channels, src_rate,
                                       dst_format, dst_channels, dst_rate);
        if (result < 0)
            throw error{};
        needed = result;
    }


    void
    converter::convert()
    {
        if (SDL_ConvertAudio(this) < 0)
            throw error{};
    }


    void
    converter::operator ()()
    {
        convert();
    }


    stream::stream(SDL_AudioStream* src)
        noexcept :
        ptr{src}
    {}


    stream::stream(format_t src_format,
                   Uint8 src_channels,
                   int src_rate,
                   format_t dst_format,
                   Uint8 dst_channels,
                   int dst_rate)
    {
        create(src_format, src_channels, src_rate,
               dst_format, dst_channels, dst_rate);
    }


    stream::stream(stream&& other)
        noexcept :
        ptr{other.ptr}
    {
        other.ptr = nullptr;
    }


    stream::~stream()
        noexcept
    {
        destroy();
    }


    stream&
    stream::operator =(stream&& other)
        noexcept
    {
        if (this != &other) {
            destroy();
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }


    void
    stream::create(format_t src_format,
                   Uint8 src_channels,
                   int src_rate,
                   format_t dst_format,
                   Uint8 dst_channels,
                   int dst_rate)
    {
        destroy();
        ptr = SDL_NewAudioStream(src_format, src_channels, src_rate,
                                 dst_format, dst_channels, dst_rate);
        if (!ptr)
            throw error{};
    }


    void
    stream::destroy()
        noexcept
    {
        if (ptr) {
            SDL_FreeAudioStream(ptr);
            ptr = nullptr;
        }
    }


    bool
    stream::is_valid()
        const noexcept
    {
        return ptr;
    }


    stream::operator bool()
        const noexcept
    {
        return ptr;
    }


    SDL_AudioStream*
    stream::data()
        noexcept
    {
        return ptr;
    }


    const SDL_AudioStream*
    stream::data()
        const noexcept
    {
        return ptr;
    }


    void
    stream::put(const void* buf,
                std::size_t size)
    {
        if (SDL_AudioStreamPut(ptr, buf, size) < 0)
            throw error{};
    }


    void
    stream::put(std::span<const char> samples)
    {
        put(samples.data(), samples.size());
    }


    void
    stream::put(std::span<const Uint8> samples)
    {
        put(samples.data(), samples.size());
    }


    void
    stream::put(const blob& samples)
    {
        put(samples.data());
    }


    std::size_t
    stream::get(void* buf,
                std::size_t size)
    {
        int result = SDL_AudioStreamGet(ptr, buf, size);
        if (result < 0)
            throw error{};
        return result;
    }


    std::size_t
    stream::get_available()
        const noexcept
    {
        return SDL_AudioStreamAvailable(ptr);
    }


    void
    stream::flush()
    {
        if (SDL_AudioStreamFlush(ptr) < 0)
            throw error{};
    }


    void
    stream::clear()
        noexcept
    {
        SDL_AudioStreamClear(ptr);
    }


    void
    mix_audio(void* dst,
              const void* src,
              format_t format,
              std::size_t size,
              int volume)
        noexcept
    {
        SDL_MixAudioFormat(static_cast<Uint8*>(dst),
                           static_cast<const Uint8*>(src),
                           format,
                           size,
                           volume);
    }



} // namespace sdl::audio
