#ifndef __GAME_WINDOW_H
#define __GAME_WINDOW_H

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

struct Rect;

void initialize_game_window(void);

void draw_interface(void);
void ensure_HUD_buffer(void);
void update_interface(short time_elapsed);
void scroll_inventory(short dy);

void OGL_DrawHUD(Rect& dest, short time_elapsed);

void mark_ammo_display_as_dirty(void);
void mark_shield_display_as_dirty(void);
void mark_oxygen_display_as_dirty(void);
void mark_weapon_display_as_dirty(void);
void mark_player_inventory_screen_as_dirty(short player_index, short screen);
void mark_player_inventory_as_dirty(short player_index, short dirty_item);
void mark_interface_collections(bool loading);
void mark_player_network_stats_as_dirty(short player_index);

class InfoTree;
void parse_mml_interface(const InfoTree& root);
void reset_mml_interface();

#endif
