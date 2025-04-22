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
    using os                  = SDL_OSEvent;
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
         Uint32 min_type = SDL_FIRSTEVENT,
         Uint32 max_type = SDL_LASTEVENT);


    [[nodiscard]]
    unsigned
    add(std::span<event> events);


    [[nodiscard]]
    unsigned
    peek(std::span<event> events,
         Uint32 min_type = SDL_FIRSTEVENT,
         Uint32 max_type = SDL_LASTEVENT);

    [[nodiscard]]
    unsigned
    get(std::span<event> events,
        Uint32 min_type = SDL_FIRSTEVENT,
        Uint32 max_type = SDL_LASTEVENT);


    [[nodiscard]]
    bool
    has_event(Uint32 type)
        noexcept;

    [[nodiscard]]
    bool
    has_event(Uint32 min_type,
              Uint32 max_type)
        noexcept;


    void
    flush(Uint32 type)
        noexcept;

    void
    remove(Uint32 type)
        noexcept;

    void
    flush(Uint32 min_type,
          Uint32 max_type)
        noexcept;

    void
    remove(Uint32 min_type,
           Uint32 max_type)
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


    enum class state {
        query   = SDL_QUERY,
        ignore  = SDL_IGNORE,
        disable = SDL_DISABLE,
        enable  = SDL_ENABLE,
    };

    bool
    set_state(Uint32 type,
              state st)
        noexcept;

    [[nodiscard]]
    bool
    get_state(Uint32 type)
        noexcept;


    [[nodiscard]]
    std::optional<Uint32>
    allocate(unsigned count)
        noexcept;


} // namespace sdl::events

#endif
