/*
 *
 *  Aleph Bet is copyright ©1994-2024 Bungie Inc., the Aleph One developers,
 *  and the Aleph Bet developers.
 *
 *  Aleph Bet is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Aleph Bet is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 *  This license notice applies only to the Aleph Bet engine itself, and
 *  does not apply to Marathon, Marathon 2, or Marathon Infinity scenarios
 *  and assets, nor to elements of any third-party scenarios.
 *
 */

/*
 *  SDL_rwops_ostream.cpp - create an SDL_RWops structure from an ostream
 */

#include "SDL_rwops_ostream.hpp"

static Sint64 stream_size(struct SDL_RWops* context) { return -1; }

static Sint64 stream_seek(struct SDL_RWops* context, Sint64 offset, int whence) {
    std::ostream* strm = static_cast<std::ostream*>(context->hidden.unknown.data1);
    switch (whence) {
        case RW_SEEK_SET:
            strm->seekp(offset, std::ios::beg);
            break;
        case RW_SEEK_CUR:
            strm->seekp(offset, std::ios::cur);
            break;
        case RW_SEEK_END:
            strm->seekp(offset, std::ios::end);
            break;
    }
    return strm->tellp();
}

static size_t stream_read(struct SDL_RWops* context, void* ptr, size_t size, size_t maxnum) { return 0; }

static size_t stream_write(struct SDL_RWops* context, const void* ptr, size_t size, size_t num) {
    std::ostream* strm = static_cast<std::ostream*>(context->hidden.unknown.data1);
    strm->write(static_cast<const char*>(ptr), size * num);
    if (strm->bad()) {
        return 0;
    } else {
        return num;
    }
}

static int stream_close(struct SDL_RWops* context) {
    if (context)
        SDL_FreeRW(context);
    return 0;
}

// the ostream must remain valid for RWops' lifetime
SDL_RWops* SDL_RWFromOStream(std::ostream& strm) {
    SDL_RWops* ops = SDL_AllocRW();
    if (ops) {
        ops->size                 = stream_size;
        ops->seek                 = stream_seek;
        ops->read                 = stream_read;
        ops->write                = stream_write;
        ops->close                = stream_close;
        ops->hidden.unknown.data1 = &strm;
    }
    return ops;
}
