/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_OWNER_WRAPPER_HPP
#define SDL2XX_OWNER_WRAPPER_HPP

#include <tuple>
#include <utility>

#include "basic_wrapper.hpp"


namespace sdl {

    // Source: https://github.com/dkosmari/basic_wrapper

    // This wrapper has an "owner" flag. When false, it will not destroy the raw object.
    template<typename T,
             T InvalidValue = T{}>
    class owner_wrapper : public basic_wrapper<T, InvalidValue> {

    protected:

        bool owner = true;


    public:

        using parent_type = basic_wrapper<T, InvalidValue>;

        using state_type = std::tuple<typename parent_type::state_type, bool>;


        // Inherit constructors.
        using parent_type::parent_type;


        /// Move constructor.
        owner_wrapper(owner_wrapper&& other)
            noexcept
        {
            acquire(other.release());
        }


        /// Move assignment.
        owner_wrapper&
        operator =(owner_wrapper&& other)
            noexcept
        {
            if (this != &other) {
                this->destroy();
                acquire(other.release());
            }
            return *this;
        }


        void
        acquire(state_type state)
            noexcept
        {
            parent_type::acquire(std::move(get<0>(state)));
            owner = get<1>(state);
        }


        void
        acquire(typename parent_type::state_type new_parent_state,
                bool new_owner = true)
            noexcept
        {
            acquire(state_type{new_parent_state, new_owner});
        }


        state_type
        release()
            noexcept
        {
            bool old_owner = owner;
            owner = false;
            return state_type{
                parent_type::release(),
                old_owner
            };
        }

    }; // class owner_wrapper

} // namespace sdl

#endif
