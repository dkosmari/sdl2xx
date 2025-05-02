/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_GAME_CONTROLLERS_HPP
#define SDL2XX_GAME_CONTROLLERS_HPP

#include <chrono>
#include <expected>
#include <filesystem>
#include <iosfwd>
#include <optional>
#include <span>
#include <utility>

#include <SDL_gamecontroller.h>

#include "basic_wrapper.hpp"
#include "error.hpp"
#include "joysticks.hpp"
#include "sensors.hpp"
#include "string.hpp"
#include <vector>


namespace sdl::game_controllers {

    using std::chrono::milliseconds;

    using std::filesystem::path;

    using guid = joysticks::guid;


    enum class type {
        amazon_luna                  = SDL_CONTROLLER_TYPE_AMAZON_LUNA,
        google_stadia                = SDL_CONTROLLER_TYPE_GOOGLE_STADIA,
        nintendo_switch_joycon_left  = SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT,
        nintendo_switch_joycon_pair  = SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR,
        nintendo_switch_joycon_right = SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT,
        nintendo_switch_pro          = SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO,
        nvidia_shield                = SDL_CONTROLLER_TYPE_NVIDIA_SHIELD,
        ps3                          = SDL_CONTROLLER_TYPE_PS3,
        ps4                          = SDL_CONTROLLER_TYPE_PS4,
        ps5                          = SDL_CONTROLLER_TYPE_PS5,
        unknown                      = SDL_CONTROLLER_TYPE_UNKNOWN,
        virtual_controller           = SDL_CONTROLLER_TYPE_VIRTUAL,
        xbox_360                     = SDL_CONTROLLER_TYPE_XBOX360,
        xbox_one                     = SDL_CONTROLLER_TYPE_XBOXONE,
    };


    [[nodiscard]]
    constexpr
    type
    convert(SDL_GameControllerType t)
        noexcept
    {
        return static_cast<type>(t);
    }


    [[nodiscard]]
    constexpr
    SDL_GameControllerType
    convert(type t)
        noexcept
    {
        return static_cast<SDL_GameControllerType>(t);
    }


    enum class bind_type {
        axis   = SDL_CONTROLLER_BINDTYPE_AXIS,
        button = SDL_CONTROLLER_BINDTYPE_BUTTON,
        hat    = SDL_CONTROLLER_BINDTYPE_HAT,
        none   = SDL_CONTROLLER_BINDTYPE_NONE,
    };


    using button_bind = SDL_GameControllerButtonBind;


    enum class axis {
        invalid       = SDL_CONTROLLER_AXIS_INVALID,
        left_x        = SDL_CONTROLLER_AXIS_LEFTX,
        left_y        = SDL_CONTROLLER_AXIS_LEFTY,
        right_x       = SDL_CONTROLLER_AXIS_RIGHTX,
        right_y       = SDL_CONTROLLER_AXIS_RIGHTY,
        trigger_left  = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
        trigger_right = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
        max           = SDL_CONTROLLER_AXIS_MAX,
    };


    [[nodiscard]]
    constexpr
    axis
    convert(SDL_GameControllerAxis a)
        noexcept
    {
        return static_cast<axis>(a);
    }


    [[nodiscard]]
    constexpr
    SDL_GameControllerAxis
    convert(axis a)
        noexcept
    {
        return static_cast<SDL_GameControllerAxis>(a);
    }


    [[nodiscard]]
    axis
    to_axis(const char* str)
        noexcept;

    [[nodiscard]]
    inline
    axis
    to_axis(const concepts::string auto& str)
        noexcept
    {
        return to_axis(str.data());
    }


    [[nodiscard]]
    string
    to_string(axis a);


    std::istream&
    operator >>(std::istream& in,
                axis& a);


    std::ostream&
    operator <<(std::ostream& out,
                axis a);


