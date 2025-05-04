/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <stdexcept>

#include "events.hpp"

#include "error.hpp"
#include "unique_ptr.hpp"


namespace sdl::events {

    void
    pump()
        noexcept
    {
        SDL_PumpEvents();
    }


    unsigned
    peep(std::span<event> events,
         action act,
         Uint32 min,
         Uint32 max)
    {
        int result = SDL_PeepEvents(events.data(),
                                    events.size(),
                                    static_cast<SDL_eventaction>(act),
                                    min,
                                    max);
        if (result < 0)
            throw error{};
        return result;
    }


    unsigned
    add(std::span<event> events)
    {
        int result = SDL_PeepEvents(events.data(),
                                    events.size(),
                                    SDL_ADDEVENT,
                                    0,
                                    0);
        if (result < 0)
            throw error{};
        return result;
    }


    unsigned
    peek(std::span<event> events,
         Uint32 min,
         Uint32 max)
    {
        int result = SDL_PeepEvents(events.data(),
                                    events.size(),
                                    SDL_PEEKEVENT,
                                    min,
                                    max);
        if (result < 0)
            throw error{};
        return result;
    }


    unsigned
    get(std::span<event> events,
        Uint32 min,
        Uint32 max)
    {
        int result = SDL_PeepEvents(events.data(),
                                    events.size(),
                                    SDL_GETEVENT,
                                    min,
                                    max);
        if (result < 0)
            throw error{};
        return result;
    }


    bool
    has_event(Uint32 t)
        noexcept
    {
        return SDL_HasEvent(t);
    }


    bool
    has_event(Uint32 min,
              Uint32 max)
        noexcept
    {
        return SDL_HasEvents(min, max);
    }


    void
    flush(Uint32 t)
        noexcept
    {
        SDL_FlushEvent(t);
    }

    void
    flush(Uint32 min,
          Uint32 max)
        noexcept
    {
        SDL_FlushEvents(min, max);
    }


    void
    remove(Uint32 t)
        noexcept
    {
        flush(t);
    }


    void
    remove(Uint32 min,
           Uint32 max)
        noexcept
    {
        flush(min, max);
    }


    bool
    poll(event& e)
        noexcept
    {
        return SDL_PollEvent(&e);
    }


    [[nodiscard]]
    std::optional<event>
    poll()
        noexcept
    {
        std::optional<event> result{std::in_place};
        if (!SDL_PollEvent(&*result))
            result.reset();
        return result;
    }


    void
    wait(event& e)
    {
        if (!SDL_WaitEvent(&e))
            throw error{};
    }


    event
    wait()
    {
        event result;
        if (!SDL_WaitEvent(&result))
            throw error{};
        return result;
    }


    void
    wait(event& e,
         std::chrono::milliseconds timeout)
    {
        if (!SDL_WaitEventTimeout(&e, timeout.count()))
            throw error{};
    }


    event
    wait(std::chrono::milliseconds timeout)
    {
        event result;
        if (!SDL_WaitEventTimeout(&result, timeout.count()))
            throw error{};
        return result;
    }


    bool
    try_wait(event& e,
             std::chrono::milliseconds timeout)
        noexcept
    {
        return SDL_WaitEventTimeout(&e, timeout.count());
    }


    std::optional<event>
    try_wait(std::chrono::milliseconds timeout)
        noexcept
    {
        std::optional<event> result{std::in_place};
        if (!SDL_WaitEventTimeout(&*result, timeout.count()))
            result.reset();
        return result;
    }


    bool
    push(event& e)
    {
        int result = SDL_PushEvent(&e);
        if (result < 0)
            throw error{};
        return result;
    }


    void
    set_filter(SDL_EventFilter filter,
               void* ctx)
        noexcept
    {
        SDL_SetEventFilter(filter, ctx);
    }


    void
    reset_filter()
        noexcept
    {
        SDL_SetEventFilter(nullptr, nullptr);
    }


    std::pair<SDL_EventFilter, void*>
    get_filter()
        noexcept
    {
        std::pair<SDL_EventFilter, void*> result{nullptr, nullptr};
        SDL_GetEventFilter(&result.first, &result.second);
        return result;
    }


    void
    add_watch(SDL_EventFilter watch,
              void* ctx)
        noexcept
    {
        SDL_AddEventWatch(watch, ctx);
    }

    void
    remove_watch(SDL_EventFilter watch,
                 void* ctx)
        noexcept
    {
        SDL_DelEventWatch(watch, ctx);
    }


    void
    filter_now(SDL_EventFilter filter,
               void* ctx)
        noexcept
    {
        SDL_FilterEvents(filter, ctx);
    }


    bool
    set_enabled(Uint32 t,
                bool enabled)
        noexcept
    {
        int result = SDL_EventState(t, enabled ? SDL_ENABLE : SDL_DISABLE);
        return result == SDL_ENABLE;
    }


    bool
    enable(Uint32 t)
        noexcept
    {
        return set_enabled(t, true);
    }


    bool
    disable(Uint32 t)
        noexcept
    {
        return set_enabled(t, false);
    }


    bool
    is_enabled(Uint32 t)
        noexcept
    {
        int result = SDL_EventState(t, SDL_QUERY);
        return result == SDL_ENABLE;
    }


    std::optional<Uint32>
    allocate(unsigned count)
        noexcept
    {
        auto result = SDL_RegisterEvents(count);
        if (result == ~0u)
            return {};
        return result;
    }

} // namespace sdl::events
