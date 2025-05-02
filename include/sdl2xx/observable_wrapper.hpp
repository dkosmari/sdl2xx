/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_OBSERVABLE_WRAPPER_HPP
#define SDL2XX_OBSERVABLE_WRAPPER_HPP

#include <tuple>
#include <utility>

#include "basic_wrapper.hpp"


namespace sdl {

    // This wrapper allows an instance to be just an observer.
    // Observer instances do not destroy the raw object.
    template<typename T>
    class observable_wrapper : public basic_wrapper<T> {

    protected:

        bool owner = true;


    public:

        using parent_t = basic_wrapper<T>;

        using state_t = std::tuple<typename parent_t::state_t,
                                   bool>;


        // Inherit constructors.
        using parent_t::parent_t;


        /// Move constructor.
        observable_wrapper(observable_wrapper&& other)
            noexcept
        {
            acquire(other.release());
        }


        /// Move assignment.
        observable_wrapper&
        operator =(observable_wrapper&& other)
            noexcept
        {
            if (this != &other) {
                this->destroy();
                acquire(other.release());
            }
            return *this;
        }


        void
        acquire(state_t state)
            noexcept
        {
            parent_t::acquire(std::move(get<0>(state)));
            owner = get<1>(state);
        }


        void
        acquire(typename parent_t::state_t new_p_state,
                bool new_owner = true)
            noexcept
        {
            acquire(state_t{new_p_state, new_owner});
        }


        state_t
        release()
            noexcept
        {
            bool old_owner = owner;
            owner = false;
            return state_t{
                parent_t::release(),
                old_owner
            };
        }

    };

} // namespace sdl

#endif
