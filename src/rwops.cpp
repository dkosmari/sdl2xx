/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <utility>
#include <streambuf>

#include <SDL_error.h>

#include "rwops.hpp"


using std::expected;
using std::unexpected;


namespace sdl {

    namespace {

        Sint64
        streambuf_seek(SDL_RWops* ctx,
                       Sint64 offset,
                       int whence)
            noexcept;

        Sint64
        streambuf_size(SDL_RWops* ctx)
            noexcept
        {
            auto pos = SDL_RWseek(ctx, 0, RW_SEEK_CUR);
            if (pos < 0)
                return -1;
            auto size = SDL_RWseek(ctx, 0, RW_SEEK_END);
            SDL_RWseek(ctx, pos, RW_SEEK_SET);
            return size;
        }


        Sint64
        streambuf_seek(SDL_RWops* ctx,
                       Sint64 offset,
                       int whence)
            noexcept
        {
            auto stream = reinterpret_cast<std::streambuf*>(ctx->hidden.unknown.data1);
            std::ios_base::seekdir dir;
            switch (whence) {
                case RW_SEEK_SET:
                    dir = std::ios_base::beg;
                    break;
                case RW_SEEK_CUR:
                    dir = std::ios_base::cur;
                    break;
                case RW_SEEK_END:
                    dir = std::ios_base::end;
                    break;
                default:
                    return SDL_SetError("streambuf_seek(): unknown value for 'whence'");
            }
            std::streamsize result;
            // Test if read/write streambuf.
            result = stream->pubseekoff(offset, dir);
            if (result != -1)
                return result;
            // Test if read-only streambuf.
            result = stream->pubseekoff(offset, dir, std::ios_base::in);
            if (result != -1)
                return result;
            // Test if write-only streambuf.
            result = stream->pubseekoff(offset, dir, std::ios_base::out);
            if (result != -1)
                return result;
            return SDL_Error(SDL_EFSEEK);
        }


        std::size_t
        streambuf_read(SDL_RWops* ctx,
                       void* buf,
                       std::size_t elem_size,
                       std::size_t count)
            noexcept
        {
            auto stream = reinterpret_cast<std::streambuf*>(ctx->hidden.unknown.data1);
            auto size_bytes = elem_size * count;
            std::size_t r = stream->sgetn(static_cast<char*>(buf), size_bytes);
            // Note: EOF is not an error.
            // if (size_bytes > 0 && r == 0)
            //     SDL_SetError("streambuf_read(): sgetn() failed");
            return r / elem_size;
        }


        std::size_t
        streambuf_write(SDL_RWops* ctx,
                        const void* buf,
                        std::size_t elem_size,
                        std::size_t count)
            noexcept
        {
            auto stream = reinterpret_cast<std::streambuf*>(ctx->hidden.unknown.data1);
            auto size_bytes = elem_size * count;
            std::size_t w = stream->sputn(static_cast<const char*>(buf), size_bytes);
            if (size_bytes > 0 && w == 0)
                SDL_SetError("streambuf_write(): sputn() failed");
            return w / elem_size;
        }


        int
        streambuf_close(SDL_RWops* ctx)
            noexcept
        {
            auto stream = reinterpret_cast<std::streambuf*>(ctx->hidden.unknown.data1);
            SDL_FreeRW(ctx);
            int status = 0;
            try {
                status = stream->pubsync();
            }
            catch (std::exception& e) {
                status = SDL_SetError(e.what());
            }
            return status;
        }

    } // namespace



    rwops::rwops(const path& filename,
                 const char* mode)
    {
        create(filename, mode);
    }


    rwops::rwops(FILE* fp,
                 bool close_fp)
    {
        create(fp, close_fp);
    }


    rwops::rwops(void* mem,
                 int size)
    {
        create(mem, size);
    }


    rwops::rwops(const void* mem,
                 int size)
    {
        create(mem, size);
    }


    rwops::rwops(std::streambuf& stream)
    {
        create(stream);
    }


    rwops::rwops(std::streambuf* stream)
    {
        create(stream);
    }


    rwops::~rwops()
        noexcept
    {
        destroy();
    }


