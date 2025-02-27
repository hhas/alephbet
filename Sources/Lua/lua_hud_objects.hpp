#ifndef __LUA_HUD_OBJECTS_H
#define __LUA_HUD_OBJECTS_H

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

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include "items.hpp"
#include "lua_templates.hpp"
#include "map.hpp"

extern char Lua_HUDPlayer_Name[]; // "Player"
typedef L_Class<Lua_HUDPlayer_Name> Lua_HUDPlayer;

extern char Lua_HUDGame_Name[]; // "Game"
typedef L_Class<Lua_HUDGame_Name> Lua_HUDGame;

extern char Lua_HUDScreen_Name[]; // "Screen"
typedef L_Class<Lua_HUDScreen_Name> Lua_HUDScreen;


extern char Lua_InterfaceColor_Name[]; // "interface_color"
typedef L_Enum<Lua_InterfaceColor_Name> Lua_InterfaceColor;

extern char Lua_InterfaceColors_Name[]; // "InterfaceColors"
typedef L_EnumContainer<Lua_InterfaceColors_Name, Lua_InterfaceColor> Lua_InterfaceColors;

extern char Lua_InterfaceFont_Name[]; // "interface_font"
typedef L_Enum<Lua_InterfaceFont_Name> Lua_InterfaceFont;

extern char Lua_InterfaceFonts_Name[]; // "InterfaceFonts"
typedef L_EnumContainer<Lua_InterfaceFonts_Name, Lua_InterfaceFont> Lua_InterfaceFonts;

extern char Lua_InterfaceRect_Name[]; // "interface_rect"
typedef L_Enum<Lua_InterfaceRect_Name> Lua_InterfaceRect;

extern char Lua_InterfaceRects_Name[]; // "InterfaceRects"
typedef L_EnumContainer<Lua_InterfaceRects_Name, Lua_InterfaceRect> Lua_InterfaceRects;

extern char Lua_InventorySection_Name[]; // "inventory_section"
typedef L_Enum<Lua_InventorySection_Name> Lua_InventorySection;

extern char Lua_InventorySections_Name[]; // "InventorySections"
typedef L_EnumContainer<Lua_InventorySections_Name, Lua_InventorySection> Lua_InventorySections;

extern char Lua_RendererType_Name[]; // "renderer_type"
typedef L_Enum<Lua_RendererType_Name> Lua_RendererType;

extern char Lua_RendererTypes_Name[]; // "RendererTypes"
typedef L_EnumContainer<Lua_RendererTypes_Name, Lua_RendererType> Lua_RendererTypes;

extern char Lua_SensorBlipType_Name[]; // "sensor_blip"
typedef L_Enum<Lua_SensorBlipType_Name> Lua_SensorBlipType;

extern char Lua_SensorBlipTypes_Name[]; // "SensorBlipTypes"
typedef L_EnumContainer<Lua_SensorBlipTypes_Name, Lua_SensorBlipType> Lua_SensorBlipTypes;

extern char Lua_TextureType_Name[]; // "texture_type";
typedef L_Enum<Lua_TextureType_Name> Lua_TextureType;

extern char Lua_TextureTypes_Name[]; // "TextureTypes";
typedef L_EnumContainer<Lua_TextureTypes_Name, Lua_TextureType> Lua_TextureTypes;
#define NUMBER_OF_LUA_TEXTURE_TYPES 5

int Lua_HUDObjects_register(lua_State* L);

#endif
