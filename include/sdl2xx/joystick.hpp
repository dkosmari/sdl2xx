/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_JOYSTICK_HPP
#define SDL2XX_JOYSTICK_HPP

#include <chrono>
#include <expected>
#include <iosfwd>
#include <mutex>
#include <optional>
#include <span>

#include <SDL_joystick.h>
#include <SDL_version.h>

#include "basic_wrapper.hpp"
#include "color.hpp"
#include "error.hpp"
#include "guid.hpp"
#include "string.hpp"
#include "vec2.hpp"


namespace sdl::joystick {

    using std::chrono::milliseconds;

    using guid = sdl::guid;

    using instance_id = SDL_JoystickID;


#if SDL_VERSION_ATLEAST(2, 0, 6)

    enum class type {
        arcade_pad      = SDL_JOYSTICK_TYPE_ARCADE_PAD,
        arcade_stick    = SDL_JOYSTICK_TYPE_ARCADE_STICK,
        dance_pad       = SDL_JOYSTICK_TYPE_DANCE_PAD,
        drum_kit        = SDL_JOYSTICK_TYPE_DRUM_KIT,
        flight_stick    = SDL_JOYSTICK_TYPE_FLIGHT_STICK,
        game_controller = SDL_JOYSTICK_TYPE_GAMECONTROLLER,
        guitar          = SDL_JOYSTICK_TYPE_GUITAR,
        throttle        = SDL_JOYSTICK_TYPE_THROTTLE,
        unknown         = SDL_JOYSTICK_TYPE_UNKNOWN,
        wheel           = SDL_JOYSTICK_TYPE_WHEEL,
    };


    [[nodiscard]]
    constexpr
    type
    convert(SDL_JoystickType t)
        noexcept
    {
        return static_cast<type>(t);
    }


    [[nodiscard]]
    constexpr
    SDL_JoystickType
    convert(type t)
        noexcept
    {
        return static_cast<SDL_JoystickType>(t);
    }


    string
    to_string(type t);


    std::ostream&
    operator <<(std::ostream& out,
                type t);

#endif // SDL_VERSION_ATLEAST(2, 0, 6)


    enum class power_level {
        empty   = SDL_JOYSTICK_POWER_EMPTY,
        full    = SDL_JOYSTICK_POWER_FULL,
        low     = SDL_JOYSTICK_POWER_LOW,
        medium  = SDL_JOYSTICK_POWER_MEDIUM,
        unknown = SDL_JOYSTICK_POWER_UNKNOWN,
        wired   = SDL_JOYSTICK_POWER_WIRED,
    };


    [[nodiscard]]
    constexpr
    power_level
    convert(SDL_JoystickPowerLevel lvl)
        noexcept
    {
        return static_cast<power_level>(lvl);
    }


    [[nodiscard]]
    constexpr
    SDL_JoystickPowerLevel
    convert(power_level lvl)
        noexcept
    {
        return static_cast<SDL_JoystickPowerLevel>(lvl);
    }


    string
    to_string(power_level lvl);


    std::ostream&
    operator <<(std::ostream& out,
                power_level lvl);


    enum hat_dir : Uint8 {
        centered   = SDL_HAT_CENTERED,
        down       = SDL_HAT_DOWN,
        down_left  = SDL_HAT_LEFTDOWN,
        down_right = SDL_HAT_RIGHTDOWN,
        left       = SDL_HAT_LEFT,
        left_down  = SDL_HAT_LEFTDOWN,  // same as down_left
        left_up    = SDL_HAT_LEFTUP,
        right      = SDL_HAT_RIGHT,
        right_down = SDL_HAT_RIGHTDOWN, // same as down_right
        right_up   = SDL_HAT_RIGHTUP,
        up         = SDL_HAT_UP,
        up_left    = SDL_HAT_LEFTUP,    // same as left_up
        up_right   = SDL_HAT_RIGHTUP,   // same as right_up
    };


    [[nodiscard]]
    constexpr
    Uint8
    convert(hat_dir dir)
        noexcept
    {
        return static_cast<Uint8>(dir);
    }


    string
    to_string(hat_dir dir);


    std::ostream&
    operator <<(std::ostream& out,
                hat_dir dir);


#if SDL_VERSION_ATLEAST(2, 0, 7)

