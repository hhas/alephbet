#ifndef NETWORKGAMEPROTOCOL_H
#define NETWORKGAMEPROTOCOL_H

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
 *  Interface to a generic game protocol module.
 */

#include "network_private.h"

class NetworkGameProtocol
{
public:
	/* Distribute information to the whole net. */
	virtual bool	Enter(short* inNetStatePtr) = 0;
	virtual void	Exit1() = 0;
	virtual void	Exit2() = 0;
	virtual void	DistributeInformation(short type, void *buffer, short buffer_size, bool send_to_self, bool only_send_to_team) = 0;
	virtual bool	Sync(NetTopology* inTopology, int32 inSmallestGameTick, int inLocalPlayerIndex, int inServerPlayerIndex) = 0;
	virtual bool	UnSync(bool inGraceful, int32 inSmallestPostgameTick) = 0;
	virtual int32	GetNetTime() = 0;
	virtual void	PacketHandler(DDPPacketBuffer* inPacket) = 0;
	virtual		~NetworkGameProtocol() {}

	// action flags we can use for prediction, but aren't authoritative yet
	virtual int32   GetUnconfirmedActionFlagsCount() = 0;
	virtual uint32  PeekUnconfirmedActionFlag(int32 offset) = 0;
	virtual void    UpdateUnconfirmedActionFlags() = 0;

	virtual bool CheckWorldUpdate() = 0;
};

#endif // NETWORKGAMEPROTOCOL_H
