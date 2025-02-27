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
 *  Decodes music and external sounds
 */

#include "Decoder.hpp"
#include "SndfileDecoder.hpp"
#include "FFmpegDecoder.hpp"
#include <memory>

using std::unique_ptr;

unique_ptr<StreamDecoder> StreamDecoder::Get(FileSpecifier& File)
{
	unique_ptr<SndfileDecoder> sndfileDecoder(std::make_unique<SndfileDecoder>());
	if (sndfileDecoder->Open(File))
		return sndfileDecoder;

#ifdef HAVE_FFMPEG
	{
		unique_ptr<FFmpegDecoder> ffmpegDecoder(std::make_unique<FFmpegDecoder>());
		if (ffmpegDecoder->Open(File))
			return ffmpegDecoder;
	}
#endif

	return 0;
}

Decoder* Decoder::Get(FileSpecifier& File)
{
	unique_ptr<SndfileDecoder> sndfileDecoder(std::make_unique<SndfileDecoder>());
	if (sndfileDecoder->Open(File))
		return sndfileDecoder.release();

	return 0;
}