    void
    lock()
        noexcept;

    void
    unlock()
        noexcept;


    class locker {

        bool locked = false;

    public:

        locker()
            noexcept;

        explicit
        locker(std::defer_lock_t defer)
            noexcept;


        ~locker()
            noexcept;


        /// Move constructor.
        locker(locker&& other)
            noexcept;

        /// Move assignment.
        locker&
        operator =(locker&& other)
            noexcept;


        void
        lock()
            noexcept;

        void
        unlock()
            noexcept;

    }; // class locker

#endif // SDL_VERSION_ATLEAST(2, 0, 7)


    [[nodiscard]]
    unsigned
    get_num_devices();


    [[nodiscard]]
    const char*
    get_name(unsigned index);

    [[nodiscard]]
    std::expected<const char*, error>
    try_get_name(unsigned index)
        noexcept;


#if SDL_VERSION_ATLEAST(2, 24, 0)

    [[nodiscard]]
    const char*
    get_path(unsigned index);

    [[nodiscard]]
    std::expected<const char*, error>
    try_get_path(unsigned index)
        noexcept;

#endif // SDL_VERSION_ATLEAST(2, 24, 0)


#if SDL_VERSION_ATLEAST(2, 0, 9)

    [[nodiscard]]
    int
    get_player(unsigned index)
        noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 9)


    [[nodiscard]]
    guid
    get_guid(unsigned index)
        noexcept;


#if SDL_VERSION_ATLEAST(2, 0, 6)

    [[nodiscard]]
    Uint16
    get_vendor(unsigned index)
        noexcept;


    [[nodiscard]]
    Uint16
    get_product(unsigned index)
        noexcept;


    [[nodiscard]]
    Uint16
    get_version(unsigned index)
        noexcept;


    [[nodiscard]]
    type
    get_type(unsigned index)
        noexcept;


    [[nodiscard]]
    instance_id
    get_id(unsigned index);


    // Convenience: get index from id.
    [[nodiscard]]
    unsigned
    get_index(instance_id id);


    // Convenience: get index from id.
    [[nodiscard]]
    std::expected<unsigned, error>
    try_get_index(instance_id id)
        noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 6)


    struct device : basic_wrapper<SDL_Joystick*> {

        using base_type = basic_wrapper<SDL_Joystick*>;


        // Inherit constructors.
        using base_type::base_type;

        /// Move constructor.
        device(device&& other)
            noexcept = default;


        explicit
        device(unsigned index);


        // Named constructor: from instance id.
        [[nodiscard]]
        static
        device
        from_id(instance_id id);


        // Named constructor: from player index.
        [[nodiscard]]
        static
        device
        from_player(int player);


        ~device()
            noexcept;


        /// Move assignment.
        device&
        operator =(device&& other)
            noexcept = default;


        void
        destroy()
            noexcept;


#if SDL_VERSION_ATLEAST(2, 0, 14)

        void
        set_virtual_axis(unsigned axis,
                         double value);


        void
        set_virtual_button(unsigned button,
                           bool value);


        void
        set_virtual_hat(unsigned hat,
                        hat_dir value);

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


        [[nodiscard]]
        const char*
        get_name()
            const;

        [[nodiscard]]
        std::expected<const char*, error>
        try_get_name()
            const noexcept;


#if SDL_VERSION_ATLEAST(2, 24, 0)

        [[nodiscard]]
        const char*
        get_path()
            const;

        [[nodiscard]]
        std::expected<const char*, error>
        try_get_path()
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 24, 0)


#if SDL_VERSION_ATLEAST(2, 0, 9)

        [[nodiscard]]
        int
        get_player()
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 9)


#if SDL_VERSION_ATLEAST(2, 0, 12)

        void
        set_player(int player)
            noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 12)


        [[nodiscard]]
        guid
        get_guid()
            const noexcept;


#if SDL_VERSION_ATLEAST(2, 0, 6)

        [[nodiscard]]
        Uint16
        get_vendor()
            const noexcept;


        [[nodiscard]]
        Uint16
        get_product()
            const noexcept;


        [[nodiscard]]
        Uint16
        get_version()
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 6)


#if SDL_VERSION_ATLEAST(2, 24, 0)

        [[nodiscard]]
        Uint16
        get_firmware()
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 24, 0)


