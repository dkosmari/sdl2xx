/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_BASIC_LOCKER_HPP
#define SDL2XX_BASIC_LOCKER_HPP

#include <utility>
#include <mutex>


namespace sdl {

    template<typename T>
    class basic_locker {

    protected:

        const T* guarded = nullptr;
        bool locked = false;


        basic_locker(const T* src)
            noexcept :
            guarded{src}
        {}


        basic_locker(const T& src)
            noexcept :
            guarded{&src}
        {}


        ~basic_locker()
            noexcept = default;

    public:

        using guarded_type = T;
        using state_type = std::pair<const guarded_type*, bool>;


        constexpr
        basic_locker()
            noexcept = default;


        constexpr
        basic_locker(const T* src,
                     std::defer_lock_t)
            noexcept :
            guarded{src}
        {}


        constexpr
        basic_locker(const T& src,
                     std::defer_lock_t)
            noexcept :
            guarded{&src}
        {}


        /// Move constructor.
        constexpr
        basic_locker(basic_locker&& other)
            noexcept
        {
            acquire(other.release());
        }


        /// Move assignment.
        basic_locker&
        operator =(basic_locker&& other)
            noexcept
        {
            if (this != &other) {
                unlock();
                acquire(other.release());
            }
            return *this;
        }


        // Remember to always call unlock() from the derived class' destructor.
        virtual
        void
        unlock()
            noexcept = 0;



        [[nodiscard]]
        bool
        is_guarded()
            const noexcept
        {
            return guarded && *guarded;
        }


        [[nodiscard]]
        bool
        is_locked()
            const noexcept
        {
            return locked;
        }


        void
        acquire(state_type new_state)
            noexcept
        {
            std::tie(guarded, locked) = new_state;
        }


        state_type
        release()
            noexcept
        {
            auto old_guarded = guarded;
            auto old_locked = locked;
            guarded = nullptr;
            locked = false;
            return {old_guarded, old_locked};
        }

    };

} // namespace sdl

#endif
