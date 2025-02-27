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
 *  network_dummy.cpp - Dummy network functions
 */

#include "cseries.hpp"
#include "map.hpp"
#include "network.hpp"
#include "network_games.hpp"

void NetExit(void) {}

bool NetSync(void) { return true; }

bool NetUnSync(void) { return true; }

short NetGetLocalPlayerIndex(void) { return 0; }

short NetGetPlayerIdentifier(short player_index) { return 0; }

short NetGetNumberOfPlayers(void) { return 1; }

void* NetGetPlayerData(short player_index) { return NULL; }

void* NetGetGameData(void) { return NULL; }

bool NetChangeMap(struct entry_point* entry) { return false; }

int32 NetGetNetTime(void) { return 0; }

void display_net_game_stats(void) {}

bool network_gather(void) { return false; }

int network_join(void) { return false; }

bool current_game_has_balls(void) { return false; }

bool NetAllowBehindview(void) { return false; }

bool NetAllowCrosshair(void) { return false; }

bool NetAllowTunnelVision(void) { return false; }