    enum class button {
        a              = SDL_CONTROLLER_BUTTON_A,
        b              = SDL_CONTROLLER_BUTTON_B,
        back           = SDL_CONTROLLER_BUTTON_BACK,
        dpad_down      = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
        dpad_left      = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
        dpad_right     = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
        dpad_up        = SDL_CONTROLLER_BUTTON_DPAD_UP,
        guide          = SDL_CONTROLLER_BUTTON_GUIDE,
        invalid        = SDL_CONTROLLER_BUTTON_INVALID,
        left_shoulder  = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
        left_stick     = SDL_CONTROLLER_BUTTON_LEFTSTICK,
        max            = SDL_CONTROLLER_BUTTON_MAX,
        misc_1         = SDL_CONTROLLER_BUTTON_MISC1,
        paddle_1       = SDL_CONTROLLER_BUTTON_PADDLE1,
        paddle_2       = SDL_CONTROLLER_BUTTON_PADDLE2,
        paddle_3       = SDL_CONTROLLER_BUTTON_PADDLE3,
        paddle_4       = SDL_CONTROLLER_BUTTON_PADDLE4,
        right_shoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
        right_stick    = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
        start          = SDL_CONTROLLER_BUTTON_START,
        touchpad       = SDL_CONTROLLER_BUTTON_TOUCHPAD,
        x              = SDL_CONTROLLER_BUTTON_X,
        y              = SDL_CONTROLLER_BUTTON_Y,
    };


    [[nodiscard]]
    constexpr
    button
    convert(SDL_GameControllerButton b)
        noexcept
    {
        return static_cast<button>(b);
    }


    [[nodiscard]]
    constexpr
    SDL_GameControllerButton
    convert(button b)
        noexcept
    {
        return static_cast<SDL_GameControllerButton>(b);
    }


    [[nodiscard]]
    button
    to_button(const char* s)
        noexcept;

    [[nodiscard]]
    inline
    button
    to_button(const concepts::string auto& s)
        noexcept
    {
        return to_button(s.data());
    }


    [[nodiscard]]
    string
    to_string(button b);


    std::istream&
    operator >>(std::istream& in,
                button& b);


    std::ostream&
    operator <<(std::ostream& out,
                button b);


#if SDL_VERSION_ATLEAST(2, 0, 2)

    unsigned
    add_mappings(SDL_RWops* src,
                 bool close_src);

    unsigned
    add_mappings(const path& filename);

    std::expected<unsigned, error>
    try_add_mappings(SDL_RWops* src,
                     bool close_src)
        noexcept;

    std::expected<unsigned, error>
    try_add_mappings(const path& filename)
        noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 2)


    bool
    add_mapping(const char* str);

    inline
    bool
    add_mapping(const concepts::string auto& str)
    {
        return add_mapping(str.data());
    }


    std::expected<bool, error>
    try_add_mapping(const char* str)
        noexcept;

    inline
    std::expected<bool, error>
    try_add_mapping(const concepts::string auto& str)
        noexcept
    {
        return try_add_mapping(str.data());
    }


#if SDL_VERSION_ATLEAST(2, 0, 6)

    [[nodiscard]]
    unsigned
    get_num_mappings()
        noexcept;


    [[nodiscard]]
    string
    get_mapping(unsigned index);


    [[nodiscard]]
    std::expected<string, error>
    try_get_mapping(unsigned index)
        noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 6)


    [[nodiscard]]
    string
    get_mapping(const guid& id);


    [[nodiscard]]
    std::expected<string, error>
    try_get_mapping(const guid& id)
        noexcept;


#if SDL_VERSION_ATLEAST(2, 0, 9)

    [[nodiscard]]
    std::optional<string>
    get_mapping_for_device(unsigned index)
        noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 9)


    [[nodiscard]]
    bool
    is_game_controller(unsigned index)
        noexcept;


    [[nodiscard]]
    std::optional<string>
    get_name(unsigned index)
        noexcept;


#if SDL_VERSION_ATLEAST(2, 24, 0)

    [[nodiscard]]
    std::optional<string>
    get_path(unsigned index)
        noexcept;

#endif // SDL_VERSION_ATLEAST(2, 24, 0)


#if SDL_VERSION_ATLEAST(2, 0, 12)

    [[nodiscard]]
    type
    get_type(unsigned index)
        noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 12)

    // TODO: allow non-owning wrapper

    struct game_controller : basic_wrapper<SDL_GameController*> {

        // Inherit constructors.
        using basic_wrapper::basic_wrapper;


        explicit
        game_controller(unsigned index);


        /// Move constructor.
        game_controller(game_controller&& other)
            noexcept;

        ~game_controller()
            noexcept;

        /// Move assignment.
        game_controller&
        operator =(game_controller&& other)
            noexcept;


        void
        create(unsigned index);


        void
        destroy()
            noexcept;


        [[nodiscard]]
        string
        get_mapping()
            const;

        [[nodiscard]]
        std::expected<string, error>
        try_get_mapping()
            const;


        [[nodiscard]]
        std::optional<const char*>
        get_name()
            const noexcept;


#if SDL_VERSION_ATLEAST(2, 24, 0)

        [[nodiscard]]
        std::optional<const char*>
        get_path()
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 24, 0)


