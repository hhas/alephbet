#ifndef __LUA_MONSTERS_H
#define __LUA_MONSTERS_H

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

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "map.h"
#include "monsters.h"
#include "lua_templates.h"

extern char Lua_Monster_Name[]; // "monster"
typedef L_Class<Lua_Monster_Name> Lua_Monster;

extern char Lua_Monsters_Name[]; // "Monsters"
typedef L_Container<Lua_Monsters_Name, Lua_Monster> Lua_Monsters;

extern char Lua_MonsterAction_Name[]; // "monster_action"
typedef L_Enum<Lua_MonsterAction_Name> Lua_MonsterAction;

extern char Lua_MonsterType_Name[]; // "monster_type"
typedef L_Enum<Lua_MonsterType_Name> Lua_MonsterType;

int Lua_Monsters_register(lua_State *L);

#endif
