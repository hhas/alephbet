
#ifndef LUA_HUD_SCRIPT_H
#define LUA_HUD_SCRIPT_H
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

void L_Call_HUDInit();
void L_Call_HUDCleanup();
void L_Call_HUDDraw();
void L_Call_HUDResize();

bool LoadLuaHUDScript(const char *buffer, size_t len);
bool RunLuaHUDScript();
bool LuaHUDRunning();
void LoadHUDLua();
void CloseLuaHUDScript();

void SetLuaHUDScriptPath(const std::string& path);
std::string GetLuaHUDScriptPath();

void MarkLuaHUDCollections(bool loading);


#endif
