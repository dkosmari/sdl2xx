/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_ANGLE_HPP
#define SDL2XX_ANGLE_HPP

#include <cmath>
#include <compare>
#include <concepts>
#include <iosfwd>
#include <numbers>
#include <type_traits>

#include "string.hpp"


namespace sdl {

    enum class angle_unit {
        degrees,
        radians
    };


    namespace {

        namespace detail {

            template<typename T,
                     angle_unit Unit>
            inline
            constexpr
            T half_circle;


            template<typename T>
            inline constexpr T half_circle<T, angle_unit::degrees> = 180;

            template<typename T>
            inline constexpr T half_circle<T, angle_unit::radians> = std::numbers::pi_v<T>;

        } // namespace detail

    } // namespace


    template<typename T,
             angle_unit Unit>
    class basic_angle {

        T val;


    public:

        using value_type = T;

        static
        inline constexpr
        angle_unit unit = Unit;

        static
        inline constexpr
        T half_circle = detail::half_circle<T, Unit>;


        // Constructors

        constexpr
        basic_angle()
            noexcept = default;


        explicit
        constexpr
        basic_angle(T v)
            noexcept :
            val{v}
        {}


        // Allow implicit conversion from same unit.
        template<typename OtherT>
        constexpr
        basic_angle(const basic_angle<OtherT, Unit>& other)
            noexcept :
            val{static_cast<T>(other.value())}
        {}


        // Allow implicit conversion from different unit.
        template<typename OtherT,
                 angle_unit OtherUnit>
        requires(Unit != OtherUnit)
        basic_angle(const basic_angle<OtherT, OtherUnit>& other)
            noexcept :
            val{static_cast<T>(other.value() * half_circle / other.half_circle)}
        {}


        // Accessors

        T
        value()
            const noexcept
        {
            return val;
        }


        T&
        value()
            noexcept
        {
            return val;
        }


        explicit
        constexpr
        operator bool()
            const noexcept
        {
            return val;
        }


        // Unit conversions.

        constexpr
        basic_angle<T, angle_unit::degrees>
        as_degrees()
            const noexcept
        {
            return basic_angle<T, angle_unit::degrees>{*this};
        }

        constexpr
        basic_angle<T, angle_unit::radians>
        as_radians()
            const noexcept
        {
            return basic_angle<T, angle_unit::radians>{*this};
        }


        // Assignment operators.

        template<typename U>
        constexpr
        basic_angle&
        operator +=(const basic_angle<U, Unit>& other)
            noexcept
        {
            val += other.value();
            return *this;
        }

        template<std::convertible_to<T> S>
        constexpr
        basic_angle&
        operator +=(S other)
            noexcept
        {
            val += other;
            return *this;
        }


        template<typename U>
        constexpr
        basic_angle&
        operator -=(const basic_angle<U, Unit>& other)
            noexcept
        {
            val -= other.value();
            return *this;
        }

        template<std::convertible_to<T> S>
        constexpr
        basic_angle&
        operator -=(S other)
            noexcept
        {
            val -= other;
            return *this;
        }


        template<typename U>
        constexpr
        basic_angle&
        operator *=(const basic_angle<U, Unit>& other)
            noexcept
        {
            val *= other.value();
            return *this;
        }

        template<std::convertible_to<T> S>
        constexpr
        basic_angle&
        operator *=(S other)
            noexcept
        {
            val *= other;
            return *this;
        }


        template<typename U>
        constexpr
        basic_angle&
        operator /=(const basic_angle<U, Unit>& other)
            noexcept
        {
            val /= other.value();
            return *this;
        }

        template<std::convertible_to<T> S>
        constexpr
        basic_angle&
        operator /=(S other)
            noexcept
        {
            val /= other;
            return *this;
        }


        // Increment/decrement operators


        constexpr
        basic_angle&
        operator++()
            noexcept
        {
            ++val;
            return *this;
        }


        constexpr
        basic_angle&
        operator--()
            noexcept
        {
            --val;
            return *this;
        }


        constexpr
        basic_angle
        operator++(int)
            noexcept
        {
            return basic_angle{val++};
        }


        constexpr
        basic_angle
        operator--(int)
            noexcept
        {
            return basic_angle{val--};
        }


        // Comparison operators

        template<typename U>
        constexpr
        bool
        operator ==(const basic_angle<U, Unit>& other)
            const noexcept
        {
            return val == other.value();
        }


