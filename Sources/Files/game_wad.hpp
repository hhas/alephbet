#ifndef __GAME_WAD_H
#define __GAME_WAD_H

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

#include "cstypes.hpp"
#include "map.hpp"
#include <string>

class FileSpecifier;

bool save_game_file(FileSpecifier& File, const std::string& metadata, const std::string& imagedata);
struct wad_data* build_meta_game_wad(const std::string& metadata, const std::string& imagedata,
                                     struct wad_header* header, int32* length);

bool export_level(FileSpecifier& File);

/* -------------- New functions */
void pause_game(void);
void resume_game(void);
void get_current_saved_game_name(FileSpecifier& File);
// ZZZ: split this out from new_game; it sets a default filespec in the revert-game info
void set_saved_game_name_to_default();

// ZZZ: exposed this for netgame-resuming code
bool process_map_wad(struct wad_data* wad, bool restoring_game, short version);

bool match_checksum_with_map(short vRefNum, long dirID, uint32 checksum, FileSpecifier& File);
void set_map_file(FileSpecifier& File, bool runScript = true);
dynamic_data get_dynamic_data_from_save(FileSpecifier& File);
bool get_dynamic_data_from_wad(wad_data* wad, dynamic_data* dest);
// CP Addition: get_map_file returns the FileDesc pointer to the current map
FileSpecifier& get_map_file(void);

void level_has_embedded_physics_lua(int Level, bool& HasPhysics, bool& HasLua);

/* --------- from PREPROCESS_MAP_MAC.C */
// Most of the get_default_filespecs moved to interface.h
void get_savegame_filedesc(FileSpecifier& File);

void add_finishing_touches_to_save_file(FileSpecifier& File);

const int SAVE_GAME_METADATA_INDEX = 1000;

#endif
