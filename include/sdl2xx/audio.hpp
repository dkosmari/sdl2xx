/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_AUDIO_HPP
#define SDL2XX_AUDIO_HPP

#include <concepts>
#include <filesystem>
#include <utility>

#include <span>

#include <SDL_audio.h>

#include "basic_locker.hpp"
#include "basic_wrapper.hpp"
#include "blob.hpp"
#include "rwops.hpp"
#include "string.hpp"
#include "vector.hpp"


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


    enum class allow_change : Uint32 {
        any       = SDL_AUDIO_ALLOW_ANY_CHANGE,
        channels  = SDL_AUDIO_ALLOW_CHANNELS_CHANGE,
        format    = SDL_AUDIO_ALLOW_FORMAT_CHANGE,
        frequency = SDL_AUDIO_ALLOW_FREQUENCY_CHANGE,
        nothing   = 0,
        samples   = SDL_AUDIO_ALLOW_SAMPLES_CHANGE,
    };


    template<std::same_as<allow_change>... Allow>
    requires(sizeof...(Allow) > 0)
    [[nodiscard]]
    constexpr
    Uint32
    convert(Allow... changes)
        noexcept
    {
        return (static_cast<Uint32>(changes) | ...);
    }


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

        using base_type = basic_wrapper<SDL_AudioDeviceID>;

        // Inherit constructors.
        using base_type::base_type;


        // TODO: add obtained overloads

        // generic
        device(const char* name,
               bool is_capture,
               const spec& desired,
               Uint32 allowed_changes = convert(allow_change::any));

        // nameless
        device(bool is_capture,
               const spec& desired,
               Uint32 allowed_changes = convert(allow_change::any));

        // stringy name
        device(const concepts::string auto& name,
               bool is_capture,
               const spec& desired,
               Uint32 allowed_changes = convert(allow_change::any));

        // changes list
        template<std::same_as<allow_change>... Allow>
        requires(sizeof...(Allow) > 0)
        device(const char* name,
               bool is_capture,
               const spec& desired,
               Allow... changes);

        // nameless + changes list
        template<std::same_as<allow_change>... Allow>
        requires(sizeof...(Allow) > 0)
        device(bool is_capture,
               const spec& desired,
               Allow... changes);

        // stringy name + changes list
        template<concepts::string Str,
                 std::same_as<allow_change>... Allow>
        requires(sizeof...(Allow) > 0)
        device(const Str& name,
               bool is_capture,
               const spec& desired,
               Allow... changes);

        // obtained
        device(const char* name,
               bool is_capture,
               const spec& desired,
               spec& obtained,
               Uint32 allowed_changes = convert(allow_change::any));

        // nameless + obtained
        device(bool is_capture,
               const spec& desired,
               spec& obtained,
               Uint32 allowed_changes = convert(allow_change::any));

        // stringy name + obtained
        device(const concepts::string auto& name,
               bool is_capture,
               const spec& desired,
               spec& obtained,
               Uint32 allowed_changes = convert(allow_change::any));

        // obtained + changes list
        template<std::same_as<allow_change>... Allow>
        requires(sizeof...(Allow) > 0)
        device(const char* name,
               bool is_capture,
               const spec& desired,
               spec& obtained,
               Allow... changes);

        // nameless + obtained + changes list
        template<std::same_as<allow_change>... Allow>
        requires(sizeof...(Allow) > 0)
        device(bool is_capture,
               const spec& desired,
               spec& obtained,
               Allow... changes);

        // stringy name + obtained + changes list
        template<concepts::string Str,
                 std::same_as<allow_change>... Allow>
        requires(sizeof...(Allow) > 0)
        device(const Str& name,
               bool is_capture,
               const spec& desired,
               spec& obtained,
               Allow... changes);


        /// Move constructor.
        device(device&& other)
            noexcept = default;


        ~device()
            noexcept;


        /// Move assignment.
        device&
        operator =(device&& other)
            noexcept = default;

        // generic
        void
        create(const char* name,
               bool is_capture,
               const spec& desired,
               Uint32 allowed_changes = convert(allow_change::any));

        // nameless
        void
        create(bool is_capture,
               const spec& desired,
               Uint32 allowed_changes = convert(allow_change::any));


        // stringy name
        void
        create(const concepts::string auto& name,
               bool is_capture,
               const spec& desired,
               Uint32 allowed_changes = convert(allow_change::any));

        // changes list
        template<std::same_as<allow_change>... Allow>
        requires(sizeof...(Allow) > 0)
        void
        create(const char* name,
               bool is_capture,
               const spec& desired,
               Allow... changes);

        // nameless + changes list
        template<std::same_as<allow_change>... Allow>
        requires(sizeof...(Allow) > 0)
        void
        create(bool is_capture,
               const spec& desired,
               Allow... changes);

        // stringy name + changes list
        template<concepts::string Str,
                 std::same_as<allow_change>... Allow>
        requires(sizeof...(Allow) > 0)
        void
        create(const Str& name,
               bool is_capture,
               const spec& desired,
               Allow... changes);

        // obtained
        void
        create(const char* name,
               bool is_capture,
               const spec& desired,
               spec& obtained,
               Uint32 allowed_changes = convert(allow_change::any));

        // nameless + obtained
        void
        create(bool is_capture,
               const spec& desired,
               spec& obtained,
               Uint32 allowed_changes = convert(allow_change::any));

        // stringy name + obtained
        void
        create(const concepts::string auto& name,
               bool is_capture,
               const spec& desired,
               spec& obtained,
               Uint32 allowed_changes = convert(allow_change::any));

        // obtained + changes list
        template<std::same_as<allow_change>... Allow>
        requires(sizeof...(Allow) > 0)
        void
        create(const char* name,
               bool is_capture,
               const spec& desired,
               spec& obtained,
               Allow... changes);

        // nameless + obtained + changes list
        template<std::same_as<allow_change>... Allow>
        requires(sizeof...(Allow) > 0)
        void
        create(bool is_capture,
               const spec& desired,
               spec& obtained,
               Allow... changes);

        // stringy name + obtained + changes list
        template<concepts::string Str,
                 std::same_as<allow_change>... Allow>
        requires(sizeof...(Allow) > 0)
        void
        create(const Str& name,
               bool is_capture,
               const spec& desired,
               spec& obtained,
               Allow... changes);


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

        template<typename T,
                 std::size_t E>
        void
        play(std::span<const T, E> samples)
        {
            play(samples.data(), samples.size_bytes());
        }


        [[nodiscard]]
        std::size_t
        capture(void* buf,
            std::size_t size)
            noexcept;

        template<typename T,
                 std::size_t E>
        [[nodiscard]]
        std::span<T>
        capture(std::span<T, E> buf)
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
            const noexcept;


        void
        unlock()
            const noexcept;


        struct locker : basic_locker<device> {

            using base_type = basic_locker<device>;


            // Inherit constructors.
            using base_type::base_type;


            explicit
            locker(const device* d);

            explicit
            locker(const device& d);


            ~locker()
                noexcept;


            /// Move constructor.
            locker(locker&& other)
                noexcept = default;


            /// Move assignment.
            locker&
            operator =(locker&& other)
                noexcept = default;


            void
            lock();


            void
            unlock()
                noexcept override;


        }; // struct locker


    }; // struct device


    [[nodiscard]]
    std::pair<blob, spec>
    load_wav(SDL_RWops* src,
             bool close_src);

    [[nodiscard]]
    std::pair<blob, spec>
    load_wav(rwops& src);

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

        using base_type = basic_wrapper<SDL_AudioStream*>;


        // Inherit constructors.
        using base_type::base_type;


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

        template<typename T,
                 std::size_t E>
        void
        put(std::span<const T, E> samples)
        {
            put(samples.data(), samples.size_bytes());
        }

        void
        put(const blob& samples);


        [[nodiscard]]
        std::size_t
        get(void* buf,
            std::size_t size);


        template<typename T,
                 std::size_t E>
        std::size_t
        get(std::span<T, E> buf)
        {
            return get(buf.data(), buf.size_bytes());
        }

        vector<Uint8>
        get(std::size_t size);


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

    template<typename T,
             std::size_t E>
    void
    mix_audio(std::span<T, E> dst,
              std::span<T, E> src,
              format fmt,
              int volume)
    {
        mix_audio(dst.data(),
                  src.data(),
                  fmt,
                  src.size_bytes(),
                  volume);
    }


    // Implementation of templated methods.


    // stringy name
    inline
    device::device(const concepts::string auto& name,
                   bool is_capture,
                   const spec& desired,
                   Uint32 allowed_changes) :
        device{name.empty() ? nullptr : name.data(),
               is_capture, desired, allowed_changes}
    {}


    // changes list
    template<std::same_as<allow_change>... Allow>
    requires(sizeof...(Allow) > 0)
    device::device(const char* name,
                   bool is_capture,
                   const spec& desired,
                   Allow... changes) :
        device{name, is_capture, desired, convert(changes...)}
    {}


    // nameless + changes list
    template<std::same_as<allow_change>... Allow>
    requires(sizeof...(Allow) > 0)
    device::device(bool is_capture,
                   const spec& desired,
                   Allow... changes) :
        device{nullptr, desired, convert(changes...)}
    {}


    // stringy name + changes list
    template<concepts::string Str,
             std::same_as<allow_change>... Allow>
    requires(sizeof...(Allow) > 0)
    device::device(const Str& name,
                   bool is_capture,
                   const spec& desired,
                   Allow... changes) :
        device{name.data(), is_capture, desired, convert(changes...)}
    {}


    // stringy name + obtained
    device::device(const concepts::string auto& name,
                   bool is_capture,
                   const spec& desired,
                   spec& obtained,
                   Uint32 allowed_changes) :
        device{name.data(), is_capture, desired, obtained, allowed_changes}
    {}


    // obtained + changes list
    template<std::same_as<allow_change>... Allow>
    requires(sizeof...(Allow) > 0)
    device::device(const char* name,
                   bool is_capture,
                   const spec& desired,
                   spec& obtained,
                   Allow... changes) :
        device{name, is_capture, desired, obtained, convert(changes...)}
    {}


    // nameless + obtained + changes list
    template<std::same_as<allow_change>... Allow>
    requires(sizeof...(Allow) > 0)
    device::device(bool is_capture,
                   const spec& desired,
                   spec& obtained,
                   Allow... changes) :
        device{is_capture, desired, obtained, convert(changes...)}
    {}


    // stringy name + obtained + changes list
    template<concepts::string Str,
             std::same_as<allow_change>... Allow>
    requires(sizeof...(Allow) > 0)
    device::device(const Str& name,
                   bool is_capture,
                   const spec& desired,
                   spec& obtained,
                   Allow... changes) :
        device{name, is_capture, desired, obtained, convert(changes...)}
    {}


    // stringy name
    void
    device::create(const concepts::string auto& name,
                   bool is_capture,
                   const spec& desired,
                   Uint32 allowed_changes)
    {
        create(name.data(), is_capture, desired, allowed_changes);
    }


    // changes list
    template<std::same_as<allow_change>... Allow>
    requires(sizeof...(Allow) > 0)
    void
    device::create(const char* name,
                   bool is_capture,
                   const spec& desired,
                   Allow... changes)
    {
        create(name, is_capture, desired, convert(changes...));
    }


    // nameless + changes list
    template<std::same_as<allow_change>... Allow>
    requires(sizeof...(Allow) > 0)
    void
    device::create(bool is_capture,
                   const spec& desired,
                   Allow... changes)
    {
        create(is_capture, desired, changes...);
    }


    // stringy name + changes list
    template<concepts::string Str,
             std::same_as<allow_change>... Allow>
    requires(sizeof...(Allow) > 0)
    void
    device::create(const Str& name,
                   bool is_capture,
                   const spec& desired,
                   Allow... changes)
    {
        create(name, is_capture, desired, convert(changes...));
    }


    // stringy name + obtained
    void
    device::create(const concepts::string auto& name,
                   bool is_capture,
                   const spec& desired,
                   spec& obtained,
                   Uint32 allowed_changes)
    {
        create(name.data(), is_capture, desired, obtained, allowed_changes);
    }


    // obtained + changes list
    template<std::same_as<allow_change>... Allow>
    requires(sizeof...(Allow) > 0)
    void
    device::create(const char* name,
                   bool is_capture,
                   const spec& desired,
                   spec& obtained,
                   Allow... changes)
    {
        create(name, is_capture, desired, obtained, convert(changes...));
    }


    // nameless + obtained + changes list
    template<std::same_as<allow_change>... Allow>
    requires(sizeof...(Allow) > 0)
    void
    device::create(bool is_capture,
                   const spec& desired,
                   spec& obtained,
                   Allow... changes)
    {
        create(is_capture, desired, obtained, convert(changes...));
    }


    // stringy name + obtained + changes list
    template<concepts::string Str,
             std::same_as<allow_change>... Allow>
    requires(sizeof...(Allow) > 0)
    void
    device::create(const Str& name,
                   bool is_capture,
                   const spec& desired,
                   spec& obtained,
                   Allow... changes)
    {
        create(name, is_capture, desired, obtained, convert(changes...));
    }


} // namespace sdl::audio

#endif
