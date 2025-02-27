#ifndef NETWORK_STAR_H
#define NETWORK_STAR_H

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

#include "TickBasedCircularQueue.hpp"
#include "ActionQueues.hpp"

#include "sdl_network.hpp"

// We avoid including half the world just to get TICKS_PER_SECOND for standalone hub...
#ifndef AB_NETWORK_STANDALONE_HUB
#include "map.hpp" // TICKS_PER_SECOND
#else
enum {
	TICKS_PER_SECOND = 30
};
#endif

#include <stdio.h>

enum {
        kEndOfMessagesMessageType = 0x454d,	// 'EM'
        kTimingAdjustmentMessageType = 0x5441,	// 'TA'
        kPlayerNetDeadMessageType = 0x4e44,	// 'ND'
	kSpokeToHubLossyByteStreamMessageType = 0x534c,	// 'SL'
	kHubToSpokeLossyByteStreamMessageType = 0x484c, // 'HL'

	kSpokeToHubIdentification = 0x4944,   // 'ID'
	kSpokeToHubGameDataPacketV1Magic = 0x5331, // 'S1'
	kHubToSpokeGameDataPacketV1Magic = 0x4831, // 'H1'
	kHubToSpokeGameDataPacketWithSpokeFlagsV1Magic = 0x4631, // 'F1'
	kPingRequestPacket = 0x5051, // 'PQ'
	kPingResponsePacket = 0x5052, // 'PR'

        kPregameTicks = TICKS_PER_SECOND * 3,	// Synchronization/timing adjustment before real data
        kActionFlagsSerializedLength = 4,	// bytes for each serialized action_flags_t (should be elsewhere)
	
	kStarPacketHeaderSize = 4, // 2 bytes for packet magic, 2 for CRC
};

typedef uint32 action_flags_t;	// (should be elsewhere)
typedef ConcreteTickBasedCircularQueue<action_flags_t> TickBasedActionQueue;
typedef WritableTickBasedCircularQueue<action_flags_t> WritableTickBasedActionQueue;


class InfoTree;

extern void hub_initialize(int32 inStartingTick, int inNumPlayers, const NetAddrBlock* const* inPlayerAddresses, int inLocalPlayerIndex);
extern void hub_cleanup(bool inGraceful, int32 inSmallestPostGameTick);
extern void hub_received_network_packet(DDPPacketBufferPtr inPacket);
extern bool hub_is_active();
extern void DefaultHubPreferences();
extern InfoTree HubPreferencesTree();
extern void HubParsePreferencesTree(InfoTree prefs, std::string version);

extern void spoke_initialize(const NetAddrBlock& inHubAddress, int32 inFirstTick, size_t inNumberOfPlayers, WritableTickBasedActionQueue* const inPlayerQueues[], bool inPlayerConnectedStatus[], size_t inLocalPlayerIndex, bool inHubIsLocal);
extern void spoke_cleanup(bool inGraceful);
extern void spoke_received_network_packet(DDPPacketBufferPtr inPacket);
extern int32 spoke_get_net_time();
// "Distribute to everyone" helps to match the existing (legacy) interfaces etc.
extern void spoke_distribute_lossy_streaming_bytes_to_everyone(int16 inDistributionType, byte* inBytes, uint16 inLength, bool inExcludeLocalPlayer, bool onlySendToTeam);
// distribute_lossy_streaming_bytes offers a more direct interface (not yet used) to star's lossy
//	distribution mechanism.  (e.g., can select certain recipients, send unregistered dist types, etc.)
extern void spoke_distribute_lossy_streaming_bytes(int16 inDistributionType, uint32 inDestinationsBitmask, byte* inBytes, uint16 inLength);
extern int32 spoke_latency(); // in ms, kNetLatencyInvalid if not yet valid
extern int32 hub_latency(int player_index); // in ms, kNetLatencyInvalid if not valid, kNetLatencyDisconnected if d/c
extern TickBasedActionQueue* spoke_get_unconfirmed_flags_queue();
extern int32 spoke_get_smallest_unconfirmed_tick();
extern bool spoke_check_world_update();
extern void DefaultSpokePreferences();
extern InfoTree SpokePreferencesTree();
extern void SpokeParsePreferencesTree(InfoTree prefs, std::string version);

#endif // NETWORK_STAR_H
