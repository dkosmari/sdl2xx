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


namespace sdl::event {

    void
    pump()
        noexcept
    {
        SDL_PumpEvents();
    }


    unsigned
    peep(std::span<event> events,
         action act,
         Uint32 min_type,
         Uint32 max_type)
    {
        int result = SDL_PeepEvents(events.data(),
                                    events.size(),
                                    static_cast<SDL_eventaction>(act),
                                    min_type,
                                    max_type);
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
         Uint32 min_type,
         Uint32 max_type)
    {
        int result = SDL_PeepEvents(events.data(),
                                    events.size(),
                                    SDL_PEEKEVENT,
                                    min_type,
                                    max_type);
        if (result < 0)
            throw error{};
        return result;
    }


    unsigned
    get(std::span<event> events,
        Uint32 min_type,
        Uint32 max_type)
    {
        int result = SDL_PeepEvents(events.data(),
                                    events.size(),
                                    SDL_GETEVENT,
                                    min_type,
                                    max_type);
        if (result < 0)
            throw error{};
        return result;
    }


    bool
    has_event(Uint32 type)
        noexcept
    {
        return SDL_HasEvent(type);
    }


    bool
    has_event(Uint32 min_type,
              Uint32 max_type)
        noexcept
    {
        return SDL_HasEvents(min_type, max_type);
    }


    void
    flush(Uint32 type)
        noexcept
    {
        SDL_FlushEvent(type);
    }


    void
    remove(Uint32 type)
        noexcept
    {
        flush(type);
    }


    void
    flush(Uint32 min_type,
          Uint32 max_type)
        noexcept
    {
        SDL_FlushEvents(min_type, max_type);
    }


    void
    remove(Uint32 min_type,
           Uint32 max_type)
        noexcept
    {
        flush(min_type, max_type);
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
    set_state(Uint32 type,
              state st)
        noexcept
    {
        auto res = SDL_EventState(type, static_cast<int>(st));
        return res == SDL_ENABLE;
    }


    bool
    get_state(Uint32 type)
        noexcept
    {
        return set_state(type, state::query);
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

} // namespace sdl::event
