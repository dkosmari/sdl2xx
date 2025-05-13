/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include "sensor.hpp"


#if SDL_VERSION_ATLEAST(2, 0, 9)

using std::expected;
using std::unexpected;


namespace sdl::sensor {

#if SDL_VERSION_ATLEAST(2, 0, 14)

    void
    lock()
        noexcept
    {
        SDL_LockSensors();
    }


    void
    unlock()
        noexcept
    {
        SDL_UnlockSensors();
    }


    lock_guard::lock_guard()
        noexcept
    {
        lock();
    }


    lock_guard::lock_guard(lock_guard&& other)
        noexcept :
        locked{other.locked}
    {
        other.locked = false;
    }


    lock_guard::~lock_guard()
        noexcept
    {
        if (locked)
            unlock();
    }


    lock_guard&
    lock_guard::operator =(lock_guard&& other)
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
    lock_guard::lock()
        noexcept
    {
        if (!locked) {
            locked = true;
            sdl::sensor::lock();
        }
    }


    void
    lock_guard::unlock()
        noexcept
    {
        if (locked) {
            sdl::sensor::unlock();
            locked = false;
        }
    }

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


    unsigned
    get_num_devices()
        noexcept
    {
        return SDL_NumSensors();
    }


    const char*
    get_name(unsigned index)
    {
        auto result = try_get_name(index);
        if (!result)
            throw result.error();
        return *result;
    }


    expected<const char*, error>
    try_get_name(unsigned index)
        noexcept
    {
        const char* name = SDL_SensorGetDeviceName(index);
        if (!name)
            return unexpected{error{}};
        return name;
    }


    type
    get_type(unsigned index)
        noexcept
    {
        return convert(SDL_SensorGetDeviceType(index));
    }


    int
    get_platform_type(unsigned index)
        noexcept
    {
        return SDL_SensorGetDeviceNonPortableType(index);
    }


    instance_id
    get_id(unsigned index)
    {
        auto result = SDL_SensorGetDeviceInstanceID(index);
        if (result == -1)
            throw error{};
        return result;
    }


    device::device(unsigned index)
    {
        create(index);
    }


    device
    device::from_id(instance_id id)
    {
        auto raw = SDL_SensorFromInstanceID(id);
        if (!raw)
            throw error{};
        for (unsigned i = 0; i < get_num_devices(); ++i)
            if (id == sensor::get_id(i))
                return device{i};
        throw error{"unknown error"};
    }


    device::~device()
        noexcept
    {
        destroy();
    }


    void
    device::create(unsigned index)
    {
        auto ptr = SDL_SensorOpen(index);
        if (!ptr)
            throw error{};
        destroy();
        acquire(ptr);
    }


    void
    device::destroy()
        noexcept
    {
        if (is_valid())
            SDL_SensorClose(release());
    }


    const char*
    device::get_name()
        const noexcept
    {
        return SDL_SensorGetName(raw);
    }


    type
    device::get_type()
        const noexcept
    {
        return convert(SDL_SensorGetType(raw));
    }


    int
    device::get_platform_type()
        const noexcept
    {
        return SDL_SensorGetNonPortableType(raw);
    }


    instance_id
    device::get_id()
        const noexcept
    {
        return SDL_SensorGetInstanceID(raw);
    }


    bool
    device::get_values(float* buf,
                       std::size_t count)
        noexcept
    {
        return !SDL_SensorGetData(raw, buf, count);
    }


    bool
    device::get_values(std::span<float> buf)
        noexcept
    {
        return get_values(buf.data(), buf.size());
    }


    vector<float>
    device::get_values(std::size_t count)
    {
        vector<float> values(count);
        if (!get_values(std::span(values)))
            return {};
        return values;
    }


#if SDL_VERSION_ATLEAST(2, 26, 0)

    std::pair<bool, Uint64>
    device::get_values_timestamp(float* buf,
                                 std::size_t count)
        noexcept
    {
        Uint64 timestamp;
        int r = SDL_SensorGetDataWithTimestamp(raw, &timestamp, buf, count);
        return {
            r == 0,
            timestamp
        };
    }


    std::pair<bool, Uint64>
    device::get_values_timestamp(std::span<float> buf)
        noexcept
    {
        return get_values_timestamp(buf.data(), buf.size());
    }


    std::pair<vector<float>, Uint64>
    device::get_values_timestamp(std::size_t count)
    {
        vector<float> values(count);
        auto [success, timestamp] = get_values_timestamp(std::span(values));
        if (!success)
            return {{}, {}};
        return {
            std::move(values),
            timestamp
        };
    }

#endif // SDL_VERSION_ATLEAST(2, 26, 0)


    void
    update()
        noexcept
    {
        SDL_SensorUpdate();
    }

} // namespace sdl::sensor

#endif // SDL_VERSION_ATLEAST(2, 0, 9)