    void
    rwops::create(const path& filename,
                  const char* mode)
    {
        auto new_raw = SDL_RWFromFile(filename.c_str(), mode);
        if (!new_raw)
            throw error{};
        destroy();
        acquire(new_raw);
    }


    void
    rwops::create(FILE* fp,
                  bool close_fp)
    {
        auto new_raw = SDL_RWFromFP(fp, close_fp ? SDL_TRUE : SDL_FALSE);
        if (!new_raw)
            throw error{};
        destroy();
        acquire(new_raw);
    }


    void
    rwops::create(void* mem,
                  int size)
    {
        auto new_raw = SDL_RWFromMem(mem, size);
        if (!new_raw)
            throw error{};
        destroy();
        acquire(new_raw);
    }


    void
    rwops::create(const void* mem,
                  int size)
    {
        auto new_raw = SDL_RWFromConstMem(mem, size);
        if (!new_raw)
            throw error{};
        destroy();
        acquire(new_raw);
    }


    void
    rwops::create(std::streambuf& stream)
    {
        create(&stream);
    }


    void
    rwops::create(std::streambuf* stream)
    {
        auto new_raw = SDL_AllocRW();
        if (!new_raw)
            throw error{};

        new_raw->size = streambuf_size;
        new_raw->seek = streambuf_seek;
        new_raw->read = streambuf_read;
        new_raw->write = streambuf_write;
        new_raw->close = streambuf_close;
        new_raw->hidden.unknown.data1 = stream;

        destroy();
        acquire(new_raw);
    }


    void
    rwops::destroy()
        noexcept
    {
        if (is_valid())
            SDL_RWclose(release());
    }


    Sint64
    rwops::get_size()
        const
    {
        auto result = try_get_size();
        if (!result)
            throw result.error();
        return *result;
    }


    expected<Sint64, error>
    rwops::try_get_size()
        const noexcept
    {
        auto sz = SDL_RWsize(raw);
        if (sz < -1)
            return unexpected{error{}};
        return sz;
    }


    Sint64
    rwops::seek(Sint64 offset,
                seekdir whence)
    {
        auto result = try_seek(offset, whence);
        if (!result)
            throw result.error();
        return *result;
    }


    Sint64
    rwops::seek(Sint64 offset,
                std::ios_base::seekdir whence)
    {
        auto result = try_seek(offset, whence);
        if (!result)
            throw result.error();
        return *result;
    }


    expected<Sint64, error>
    rwops::try_seek(Sint64 offset,
                    seekdir whence)
        noexcept
    {
        auto result = SDL_RWseek(raw, offset, convert(whence));
        if (result < 0)
            return unexpected{error{}};
        return result;
    }


    namespace {

        rwops::seekdir
        convert(std::ios_base::seekdir x)
            noexcept
        {
            switch (x) {
                case std::ios_base::beg:
                    return rwops::seekdir::beg;
                case std::ios_base::cur:
                    return rwops::seekdir::cur;
                case std::ios_base::end:
                    return rwops::seekdir::end;
                default:
                    return rwops::seekdir::beg;
            }
        }

    }


    expected<Sint64, error>
    rwops::try_seek(Sint64 offset,
                    std::ios_base::seekdir whence)
        noexcept
    {
        return try_seek(offset, convert(whence));
    }


    Sint64
    rwops::tell()
        const noexcept
    {
        return SDL_RWtell(raw);
    }


    std::size_t
    rwops::read(void* buf,
                std::size_t elem_size,
                std::size_t count)
    {
        auto result = try_read(buf, elem_size, count);
        if (!result)
            throw result.error();
        return *result;
    }


    expected<std::size_t, error>
    rwops::try_read(void* buf,
                    std::size_t elem_size,
                    std::size_t count)
        noexcept
    {
        auto r = SDL_RWread(raw, buf, elem_size, count);
        if (r < 1) // TODO: should EOF be error?
            return unexpected{error{}};
        return r;
    }


    std::size_t
    rwops::write(const void* buf,
                 std::size_t elem_size,
                 std::size_t count)
    {
        auto result = try_write(buf, elem_size, count);
        if (!result)
            throw result.error();
        return *result;
    }


