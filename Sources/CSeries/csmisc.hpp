#ifndef _CSERIES_MISC_
#define _CSERIES_MISC_

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

#define MACHINE_TICKS_PER_SECOND 1000

extern uint32 machine_tick_count(void);
extern void sleep_for_machine_ticks(uint32 ticks);
extern void sleep_until_machine_tick_count(uint32 ticks);
extern void yield(void);
extern bool wait_for_click_or_keypress(
	uint32 ticks);

extern void kill_screen_saver(void);

#ifdef DEBUG
extern void initialize_debugger(bool on);
#endif

#endif
