#ifndef __LUA_PLAYER_H
#define __LUA_PLAYER_H

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

#include "cseries.hpp"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "map.hpp"
#include "lua_templates.hpp"

extern char Lua_Player_Name[]; // "player"
typedef L_Class<Lua_Player_Name> Lua_Player;

extern char Lua_Players_Name[]; // "Players"
typedef L_Container<Lua_Players_Name, Lua_Player> Lua_Players;

extern char Lua_PlayerColor_Name[]; // "player_color"
typedef L_Enum<Lua_PlayerColor_Name> Lua_PlayerColor;

extern char Lua_PlayerColors_Name[]; // "PlayerColors"
typedef L_EnumContainer<Lua_PlayerColors_Name, Lua_PlayerColor> Lua_PlayerColors;

int Lua_Player_register (lua_State *L);

#endif
