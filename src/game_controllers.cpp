/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <istream>
#include <ostream>
#include <utility>

#include <SDL_events.h>         // SDL_QUERY

#include "game_controllers.hpp"
#include "unique_ptr.hpp"
#include "impl/utils.hpp"


using std::expected;
using std::unexpected;


namespace sdl::game_controllers {

    using impl::utils::map_to_uint16;


    axis
    to_axis(const char* str)
        noexcept
    {
        return convert(SDL_GameControllerGetAxisFromString(str));
    }


    string
    to_string(axis a)
    {
        return SDL_GameControllerGetStringForAxis(convert(a));
    }


    std::istream&
    operator >>(std::istream& in,
                axis& a)
    {
        string s;
        if (in >> s)
            a = to_axis(s);
        return in;
    }


    std::ostream&
    operator <<(std::ostream& out,
                axis a)
    {
        return out << SDL_GameControllerGetStringForAxis(convert(a));
    }


    button
    to_button(const char* s)
        noexcept
    {
        return convert(SDL_GameControllerGetButtonFromString(s));
    }


    string
    to_string(button b)
    {
        return SDL_GameControllerGetStringForButton(convert(b));
    }


    std::istream&
    operator >>(std::istream& in,
                button& b)
    {
        string s;
        if (in >> s)
            b = to_button(s);
        return in;
    }


    std::ostream&
    operator <<(std::ostream& out,
                button b)
    {
        return out << SDL_GameControllerGetStringForButton(convert(b));
    }


#if SDL_VERSION_ATLEAST(2, 0, 2)

    unsigned
    add_mappings(SDL_RWops* src,
                 bool close_src)
    {
        auto result = try_add_mappings(src, close_src);
        if (!result)
            throw result.error();
        return *result;
    }


    unsigned
    add_mappings(const path& filename)
    {
        auto result = try_add_mappings(filename);
        if (!result)
            throw result.error();
        return *result;
    }


    expected<unsigned, error>
    try_add_mappings(SDL_RWops* src,
                     bool close_src)
        noexcept
    {
        int r = SDL_GameControllerAddMappingsFromRW(src, close_src);
        if (r < 0)
            return unexpected{error{}};
        return r;
    }


    expected<unsigned, error>
    try_add_mappings(const path& filename)
        noexcept
    {
        int r = SDL_GameControllerAddMappingsFromFile(filename.c_str());
        if (r < 0)
            return unexpected{error{}};
        return r;
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 2)


    bool
    add_mapping(const char* str)
    {
        auto result = try_add_mapping(str);
        if (!result)
            throw result.error();
        return *result;
    }


    expected<bool, error>
    try_add_mapping(const char* str)
        noexcept
    {
        int r = SDL_GameControllerAddMapping(str);
        if (r < 0)
            return unexpected{error{}};
        return r;
    }


#if SDL_VERSION_ATLEAST(2, 0, 6)

    [[nodiscard]]
    unsigned
    get_num_mappings()
        noexcept
    {
        return SDL_GameControllerNumMappings();
    }


