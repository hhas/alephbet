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

#include "ReplacementSounds.hpp"
#include "Decoder.hpp"
#include "SoundManager.hpp"

std::shared_ptr<SoundData> ExternalSoundHeader::LoadExternal(FileSpecifier& File) {
    std::shared_ptr<SoundData> p;
    std::unique_ptr<Decoder> decoder(Decoder::Get(File));
    if (!decoder.get())
        return p;

    length = decoder->Frames() * decoder->BytesPerFrame();
    if (!length)
        return p;

    p = std::make_shared<SoundData>(length);

    decoder->Rewind();
    if (decoder->Decode(&(*p)[0], length) != length) {
        p.reset();
        length = 0;
        return p;
    }

    audio_format    = decoder->GetAudioFormat();
    stereo          = decoder->IsStereo();
    bytes_per_frame = decoder->BytesPerFrame();
    little_endian   = decoder->IsLittleEndian();
    loop_start = loop_end = 0;
    rate                  = (uint32 /* unsigned fixed */)(FIXED_ONE * decoder->Rate());

    return p;
}

SoundOptions* SoundReplacements::GetSoundOptions(short Index, short Slot) {
    auto it = m_hash.find(std::make_pair(Index, Slot));
    if (it != m_hash.end()) {
        return &it->second;
    } else {
        return 0;
    }
}

void SoundReplacements::Add(const SoundOptions& Data, short Index, short Slot) {
    SoundManager::instance()->UnloadSound(Index);
    m_hash[std::make_pair(Index, Slot)] = Data;
}

void SoundReplacements::Reset() {
    for (auto kvp : m_hash) { SoundManager::instance()->UnloadSound(kvp.first.first); }
    m_hash.clear();
}
