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


        // Ensure it can only be destructed by derived classes.
        constexpr
        ~basic_wrapper()
            noexcept = default;

    public:

        using raw_type = T;


        constexpr
        basic_wrapper()
            noexcept = default;


        constexpr
        basic_wrapper(T src)
            noexcept :
            raw{src}
        {}


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
        acquire(raw_type new_raw)
            noexcept
        {
            raw = new_raw;
        }


        raw_type
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