#if SDL_VERSION_ATLEAST(2, 0, 14)

        [[nodiscard]]
        const char*
        get_serial()
            const;

        [[nodiscard]]
        std::expected<const char*, error>
        try_get_serial()
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


#if SDL_VERSION_ATLEAST(2, 0, 6)

        [[nodiscard]]
        type
        get_type()
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 6)


        [[nodiscard]]
        bool
        is_attached()
            const noexcept;


        [[nodiscard]]
        instance_id
        get_id()
            const;


        [[nodiscard]]
        unsigned
        get_num_axes()
            const;


        [[nodiscard]]
        unsigned
        get_num_balls()
            const;


        [[nodiscard]]
        unsigned
        get_num_hats()
            const;


        [[nodiscard]]
        unsigned
        get_num_buttons()
            const;


        [[nodiscard]]
        double
        get_axis(unsigned axis)
            const noexcept;


#if SDL_VERSION_ATLEAST(2, 0, 6)

        [[nodiscard]]
        std::optional<double>
        get_axis_initial_state(unsigned axis)
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 6)


        [[nodiscard]]
        hat_dir
        get_hat(unsigned hat)
            const noexcept;


        [[nodiscard]]
        vec2
        get_ball(unsigned ball)
            const;


        [[nodiscard]]
        bool
        get_button(unsigned button)
            const noexcept;


#if SDL_VERSION_ATLEAST(2, 0, 9)

        bool
        rumble(float low,
               float high,
               milliseconds duration)
            noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 9)


#if SDL_VERSION_ATLEAST(2, 0, 14)

        bool
        rumble_triggers(float left,
                        float right,
                        milliseconds duration)
            noexcept;


        [[nodiscard]]
        bool
        has_led()
            const noexcept;


        bool
        set_led(Uint8 red,
                Uint8 green,
                Uint8 blue)
            noexcept;

        bool
        set_led(color c)
            noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


#if SDL_VERSION_ATLEAST(2, 0, 18)

        [[nodiscard]]
        bool
        has_rumble()
            const noexcept;


        [[nodiscard]]
        bool
        has_rumble_on_triggers()
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 18)


#if SDL_VERSION_ATLEAST(2, 0, 16)

        bool
        send_effect(const void* buf,
                    std::size_t size)
            noexcept;

        template<typename T,
                 std::size_t E>
        inline
        bool
        send_effect(std::span<const T, E> payload)
            noexcept
        {
            return send_effect(payload.data(), payload.size_bytes());
        }

#endif // SDL_VERSION_ATLEAST(2, 0, 16)


#if SDL_VERSION_ATLEAST(2, 0, 4)

        [[nodiscard]]
        power_level
        get_power_level()
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 4)

    }; // struct device


#if SDL_VERSION_ATLEAST(2, 0, 14)

    [[nodiscard]]
    unsigned
    attach_virtual(type t,
                   unsigned axes,
                   unsigned buttons,
                   unsigned hats);


    void
    detach_virtual(unsigned index);


    [[nodiscard]]
    bool
    is_virtual(unsigned index);

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


#if SDL_VERSION_ATLEAST(2, 24, 0)

    struct virtual_descriptor : SDL_VirtualJoystickDesc {

        constexpr
        virtual_descriptor()
            noexcept :
            SDL_VirtualJoystickDesc{}
        {
            version = SDL_VIRTUAL_JOYSTICK_DESC_VERSION;
        }

    }; // struct virtual_descriptor


    [[nodiscard]]
    unsigned
    attach_virtual(const virtual_descriptor& desc);

#endif // SDL_VERSION_ATLEAST(2, 24, 0)


    struct guid_info {
        Uint16 vendor;
        Uint16 product;
        Uint16 version;
        Uint16 crc16;
    };


#if SDL_VERSION_ATLEAST(2, 26, 0)

    [[nodiscard]]
    guid_info
    parse(const guid& id)
        noexcept;

#endif // SDL_VERSION_ATLEAST(2, 26, 0)


    void
    update()
        noexcept;


    [[nodiscard]]
    bool
    get_event_polling();


    void
    set_event_polling(bool enabled);


    static constexpr double axis_dead_zone = 0.25;

} // namespace sdk::joystick

#endif
