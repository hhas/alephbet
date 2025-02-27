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
 *  A circular queue of bytes, with support for mass enqueueing from/peeking to
 *  caller's buffer
 */

#include "cseries.hpp" // assert()

#include "CircularByteBuffer.hpp"

#include <algorithm> // std::min()

// static
std::pair<unsigned int, unsigned int>
CircularByteBuffer::splitIntoChunks(unsigned int inByteCount, unsigned int inStartingIndex, unsigned int inQueueSize)
{
	// Copy, potentially, two separate chunks (one at end of buffer; one at beginning)
	unsigned int theSpaceAtEndOfBuffer = inQueueSize - inStartingIndex;
	unsigned int theFirstChunkSize = std::min(inByteCount, theSpaceAtEndOfBuffer);
	unsigned int theSecondChunkSize = inByteCount - theFirstChunkSize;

	return std::pair<unsigned int, unsigned int>(theFirstChunkSize, theSecondChunkSize);
}


void
CircularByteBuffer::enqueueBytes(const void* inBytes, unsigned int inByteCount)
{
	// I believe everything works right without this check, but it makes me feel safer anyway.
	if(inByteCount > 0)
	{
		assert(inByteCount <= getRemainingSpace());
	
		const char* theBytes = static_cast<const char*>(inBytes);
	
		std::pair<unsigned int, unsigned int> theChunkSizes = splitIntoChunks(inByteCount, mWriteIndex, mQueueSize);
	
		memcpy(&(mData[mWriteIndex]), theBytes, theChunkSizes.first);
	
		if(theChunkSizes.second > 0)
			memcpy(mData, &(theBytes[theChunkSizes.first]), theChunkSizes.second);
	
		advanceWriteIndex(inByteCount);
	}
}


void
CircularByteBuffer::enqueueBytesNoCopyStart(unsigned int inByteCount, void** outFirstBytes, unsigned int* outFirstByteCount,
			      void** outSecondBytes, unsigned int* outSecondByteCount)
{
	void* theFirstBytes = NULL;
	void* theSecondBytes = NULL;
	unsigned int theFirstByteCount = 0;
	unsigned int theSecondByteCount = 0;
	
	if(inByteCount > 0)
	{
		assert(inByteCount <= getRemainingSpace());

		std::pair<unsigned int, unsigned int> theChunkSizes = splitIntoChunks(inByteCount, mWriteIndex, mQueueSize);

		theFirstBytes = &(mData[getWriteIndex()]);
		theFirstByteCount = theChunkSizes.first;
		theSecondByteCount = theChunkSizes.second;
		theSecondBytes = (theSecondByteCount > 0) ? mData : NULL;
	}

	if(outFirstBytes != NULL)
		*outFirstBytes = theFirstBytes;

	if(outFirstByteCount != NULL)
		*outFirstByteCount = theFirstByteCount;

	if(outSecondBytes != NULL)
		*outSecondBytes = theSecondBytes;

	if(outSecondByteCount != NULL)
		*outSecondByteCount = theSecondByteCount;
}


void
CircularByteBuffer::enqueueBytesNoCopyFinish(unsigned int inActualByteCount)
{
	if(inActualByteCount > 0)
		advanceWriteIndex(inActualByteCount);
}


void
CircularByteBuffer::peekBytes(void* outBytes, unsigned int inByteCount)
{
	// I believe everything works right without this check, but it makes me feel safer anyway.
	if(inByteCount > 0)
	{
		assert(inByteCount <= getCountOfElements());

		char* theBytes = static_cast<char*>(outBytes);
	
		std::pair<unsigned int, unsigned int> theChunkSizes = splitIntoChunks(inByteCount, mReadIndex, mQueueSize);
	
		memcpy(theBytes, &(mData[mReadIndex]), theChunkSizes.first);
	
		if(theChunkSizes.second > 0)
			memcpy(&(theBytes[theChunkSizes.first]), mData, theChunkSizes.second);
	}
}


void
CircularByteBuffer::peekBytesNoCopy(unsigned int inByteCount, const void** outFirstBytes, unsigned int* outFirstByteCount,
				    const void** outSecondBytes, unsigned int* outSecondByteCount)
{
	void* theFirstBytes = NULL;
	void* theSecondBytes = NULL;
	unsigned int theFirstByteCount = 0;
	unsigned int theSecondByteCount = 0;
	
	if(inByteCount > 0)
	{
		assert(inByteCount <= getCountOfElements());

		std::pair<unsigned int, unsigned int> theChunkSizes = splitIntoChunks(inByteCount, mReadIndex, mQueueSize);

		theFirstBytes = &(mData[getReadIndex()]);
		theFirstByteCount = theChunkSizes.first;
		theSecondByteCount = theChunkSizes.second;
		theSecondBytes = (theSecondByteCount > 0) ? mData : NULL;
	}

	if(outFirstBytes != NULL)
		*outFirstBytes = theFirstBytes;

	if(outFirstByteCount != NULL)
		*outFirstByteCount = theFirstByteCount;

	if(outSecondBytes != NULL)
		*outSecondBytes = theSecondBytes;

	if(outSecondByteCount != NULL)
		*outSecondByteCount = theSecondByteCount;	
}
