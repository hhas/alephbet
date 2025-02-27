#ifndef __DECODER_H
#define __DECODER_H

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

#include <memory>

#include "cseries.hpp"
#include "FileHandler.hpp"
#include "SoundManagerEnums.hpp"

class StreamDecoder
{
public:
	virtual bool Open(FileSpecifier &File) = 0;
	virtual int32 Decode(uint8* buffer, int32 max_length) = 0;
	virtual void Rewind() = 0;
	virtual void Close() = 0;

	virtual AudioFormat GetAudioFormat() = 0;
	virtual bool IsStereo() = 0;
	virtual int BytesPerFrame() = 0;
	virtual float Rate() = 0;
	virtual bool IsLittleEndian() = 0;

	StreamDecoder() { }
	virtual ~StreamDecoder() { }

	static std::unique_ptr<StreamDecoder> Get(FileSpecifier &File); // can return 0
};

class Decoder : public StreamDecoder
{
public:
	Decoder() : StreamDecoder() { }
	~Decoder() { }

	// total number of frames in the file
	virtual int32 Frames() = 0; 

	static Decoder* Get(FileSpecifier &File); // can return 0
};

#endif