#if SDL_VERSION_ATLEAST(2, 0, 12)

        [[nodiscard]]
        type
        get_type()
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 12)


#if SDL_VERSION_ATLEAST(2, 0, 9)

        [[nodiscard]]
        int
        get_player()
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 9)


#if SDL_VERSION_ATLEAST(2, 0, 12)

        void
        set_player(int index)
            noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 12)


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
        std::optional<const char*>
        get_serial()
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


        [[nodiscard]]
        bool
        is_attached()
            const noexcept;


        [[nodiscard]]
        button_bind
        get_bind(axis a)
            const noexcept;


#if SDL_VERSION_ATLEAST(2, 0, 14)

        [[nodiscard]]
        bool
        has_axis(axis a)
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


        [[nodiscard]]
        Sint16
        get_axis(axis a)
            const noexcept;


        [[nodiscard]]
        button_bind
        get_bind(button b)
            const noexcept;


#if SDL_VERSION_ATLEAST(2, 0, 14)

        [[nodiscard]]
        bool
        has_button(button b)
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


        [[nodiscard]]
        bool
        get_button(button b)
            const noexcept;


#if SDL_VERSION_ATLEAST(2, 0, 14)

        [[nodiscard]]
        unsigned
        get_num_touchpads()
            const noexcept;


        [[nodiscard]]
        unsigned
        get_num_touchpad_fingers(unsigned touchpad)
            const noexcept;


        struct finger_state {
            Uint8 state;
            float x;
            float y;
            float pressure;
        };


        [[nodiscard]]
        finger_state
        get_touchpad_finger(unsigned touchpad,
                            unsigned finger)
            const;


        [[nodiscard]]
        bool
        has_sensor(sensors::type t)
            const noexcept;


        bool
        set_sensor(sensors::type t,
                   bool enabled)
            noexcept;


        [[nodiscard]]
        bool
        is_enabled(sensors::type t)
            const noexcept;


        [[nodiscard]]
        bool
        get_values(sensors::type t,
                   float* buf,
                   std::size_t count)
            noexcept;

        [[nodiscard]]
        bool
        get_values(sensors::type t,
                   std::span<float> buf)
            noexcept;

        [[nodiscard]]
        vector<float>
        get_values(sensors::type t,
                   std::size_t count);

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


#if SDL_VERSION_ATLEAST(2, 0, 16)

        [[nodiscard]]
        float
        get_sensor_rate(sensors::type t)
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 16)


#if SDL_VERSION_ATLEAST(2, 26, 0)

        [[nodiscard]]
        std::pair<bool, Uint64>
        get_values_timestamp(sensors::type t,
                             float* buf,
                             std::size_t count)
            noexcept;

        [[nodiscard]]
        std::pair<bool, Uint64>
        get_values_timestamp(sensors::type t,
                             std::span<float> buf)
            noexcept;


        [[nodiscard]]
        std::pair<vector<float>, Uint64>
        get_values_timestamp(sensors::type t,
                             std::size_t count);

#endif // SDL_VERSION_ATLEAST(2, 26, 0)


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

        template<typename T>
        inline
        bool
        send_effect(std::span<const T> payload)
            noexcept
        {
            return send_effect(payload.data(), payload.size_bytes());
        }

#endif // SDL_VERSION_ATLEAST(2, 0, 16)


#if SDL_VERSION_ATLEAST(2, 0, 18)

        [[nodiscard]]
        const char*
        get_apple_sf_symbol(button b)
            const noexcept;

        [[nodiscard]]
        const char*
        get_apple_sf_symbol(axis a)
            const noexcept;

#endif // SDL_VERSION_ATLEAST(2, 0, 18)



    }; // struct game_controller



    void
    set_state(bool enable)
        noexcept;


    [[nodiscard]]
    bool
    get_state()
        noexcept;


    void
    update()
        noexcept;


    using joysticks::axis_min;
    using joysticks::axis_max;
    using joysticks::axis_dead_zone;



#if SDL_VERSION_ATLEAST(2, 0, 4)

// TODO: wrap SDL_GameControllerFromInstanceID()

#endif // SDL_VERSION_ATLEAST(2, 0, 4)


#if SDL_VERSION_ATLEAST(2, 0, 12)

// TODO: wrap SDL_GameControllerFromPlayerIndex()

#endif // SDL_VERSION_ATLEAST(2, 0, 12)


// TODO: wrap SDL_GameControllerGetJoystick()


} // namespace sdl::game_controllers

#endif
