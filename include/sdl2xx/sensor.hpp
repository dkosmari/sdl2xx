/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_SENSOR_HPP
#define SDL2XX_SENSOR_HPP

#include <expected>
#include <mutex>
#include <span>
#include <utility>

#include <SDL_sensor.h>
#include <SDL_version.h>

#if SDL_VERSION_ATLEAST(2, 0, 9)

#include "basic_wrapper.hpp"
#include "error.hpp"
#include "vector.hpp"


namespace sdl::sensor {

    using instance_id = SDL_SensorID;


    enum class type {
        accel   = SDL_SENSOR_ACCEL,
        accel_l = SDL_SENSOR_ACCEL_L,
        accel_r = SDL_SENSOR_ACCEL_R,
        gyro    = SDL_SENSOR_GYRO,
        gyro_l  = SDL_SENSOR_GYRO_L,
        gyro_r  = SDL_SENSOR_GYRO_R,
        invalid = SDL_SENSOR_INVALID,
        unknown = SDL_SENSOR_UNKNOWN,
    };


    [[nodiscard]]
    constexpr
    type
    convert(SDL_SensorType t)
        noexcept
    {
        return static_cast<type>(t);
    }


    [[nodiscard]]
    constexpr
    SDL_SensorType
    convert(type t)
        noexcept
    {
        return static_cast<SDL_SensorType>(t);
    }


#if SDL_VERSION_ATLEAST(2, 0, 14)

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

#endif // SDL_VERSION_ATLEAST(2, 0, 14)


    [[nodiscard]]
    unsigned
    get_num_devices()
        noexcept;


    [[nodiscard]]
    const char*
    get_name(unsigned index);

    [[nodiscard]]
    std::expected<const char*, error>
    try_get_name(unsigned index)
        noexcept;


    [[nodiscard]]
    type
    get_type(unsigned index)
        noexcept;


    [[nodiscard]]
    int
    get_platform_type(unsigned index)
        noexcept;


    [[nodiscard]]
    instance_id
    get_id(unsigned index);


    struct device : basic_wrapper<SDL_Sensor*> {

        using base_type = basic_wrapper<SDL_Sensor*>;

        // Inherit constructors.
        using base_type::base_type;


        explicit
        device(unsigned index);


        /// Move constructor.
        device(device&& other)
            noexcept = default;


        // Named constructor: from instance id.
        [[nodiscard]]
        static
        device
        from_id(instance_id id);


        ~device()
            noexcept;


        /// Move assignment.
        device&
        operator =(device&& other)
            noexcept = default;


        void
        create(unsigned index);


        void
        destroy()
            noexcept;


        [[nodiscard]]
        const char*
        get_name()
            const noexcept;


        [[nodiscard]]
        type
        get_type()
            const noexcept;


        [[nodiscard]]
        int
        get_platform_type()
            const noexcept;


        [[nodiscard]]
        instance_id
        get_id()
            const noexcept;


        [[nodiscard]]
        bool
        get_values(float* buf,
                   std::size_t count)
            noexcept;

        [[nodiscard]]
        bool
        get_values(std::span<float> buf)
            noexcept;

        [[nodiscard]]
        vector<float>
        get_values(std::size_t count);


#if SDL_VERSION_ATLEAST(2, 26, 0)

        [[nodiscard]]
        std::pair<bool, Uint64>
        get_values_timestamp(float* buf,
                             std::size_t count)
            noexcept;

        [[nodiscard]]
        std::pair<bool, Uint64>
        get_values_timestamp(std::span<float> buf)
            noexcept;

        [[nodiscard]]
        std::pair<vector<float>, Uint64>
        get_values_timestamp(std::size_t count);

#endif // SDL_VERSION_ATLEAST(2, 26, 0)

    }; // struct device


    static constexpr float gravity = SDL_STANDARD_GRAVITY;


    void
    update()
        noexcept;

} // namespace sdl::sensor

#endif // SDL_VERSION_ATLEAST(2, 0, 9)

#endif
