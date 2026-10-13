/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025-2026  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <streambuf>

#include <SDL_error.h>

#include "sdl2xx/rwops.hpp"

#include "impl/value_or_throw.hpp"


using sdl::impl::value_or_throw;


namespace sdl {

    namespace {

        /*-----------------------*/
        /* Function declarations */
        /*-----------------------*/

        int
        streambuf_close(SDL_RWops* ctx)
            noexcept;

        std::size_t
        streambuf_read(SDL_RWops* ctx,
                       void* buf,
                       std::size_t elem_size,
                       std::size_t count)
            noexcept;

        Sint64
        streambuf_seek(SDL_RWops* ctx,
                       Sint64 offset,
                       int whence)
            noexcept;

        Sint64
        streambuf_size(SDL_RWops* ctx)
            noexcept;

        std::size_t
        streambuf_write(SDL_RWops* ctx,
                        const void* buf,
                        std::size_t elem_size,
                        std::size_t count)
            noexcept;

        rwops::seekdir
        to_sdl_seekdir(std::ios::seekdir dir)
            noexcept;


        /*----------------------*/
        /* Function definitions */
        /*----------------------*/

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


        std::size_t
        streambuf_read(SDL_RWops* ctx,
                       void* buf,
                       std::size_t elem_size,
                       std::size_t count)
            noexcept
        {
            auto stream = reinterpret_cast<std::streambuf*>(ctx->hidden.unknown.data1);
            const auto request_size = elem_size * count;
            const auto read_size = stream->sgetn(static_cast<char*>(buf), request_size);
            // NOTE: EOF is not reported as an error.
            if (read_size <= 0)
                return 0;
            const auto complete = read_size / elem_size;
            const auto complete_size = complete * elem_size;
            if (std::cmp_greater(read_size, complete_size)) {
                // handle partial reads
                auto partial_size = read_size - complete_size;
                auto new_pos = stream->pubseekoff(
                    -static_cast<std::streambuf::off_type>(partial_size),
                    std::ios::cur,
                    std::ios::in
                );
                if (new_pos == std::streambuf::pos_type(-1)) {
                    SDL_SetError("streambuf_read(): could not undo partial read");
                    return complete;
                }
            }
            return complete;
        }


        Sint64
        streambuf_seek(SDL_RWops* ctx,
                       Sint64 offset,
                       int whence)
            noexcept
        {
            const auto invalid_pos = std::streambuf::pos_type(-1);
            auto stream = reinterpret_cast<std::streambuf*>(ctx->hidden.unknown.data1);
            std::ios::seekdir dir;
            switch (whence) {
                case RW_SEEK_SET:
                    dir = std::ios::beg;
                    break;
                case RW_SEEK_CUR:
                    dir = std::ios::cur;
                    break;
                case RW_SEEK_END:
                    dir = std::ios::end;
                    break;
                default:
                    return SDL_SetError("streambuf_seek(): unknown value for 'whence'");
            }
            std::streamsize result;
            // Test if read/write streambuf.
            result = stream->pubseekoff(offset, dir);
            if (result != invalid_pos)
                return result;
            // Test if read-only streambuf.
            result = stream->pubseekoff(offset, dir, std::ios::in);
            if (result != invalid_pos)
                return result;
            // Test if write-only streambuf.
            result = stream->pubseekoff(offset, dir, std::ios::out);
            if (result != invalid_pos)
                return result;
            return SDL_Error(SDL_EFSEEK);
        }


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


        std::size_t
        streambuf_write(SDL_RWops* ctx,
                        const void* buf,
                        std::size_t elem_size,
                        std::size_t count)
            noexcept
        {
            if (elem_size == 0 || count == 0)
                return 0;
            auto stream = reinterpret_cast<std::streambuf*>(ctx->hidden.unknown.data1);
            auto request_size = elem_size * count;
            auto written_size = stream->sputn(static_cast<const char*>(buf),
                                              request_size);
            if (written_size <= 0) {
                SDL_SetError("streambuf_write(): sputn() failed");
                return 0;
            }

            const auto complete = written_size / elem_size;
            const auto complete_size = complete * elem_size;

            if (std::cmp_greater(written_size, complete_size))
                SDL_SetError("streambuf_write(): partial write");

            return complete;
        }