        template<typename U>
        constexpr
        std::partial_ordering
        operator <=>(const basic_angle<U, Unit>& other)
            const noexcept
        {
            return val <=> other.value();
        }

    }; // class basic_angle


    template<typename T>
    struct is_angle : std::false_type {};

    template<typename T,
             angle_unit Unit>
    struct is_angle<basic_angle<T, Unit>> : std::true_type {};


    template<typename T>
    struct is_angle_degrees : std::false_type {};

    template<typename T>
    struct is_angle_degrees<basic_angle<T, angle_unit::degrees>> : std::true_type {};


    template<typename T>
    struct is_angle_radians : std::false_type {};

    template<typename T>
    struct is_angle_radians<basic_angle<T, angle_unit::radians>> : std::true_type {};


    namespace concepts {

        template<typename T>
        concept angle = is_angle<T>::value;

        template<typename T>
        concept angle_degrees = is_angle_degrees<T>::value;

        template<typename T>
        concept angle_radians = is_angle_radians<T>::value;


        template<typename T>
        concept arithmetic = std::is_arithmetic_v<T>;

    } // namespace concepts


} // namespace sdl


// Specialize std::common_type
namespace std {

    template<typename T1,
             typename T2,
             sdl::angle_unit Unit>
    struct common_type<sdl::basic_angle<T1, Unit>,
                       sdl::basic_angle<T2, Unit>> {
        using type = sdl::basic_angle<common_type_t<T1, T2>, Unit>;
    };

    // between arithmetic and angle
    template<typename S,
             typename T,
             sdl::angle_unit Unit>
    struct common_type<S,
                       sdl::basic_angle<T, Unit>> {
        using type = sdl::basic_angle<common_type_t<S, T>, Unit>;
    };

    // between angle and arithmetic
    template<typename T,
             sdl::angle_unit Unit,
             typename S>
    struct common_type<sdl::basic_angle<T, Unit>,
                       S> {
        using type = sdl::basic_angle<common_type_t<T, S>, Unit>;
    };

} // namespace std

namespace sdl {



    // Arithmetic operators

    template<concepts::angle A>
    constexpr
    A
    operator +(A a)
        noexcept
    {
        return a;
    }


    template<concepts::angle A>
    constexpr
    A
    operator -(A a)
        noexcept
    {
        return A{-a.value()};
    }


    template<concepts::angle A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    operator +(A a,
               B b)
        noexcept
    {
        using C = std::common_type_t<A, B>;
        return C{a.value() + b.value()};
    }


    template<concepts::angle A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    operator -(A a,
               B b)
        noexcept
    {
        using C = std::common_type_t<A, B>;
        return C{a.value() - b.value()};
    }


    template<concepts::angle A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    operator *(A a,
               B b)
        noexcept
    {
        using C = std::common_type_t<A, B>;
        return C{a.value() * b.value()};
    }

    template<concepts::arithmetic A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    operator *(A a,
               B b)
        noexcept
    {
        using C = std::common_type_t<A, B>;
        return C{a * b.value()};
    }

    template<concepts::angle A,
             concepts::arithmetic B>
    constexpr
    std::common_type_t<A, B>
    operator *(A a,
               B b)
        noexcept
    {
        using C = std::common_type_t<A, B>;
        return C{a.value() * b};
    }


    template<concepts::angle A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    operator /(A a,
               B b)
        noexcept
    {
        using C = std::common_type_t<A, B>;
        return C{a.value() / b.value()};
    }

    template<concepts::arithmetic A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    operator /(A a,
               B b)
        noexcept
    {
        using C = std::common_type_t<A, B>;
        return C{a / b.value()};
    }

    template<concepts::angle A,
             concepts::arithmetic B>
    constexpr
    std::common_type_t<A, B>
    operator /(A a,
               B b)
        noexcept
    {
        using C = std::common_type_t<A, B>;
        return C{a.value() / b};
    }


    template<concepts::angle A>
    constexpr
    A
    abs(A a)
        noexcept
    {
        return A{std::abs(a)};
    }


    template<concepts::angle A>
    constexpr
    A
    fabs(A a)
        noexcept
    {
        return A{std::abs(a)};
    }


    template<concepts::angle A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    fmod(A a,
         B b)
    {
        using C = std::common_type_t<A, B>;
        return C{std::fmod(a.value(), b.value())};
    }

    template<concepts::arithmetic A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    fmod(A a,
         B b)
    {
        using C = std::common_type_t<A, B>;
        return C{std::fmod(a, b.value())};
    }

