#ifndef __LUA_PROJECTILES_H
#define __LUA_PROJECTILES_H

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

#include "lua_templates.hpp"

extern char Lua_Projectile_Name[]; // "projectile"
typedef L_Class<Lua_Projectile_Name> Lua_Projectile;

extern char Lua_Projectiles_Name[]; // "Projectiles"
typedef L_Container<Lua_Projectiles_Name, Lua_Projectile> Lua_Projectiles;

extern char Lua_ProjectileType_Name[]; // "projectile_type"
typedef L_Enum<Lua_ProjectileType_Name> Lua_ProjectileType;

extern char Lua_ProjectileTypes_Name[]; // "ProjectileTypes"
typedef L_EnumContainer<Lua_ProjectileTypes_Name, Lua_ProjectileType> Lua_ProjectileTypes;

int Lua_Projectiles_register(lua_State *L);

#endif
