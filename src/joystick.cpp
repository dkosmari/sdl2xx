/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025-2026  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <ostream>

#include <SDL_events.h>         // SDL_QUERY

#include "sdl2xx/joystick.hpp"

#include "impl/remap.hpp"
#include "impl/value_or_throw.hpp"


using sdl::impl::remap::to_int;
using sdl::impl::remap::to_norm;
using sdl::impl::remap::to_uint16;
using sdl::impl::value_or_throw;


namespace sdl::joystick {

#if SDL_VERSION_ATLEAST(2, 0, 6)

    string
    to_string(type t)
    {
        switch (t) {
            case type::arcade_pad:
                return "arcade pad";
            case type::arcade_stick:
                return "arcade stick";
            case type::dance_pad:
                return "dance pad";
            case type::drum_kit:
                return "drum kit";
            case type::flight_stick:
                return "flight stick";
            case type::game_controller:
                return "game controller";
            case type::guitar:
                return "guitar";
            case type::throttle:
                return "throttle";
            case type::unknown:
                return "unknown";
            case type::wheel:
                return "wheel";
            default:
                return "invalid";
        }
    }

    std::ostream&
    operator <<(std::ostream& out,
                type t)
    {
        return out << to_string(t);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 6)


    string
    to_string(power_level lvl)
    {
        switch (lvl) {
            case power_level::empty:
                return "empty";
            case power_level::full:
                return "full";
            case power_level::low:
                return "low";
            case power_level::medium:
                return "medium";
            case power_level::unknown:
                return "unknown";
            case power_level::wired:
                return "wired";
            default:
                return "invalid";
        }
    }


    std::ostream&
    operator <<(std::ostream& out,
                power_level lvl)
    {
        return out << to_string(lvl);
    }



    string
    to_string(hat_dir dir)
    {
        switch (dir) {
            case hat_dir::centered:
                return "•";
            case hat_dir::left_down:
                return "↙";
            case hat_dir::left_up:
                return "↖";
            case hat_dir::right_down:
                return "↘";
            case hat_dir::right_up:
                return "↗";
            default:
            {
                // Note: SDL hats are not USB HID hats, you can have arbitrary
                // combinations of bits.
                string result;
                if (dir & hat_dir::down)
                    result += "↓";
                if (dir & hat_dir::left)
                    result += "←";
                if (dir & hat_dir::right)
                    result += "→";
                if (dir & hat_dir::up)
                    result += "↑";
                return result;
            }
        }
    }


    std::ostream&
    operator <<(std::ostream& out,
                hat_dir dir)
    {
        return out << to_string(dir);
    }


#if SDL_VERSION_ATLEAST(2, 0, 7)

    void
    lock()
        noexcept
    {
        SDL_LockJoysticks();
    }


    void
    unlock()
        noexcept
    {
        SDL_UnlockJoysticks();
    }


    locker::locker()
        noexcept
    {
        lock();
    }


    locker::locker(std::defer_lock_t)
        noexcept
    {}


    locker::~locker()
        noexcept
    {
        unlock();
    }


    locker::locker(locker&& other)
        noexcept :
        locked{other.locked}
    {
        other.locked = false;
    }


    locker&
    locker::operator =(locker&& other)
        noexcept
    {
        if (this != &other) {
            unlock();
            locked = other.locked;
            other.locked = false;
        }
        return *this;
    }


    void
    locker::lock()
        noexcept
    {
        if (!locked) {
            locked = true;
            sdl::joystick::lock();
        }
    }


    void
    locker::unlock()
        noexcept
    {
        if (locked) {
            sdl::joystick::unlock();
            locked = false;
        }
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 7)


    unsigned
    get_num_devices()
    {
        return value_or_throw(try_get_num_devices());
    }


    std::expected<unsigned, error>
    try_get_num_devices()
        noexcept
    {
        int result = SDL_NumJoysticks();
        if (result < 0)
            return std::unexpected{error{}};
        return static_cast<unsigned>(result);
    }


    std::string_view
    get_name(unsigned index)
    {
        return value_or_throw(try_get_name(index));
    }


    std::expected<std::string_view, error>
    try_get_name(unsigned index)
        noexcept
    {
        const char* name = SDL_JoystickNameForIndex(index);
        if (!name)
            return std::unexpected{error{}};
        return name;
    }


#if SDL_VERSION_ATLEAST(2, 24, 0)

    std::string_view
    get_path(unsigned index)
    {
        return value_or_throw(try_get_path(index));
    }


    std::expected<std::string_view, error>
    try_get_path(unsigned index)
        noexcept
    {
        const char* p = SDL_JoystickPathForIndex(index);
        if (!p)
            return std::unexpected{error{}};
        return p;
    }

#endif // SDL_VERSION_ATLEAST(2, 24, 0)


#if SDL_VERSION_ATLEAST(2, 0, 9)