        rwops::seekdir
        to_sdl_seekdir(std::ios::seekdir dir)
            noexcept
        {
            switch (dir) {
                case std::ios::beg:
                    return rwops::seekdir::beg;
                case std::ios::cur:
                    return rwops::seekdir::cur;
                case std::ios::end:
                    return rwops::seekdir::end;
                default:
                    return rwops::seekdir::beg;
            }
        }

    } // namespace


    /*------------------*/
    /* Public functions */
    /*------------------*/

    rwops::rwops(const std::filesystem::path& filename,
                 const char* mode)
    {
        create(filename, mode);
    }


    rwops::rwops(FILE* fp,
                 bool close_fp)
    {
        create(fp, close_fp);
    }


    rwops::rwops(std::span<std::byte> content)
    {
        create(content);
    }


    rwops::rwops(std::span<const std::byte> content)
    {
        create(content);
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
    rwops::create(const std::filesystem::path& filename,
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
    rwops::create(std::span<std::byte> content)
    {
        auto new_raw = SDL_RWFromMem(content.data(),
                                     static_cast<int>(content.size()));
        if (!new_raw)
            throw error{};
        destroy();
        acquire(new_raw);
    }


    void
    rwops::create(std::span<const std::byte> content)
    {
        auto new_raw = SDL_RWFromConstMem(content.data(),
                                          static_cast<int>(content.size()));
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
        return value_or_throw(try_get_size());
    }


    std::expected<Sint64, error>
    rwops::try_get_size()
        const noexcept
    {
        auto sz = SDL_RWsize(raw);
        if (sz < -1)
            return std::unexpected{error{}};
        return sz;
    }


    Sint64
    rwops::seek(Sint64 offset,
                seekdir whence)
    {
        return value_or_throw(try_seek(offset, whence));
    }


    Sint64
    rwops::seek(Sint64 offset,
                std::ios::seekdir whence)
    {
        return value_or_throw(try_seek(offset, whence));
    }


    std::expected<Sint64, error>
    rwops::try_seek(Sint64 offset,
                    seekdir whence)
        noexcept
    {
        auto result = SDL_RWseek(raw, offset, std::to_underlying(whence));
        if (result < 0)
            return std::unexpected{error{}};
        return result;
    }


    std::expected<Sint64, error>
    rwops::try_seek(Sint64 offset,
                    std::ios::seekdir whence)
        noexcept
    {
        return try_seek(offset, to_sdl_seekdir(whence));
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
        return value_or_throw(try_read(buf, elem_size, count));
    }


    std::expected<std::size_t, error>
    rwops::try_read(void* buf,
                    std::size_t elem_size,
                    std::size_t count)
        noexcept
    {
        auto r = SDL_RWread(raw, buf, elem_size, count);
        if (r < 1) // TODO: should EOF be error?
            return std::unexpected{error{}};
        return r;
    }


    std::size_t
    rwops::write(const void* buf,
                 std::size_t elem_size,
                 std::size_t count)
    {
        return value_or_throw(try_write(buf, elem_size, count));
    }


    std::expected<std::size_t, error>
    rwops::try_write(const void* buf,
                     std::size_t elem_size,
                     std::size_t count)
        noexcept
    {
        auto w = SDL_RWwrite(raw, buf, elem_size, count);
        if (w < count) // TODO: even with error, maybe return w?
            return std::unexpected{error{}};
        return w;
    }


    blob
    rwops::load()
    {
        return value_or_throw(try_load());
    }


    std::expected<blob, error>
    rwops::try_load()
        noexcept
    {
        std::size_t sz;
        auto ptr = SDL_LoadFile_RW(raw, &sz, false);
        if (!ptr)
            return std::unexpected{error{}};
        return blob{reinterpret_cast<std::byte*>(ptr), sz};
    }


    Uint8
    rwops::read_u8()
    {
        return value_or_throw(try_read_u8());
    }


    std::expected<Uint8, error>
    rwops::try_read_u8()
        noexcept
    {
        SDL_ClearError();
        auto r = SDL_ReadU8(raw);
        if (SDL_GetError()[0])
            return std::unexpected{error{}};
        return r;
    }


    template<>
    std::expected<Uint8, error>
    rwops::try_read_le<Uint8>()
        noexcept
    {
        return try_read_u8();
    }


    template<>
    std::expected<Uint8, error>
    rwops::try_read_be<Uint8>()
        noexcept
    {
        return try_read_u8();
    }


    template<>
    std::expected<Sint8, error>
    rwops::try_read_le<Sint8>()
        noexcept
    {
        return try_read_u8();
    }


    template<>
    std::expected<Sint8, error>
    rwops::try_read_be<Sint8>()
        noexcept
    {
        return try_read_u8();
    }


    template<>
    std::expected<Uint16, error>
    rwops::try_read_le<Uint16>()
        noexcept
    {
        SDL_ClearError();
        auto x = SDL_ReadLE16(raw);
        if (SDL_GetError()[0])
            return std::unexpected{error{}};
        return x;
    }


    template<>
    std::expected<Uint16, error>
    rwops::try_read_be<Uint16>()
        noexcept
    {
        SDL_ClearError();
        auto x = SDL_ReadBE16(raw);
        if (SDL_GetError()[0])
            return std::unexpected{error{}};
        return x;
    }


    template<>
    std::expected<Sint16, error>
    rwops::try_read_le<Sint16>()
        noexcept
    {
        return try_read_le<Uint16>();
    }


    template<>
    std::expected<Sint16, error>
    rwops::try_read_be<Sint16>()
        noexcept
    {
        return try_read_be<Uint16>();
    }


    template<>
    std::expected<Uint32, error>
    rwops::try_read_le<Uint32>()
        noexcept
    {
        SDL_ClearError();
        auto x = SDL_ReadLE32(raw);
        if (SDL_GetError()[0])
            return std::unexpected{error{}};
        return x;
    }


    template<>
    std::expected<Uint32, error>
    rwops::try_read_be<Uint32>()
        noexcept
    {
        SDL_ClearError();
        auto x = SDL_ReadBE32(raw);
        if (SDL_GetError()[0])
            return std::unexpected{error{}};
        return x;
    }


    template<>
    std::expected<Sint32, error>
    rwops::try_read_le<Sint32>()
        noexcept
    {
        return try_read_le<Uint32>();
    }


    template<>
    std::expected<Sint32, error>
    rwops::try_read_be<Sint32>()
        noexcept
    {
        return try_read_be<Uint32>();
    }


    template<>
    std::expected<Uint64, error>
    rwops::try_read_le<Uint64>()
        noexcept
    {
        SDL_ClearError();
        auto x = SDL_ReadLE64(raw);
        if (SDL_GetError()[0])
            return std::unexpected{error{}};
        return x;
    }


    template<>
    std::expected<Uint64, error>
    rwops::try_read_be<Uint64>()
        noexcept
    {
        SDL_ClearError();
        auto x = SDL_ReadBE64(raw);
        if (SDL_GetError()[0])
            return std::unexpected{error{}};
        return x;
    }


    template<>
    std::expected<Sint64, error>
    rwops::try_read_le<Sint64>()
        noexcept
    {
        return try_read_le<Uint64>();
    }


    template<>
    std::expected<Sint64, error>
    rwops::try_read_be<Sint64>()
        noexcept
    {
        return try_read_be<Uint64>();
    }



    void
    rwops::write_u8(Uint8 value)
    {
        return value_or_throw(try_write_u8(value));
    }


    std::expected<void, error>
    rwops::try_write_u8(Uint8 value)
        noexcept
    {
        if (!SDL_WriteU8(raw, value))
            return std::unexpected{error{}};
        return {};
    }


    template<>
    std::expected<void, error>
    rwops::try_write_le<Uint8>(Uint8 value)
        noexcept
    {
        return try_write_u8(value);
    }


    template<>
    std::expected<void, error>
    rwops::try_write_be<Uint8>(Uint8 value)
        noexcept
    {
        return try_write_u8(value);
    }


    template<>
    std::expected<void, error>
    rwops::try_write_le<Sint8>(Sint8 value)
        noexcept
    {
        return try_write_u8(value);
    }


    template<>
    std::expected<void, error>
    rwops::try_write_be<Sint8>(Sint8 value)
        noexcept
    {
        return try_write_u8(value);
    }


    template<>
    std::expected<void, error>
    rwops::try_write_le<Uint16>(Uint16 value)
        noexcept
    {
        if (!SDL_WriteLE16(raw, value))
            return std::unexpected{error{}};
        return {};
    }


    template<>
    std::expected<void, error>
    rwops::try_write_be<Uint16>(Uint16 value)
        noexcept
    {
        if (!SDL_WriteBE16(raw, value))
            return std::unexpected{error{}};
        return {};
    }


    template<>
    std::expected<void, error>
    rwops::try_write_le<Sint16>(Sint16 value)
        noexcept
    {
        if (!SDL_WriteLE16(raw, value))
            return std::unexpected{error{}};
        return {};
    }


    template<>
    std::expected<void, error>
    rwops::try_write_be<Sint16>(Sint16 value)
        noexcept
    {
        if (!SDL_WriteBE16(raw, value))
            return std::unexpected{error{}};
        return {};
    }


    template<>
    std::expected<void, error>
    rwops::try_write_le<Uint32>(Uint32 value)
        noexcept
    {
        if (!SDL_WriteLE32(raw, value))
            return std::unexpected{error{}};
        return {};
    }


    template<>
    std::expected<void, error>
    rwops::try_write_be<Uint32>(Uint32 value)
        noexcept
    {
        if (!SDL_WriteBE32(raw, value))
            return std::unexpected{error{}};
        return {};
    }


    template<>
    std::expected<void, error>
    rwops::try_write_le<Sint32>(Sint32 value)
        noexcept
    {
        if (!SDL_WriteLE32(raw, value))
            return std::unexpected{error{}};
        return {};
    }


    template<>
    std::expected<void, error>
    rwops::try_write_be<Sint32>(Sint32 value)
        noexcept
    {
        if (!SDL_WriteBE32(raw, value))
            return std::unexpected{error{}};
        return {};
    }


    template<>
    std::expected<void, error>
    rwops::try_write_le<Uint64>(Uint64 value)
        noexcept
    {
        if (!SDL_WriteLE64(raw, value))
            return std::unexpected{error{}};
        return {};
    }


    template<>
    std::expected<void, error>
    rwops::try_write_be<Uint64>(Uint64 value)
        noexcept
    {
        if (!SDL_WriteBE64(raw, value))
            return std::unexpected{error{}};
        return {};
    }


    template<>
    std::expected<void, error>
    rwops::try_write_le<Sint64>(Sint64 value)
        noexcept
    {
        if (!SDL_WriteLE64(raw, value))
            return std::unexpected{error{}};
        return {};
    }


    template<>
    std::expected<void, error>
    rwops::try_write_be<Sint64>(Sint64 value)
        noexcept
    {
        if (!SDL_WriteBE64(raw, value))
            return std::unexpected{error{}};
        return {};
    }


    blob
    load_file(const std::filesystem::path& filename)
    {
        return value_or_throw(try_load_file(filename));
    }


    std::expected<blob, error>
    try_load_file(const std::filesystem::path& filename)
        noexcept
    {
        std::size_t sz;
        auto ptr = SDL_LoadFile(filename.c_str(), &sz);
        if (!ptr)
            return std::unexpected{error{}};
        return blob{reinterpret_cast<std::byte*>(ptr), sz};
    }

} // namespace sdl
