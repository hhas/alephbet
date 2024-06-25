#ifndef __FFMPEGDECODER_H
#define __FFMPEGDECODER_H

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
 *  Decodes sound files with lbav/ffmpeg
 */

#include "Decoder.h"

#ifdef HAVE_FFMPEG

class FFmpegDecoder : public StreamDecoder
{
public:
	bool Open(FileSpecifier& File);
	int32 Decode(uint8* buffer, int32 max_length);
	void Rewind();
	void Close();

	AudioFormat GetAudioFormat() { return AudioFormat::_32_float; }
	bool IsStereo() { return (channels == 2); }
	int BytesPerFrame() { return 4 * (IsStereo() ? 2 : 1); }
	float Rate() { return rate; }
	bool IsLittleEndian() { return PlatformIsLittleEndian(); }

	FFmpegDecoder();
	~FFmpegDecoder();
private:
	struct ffmpeg_vars *av;

	bool GetAudio();
	
	float rate;
	int channels;
};

#endif

#endif