    template<concepts::angle A,
             concepts::arithmetic B>
    constexpr
    std::common_type_t<A, B>
    fmod(A a,
         B b)
    {
        using C = std::common_type_t<A, B>;
        return C{std::fmod(a.value(), b)};
    }


    template<concepts::angle A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    remainder(A a,
              B b)
    {
        using C = std::common_type_t<A, B>;
        return C{std::remainder(a.value(), b.value())};
    }

    template<concepts::arithmetic A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    remainder(A a,
              B b)
    {
        using C = std::common_type_t<A, B>;
        return C{std::remainder(a, b.value())};
    }

    template<concepts::angle A,
             concepts::arithmetic B>
    constexpr
    std::common_type_t<A, B>
    remainder(A a,
              B b)
    {
        using C = std::common_type_t<A, B>;
        return C{std::remainder(a.value(), b)};
    }


    template<concepts::angle A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    fmax(A a,
         B b)
    {
        using C = std::common_type_t<A, B>;
        return C{std::fmax(a.value(), b.value())};
    }

    template<concepts::arithmetic A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    fmax(A a,
         B b)
    {
        using C = std::common_type_t<A, B>;
        return C{std::fmax(a, b.value())};
    }

    template<concepts::angle A,
             concepts::arithmetic B>
    constexpr
    std::common_type_t<A, B>
    fmax(A a,
         B b)
    {
        using C = std::common_type_t<A, B>;
        return C{std::fmax(a.value(), b)};
    }


    template<concepts::angle A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    fmin(A a,
         B b)
    {
        using C = std::common_type_t<A, B>;
        return C{std::fmin(a.value(), b.value())};
    }

    template<concepts::arithmetic A,
             concepts::angle B>
    constexpr
    std::common_type_t<A, B>
    fmin(A a,
         B b)
    {
        using C = std::common_type_t<A, B>;
        return C{std::fmin(a, b.value())};
    }

    template<concepts::angle A,
             concepts::arithmetic B>
    constexpr
    std::common_type_t<A, B>
    fmin(A a,
         B b)
    {
        using C = std::common_type_t<A, B>;
        return C{std::fmin(a.value(), b)};
    }


    template<concepts::angle A>
    A
    sqrt(A a)
        noexcept
    {
        return A{std::sqrt(a.value())};
    }


    template<concepts::angle A>
    A
    cbrt(A a)
        noexcept
    {
        return A{std::cbrt(a.value())};
    }


    // Trig functions

    template<concepts::angle_radians A>
    A::value_type
    sin(A a)
        noexcept
    {
        return std::sin(a.value());
    }

    template<concepts::angle_degrees A>
    A::value_type
    sin(A a)
        noexcept
    {
        return sin(a.as_radians());
    }


    template<concepts::angle_radians A>
    A::value_type
    cos(A a)
        noexcept
    {
        return std::cos(a.value());
    }

    template<concepts::angle_degrees A>
    A::value_type
    cos(A a)
        noexcept
    {
        return cos(a.as_radians());
    }



    template<concepts::angle_radians A>
    A::value_type
    tan(A a)
        noexcept
    {
        return std::tan(a.value());
    }

    template<concepts::angle_degrees A>
    A::value_type
    tan(A a)
        noexcept
    {
        return tan(a.as_radians());
    }


    template<concepts::arithmetic T>
    basic_angle<T, angle_unit::radians>
    asin(T x)
        noexcept
    {
        return basic_angle<T, angle_unit::radians>{std::asin(x)};
    }


    template<concepts::arithmetic T>
    basic_angle<T, angle_unit::radians>
    acos(T x)
        noexcept
    {
        return basic_angle<T, angle_unit::radians>{std::acos(x)};
    }


    template<concepts::arithmetic T>
    basic_angle<T, angle_unit::radians>
    atan(T x)
        noexcept
    {
        return basic_angle<T, angle_unit::radians>{std::atan(x)};
    }


    template<concepts::arithmetic T1,
             concepts::arithmetic T2>
    basic_angle<std::common_type_t<T1, T2>, angle_unit::radians>
    atan2(T1 y,
          T2 x)
        noexcept
    {
        using T = std::common_type_t<T1, T2>;
        return basic_angle<T, angle_unit::radians>{std::atan2(y, x)};
    }


    template<concepts::angle A>
    constexpr
    A
    ceil(A a)
        noexcept
    {
        return A{std::ceil(a.value())};
    }