    int
    get_player(unsigned index)
        noexcept
    {
        return SDL_JoystickGetDevicePlayerIndex(index);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 9)


    guid
    get_guid(unsigned index)
        noexcept
    {
        return SDL_JoystickGetDeviceGUID(index);
    }


#if SDL_VERSION_ATLEAST(2, 0, 6)

    Uint16
    get_vendor(unsigned index)
        noexcept
    {
        return SDL_JoystickGetDeviceVendor(index);
    }


    Uint16
    get_product(unsigned index)
        noexcept
    {
        return SDL_JoystickGetDeviceProduct(index);
    }


    Uint16
    get_version(unsigned index)
        noexcept
    {
        return SDL_JoystickGetDeviceProductVersion(index);
    }


    type
    get_type(unsigned index)
        noexcept
    {
        return convert(SDL_JoystickGetDeviceType(index));
    }


    instance_id
    get_id(unsigned index)
    {
        return value_or_throw(try_get_id(index));
    }


    std::expected<instance_id, error>
    try_get_id(unsigned index)
        noexcept
    {
        auto id = SDL_JoystickGetDeviceInstanceID(index);
        if (id == -1)
            return std::unexpected{error{}};
        return id;
    }


    unsigned
    get_index(instance_id id)
    {
        return value_or_throw(try_get_index(id));
    }


    std::expected<unsigned, error>
    try_get_index(instance_id id)
        noexcept
    {
        unsigned n = get_num_devices();
        for (unsigned i = 0; i < n; ++i)
            if (id == get_id(i))
                return i;
        return std::unexpected{error{"id not found"}};
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 6)


    device::device(unsigned index)
    {
        auto j = SDL_JoystickOpen(index);
        if (!j)
            throw error{};
        acquire(j);
    }


    device
    device::from_id(instance_id id)
    {
        return value_or_throw(try_from_id(id));
    }


    std::expected<device, error>
    device::try_from_id(instance_id id)
        noexcept
    {
        for (unsigned i = 0; i < get_num_devices(); ++i)
            if (id == joystick::get_id(i))
                return device{i};
        return std::unexpected{error{"invalid instance id"}};
    }


    device
    device::from_player(int player)
    {
        return value_or_throw(try_from_player(player));
    }


    std::expected<device, error>
    device::try_from_player(int player)
        noexcept
    {
        for (unsigned i = 0; i < get_num_devices(); ++i)
            if (player == joystick::get_player(i))
                return device{i};
        return std::unexpected{error{"invalid player index"}};
    }


    device::~device()
        noexcept
    {
        destroy();
    }


    void
    device::destroy()
        noexcept
    {
        if (is_valid())
            SDL_JoystickClose(release());
    }


#if SDL_VERSION_ATLEAST(2, 0, 14)

    void
    device::set_virtual_axis(unsigned axis,
                             double value)
    {
        return value_or_throw(try_set_virtual_axis(axis, value));
    }


    std::expected<void, error>
    device::try_set_virtual_axis(unsigned axis,
                                 double value)
        noexcept
    {
        auto ivalue = to_int(value, SDL_JOYSTICK_AXIS_MIN, SDL_JOYSTICK_AXIS_MAX);
        if (SDL_JoystickSetVirtualAxis(raw, axis, ivalue))
            return std::unexpected{error{}};
        return {};
    }


    void
    device::set_virtual_button(unsigned button,
                               bool value)
    {
        if (SDL_JoystickSetVirtualButton(raw, button, value))
            throw error{};
    }


