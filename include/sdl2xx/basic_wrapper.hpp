/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_BASIC_WRAPPER_HPP
#define SDL2XX_BASIC_WRAPPER_HPP


namespace sdl {

    template<typename T>
    class basic_wrapper {

    protected:

        T raw = {};


        ~basic_wrapper()
            noexcept = default;

    public:

        using raw_type = T;
        using state_t = raw_type;


        constexpr
        basic_wrapper()
            noexcept = default;


        constexpr
        explicit
        basic_wrapper(T src)
            noexcept :
            raw{src}
        {}


        /// Move constructor.
        constexpr
        basic_wrapper(basic_wrapper&& other)
            noexcept
        {
            acquire(other.release());
        }


        /// Move assignment.
        basic_wrapper&
        operator =(basic_wrapper&& other)
            noexcept
        {
            if (this != &other) {
                destroy();
                acquire(other.release());
            }
            return *this;
        }


        virtual
        void
        destroy()
            noexcept = 0;


        [[nodiscard]]
        bool
        is_valid()
            const noexcept
        {
            return raw;
        }


        [[nodiscard]]
        explicit
        operator bool()
            const noexcept
        {
            return raw;
        }


        raw_type
        data()
            noexcept
        {
            return raw;
        }


        const raw_type
        data()
            const noexcept
        {
            return raw;
        }


        void
        acquire(state_t new_state)
            noexcept
        {
            raw = new_state;
        }


        state_t
        release()
            noexcept
        {
            auto old_raw = raw;
            raw = {};
            return old_raw;
        }

    }; // class basic_wrapper

} // namespace sdl

#endif
