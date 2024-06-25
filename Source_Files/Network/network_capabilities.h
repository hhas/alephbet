#ifndef NETWORK_CAPABILITIES_H
#define NETWORK_CAPABILITIES_H

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

#include "cseries.h"

#include <string>
#include <map>

using std::string;

typedef std::map<string, uint32> capabilities_t;

class Capabilities : public capabilities_t
{
 public:
  enum { kMaxKeySize = 1024 };

  static const int kGameworldVersion = 5;
  static const int kGameworldM1Version = 4;
  static const int kStarVersion = 6;
  static const int kRingVersion = 2;
  static const int kLuaVersion = 2;
  static const int kGatherableVersion = 1;
  static const int kZippedDataVersion = 1; // map, lua, physics
  static const int kNetworkStatsVersion = 1; // latency, jitter, errors
  static const int kRugbyVersion = 1; // sane score limit

  static const string kGameworld;    // the PRNG, physics, etc.
  static const string kGameworldM1;  // like gameworld, but for Marathon 1 compatibility
  static const string kStar;         // the star network protocol
  static const string kRing;         // the ring network protocol
  static const string kLua;          // Lua script support
  static const string kGatherable;   // joiner's response indicating he can be
                                     // gathered
  static const string kZippedData;   // can receive zipped data
  static const string kNetworkStats; // can receive network stats
  static const string kRugby;        // rugby version
  
  uint32& operator[](const string& k) { 
    assert(k.length() < kMaxKeySize);
    return capabilities_t::operator[](k);
  }
};

#endif

