#ifndef NETWORK_DATA_FORMATS_H
#define NETWORK_DATA_FORMATS_H

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

#include "cseries.hpp" // Need ALEPHBET_LITTLE_ENDIAN, if appropriate.
#include "network.hpp"
#include "network_private.hpp"


// Note: no further interpretation/manipulation of a packet is attempted here.  That's up
// to whomever actually deals with receiving and interpreting the packet (though they will
// probably make use of the netcpy's for the next couple structures below).

const int SIZEOF_NetPacketHeader = 6;

struct NetPacketHeader_NET {
    uint8 data[SIZEOF_NetPacketHeader];
};

extern void netcpy(NetPacketHeader_NET* dest, const NetPacketHeader* src);
extern void netcpy(NetPacketHeader* dest, const NetPacketHeader_NET* src);


// Note: we do not attempt any manipulations on the actual action_flags, as we do not claim
// to compute the number that would be there.  (I suppose, knowing that the only stuff in the
// buffer will be action flags, that we could just walk through and swap all of it, but...)
// We'll leave it to whomever interprets or writes to the action_flags data segment to do the
// necessary manipulations.

const int SIZEOF_NetPacket = 2 * MAXIMUM_NUMBER_OF_NETWORK_PLAYERS + 8;

struct NetPacket_NET {
    uint8 data[SIZEOF_NetPacket];
};

extern void netcpy(NetPacket_NET* dest, const NetPacket* src);
extern void netcpy(NetPacket* dest, const NetPacket_NET* src);


// For action flags - note length is in bytes, not number of flags.  This is 'bidirectional',
// i.e. same function is used to copy from _NET to unpacked as the other way around.
// Note, since there is no packing to do - only byte swapping - we can pass along to memcpy if we're
// on a big-endian architecture.
#ifdef ALEPHBET_LITTLE_ENDIAN
extern void netcpy(uint32* dest, const uint32* src, size_t length);
#else
__inline__ void netcpy(uint32* dest, const uint32* src, size_t length) { memcpy(dest, src, length); }
#endif


// Note: we do not attempt any sort of processing on the "data" segment here,
// since we may not understand its format.  Whoever interprets it will have to
// do the necessary packing/unpacking, byte-swapping, etc.

const int SIZEOF_NetDistributionPacket = 6;

struct NetDistributionPacket_NET {
    uint8 data[SIZEOF_NetDistributionPacket];
};

extern void netcpy(NetDistributionPacket_NET* dest, const NetDistributionPacket* src);
extern void netcpy(NetDistributionPacket* dest, const NetDistributionPacket_NET* src);


// Note: unlike other _NET structures, neither 'host' nor 'port' here is byte-swapped
// in conversion to/from the non-_NET structures.
// It is believed that both should ALWAYS be dealt with in network byte order.

const int SIZEOF_IPaddress = 6;

struct IPaddress_NET {
    uint8 data[SIZEOF_IPaddress];
};

extern void netcpy(IPaddress_NET* dest, const IPaddress* src);
extern void netcpy(IPaddress* dest, const IPaddress_NET* src);

#endif // NETWORK_DATA_FORMATS_H
