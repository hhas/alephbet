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

#ifndef __SDL__NETWORK_H
#define __SDL__NETWORK_H

#include <SDL2/SDL_net.h>
#include <string>

#include "cseries.h"

#include "network.h"


/* missing from AppleTalk.h */
// ZZZ: note that this determines only the amount of storage allocated for packets, not
// the size of actual packets sent.  I believe UDP on Ethernet should be able to carry
// around 1.5K per packet, not sure of the exact figure off the top of my head though.
// Note that the SDL network microphone code is the one sending "big" packets these days.
#define ddpMaxData 1500

typedef char NetEntityName[32];
typedef IPaddress NetAddrBlock;

/* ---------- DDPFrame and PacketBuffer (DDP) */

struct DDPFrame
{
	uint16 data_size;
	byte data[ddpMaxData];
	UDPsocket socket;
};
typedef struct DDPFrame DDPFrame, *DDPFramePtr;

struct DDPPacketBuffer
{
	byte protocolType;
	NetAddrBlock sourceAddress;
	
	uint16 datagramSize;
	byte datagramData[ddpMaxData];
};
typedef struct DDPPacketBuffer DDPPacketBuffer, *DDPPacketBufferPtr;

/* ---------- ConnectionEnd (ADSP) */

struct ConnectionEnd
{
	TCPsocket		socket;
        TCPsocket		server_socket;
        SDLNet_SocketSet	server_socket_set;
};
typedef struct ConnectionEnd ConnectionEnd, *ConnectionEndPtr;

/* ---------- types */

typedef NetEntityName *NetEntityNamePtr;

typedef void (*lookupUpdateProcPtr)(short message, short index);
typedef bool (*lookupFilterProcPtr)(NetEntityName *entity, NetAddrBlock *address);
typedef void (*PacketHandlerProcPtr)(DDPPacketBufferPtr packet);

/* ---------- prototypes/NETWORK.C */

short NetState(void);
std::string NetSessionIdentifier(void);

void NetSetServerIdentifier(short identifier);

/* for giving to NetLookupOpen() as a filter procedure */
bool NetEntityNotInGame(NetEntityName *entity, NetAddrBlock *address);

/* ---------- prototypes/NETWORK_NAMES.C */

// ZZZ: moved to network_lookup_sdl.h to localize changes.

/* ---------- prototypes/NETWORK_LOOKUP.C */

// ZZZ: moved to network_lookup_sdl.h to localize changes.

/* ---------- prototypes/NETWORK_DDP.C */

OSErr NetDDPOpen(void);
OSErr NetDDPClose(void);

// ZZZ: this is a bit confusing; in the original AppleTalk DDP code, the socket routines
// took and returned a socket number, which is a bit like the file descriptor one gets for
// a UNIX socket.  Now with NETWORK_IP, that portion of the API is used to pass an IP port number.
// (The argument to NetDDPCloseSocket() is now ignored, then; we only currently support one open UDP socket.)
// NETWORK_IP: *ioPortNumber is in network byte order ("big-endian")
OSErr NetDDPOpenSocket(short *ioPortNumber, PacketHandlerProcPtr packetHandler);
OSErr NetDDPCloseSocket(short ignored);

DDPFramePtr NetDDPNewFrame(void);
void NetDDPDisposeFrame(DDPFramePtr frame);

OSErr NetDDPSendFrame(DDPFramePtr frame, const NetAddrBlock *address, short protocolType, short socket);

/* ---------- prototypes/NETWORK_ADSP.C */

// jkvw: removed - we use TCPMess now

#endif
