/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_EVENTS_HPP
#define SDL2XX_EVENTS_HPP

#include <chrono>
#include <optional>
#include <span>
#include <utility>

#include <SDL_events.h>


namespace sdl::events {


    enum class type : Uint32 {
        audio_device_added         = SDL_AUDIODEVICEADDED,
        audio_device_removed       = SDL_AUDIODEVICEREMOVED,
        clipboard                  = SDL_CLIPBOARDUPDATE,
        controller_axis            = SDL_CONTROLLERAXISMOTION,
        controller_device_added    = SDL_CONTROLLERDEVICEADDED,
        controller_device_remapped = SDL_CONTROLLERDEVICEREMAPPED,
        controller_device_removed  = SDL_CONTROLLERDEVICEREMOVED,
        controller_down            = SDL_CONTROLLERBUTTONDOWN,
        controller_sensor          = SDL_CONTROLLERSENSORUPDATE,
        controller_touch_down      = SDL_CONTROLLERTOUCHPADDOWN,
        controller_touch_motion    = SDL_CONTROLLERTOUCHPADMOTION,
        controller_touch_up        = SDL_CONTROLLERTOUCHPADUP,
        controller_up              = SDL_CONTROLLERBUTTONUP,
        did_enter_background       = SDL_APP_DIDENTERBACKGROUND,
        did_enter_foreground       = SDL_APP_DIDENTERFOREGROUND,
        display                    = SDL_DISPLAYEVENT,
        dollar_gesture             = SDL_DOLLARGESTURE,
        dollar_record              = SDL_DOLLARRECORD,
        drop_begin                 = SDL_DROPBEGIN,
        drop_complete              = SDL_DROPCOMPLETE,
        drop_file                  = SDL_DROPFILE,
        drop_text                  = SDL_DROPTEXT,
        finger_down                = SDL_FINGERDOWN,
        finger_motion              = SDL_FINGERMOTION,
        finger_up                  = SDL_FINGERUP,
        first                      = SDL_FIRSTEVENT,
        joy_axis                   = SDL_JOYAXISMOTION,
        joy_ball                   = SDL_JOYBALLMOTION,
        joy_battery                = SDL_JOYBATTERYUPDATED,
        joy_device_added           = SDL_JOYDEVICEADDED,
        joy_device_removed         = SDL_JOYDEVICEREMOVED,
        joy_down                   = SDL_JOYBUTTONDOWN,
        joy_hat                    = SDL_JOYHATMOTION,
        joy_up                     = SDL_JOYBUTTONUP,
        key_down                   = SDL_KEYDOWN,
        key_up                     = SDL_KEYUP,
        keymap_changed             = SDL_KEYMAPCHANGED,
        last                       = SDL_LASTEVENT,
        locale_changed             = SDL_LOCALECHANGED,
        low_memory                 = SDL_APP_LOWMEMORY,
        mouse_down                 = SDL_MOUSEBUTTONDOWN,
        mouse_motion               = SDL_MOUSEMOTION,
        mouse_up                   = SDL_MOUSEBUTTONUP,
        mouse_wheel                = SDL_MOUSEWHEEL,
        multi_gesture              = SDL_MULTIGESTURE,
        poll_sentinel              = SDL_POLLSENTINEL,
        quit                       = SDL_QUIT,
        render_device_reset        = SDL_RENDER_DEVICE_RESET,
        render_targets_reset       = SDL_RENDER_TARGETS_RESET,
        sensor                     = SDL_SENSORUPDATE,
        sys_wm                     = SDL_SYSWMEVENT,
        terminating                = SDL_APP_TERMINATING,
        text_editing               = SDL_TEXTEDITING,
        text_editing_ext           = SDL_TEXTEDITING_EXT,
        text_input                 = SDL_TEXTINPUT,
        user                       = SDL_USEREVENT,
        will_enter_background      = SDL_APP_WILLENTERBACKGROUND,
        will_enter_foreground      = SDL_APP_WILLENTERFOREGROUND,
        window                     = SDL_WINDOWEVENT,
    };


    [[nodiscard]]
    constexpr
    Uint32
    convert(type t)
        noexcept
    {
        return static_cast<Uint32>(t);
    }


#if 0
    [[nodiscard]]
    constexpr
    bool
    operator ==(type a, Uint32 b)
        noexcept
    {
        return convert(a) == b;
    }


    [[nodiscard]]
    constexpr
    bool
    operator ==(Uint32 a, type b)
        noexcept
    {
        return a == convert(b);
    }
#endif


    // TODO: to_string(type t)