    template<concepts::angle A>
    constexpr
    A
    floor(A a)
        noexcept
    {
        return A{std::floor(a.value())};
    }


    template<concepts::angle A>
    constexpr
    A
    trunc(A a)
        noexcept
    {
        return A{std::trunc(a.value())};
    }


    template<concepts::angle A>
    constexpr
    A
    round(A a)
        noexcept
    {
        return A{std::round(a.value())};
    }


    // Angle manipulation.

    /// Return angle wrapped between [-half_circle, +half_circle)
    template<concepts::angle A>
    A
    wrap_zero(A x)
        noexcept
    {
        using T = A::value_type;
        constexpr auto Unit = A::unit;
        constexpr T half_circle = detail::half_circle<T, Unit>;
        const T xx = x.value() + half_circle;
        T y = std::fmod(xx, 2 * half_circle);
        if (y < 0)
            y += 2 * half_circle;
        return A{y - half_circle};
    }


    /// Return angle wrapped between [0, 2 * half_circle)
    template<concepts::angle A>
    A
    wrap_positive(A x)
        noexcept
    {
        using T = A::value_type;
        constexpr auto Unit = A::unit;
        constexpr T half_circle = detail::half_circle<T, Unit>;
        const T xx = x.value();
        T y = std::fmod(xx, 2 * half_circle);
        if (y < 0)
            y += 2 * half_circle;
        return A{std::fabs(y)};
    }



    template<typename T>
    using basic_degrees = basic_angle<T, angle_unit::degrees>;

    template<typename T>
    using basic_radians = basic_angle<T, angle_unit::radians>;


    using degrees  = basic_degrees<double>;
    using degreesf = basic_degrees<float>;

    using radians  = basic_radians<double>;
    using radiansf = basic_radians<float>;


    inline
    namespace literals {

        inline
        namespace angle_literals {

            inline
            constexpr
            degrees
            operator ""_deg(long double x)
                noexcept
            {
                return degrees{static_cast<double>(x)};
            }


            inline
            constexpr
            degrees
            operator ""_deg(unsigned long long x)
                noexcept
            {
                return degrees{static_cast<double>(x)};
            }


            inline
            constexpr
            degreesf
            operator ""_degf(long double x)
                noexcept
            {
                return degreesf{static_cast<float>(x)};
            }


            inline
            constexpr
            degreesf
            operator ""_degf(unsigned long long x)
                noexcept
            {
                return degreesf{static_cast<float>(x)};
            }


            inline
            constexpr
            radians
            operator ""_rad(long double x)
                noexcept
            {
                return radians{static_cast<double>(x)};
            }


            inline
            constexpr
            radians
            operator ""_rad(unsigned long long x)
                noexcept
            {
                return radians{static_cast<double>(x)};
            }


            inline
            constexpr
            radiansf
            operator ""_radf(long double x)
                noexcept
            {
                return radiansf{static_cast<float>(x)};
            }


            inline
            constexpr
            radiansf
            operator ""_radf(unsigned long long x)
                noexcept
            {
                return radiansf{static_cast<float>(x)};
            }


            inline
            constexpr
            radians
            operator ""_pi_rad(long double x)
                noexcept
            {
                return radians{static_cast<double>(x) * std::numbers::pi_v<double>};
            }


            inline
            constexpr
            radians
            operator ""_pi_rad(unsigned long long x)
                noexcept
            {
                return radians{static_cast<double>(x) * std::numbers::pi_v<double>};
            }


            inline
            constexpr
            radiansf
            operator ""_pi_radf(long double x)
                noexcept
            {
                return radiansf{static_cast<float>(x) * std::numbers::pi_v<float>};
            }


            inline
            constexpr
            radiansf
            operator ""_pi_radf(unsigned long long x)
                noexcept
            {
                return radiansf{static_cast<float>(x) * std::numbers::pi_v<float>};
            }

        } // namespace angle_literals

    } // namespace literals


    // Serialization

    string
    to_string(degrees d);

    string
    to_string(degreesf d);

    string
    to_string(radians d);

    string
    to_string(radiansf d);


    std::ostream&
    operator <<(std::ostream& out,
                degrees d);

    std::ostream&
    operator <<(std::ostream& out,
                degreesf d);

    std::ostream&
    operator <<(std::ostream& out,
                radians r);

    std::ostream&
    operator <<(std::ostream& out,
                radiansf r);


} // namespace sdl

#endif
