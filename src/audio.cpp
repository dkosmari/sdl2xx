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


    spec
    get_spec(unsigned index,
             bool is_capture)
    {
        spec result;
        if (SDL_GetAudioDeviceSpec(index, is_capture, &result))
            throw error{};
        return result;
    }


    std::pair<string, spec>
    get_default_info(bool is_capture)
    {
        spec sp;
        char* raw_name = nullptr;
        if (SDL_GetDefaultAudioInfo(&raw_name, &sp, is_capture))
            throw error{};
        try {
            string name;
            if (raw_name)
                name = raw_name;
            SDL_free(raw_name);
            return {std::move(name), std::move(sp)};
        }
        catch (...) {
            SDL_free(raw_name);
            throw;
        }
    }


    device::device(const char* name,
                   bool is_capture,
                   const spec& desired,
                   Uint32 allowed_changes)
    {
        create(name, is_capture, desired, allowed_changes);
    }


    device::~device()
        noexcept
    {
        destroy();
    }


    void
    device::create(const char* name,
                   bool is_capture,
                   const spec& desired,
                   Uint32 allowed_changes)
    {
        destroy();
        auto id = SDL_OpenAudioDevice(name,
                                      is_capture,
                                      &desired,
                                      nullptr,
                                      allowed_changes);
        if (!id)
            throw error{};
        acquire(id);
    }


    void
    device::create(const char* name,
                   bool is_capture,
                   const spec& desired,
                   spec& obtained,
                   Uint32 allowed_changes)
    {
        destroy();
        auto id = SDL_OpenAudioDevice(name,
                                      is_capture,
                                      &desired,
                                      &obtained,
                                      allowed_changes);
        if (!id)
            throw error{};
        acquire(id);
    }


    void
    device::destroy()
        noexcept
    {
        if (is_valid())
            SDL_CloseAudioDevice(release());
    }


    status
    device::get_status()
        const
    {
        return static_cast<status>(SDL_GetAudioDeviceStatus(raw));
    }


    void
    device::set_pause(bool paused)
        noexcept
    {
        SDL_PauseAudioDevice(raw, paused);
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
        if (SDL_QueueAudio(raw, samples, size) < 0)
            throw error{};
    }


    std::size_t
    device::capture(void* buf,
                    std::size_t size)
        noexcept
    {
        return SDL_DequeueAudio(raw, buf, size);
    }


    std::size_t
    device::get_size()
        const noexcept
    {
        return SDL_GetQueuedAudioSize(raw);
    }


    void
    device::clear()
        noexcept
    {
        SDL_ClearQueuedAudio(raw);
    }


    void
    device::lock()
        noexcept
    {
        SDL_LockAudioDevice(raw);
    }


    void
    device::unlock()
        noexcept
    {
        SDL_UnlockAudioDevice(raw);
    }


    device::lock_guard::lock_guard(device& d) :
        dev(d)
    {
        dev.lock();
    }


    device::lock_guard::lock_guard(device& d,
                                   device::lock_guard::adopt_lock_t)
        noexcept :
        dev(d)
    {}


    device::lock_guard::~lock_guard()
        noexcept
    {
        dev.unlock();
    }


    std::pair<blob, spec>
    load_wav(SDL_RWops* src,
             bool close_src)
    {
        spec sp;
        Uint8* buf;
        Uint32 size;
        if (!SDL_LoadWAV_RW(src, close_src, &sp, &buf, &size))
            throw error{};
        // Note: SDL_FreeWAV() is just SDL_free()
        return {blob{buf, size}, std::move(sp)};
    }


    std::pair<blob, spec>
    load_wav(const path& filename)
    {
        return load_wav(SDL_RWFromFile(filename.c_str(), "rb"), true);
    }


    converter::converter(format src_format,
                         Uint8 src_channels,
                         int src_rate,
                         format dst_format,
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


    stream::stream(format src_format,
                   Uint8 src_channels,
                   int src_rate,
                   format dst_format,
                   Uint8 dst_channels,
                   int dst_rate)
    {
        create(src_format, src_channels, src_rate,
               dst_format, dst_channels, dst_rate);
    }


    stream::~stream()
        noexcept
    {
        destroy();
    }


    void
    stream::create(format src_format,
                   Uint8 src_channels,
                   int src_rate,
                   format dst_format,
                   Uint8 dst_channels,
                   int dst_rate)
    {
        auto str = SDL_NewAudioStream(src_format, src_channels, src_rate,
                                      dst_format, dst_channels, dst_rate);
        if (!str)
            throw error{};
        destroy();
        acquire(str);
    }


    void
    stream::destroy()
        noexcept
    {
        if (is_valid())
            SDL_FreeAudioStream(release());
    }


    void
    stream::put(const void* buf,
                std::size_t size)
    {
        if (SDL_AudioStreamPut(raw, buf, size) < 0)
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
        int result = SDL_AudioStreamGet(raw, buf, size);
        if (result < 0)
            throw error{};
        return result;
    }


    std::size_t
    stream::get_available()
        const noexcept
    {
        return SDL_AudioStreamAvailable(raw);
    }


    void
    stream::flush()
    {
        if (SDL_AudioStreamFlush(raw) < 0)
            throw error{};
    }


    void
    stream::clear()
        noexcept
    {
        SDL_AudioStreamClear(raw);
    }


    void
    mix_audio(void* dst,
              const void* src,
              format fmt,
              std::size_t size,
              int volume)
        noexcept
    {
        SDL_MixAudioFormat(static_cast<Uint8*>(dst),
                           static_cast<const Uint8*>(src),
                           fmt,
                           size,
                           volume);
    }

} // namespace sdl::audio
