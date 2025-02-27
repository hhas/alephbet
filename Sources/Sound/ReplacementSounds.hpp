#ifndef __REPLACEMENTSOUNDS_H
#define __REPLACEMENTSOUNDS_H

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
 *  Handles MML-specified external replacement sounds
 */

#include <string>
#include <unordered_map>

#include "SoundFile.hpp"
#include "PairOfShortsHash.hpp"

class ExternalSoundHeader : public SoundInfo
{
public:
	ExternalSoundHeader() : SoundInfo() { }
	~ExternalSoundHeader() { }
	std::shared_ptr<SoundData> LoadExternal(FileSpecifier& File);
};

struct SoundOptions
{
	FileSpecifier File;
	ExternalSoundHeader Sound;
};

class SoundReplacements
{
public:
	static inline SoundReplacements* instance() { 
		static SoundReplacements *m_instance = nullptr;
		if (!m_instance) m_instance = new SoundReplacements;
		return m_instance;
	}

	SoundOptions *GetSoundOptions(short Index, short Slot);
	void Reset();
	void Add(const SoundOptions& Data, short Index, short Slot);

private:
	SoundReplacements() { }

	std::unordered_map<std::pair<short, short>, SoundOptions> m_hash;
};

#endif