    string
    get_mapping(unsigned index)
    {
        auto result = try_get_mapping(index);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    std::expected<string, error>
    try_get_mapping(unsigned index)
        noexcept
    {
        unique_ptr<char> str{SDL_GameControllerMappingForIndex(index)};
        if (!str)
            return unexpected{error{}};
        try {
            return string(str.get());
        }
        catch (std::exception& e) {
            return unexpected{error{e}};
        }
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 6)


    string
    get_mapping(const guid& id)
    {
        auto result = try_get_mapping(id);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    std::expected<string, error>
    try_get_mapping(const guid& id)
        noexcept
    {
        unique_ptr<char> str{SDL_GameControllerMappingForGUID(id)};
        if (!str)
            return unexpected{error{}};
        try {
            return string(str.get());
        }
        catch (std::exception& e) {
            return unexpected{error{e}};
        }
    }


#if SDL_VERSION_ATLEAST(2, 0, 9)

    std::optional<string>
    get_mapping_for_device(unsigned index)
        noexcept
    {
        unique_ptr<char> str{SDL_GameControllerMappingForDeviceIndex(index)};
        if (!str)
            return {};
        try {
            return string(str.get());
        }
        catch (std::exception& e) {
            return {};
        }
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 9)



    bool
    is_game_controller(unsigned index)
        noexcept
    {
        return SDL_IsGameController(index);
    }


    [[nodiscard]]
    std::optional<string>
    get_name(unsigned index)
        noexcept
    {
        const char* name = SDL_GameControllerNameForIndex(index);
        if (!name)
            return {};
        return string(name);
    }


#if SDL_VERSION_ATLEAST(2, 24, 0)

    std::optional<string>
    get_path(unsigned index)
        noexcept
    {
        const char* p = SDL_GameControllerPathForIndex(index);
        if (!p)
            return {};
        return string(p);
    }

#endif // SDL_VERSION_ATLEAST(2, 24, 0)


#if SDL_VERSION_ATLEAST(2, 0, 12)

    type
    get_type(unsigned index)
        noexcept
    {
        return convert(SDL_GameControllerTypeForIndex(index));
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 12)


    game_controller::game_controller(unsigned index)
    {
        create(index);
    }


#if SDL_VERSION_ATLEAST(2, 0, 4)

    game_controller
    game_controller::from_id(instance_id id)
    {
        auto raw = SDL_GameControllerFromInstanceID(id);
        if (!raw)
            throw error{};
        game_controller result;
        result.acquire(raw, false);
        return result;
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 4)


#if SDL_VERSION_ATLEAST(2, 0, 12)

    game_controller
    game_controller::from_player(int index)
    {
        // TODO: wrap SDL_GameControllerFromPlayerIndex()
        auto raw = SDL_GameControllerFromPlayerIndex(index);
        if (!raw)
            throw error{};
        game_controller result;
        result.acquire(raw, false);
        return result;
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 12)


    game_controller::~game_controller()
        noexcept
    {
        destroy();
    }


    void
    game_controller::create(unsigned index)
    {
        auto ptr = SDL_GameControllerOpen(index);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    game_controller::destroy()
        noexcept
    {
        if (raw) {
            auto [old_raw, old_owner] = release();
            if (old_owner)
                SDL_GameControllerClose(old_raw);
        }
    }


    string
    game_controller::get_mapping()
        const
    {
        auto result = try_get_mapping();
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    std::expected<string, error>
    game_controller::try_get_mapping()
        const
    {
        unique_ptr<char> str{SDL_GameControllerMapping(raw)};
        if(!str)
            return unexpected{error{}};
        try {
            return string(str.get());
        }
        catch (std::exception& e) {
            return unexpected{error{e}};
        }
    }


    std::optional<const char*>
    game_controller::get_name()
        const noexcept
    {
        const char* name = SDL_GameControllerName(raw);
        if (!name)
            return {};
        return name;
    }


#if SDL_VERSION_ATLEAST(2, 24, 0)

    std::optional<const char*>
    game_controller::get_path()
        const noexcept
    {
        const char* p = SDL_GameControllerPath(raw);
        if (!p)
            return {};
        return p;
    }

#endif // SDL_VERSION_ATLEAST(2, 24, 0)


#if SDL_VERSION_ATLEAST(2, 0, 12)

    type
    game_controller::get_type()
        const noexcept
    {
        return convert(SDL_GameControllerGetType(raw));
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 12)


#if SDL_VERSION_ATLEAST(2, 0, 9)

    int
    game_controller::get_player()
        const noexcept
    {
        return SDL_GameControllerGetPlayerIndex(raw);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 9)


#if SDL_VERSION_ATLEAST(2, 0, 12)

    void
    game_controller::set_player(int index)
        noexcept
    {
        SDL_GameControllerSetPlayerIndex(raw, index);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 12)


#if SDL_VERSION_ATLEAST(2, 0, 6)

    Uint16
    game_controller::get_vendor()
        const noexcept
    {
        return SDL_GameControllerGetVendor(raw);
    }


    Uint16
    game_controller::get_product()
        const noexcept
    {
        return SDL_GameControllerGetProduct(raw);
    }


    Uint16
    game_controller::get_version()
        const noexcept
    {
        return SDL_GameControllerGetProductVersion(raw);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 6)


#if SDL_VERSION_ATLEAST(2, 24, 0)

    Uint16
    game_controller::get_firmware()
        const noexcept
    {
        return SDL_GameControllerGetFirmwareVersion(raw);
    }

#endif // SDL_VERSION_ATLEAST(2, 24, 0)


#if SDL_VERSION_ATLEAST(2, 0, 14)

    std::optional<const char*>
    game_controller::get_serial()
        const noexcept
    {
        const char* serial = SDL_GameControllerGetSerial(raw);
        if (!serial)
            return {};
        return serial;
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


    bool
    game_controller::is_attached()
        const noexcept
    {
        return SDL_GameControllerGetAttached(raw);
    }


    button_bind
    game_controller::get_bind(axis a)
        const noexcept
    {
        return SDL_GameControllerGetBindForAxis(raw, convert(a));
    }


#if SDL_VERSION_ATLEAST(2, 0, 14)

    bool
    game_controller::has_axis(axis a)
        const noexcept
    {
        return SDL_GameControllerHasAxis(raw, convert(a));
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


    Sint16
    game_controller::get_axis(axis a)
        const noexcept
    {
        return SDL_GameControllerGetAxis(raw, convert(a));
    }


    button_bind
    game_controller::get_bind(button b)
        const noexcept
    {
        return SDL_GameControllerGetBindForButton(raw, convert(b));
    }


#if SDL_VERSION_ATLEAST(2, 0, 14)

    bool
    game_controller::has_button(button b)
        const noexcept
    {
        return SDL_GameControllerHasButton(raw, convert(b));
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


    bool
    game_controller::get_button(button b)
        const noexcept
    {
        return SDL_GameControllerGetButton(raw, convert(b));
    }


#if SDL_VERSION_ATLEAST(2, 0, 14)

    unsigned
    game_controller::get_num_touchpads()
        const noexcept
    {
        return SDL_GameControllerGetNumTouchpads(raw);
    }


    unsigned
    game_controller::get_num_touchpad_fingers(unsigned touchpad)
        const noexcept
    {
        return SDL_GameControllerGetNumTouchpadFingers(raw, touchpad);
    }


    game_controller::finger_state
    game_controller::get_touchpad_finger(unsigned touchpad,
                                         unsigned finger)
        const
    {
        finger_state result;
        if (SDL_GameControllerGetTouchpadFinger(raw,
                                                touchpad,
                                                finger,
                                                &result.state,
                                                &result.x,
                                                &result.y,
                                                &result.pressure) < 0)
            throw error{};
        return result;
    }


    bool
    game_controller::has_sensor(sensors::type t)
        const noexcept
    {
        return SDL_GameControllerHasSensor(raw, convert(t));
    }


    bool
    game_controller::set_sensor(sensors::type t,
                                bool enabled)
        noexcept
    {
        return !SDL_GameControllerSetSensorEnabled(raw,
                                                   convert(t),
                                                   enabled ? SDL_TRUE : SDL_FALSE);
    }


    bool
    game_controller::is_enabled(sensors::type t)
        const noexcept
    {
        return SDL_GameControllerIsSensorEnabled(raw, convert(t));
    }


    bool
    game_controller::get_values(sensors::type t,
                                float* buf,
                                std::size_t count)
        noexcept
    {
        return !SDL_GameControllerGetSensorData(raw, convert(t), buf, count);
    }


    bool
    game_controller::get_values(sensors::type t,
                                std::span<float> buf)
        noexcept
    {
        return get_values(t, buf.data(), buf.size());
    }


    vector<float>
    game_controller::get_values(sensors::type t,
                                std::size_t count)
    {
        vector<float> values(count);
        if (!get_values(t, values))
            return {};
        return values;
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


#if SDL_VERSION_ATLEAST(2, 0, 16)

    float
    game_controller::get_sensor_rate(sensors::type t)
        const noexcept
    {
        return SDL_GameControllerGetSensorDataRate(raw, convert(t));
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 16)


#if SDL_VERSION_ATLEAST(2, 26, 0)

    std::pair<bool, Uint64>
    game_controller::get_values_timestamp(sensors::type t,
                                          float* buf,
                                          std::size_t count)
        noexcept
    {
        Uint64 timestamp;
        int r = SDL_GameControllerGetSensorDataWithTimestamp(raw,
                                                             convert(t),
                                                             &timestamp,
                                                             buf,
                                                             count);
        return {
            r == 0,
            timestamp
        };
    }


    std::pair<bool, Uint64>
    game_controller::get_values_timestamp(sensors::type t,
                                          std::span<float> buf)
        noexcept
    {
        return get_values_timestamp(t, buf.data(), buf.size());
    }


    std::pair<vector<float>, Uint64>
    game_controller::get_values_timestamp(sensors::type t,
                                          std::size_t count)
    {
        vector<float> values(count);
        auto [success, timestamp] = get_values_timestamp(t, values);
        if (!success)
            return {{}, {}};
        return {
            std::move(values),
            timestamp
        };
    }

#endif // SDL_VERSION_ATLEAST(2, 26, 0)



#if SDL_VERSION_ATLEAST(2, 0, 9)

    bool
    game_controller::rumble(float low,
                            float high,
                            milliseconds duration)
        noexcept
    {
        return !SDL_GameControllerRumble(raw,
                                         map_to_uint16(low),
                                         map_to_uint16(high),
                                         duration.count());
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 9)


#if SDL_VERSION_ATLEAST(2, 0, 14)

    bool
    game_controller::rumble_triggers(float left,
                                     float right,
                                     milliseconds duration)
        noexcept
    {
        return !SDL_GameControllerRumbleTriggers(raw,
                                                 map_to_uint16(left),
                                                 map_to_uint16(right),
                                                 duration.count());
    }


    bool
    game_controller::has_led()
        const noexcept
    {
        return SDL_GameControllerHasLED(raw);
    }


    bool
    game_controller::set_led(Uint8 red,
                             Uint8 green,
                             Uint8 blue)
        noexcept
    {
        return !SDL_GameControllerSetLED(raw, red, green, blue);
    }


    bool
    game_controller::set_led(color c)
        noexcept
    {
        return set_led(c.r, c.g, c.b);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


#if SDL_VERSION_ATLEAST(2, 0, 18)

    bool
    game_controller::has_rumble()
        const noexcept
    {
        return SDL_GameControllerHasRumble(raw);
    }


    bool
    game_controller::has_rumble_on_triggers()
        const noexcept
    {
        return SDL_GameControllerHasRumbleTriggers(raw);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 18)


#if SDL_VERSION_ATLEAST(2, 0, 16)

    bool
    game_controller::send_effect(const void* buf,
                                 std::size_t size)
        noexcept
    {
        return !SDL_GameControllerSendEffect(raw, buf, size);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 16)


#if SDL_VERSION_ATLEAST(2, 0, 18)

    const char*
    game_controller::get_apple_sf_symbol(button b)
        const noexcept
    {
        return SDL_GameControllerGetAppleSFSymbolsNameForButton(raw, convert(b));
    }


    const char*
    game_controller::get_apple_sf_symbol(axis a)
        const noexcept
    {
        return SDL_GameControllerGetAppleSFSymbolsNameForAxis(raw, convert(a));
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 18)


    joysticks::joystick
    game_controller::get_joystick()
        const
    {
        auto j = SDL_GameControllerGetJoystick(raw);
        if (!j)
            throw error{};
        joysticks::joystick result;
        result.acquire(j);
        return result;
    }


    void
    set_state(bool enable)
        noexcept
    {
        SDL_GameControllerEventState(enable ? SDL_ENABLE : SDL_DISABLE);
    }


    bool
    get_state()
        noexcept
    {
        return SDL_GameControllerEventState(SDL_QUERY);
    }


    void
    update()
        noexcept
    {
        SDL_GameControllerUpdate();
    }

} // namespace sdl::game_controllers
