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

#include "MusicPlayer.h"
#include "OpenALManager.h"

std::atomic<float> MusicPlayer::default_volume = { 1 };
MusicPlayer::MusicPlayer(std::shared_ptr<StreamDecoder> decoder, MusicParameters parameters) : AudioPlayer(decoder->Rate(), decoder->IsStereo(), decoder->GetAudioFormat()) {
	this->decoder = decoder;
	this->parameters = parameters;
	this->decoder->Rewind();
}

int MusicPlayer::GetNextData(uint8* data, int length) {
	int dataSize = decoder->Decode(data, length);
	if (dataSize == length || !parameters.Get().loop) return dataSize;
	decoder->Rewind();
	return dataSize + GetNextData(data + dataSize, length - dataSize);
}

SetupALResult MusicPlayer::SetUpALSourceIdle() {
	float default_music_volume = default_volume * OpenALManager::Get()->GetMasterVolume();
	alSourcef(audio_source->source_id, AL_MAX_GAIN, default_music_volume);
	alSourcef(audio_source->source_id, AL_GAIN, default_music_volume * parameters.Get().volume);
	return SetupALResult(alGetError() == AL_NO_ERROR, true);
}