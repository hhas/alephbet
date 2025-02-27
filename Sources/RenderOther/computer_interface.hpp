#ifndef __COMPUTER_INTERFACE_H
#define __COMPUTER_INTERFACE_H

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

/* ------------ structures */
struct static_preprocessed_terminal_data {
    int16 total_length;
    int16 flags;
    int16 lines_per_page; /* Added for internationalization/sync problems */
    int16 grouping_count;
    int16 font_changes_count;
};

const int SIZEOF_static_preprocessed_terminal_data = 10;

struct view_terminal_data {
    short top, left, bottom, right;
    short vertical_offset;
};

// External-data size of current terminal state
const int SIZEOF_player_terminal_data = 20;

/* ------------ prototypes */
void initialize_terminal_manager(void);
void initialize_player_terminal_info(short player_index);
void enter_computer_interface(short player_index, short text_number, short completion_flag);
void _render_computer_interface(void);
void update_player_for_terminal_mode(short player_index);
void update_player_keys_for_terminal(short player_index, uint32 action_flags);
uint32 build_terminal_action_flags(char* keymap);
void dirty_terminal_view(short player_index);
void abort_terminal_mode(short player_index);

bool player_in_terminal_mode(short player_index);

// LP: to pack and unpack this data;
// these hide the unpacked data from the outside world.
// "Map terminal" means the terminal data read in from the map;
// "player terminal" means the terminal state for each player.
// For the map terminal data, the "count" is number of packed bytes.

extern void unpack_map_terminal_data(uint8* Stream, size_t Count);
extern void pack_map_terminal_data(uint8* Stream, size_t Count);
uint8* unpack_player_terminal_data(uint8* Stream, size_t Count);
uint8* pack_player_terminal_data(uint8* Stream, size_t Count);

extern size_t calculate_packed_terminal_data_length(void);

void clear_compiled_terminal_cache();

#endif
