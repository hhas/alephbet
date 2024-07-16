#ifndef STARGAMEPROTOCOL_H
#define STARGAMEPROTOCOL_H

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
 *  Interface between the star-topology game protocol module and the rest of the code.
 */

#include "NetworkGameProtocol.h"

#include <stdio.h>

class InfoTree;

class StarGameProtocol : public NetworkGameProtocol
{
public:
	bool	Enter(short* inNetStatePtr);
	void	Exit1();
	void	Exit2();
	void	DistributeInformation(short type, void *buffer, short buffer_size, bool send_to_self, bool only_send_to_team);
	bool	Sync(NetTopology* inTopology, int32 inSmallestGameTick, int inLocalPlayerIndex, int inServerPlayerIndex);
	bool	UnSync(bool inGraceful, int32 inSmallestPostgameTick);
	int32	GetNetTime();
	void	PacketHandler(DDPPacketBuffer* inPacket);

	static void ParsePreferencesTree(InfoTree prefs, std::string version);

	int32   GetUnconfirmedActionFlagsCount();
	uint32  PeekUnconfirmedActionFlag(int32 offset);
	void    UpdateUnconfirmedActionFlags();

	bool CheckWorldUpdate() override;
};

extern void DefaultStarPreferences();
InfoTree StarPreferencesTree();

#endif // STARGAMEPROTOCOL_H
