/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_STRING_HPP
#define SDL2XX_STRING_HPP

#include <string>
#include <type_traits>

#ifdef SDL2XX_CUSTOM_ALLOCATOR
#includde "allocator.hpp"
#endif

namespace sdl {

#ifdef SDL2XX_USE_ALLOCATOR

    template<typename T>
    using basic_string = std::basic_string<T, allocator<T>>;

    using string    = basic_string<char>;
    using u8string  = basic_string<char8_t>;
    using u16string = basic_string<char16_t>;
    using u32string = basic_string<char32_t>;


    // Allow copying between std and sdl strings.

    template<typename T>
    std::string&
    operator =(std::string& a, const string& b)
    {
        a.assign(b.begin(), b.end());
        return a;
    }

    template<typename T>
    string&
    operator =(string& a, const std::string& b)
    {
        a.assign(b.begin(), b.end());
        return a;
    }


#else

    using std::string;
    using std::u8string;
    using std::u16string;
    using std::u32string;

#endif


    template<typename T>
    struct is_string :
        std::false_type {};

    template<typename CharT,
             typename Traits,
             typename Allocator>
    struct is_string<std::basic_string<CharT, Traits, Allocator>> :
        std::true_type {};

    namespace concepts {

        // any basic_string
        template<typename T>
        concept string = is_string<T>::value;

    } // namespace concepts

} // namespace sdl

#endif
