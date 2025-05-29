/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#ifndef SDL2XX_RWOPS_HPP
#define SDL2XX_RWOPS_HPP

#include <cstdio>
#include <expected>
#include <filesystem>
#include <iosfwd>
#include <span>

#include <SDL_rwops.h>

#include "basic_wrapper.hpp"
#include "blob.hpp"
#include "error.hpp"
#include "string.hpp"


namespace sdl {

    using std::filesystem::path;


    struct rwops : basic_wrapper<SDL_RWops*> {

        using base_type = basic_wrapper<SDL_RWops*>;

        // Inherit constructors.
        using base_type::base_type;


        rwops(const path& filename,
              const char* mode);

        inline
        rwops(const path& filename,
              const concepts::string auto& mode) :
            rwops{filename, mode.data()}
        {}


        explicit
        rwops(FILE* fp,
              bool close_fp = true);


        rwops(void* mem,
              int size);

        rwops(const void* mem,
              int size);


        template<typename T,
                 std::size_t E>
        explicit
        rwops(std::span<T, E> mem) :
            rwops{static_cast<void*>(mem.data()), mem.size_bytes()}
        {}

        template<typename T,
                 std::size_t E>
        explicit
        rwops(std::span<const T, E> mem) :
            rwops{static_cast<const void*>(mem.data()), mem.size_bytes()}
        {}

        rwops(std::streambuf& stream);

        rwops(std::streambuf* stream);


        /// Move constructor.
        rwops(rwops&& other)
            noexcept = default;


        ~rwops()
            noexcept;


        /// Move assignment.
        rwops&
        operator =(rwops&& other)
            noexcept = default;


        void
        create(const path& filename,
               const char* mode);

        inline
        void
        create(const path& filename,
               const concepts::string auto& mode)
        {
            create(filename, mode.data());
        }


        void
        create(FILE* fp,
               bool close_fp = true);


        void
        create(void* mem,
               int size);

        void
        create(const void* mem,
               int size);


        template<typename T,
                 std::size_t E>
        void
        create(std::span<T, E> mem)
        {
            create(static_cast<void*>(mem.data()), mem.size_bytes());
        }

        template<typename T,
                 std::size_t E>
        void
        create(std::span<const T, E> mem)
        {
            create(static_cast<const void*>(mem.data()), mem.size_bytes());
        }


        void
        create(std::streambuf& stream);

        void
        create(std::streambuf* stream);


        void
        destroy()
            noexcept;


        [[nodiscard]]
        Sint64
        get_size()
            const;

        [[nodiscard]]
        std::expected<Sint64, error>
        try_get_size()
            const noexcept;


        enum class seekdir :int {
            beg = RW_SEEK_SET,
            cur = RW_SEEK_CUR,
            end = RW_SEEK_END,
        };

        Sint64
        seek(Sint64 offset,
             seekdir whence);

        Sint64
        seek(Sint64 offset,
             std::ios_base::seekdir whence);

        std::expected<Sint64, error>
        try_seek(Sint64 offset,
                 seekdir whence)
            noexcept;

        std::expected<Sint64, error>
        try_seek(Sint64 offset,
                 std::ios_base::seekdir whence)
            noexcept;


        [[nodiscard]]
        Sint64
        tell()
            const noexcept;


        [[nodiscard]]
        std::size_t
        read(void* buf,
             std::size_t elem_size,
             std::size_t count);

        template<typename T,
                 std::size_t E>
        [[nodiscard]]
        std::size_t
        read(std::span<T, E> buf)
        {
            return read(buf.data(), sizeof(T), buf.size());
        }


        [[nodiscard]]
        std::expected<std::size_t, error>
        try_read(void* buf,
                 std::size_t elem_size,
                 std::size_t count)
            noexcept;


        template<typename T,
                 std::size_t E>
        [[nodiscard]]
        std::expected<std::size_t, error>
        try_read(std::span<T, E> buf)
            noexcept
        {
            return try_read(buf.data(), sizeof(T), buf.size());
        }


        [[nodiscard]]
        std::size_t
        write(const void* buf,
              std::size_t elem_size,
              std::size_t count);

        template<typename T,
                 std::size_t E>
        [[nodiscard]]
        std::size_t
        write(std::span<T, E> buf)
        {
            return write(buf.data(), sizeof(T), buf.size());
        }

        [[nodiscard]]
        std::expected<std::size_t, error>
        try_write(const void* buf,
              std::size_t elem_size,
              std::size_t count)
            noexcept;

        template<typename T,
                 std::size_t E>
        [[nodiscard]]
        std::expected<std::size_t, error>
        try_write(std::span<T, E> buf)
        {
            return try_write(buf.data(), sizeof(T), buf.size());
        }


        [[nodiscard]]
        blob
        load();

        [[nodiscard]]
        std::expected<blob, error>
        try_load()
            noexcept;


        [[nodiscard]]
        Uint8
        read_u8();

        [[nodiscard]]
        std::expected<Uint8, error>
        try_read_u8()
            noexcept;


        template<typename T>
        [[nodiscard]]
        T
        read_le()
        {
            auto result = try_read_le<T>();
            if (!result)
                throw result.error();
            return *result;
        }

        template<typename T>
        [[nodiscard]]
        std::expected<T, error>
        try_read_le()
            noexcept;


        template<typename T>
        [[nodiscard]]
        T
        read_be()
        {
            auto result = try_read_be<T>();
            if (!result)
                throw result.error();
            return *result;
        }


        template<typename T>
        [[nodiscard]]
        std::expected<T, error>
        try_read_be()
            noexcept;


        void
        write_u8(Uint8 value);

        std::expected<void, error>
        try_write_u8(Uint8 value)
            noexcept;


        template<typename T>
        void
        write_le(T value)
        {
            auto result = try_write_le(value);
            if (!result)
                throw result.error();
        }

        template<typename T>
        std::expected<void, error>
        try_write_le(T value)
            noexcept;


        template<typename T>
        void
        write_be(T value)
        {
            auto result = try_write_be(value);
            if (!result)
                throw result.error();
        }

        template<typename T>
        std::expected<void, error>
        try_write_be(T value)
            noexcept;


    };


    constexpr
    int
    convert(rwops::seekdir x)
        noexcept
    {
        return static_cast<int>(x);
    }


    // TODO: should move this to another header?
    [[nodiscard]]
    blob
    load_file(const path& filename);

    [[nodiscard]]
    std::expected<blob, error>
    try_load_file(const path& filename)
        noexcept;


} // namespace sdl

#endif
