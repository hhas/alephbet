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
 *  network_udp.cpp - UDP network functions (corresponds to AppleTalk DDP)
 */

#if !defined(DISABLE_NETWORKING)

#include "cseries.hpp"
#include "network_private.hpp"
#include "sdl_network.hpp"

#include <SDL_thread.h>

#include "mytm.hpp" // mytm_mutex stuff
#include "thread_priority_sdl.hpp"

// Global variables (most comments and "sSomething" variables are ZZZ)
// Storage for incoming packet data
static UDPpacket* sUDPPacketBuffer = NULL;

// Storage for the DDP packet we pass back to the handler proc
static DDPPacketBuffer ddpPacketBuffer;

// Keep track of our one sending/receiving socket
static UDPsocket sSocket = NULL;

// Keep track of the socket-set the receiving thread uses (so we don't have to allocate/free it in that thread)
static SDLNet_SocketSet sSocketSet = NULL;

// Keep track of the function to call when we receive data
static PacketHandlerProcPtr sPacketHandler = NULL;

// Keep track of the receiving thread
static SDL_Thread* sReceivingThread = NULL;

// See if the receiving thread should exit
static volatile bool sKeepListening = false;

// ZZZ: the socket listening thread loops in this function.  It calls the registered
// packet handler when it gets something.
static int receive_thread_function(void*) {
    while (true) {
        // We listen with a timeout so we can shut ourselves down when needed.
        int theResult = SDLNet_CheckSockets(sSocketSet, 1000);

        if (!sKeepListening)
            break;

        if (theResult > 0 && take_mytm_mutex()) {
            theResult = SDLNet_UDP_Recv(sSocket, sUDPPacketBuffer);
            if (theResult > 0) {
                ddpPacketBuffer.protocolType  = kPROTOCOL_TYPE;
                ddpPacketBuffer.sourceAddress = sUDPPacketBuffer->address;
                ddpPacketBuffer.datagramSize  = sUDPPacketBuffer->len;

                // Hope the other guy is done using whatever's in there!
                // (As I recall, all uses happen in sPacketHandler and its progeny, so we should be fine.)
                memcpy(ddpPacketBuffer.datagramData, sUDPPacketBuffer->data, sUDPPacketBuffer->len);

                sPacketHandler(&ddpPacketBuffer);
            }
            release_mytm_mutex();
        }
    }

    return 0;
}

/*
 *  Initialize/shutdown module
 */

OSErr NetDDPOpen(void) {
    // fdprintf("NetDDPOpen\n");
    //  nothing to do
    return 0;
}

OSErr NetDDPClose(void) {
    // fdprintf("NetDDPClose\n");
    //  nothing to do
    return 0;
}

/*
 *  Open socket
 */
// Most of this function by ZZZ.
// Incoming port number should be in network byte order.
OSErr NetDDPOpenSocket(short* ioPortNumber, PacketHandlerProcPtr packetHandler) {
    // fdprintf("NetDDPOpenSocket\n");
    assert(packetHandler);

    // Allocate packet buffer (this is Christian's part)
    assert(!sUDPPacketBuffer);
    sUDPPacketBuffer = SDLNet_AllocPacket(ddpMaxData);
    if (sUDPPacketBuffer == NULL)
        return -1;

    // PORTGUESS
    // Open socket (SDLNet_Open seems to like port in host byte order)
    // NOTE: only SDLNet_UDP_Open wants port in host byte order.  All other uses of port in SDL_net
    // are in network byte order.
    sSocket = SDLNet_UDP_Open(SDL_SwapBE16(*ioPortNumber));
    if (sSocket == NULL) {
        SDLNet_FreePacket(sUDPPacketBuffer);
        sUDPPacketBuffer = NULL;
        return -1;
    }

    // Set up socket set
    sSocketSet = SDLNet_AllocSocketSet(1);
    SDLNet_UDP_AddSocket(sSocketSet, sSocket);

    // Set up receiver
    sKeepListening   = true;
    sPacketHandler   = packetHandler;
    sReceivingThread = SDL_CreateThread(receive_thread_function, "NetDDPOpenSocket_ReceivingThread", NULL);

    // Set receiving thread priority very high
    bool theResult = BoostThreadPriority(sReceivingThread);
    if (theResult == false)
        fdprintf("warning: BoostThreadPriority() failed; network performance may suffer\n");

    // PORTGUESS but we should generally keep port in network order, I think?
    // We really ought to return the "real" port we bound to in *ioPortNumber...
    // for now we just hand back whatever they handed us.
    //*ioPortNumber = *ioPortNumber;
    return 0;
}

/*
 *  Close socket
 */

OSErr NetDDPCloseSocket(short portNumber) {
    // fdprintf("NetDDPCloseSocket\n");
    //  ZZZ: shut down receiving thread
    if (sReceivingThread) {
        sKeepListening = false;
        SDL_WaitThread(sReceivingThread, NULL);
        sReceivingThread = NULL;
    }

    if (sSocketSet) {
        SDLNet_FreeSocketSet(sSocketSet);
        sSocketSet = NULL;
    }

    // (CB's code follows)
    if (sUDPPacketBuffer) {
        SDLNet_FreePacket(sUDPPacketBuffer);
        sUDPPacketBuffer = NULL;

        SDLNet_UDP_Close(sSocket);
        sSocket = NULL;
    }
    return 0;
}

/*
 *  Allocate frame
 */

DDPFramePtr NetDDPNewFrame(void) {
    // fdprintf("NetDDPNewFrame\n");
    DDPFramePtr frame = (DDPFramePtr)malloc(sizeof(DDPFrame));
    if (frame) {
        memset(frame, 0, sizeof(DDPFrame));
        frame->socket = sSocket;
    }
    return frame;
}

/*
 *  Dispose of frame
 */

void NetDDPDisposeFrame(DDPFramePtr frame) {
    // fdprintf("NetDDPDisposeFrame\n");
    if (frame)
        free(frame);
}

/*
 *  Send frame to remote machine
 */

OSErr NetDDPSendFrame(DDPFramePtr frame, const NetAddrBlock* address, short protocolType, short port) {
    // fdprintf("NetDDPSendFrame\n");
    assert(frame->data_size <= ddpMaxData);

    sUDPPacketBuffer->channel = -1;
    memcpy(sUDPPacketBuffer->data, frame->data, frame->data_size);
    sUDPPacketBuffer->len     = frame->data_size;
    sUDPPacketBuffer->address = *address;
    return SDLNet_UDP_Send(sSocket, -1, sUDPPacketBuffer) ? 0 : -1;
}

#endif // !defined(DISABLE_NETWORKING)