    void
    device::set_virtual_hat(unsigned hat,
                            hat_dir value)
    {
        if (SDL_JoystickSetVirtualHat(raw, hat, convert(value)))
            throw error{};
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


    std::string_view
    device::get_name()
        const
    {
        return value_or_throw(try_get_name());
    }


    std::expected<std::string_view, error>
    device::try_get_name()
        const noexcept
    {
        auto name = SDL_JoystickName(raw);
        if (!name)
            return std::unexpected{error{}};
        return name;
    }


#if SDL_VERSION_ATLEAST(2, 24, 0)

    std::string_view
    device::get_path()
        const
    {
        return value_or_throw(try_get_path());
    }


    std::expected<std::string_view, error>
    device::try_get_path()
        const noexcept
    {
        auto result = SDL_JoystickPath(raw);
        if (!result)
            return std::unexpected{error{}};
        return result;
    }

#endif // SDL_VERSION_ATLEAST(2, 24, 0)


#if SDL_VERSION_ATLEAST(2, 0, 9)

    int
    device::get_player()
        const noexcept
    {
        return SDL_JoystickGetPlayerIndex(raw);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 9)


#if SDL_VERSION_ATLEAST(2, 0, 12)

    void
    device::set_player(int player)
        noexcept
    {
        SDL_JoystickSetPlayerIndex(raw, player);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 12)


    guid
    device::get_guid()
        const noexcept
    {
        return SDL_JoystickGetGUID(raw);
    }


#if SDL_VERSION_ATLEAST(2, 0, 6)

    Uint16
    device::get_vendor()
        const noexcept
    {
        return SDL_JoystickGetVendor(raw);
    }


    Uint16
    device::get_product()
        const noexcept
    {
        return SDL_JoystickGetProduct(raw);
    }


    Uint16
    device::get_version()
        const noexcept
    {
        return SDL_JoystickGetProductVersion(raw);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 6)


#if SDL_VERSION_ATLEAST(2, 24, 0)

    Uint16
    device::get_firmware()
        const noexcept
    {
        return SDL_JoystickGetFirmwareVersion(raw);
    }

#endif // SDL_VERSION_ATLEAST(2, 24, 0)


#if SDL_VERSION_ATLEAST(2, 0, 14)

    std::string_view
    device::get_serial()
        const
    {
        auto result = try_get_serial();
        if (!result)
            throw result.error();
        return *result;
    }


    std::expected<std::string_view, error>
    device::try_get_serial()
        const noexcept
    {
        auto serial = SDL_JoystickGetSerial(raw);
        if (!serial)
            return std::unexpected{error{}};
        return serial;
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


#if SDL_VERSION_ATLEAST(2, 0, 6)

    type
    device::get_type()
        const noexcept
    {
        return convert(SDL_JoystickGetType(raw));
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 6)


    bool
    device::is_attached()
        const noexcept
    {
        return SDL_JoystickGetAttached(raw);
    }


    instance_id
    device::get_id()
        const
    {
        return value_or_throw(try_get_id());
    }


    std::expected<instance_id, error>
    device::try_get_id()
        const noexcept
    {
        int result = SDL_JoystickInstanceID(raw);
        if (result < 0)
            return std::unexpected{error{}};
        return result;
    }


    unsigned
    device::get_num_axes()
        const
    {
        return value_or_throw(try_get_num_axes());
    }


    std::expected<unsigned, error>
    device::try_get_num_axes()
        const noexcept
    {
        int axes = SDL_JoystickNumAxes(raw);
        if (axes < 0)
            return std::unexpected{error{}};
        return axes;
    }


    unsigned
    device::get_num_balls()
        const
    {
        return value_or_throw(try_get_num_balls());
    }


    std::expected<unsigned, error>
    device::try_get_num_balls()
        const noexcept
    {
        int balls = SDL_JoystickNumBalls(raw);
        if (balls < 0)
            return std::unexpected{error{}};
        return balls;
    }


    unsigned
    device::get_num_hats()
        const
    {
        return value_or_throw(try_get_num_hats());
    }


    std::expected<unsigned, error>
    device::try_get_num_hats()
        const noexcept
    {
        int hats = SDL_JoystickNumHats(raw);
        if (hats < 0)
            return std::unexpected{error{}};
        return hats;
    }


    unsigned
    device::get_num_buttons()
        const
    {
        return value_or_throw(try_get_num_buttons());
    }


    std::expected<unsigned, error>
    device::try_get_num_buttons()
        const noexcept
    {
        int buttons = SDL_JoystickNumButtons(raw);
        if (buttons < 0)
            return std::unexpected{error{}};
        return buttons;
    }


    double
    device::get_axis(unsigned axis)
        const noexcept
    {
        return to_norm(SDL_JoystickGetAxis(raw, axis),
                       SDL_JOYSTICK_AXIS_MIN,
                       SDL_JOYSTICK_AXIS_MAX);
    }


#if SDL_VERSION_ATLEAST(2, 0, 6)

    std::optional<double>
    device::get_axis_initial_state(unsigned axis)
        const noexcept
    {
        Sint16 state;
        if (!SDL_JoystickGetAxisInitialState(raw, axis, &state))
            return {};
        return to_norm(state,
                       SDL_JOYSTICK_AXIS_MIN,
                       SDL_JOYSTICK_AXIS_MAX);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 6)


    hat_dir
    device::get_hat(unsigned hat)
        const noexcept
    {
        return static_cast<hat_dir>(SDL_JoystickGetHat(raw, hat));
    }


    vec2
    device::get_ball(unsigned ball)
        const
    {
        return value_or_throw(try_get_ball(ball));
    }


    std::expected<vec2, error>
    device::try_get_ball(unsigned ball)
        const noexcept
    {
        vec2 result;
        if (SDL_JoystickGetBall(raw, ball, &result.x, &result.y) < 0)
            return std::unexpected{error{}};
        return result;
    }


    bool
    device::get_button(unsigned button)
        const noexcept
    {
        return SDL_JoystickGetButton(raw, button);
    }


#if SDL_VERSION_ATLEAST(2, 0, 9)

    bool
    device::rumble(float low,
                   float high,
                   std::chrono::milliseconds duration)
        noexcept
    {
        return !SDL_JoystickRumble(raw,
                                   to_uint16(low),
                                   to_uint16(high),
                                   duration.count());
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 9)


#if SDL_VERSION_ATLEAST(2, 0, 14)

    bool
    device::rumble_triggers(float left,
                            float right,
                            std::chrono::milliseconds duration)
        noexcept
    {
        return !SDL_JoystickRumbleTriggers(raw,
                                           to_uint16(left),
                                           to_uint16(right),
                                           duration.count());
    }


    bool
    device::has_led()
        const noexcept
    {
        return SDL_JoystickHasLED(raw);
    }


    bool
    device::set_led(Uint8 red,
                    Uint8 green,
                    Uint8 blue)
        noexcept
    {
        return !SDL_JoystickSetLED(raw, red, green, blue);
    }


    bool
    device::set_led(color c)
        noexcept
    {
        return set_led(c.r, c.g, c.b);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


#if SDL_VERSION_ATLEAST(2, 0, 18)

    bool
    device::has_rumble()
        const noexcept
    {
        return SDL_JoystickHasRumble(raw);
    }


    bool
    device::has_rumble_on_triggers()
        const noexcept
    {
        return SDL_JoystickHasRumbleTriggers(raw);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 18)


#if SDL_VERSION_ATLEAST(2, 0, 16)

    bool
    device::send_effect(const void* payload,
                        std::size_t size)
        noexcept
    {
        return !SDL_JoystickSendEffect(raw, payload, size);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 16)


#if SDL_VERSION_ATLEAST(2, 0, 4)

    power_level
    device::get_power_level()
        const noexcept
    {
        return convert(SDL_JoystickCurrentPowerLevel(raw));
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 4)


#if SDL_VERSION_ATLEAST(2, 0, 14)

    unsigned
    attach_virtual(type t,
                   unsigned axes,
                   unsigned buttons,
                   unsigned hats)
    {
        return value_or_throw(try_attach_virtual(t, axes, buttons, hats));
    }


    std::expected<unsigned, error>
    try_attach_virtual(type t,
                       unsigned axes,
                       unsigned buttons,
                       unsigned hats)
        noexcept
    {
        int idx = SDL_JoystickAttachVirtual(convert(t),
                                            axes,
                                            buttons,
                                            hats);
        if (idx < 0)
            return std::unexpected{error{}};
        return idx;
    }


    void
    detach_virtual(unsigned index)
    {
        return value_or_throw(try_detach_virtual(index));
    }


    std::expected<void, error>
    try_detach_virtual(unsigned index)
        noexcept
    {
        if (SDL_JoystickDetachVirtual(index))
            return std::unexpected{error{}};
        return {};
    }


    bool
    is_virtual(unsigned index)
        noexcept
    {
        return SDL_JoystickIsVirtual(index);
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


#if SDL_VERSION_ATLEAST(2, 24, 0)

    unsigned
    attach_virtual(const virtual_descriptor& desc)
    {
        return value_or_throw(try_attach_virtual(desc));
    }


    std::expected<unsigned, error>
    try_attach_virtual(const virtual_descriptor& desc)
        noexcept
    {
        int idx = SDL_JoystickAttachVirtualEx(&desc);
        if (idx < 0)
            return std::unexpected{error{}};
        return idx;
    }

#endif // SDL_VERSION_ATLEAST(2, 24, 0)


#if SDL_VERSION_ATLEAST(2, 26, 0)

    guid_info
    parse(const guid& id)
        noexcept
    {
        guid_info result;
        SDL_GetJoystickGUIDInfo(id,
                                &result.vendor,
                                &result.product,
                                &result.version,
                                &result.crc16);
        return result;
    }

#endif // SDL_VERSION_ATLEAST(2, 26, 0)

    void
    update()
        noexcept
    {
        SDL_JoystickUpdate();
    }


    bool
    get_event_polling()
    {
        return value_or_throw(try_get_event_polling());
    }


    std::expected<bool, error>
    try_get_event_polling()
        noexcept
    {
        int result = SDL_JoystickEventState(SDL_QUERY);
        if (result < 0)
            return std::unexpected{error{}};
        return result;
    }


    void
    set_event_polling(bool enabled)
    {
        return value_or_throw(try_set_event_polling(enabled));
    }


    std::expected<void, error>
    try_set_event_polling(bool enabled)
        noexcept
    {
        if (SDL_JoystickEventState(enabled ? SDL_ENABLE : SDL_DISABLE) < 0)
            return std::unexpected{error{}};
        return {};
    }

} // namespace sdl::joystick
