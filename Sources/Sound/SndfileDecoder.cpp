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
 *  Handles sound files with libsndfile
 */

#include "SndfileDecoder.hpp"

// these should really probably use the OpenedFile abstraction rather
// than SDL_RWops? but seek would be more complex and I'm so lazy

static sf_count_t sfd_get_filelen(void* pv) {
    SDL_RWops* rwops = reinterpret_cast<SDL_RWops*>(pv);
    int pos          = SDL_RWtell(rwops);
    SDL_RWseek(rwops, 0, SEEK_END);
    sf_count_t len = SDL_RWtell(rwops);
    SDL_RWseek(rwops, pos, SEEK_SET);
    return len;
}

static sf_count_t sfd_seek(sf_count_t offset, int whence, void* pv) {
    return SDL_RWseek(reinterpret_cast<SDL_RWops*>(pv), offset, whence);
}

static sf_count_t sfd_read(void* ptr, sf_count_t count, void* pv) {
    return SDL_RWread(reinterpret_cast<SDL_RWops*>(pv), ptr, 1, count);
}

static sf_count_t sfd_write(const void* ptr, sf_count_t count, void* pv) {
    return SDL_RWwrite(reinterpret_cast<SDL_RWops*>(pv), ptr, 1, count);
}

static sf_count_t sfd_tell(void* pv) { return SDL_RWtell(reinterpret_cast<SDL_RWops*>(pv)); }

static SF_VIRTUAL_IO sf_virtual = {&sfd_get_filelen, &sfd_seek, &sfd_read, &sfd_write, &sfd_tell};

SndfileDecoder::SndfileDecoder() : sndfile(0), rwops(0) {}

SndfileDecoder::~SndfileDecoder() { Close(); }

bool SndfileDecoder::Open(FileSpecifier& File) {
    Close();

    sfinfo.format = 0;
    OpenedFile openedFile;
    if (File.Open(openedFile)) {
        rwops   = openedFile.TakeRWops();
        sndfile = sf_open_virtual(&sf_virtual, SFM_READ, &sfinfo, rwops);
    }

    return sndfile;
}

int32 SndfileDecoder::Decode(uint8* buffer, int32 max_length) {
    if (!sndfile)
        return 0;

    return sf_read_float(sndfile, (float*)buffer, max_length / 4) * 4;
}

void SndfileDecoder::Rewind() {
    if (!sndfile)
        return;
    sf_seek(sndfile, 0, SEEK_SET);
}

void SndfileDecoder::Close() {
    if (sndfile) {
        sf_close(sndfile);
        sndfile = 0;
    }

    if (rwops) {
        SDL_RWclose(rwops);
        rwops = 0;
    }
}