    using audio_device        = SDL_AudioDeviceEvent;
    using controller_axis     = SDL_ControllerAxisEvent;
    using controller_button   = SDL_ControllerButtonEvent;
    using controller_device   = SDL_ControllerDeviceEvent;
    using controller_sensor   = SDL_ControllerSensorEvent;
    using controller_touchpad = SDL_ControllerTouchpadEvent;
    using display             = SDL_DisplayEvent;
    using dollar_gesture      = SDL_DollarGestureEvent;
    using drop                = SDL_DropEvent;
    using event               = SDL_Event;
    using joy_axis            = SDL_JoyAxisEvent;
    using joy_ball            = SDL_JoyBallEvent;
    using joy_battery         = SDL_JoyBatteryEvent;
    using joy_button          = SDL_JoyButtonEvent;
    using joy_device          = SDL_JoyDeviceEvent;
    using joy_hat             = SDL_JoyHatEvent;
    using keyboard            = SDL_KeyboardEvent;
    using mouse_button        = SDL_MouseButtonEvent;
    using mouse_motion        = SDL_MouseMotionEvent;
    using mouse_wheel         = SDL_MouseWheelEvent;
    using multi_gesture       = SDL_MultiGestureEvent;
    using quit                = SDL_QuitEvent;
    using sensor              = SDL_SensorEvent;
    using sys_wm              = SDL_SysWMEvent;
    using text_editing        = SDL_TextEditingEvent;
    using text_editing_ext    = SDL_TextEditingExtEvent;
    using text_input          = SDL_TextInputEvent;
    using touch_finger        = SDL_TouchFingerEvent;
    using user                = SDL_UserEvent;
    using window              = SDL_WindowEvent;


    void
    pump()
        noexcept;


    enum class action {
        add  = SDL_ADDEVENT,
        peek = SDL_PEEKEVENT,
        get  = SDL_GETEVENT,
    };


    [[nodiscard]]
    unsigned
    peep(std::span<event> events,
         action act,
         Uint32 min = SDL_FIRSTEVENT,
         Uint32 max = SDL_LASTEVENT);


    [[nodiscard]]
    unsigned
    add(std::span<event> events);


    [[nodiscard]]
    unsigned
    peek(std::span<event> events,
         Uint32 min = SDL_FIRSTEVENT,
         Uint32 max = SDL_LASTEVENT);

    [[nodiscard]]
    unsigned
    get(std::span<event> events,
        Uint32 min = SDL_FIRSTEVENT,
        Uint32 max = SDL_LASTEVENT);


    [[nodiscard]]
    bool
    has_event(Uint32 t)
        noexcept;


    [[nodiscard]]
    bool
    has_event(Uint32 min,
              Uint32 max)
        noexcept;


    void
    flush(Uint32 t)
        noexcept;

    void
    flush(Uint32 min,
          Uint32 max)
        noexcept;


    void
    remove(Uint32 t)
        noexcept;

    void
    remove(Uint32 min,
           Uint32 max)
        noexcept;


    [[nodiscard]]
    bool
    poll(event& e)
        noexcept;

    [[nodiscard]]
    std::optional<event>
    poll()
        noexcept;


    void
    wait(event& e);

    event
    wait();


    void
    wait(event& e,
         std::chrono::milliseconds timeout);

    event
    wait(std::chrono::milliseconds timeout);

    bool
    try_wait(event& e,
             std::chrono::milliseconds timeout)
        noexcept;

    std::optional<event>
    try_wait(std::chrono::milliseconds timeout)
        noexcept;


    bool
    push(event& e);


    void
    set_filter(SDL_EventFilter filter,
               void* ctx = nullptr)
        noexcept;

    void
    reset_filter()
        noexcept;

    [[nodiscard]]
    std::pair<SDL_EventFilter, void*>
    get_filter()
        noexcept;


    void
    add_watch(SDL_EventFilter watch,
              void* ctx = nullptr)
        noexcept;

    void
    remove_watch(SDL_EventFilter watch,
                 void* ctx = nullptr)
        noexcept;


    void
    filter_now(SDL_EventFilter filter,
               void* ctx = nullptr)
        noexcept;


    bool
    set_enabled(Uint32 t,
                bool enabled)
        noexcept;


    bool
    enable(Uint32 t)
        noexcept;


    bool
    disable(Uint32 t)
        noexcept;


    [[nodiscard]]
    bool
    is_enabled(Uint32 t)
        noexcept;


    [[nodiscard]]
    std::optional<Uint32>
    allocate(unsigned count)
        noexcept;


} // namespace sdl::events

#endif
