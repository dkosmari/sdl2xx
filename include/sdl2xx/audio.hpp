/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_AUDIO_HPP
#define SDL2XX_AUDIO_HPP

#include <filesystem>
#include <utility>

#include <span>

#include <SDL_audio.h>

#include "basic_wrapper.hpp"
#include "blob.hpp"
#include "string.hpp"


namespace sdl::audio {

    using std::filesystem::path;


    using format = SDL_AudioFormat;


    struct spec : SDL_AudioSpec {

        // Ensure everything is zero-initialized.
        constexpr
        spec()
            noexcept :
            SDL_AudioSpec{}
        {}

    };


    enum class status {
        paused  = SDL_AUDIO_PAUSED,
        playing = SDL_AUDIO_PLAYING,
        stopped = SDL_AUDIO_STOPPED,
    };


    enum allow : Uint32 {
        change_any       = SDL_AUDIO_ALLOW_ANY_CHANGE,
        change_channels  = SDL_AUDIO_ALLOW_CHANNELS_CHANGE,
        change_format    = SDL_AUDIO_ALLOW_FORMAT_CHANGE,
        change_frequency = SDL_AUDIO_ALLOW_FREQUENCY_CHANGE,
        change_nothing   = 0,
        change_samples   = SDL_AUDIO_ALLOW_SAMPLES_CHANGE,
    };


    [[nodiscard]]
    unsigned
    get_num_drivers();


    [[nodiscard]]
    const char*
    get_driver(unsigned index);


    void
    init(const char* driver);

    inline
    void
    init(const concepts::string auto& driver)
    {
        init(driver.data());
    }


    void
    quit()
        noexcept;


    [[nodiscard]]
    const char*
    get_current_driver();


    [[nodiscard]]
    int
    get_num_devices(bool is_capture = false);


    [[nodiscard]]
    const char*
    get_name(unsigned index,
             bool is_capture);


    [[nodiscard]]
    spec
    get_spec(unsigned index,
             bool is_capture);


    [[nodiscard]]
    std::pair<string, spec>
    get_default_info(bool is_capture);


    struct device : basic_wrapper<SDL_AudioDeviceID> {

        using parent_type = basic_wrapper<SDL_AudioDeviceID>;

        // Inherit constructors.
        using parent_type::parent_type;


        device(const char* name,
               bool is_capture,
               const spec& desired,
               Uint32 allowed_changes = allow::change_any);

        inline
        device(const concepts::string auto& name,
               bool is_capture,
               const spec& desired,
               Uint32 allowed_changes = allow::change_any) :
            device{name.empty() ? nullptr : name.data(),
                   is_capture, desired, allowed_changes}
        {}


        /// Move constructor.
        device(device&& other)
            noexcept = default;


        ~device()
            noexcept;


        /// Move assignment.
        device&
        operator =(device&& other)
            noexcept = default;


        void
        create(const char* name,
               bool is_capture,
               const spec& desired,
               Uint32 allowed_changes = allow::change_any);

        void
        create(const char* name,
               bool is_capture,
               const spec& desired,
               spec& obtained,
               Uint32 allowed_changes = allow::change_any);


        void
        destroy()
            noexcept;


        [[nodiscard]]
        status
        get_status()
            const;


        void
        set_pause(bool paused)
            noexcept;

        void
        pause()
            noexcept;

        void
        unpause()
            noexcept;


        void
        play(const void* samples,
             std::size_t size);

        template<typename T>
        void
        play(std::span<const T> samples)
        {
            play(samples.data(), samples.size_bytes());
        }


        [[nodiscard]]
        std::size_t
        capture(void* buf,
            std::size_t size)
            noexcept;

        template<typename T>
        [[nodiscard]]
        std::span<T>
        capture(std::span<T> buf)
            noexcept
        {
            std::size_t result = capture(buf.size(), buf.size_bytes());
            return {buf.data(), result};
        }


        [[nodiscard]]
        std::size_t
        get_size()
            const noexcept;


        void
        clear()
            noexcept;


        void
        lock()
            noexcept;


        void
        unlock()
            noexcept;


        class lock_guard {

            device& dev;

        public:

            struct adopt_lock_t {};
            static constexpr adopt_lock_t adopt_lock{};


            lock_guard(device& d);

            lock_guard(device& d,
                       adopt_lock_t adopt)
                noexcept;

            ~lock_guard()
                noexcept;

            // Disallow copying.
            lock_guard(const lock_guard& other) = delete;

        }; // class lock_guard


    }; // struct device


    [[nodiscard]]
    std::pair<blob, spec>
    load_wav(SDL_RWops* src,
             bool close_src);


    [[nodiscard]]
    std::pair<blob, spec>
    load_wav(const path& filename);


    struct converter : SDL_AudioCVT {

        bool needed = false;


        converter(format src_format,
                  Uint8 src_channels,
                  int src_rate,
                  format dst_format,
                  Uint8 dst_channels,
                  int dst_rate);


        void
        convert();

        /// Same as convert()
        void
        operator ()();

    }; // struct converter


    struct stream : basic_wrapper<SDL_AudioStream*> {

        using parent_type = basic_wrapper<SDL_AudioStream*>;


        // Inherit constructors.
        using parent_type::parent_type;


        stream(format src_format,
               Uint8 src_channels,
               int src_rate,
               format dst_format,
               Uint8 dst_channels,
               int dst_rate);

        /// Move constructor.
        stream(stream&& other)
            noexcept = default;


        ~stream()
            noexcept;


        /// Move assignment.
        stream&
        operator =(stream&& other)
            noexcept = default;



        void
        create(format src_format,
               Uint8 src_channels,
               int src_rate,
               format dst_format,
               Uint8 dst_channels,
               int dst_rate);


        void
        destroy()
            noexcept;


        void
        put(const void* buf,
            std::size_t size);

        void
        put(std::span<const char> samples);

        void
        put(std::span<const Uint8> samples);

        void
        put(const blob& samples);


        [[nodiscard]]
        std::size_t
        get(void* buf,
            std::size_t size);


        [[nodiscard]]
        std::size_t
        get_available()
            const noexcept;


        void
        flush();


        void
        clear()
            noexcept;

    }; // struct stream


    void
    mix_audio(void* dst,
              const void* src,
              format fmt,
              std::size_t size,
              int volume)
        noexcept;

    template<typename T>
    void
    mix_audio(std::span<T> dst,
              std::span<T> src,
              format fmt,
              int volume)
    {
        mix_audio(dst.data(),
                  src.data(),
                  fmt,
                  src.size_bytes(),
                  volume);
    }

} // namespace sdl::audio

#endif