    std::expected<std::size_t, error>
    rwops::try_write(const void* buf,
                     std::size_t elem_size,
                     std::size_t count)
        noexcept
    {
        auto w = SDL_RWwrite(raw, buf, elem_size, count);
        if (w < count) // TODO: even with error, maybe return w?
            return unexpected{error{}};
        return w;
    }


    blob
    rwops::load()
    {
        auto result = try_load();
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<blob, error>
    rwops::try_load()
        noexcept
    {
        std::size_t sz;
        auto ptr = SDL_LoadFile_RW(raw, &sz, false);
        if (!ptr)
            return unexpected{error{}};
        return blob{reinterpret_cast<Uint8*>(ptr), sz};
    }


    Uint8
    rwops::read_u8()
    {
        auto result = try_read_u8();
        if (!result)
            throw result.error();
        return *result;
    }


    expected<Uint8, error>
    rwops::try_read_u8()
        noexcept
    {
        SDL_ClearError();
        auto r = SDL_ReadU8(raw);
        if (SDL_GetError()[0])
            return unexpected{error{}};
        return r;
    }


    template<>
    expected<Uint8, error>
    rwops::try_read_le<Uint8>()
        noexcept
    {
        return try_read_u8();
    }


    template<>
    expected<Uint8, error>
    rwops::try_read_be<Uint8>()
        noexcept
    {
        return try_read_u8();
    }


    template<>
    expected<Sint8, error>
    rwops::try_read_le<Sint8>()
        noexcept
    {
        return try_read_u8();
    }


    template<>
    expected<Sint8, error>
    rwops::try_read_be<Sint8>()
        noexcept
    {
        return try_read_u8();
    }


    template<>
    expected<Uint16, error>
    rwops::try_read_le<Uint16>()
        noexcept
    {
        SDL_ClearError();
        auto x = SDL_ReadLE16(raw);
        if (SDL_GetError()[0])
            return unexpected{error{}};
        return x;
    }


    template<>
    expected<Uint16, error>
    rwops::try_read_be<Uint16>()
        noexcept
    {
        SDL_ClearError();
        auto x = SDL_ReadBE16(raw);
        if (SDL_GetError()[0])
            return unexpected{error{}};
        return x;
    }


    template<>
    expected<Sint16, error>
    rwops::try_read_le<Sint16>()
        noexcept
    {
        return try_read_le<Uint16>();
    }


    template<>
    expected<Sint16, error>
    rwops::try_read_be<Sint16>()
        noexcept
    {
        return try_read_be<Uint16>();
    }


    template<>
    expected<Uint32, error>
    rwops::try_read_le<Uint32>()
        noexcept
    {
        SDL_ClearError();
        auto x = SDL_ReadLE32(raw);
        if (SDL_GetError()[0])
            return unexpected{error{}};
        return x;
    }


    template<>
    expected<Uint32, error>
    rwops::try_read_be<Uint32>()
        noexcept
    {
        SDL_ClearError();
        auto x = SDL_ReadBE32(raw);
        if (SDL_GetError()[0])
            return unexpected{error{}};
        return x;
    }


    template<>
    expected<Sint32, error>
    rwops::try_read_le<Sint32>()
        noexcept
    {
        return try_read_le<Uint32>();
    }


    template<>
    expected<Sint32, error>
    rwops::try_read_be<Sint32>()
        noexcept
    {
        return try_read_be<Uint32>();
    }


    template<>
    expected<Uint64, error>
    rwops::try_read_le<Uint64>()
        noexcept
    {
        SDL_ClearError();
        auto x = SDL_ReadLE64(raw);
        if (SDL_GetError()[0])
            return unexpected{error{}};
        return x;
    }


    template<>
    expected<Uint64, error>
    rwops::try_read_be<Uint64>()
        noexcept
    {
        SDL_ClearError();
        auto x = SDL_ReadBE64(raw);
        if (SDL_GetError()[0])
            return unexpected{error{}};
        return x;
    }


    template<>
    expected<Sint64, error>
    rwops::try_read_le<Sint64>()
        noexcept
    {
        return try_read_le<Uint64>();
    }


    template<>
    expected<Sint64, error>
    rwops::try_read_be<Sint64>()
        noexcept
    {
        return try_read_be<Uint64>();
    }



    void
    rwops::write_u8(Uint8 value)
    {
        auto result = try_write_u8(value);
        if (!result)
            throw result.error();
    }


    expected<void, error>
    rwops::try_write_u8(Uint8 value)
        noexcept
    {
        if (!SDL_WriteU8(raw, value))
            return unexpected{error{}};
        return {};
    }


    template<>
    expected<void, error>
    rwops::try_write_le<Uint8>(Uint8 value)
        noexcept
    {
        return try_write_u8(value);
    }


    template<>
    expected<void, error>
    rwops::try_write_be<Uint8>(Uint8 value)
        noexcept
    {
        return try_write_u8(value);
    }


    template<>
    expected<void, error>
    rwops::try_write_le<Sint8>(Sint8 value)
        noexcept
    {
        return try_write_u8(value);
    }


    template<>
    expected<void, error>
    rwops::try_write_be<Sint8>(Sint8 value)
        noexcept
    {
        return try_write_u8(value);
    }


    template<>
    expected<void, error>
    rwops::try_write_le<Uint16>(Uint16 value)
        noexcept
    {
        if (!SDL_WriteLE16(raw, value))
            return unexpected{error{}};
        return {};
    }


    template<>
    expected<void, error>
    rwops::try_write_be<Uint16>(Uint16 value)
        noexcept
    {
        if (!SDL_WriteBE16(raw, value))
            return unexpected{error{}};
        return {};
    }


    template<>
    expected<void, error>
    rwops::try_write_le<Sint16>(Sint16 value)
        noexcept
    {
        if (!SDL_WriteLE16(raw, value))
            return unexpected{error{}};
        return {};
    }


    template<>
    expected<void, error>
    rwops::try_write_be<Sint16>(Sint16 value)
        noexcept
    {
        if (!SDL_WriteBE16(raw, value))
            return unexpected{error{}};
        return {};
    }


    template<>
    expected<void, error>
    rwops::try_write_le<Uint32>(Uint32 value)
        noexcept
    {
        if (!SDL_WriteLE32(raw, value))
            return unexpected{error{}};
        return {};
    }


    template<>
    expected<void, error>
    rwops::try_write_be<Uint32>(Uint32 value)
        noexcept
    {
        if (!SDL_WriteBE32(raw, value))
            return unexpected{error{}};
        return {};
    }


    template<>
    expected<void, error>
    rwops::try_write_le<Sint32>(Sint32 value)
        noexcept
    {
        if (!SDL_WriteLE32(raw, value))
            return unexpected{error{}};
        return {};
    }


    template<>
    expected<void, error>
    rwops::try_write_be<Sint32>(Sint32 value)
        noexcept
    {
        if (!SDL_WriteBE32(raw, value))
            return unexpected{error{}};
        return {};
    }


    template<>
    expected<void, error>
    rwops::try_write_le<Uint64>(Uint64 value)
        noexcept
    {
        if (!SDL_WriteLE64(raw, value))
            return unexpected{error{}};
        return {};
    }


    template<>
    expected<void, error>
    rwops::try_write_be<Uint64>(Uint64 value)
        noexcept
    {
        if (!SDL_WriteBE64(raw, value))
            return unexpected{error{}};
        return {};
    }


    template<>
    expected<void, error>
    rwops::try_write_le<Sint64>(Sint64 value)
        noexcept
    {
        if (!SDL_WriteLE64(raw, value))
            return unexpected{error{}};
        return {};
    }


    template<>
    expected<void, error>
    rwops::try_write_be<Sint64>(Sint64 value)
        noexcept
    {
        if (!SDL_WriteBE64(raw, value))
            return unexpected{error{}};
        return {};
    }


    blob
    load_file(const path& filename)
    {
        auto result = try_load_file(filename);
        if (!result)
            throw result.error();
        return std::move(*result);
    }


    expected<blob, error>
    try_load_file(const path& filename)
        noexcept
    {
        std::size_t sz;
        auto ptr = SDL_LoadFile(filename.c_str(), &sz);
        if (!ptr)
            return unexpected{error{}};
        return blob{reinterpret_cast<Uint8*>(ptr), sz};
    }


} // namespace sdl
