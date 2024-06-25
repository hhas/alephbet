#ifndef __PROGRESS_H
#define __PROGRESS_H

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

#include <stddef.h>

enum {
	strPROGRESS_MESSAGES= 143,
	_distribute_map_single= 0,
	_distribute_map_multiple,
	_receiving_map,
	_awaiting_map,
	_distribute_physics_single,
	_distribute_physics_multiple,
	_receiving_physics,
	// non-network ones
	_loading,
	// more network ones
	_opening_router_ports,
	_closing_router_ports,
	_checking_for_updates
};

void open_progress_dialog(size_t message_id, bool show_progress_bar = false);
void close_progress_dialog(void);

void set_progress_dialog_message(size_t message_id);

void draw_progress_bar(size_t sent, size_t total);

void reset_progress_bar(void);

#endif

