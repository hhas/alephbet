#ifndef __SCREEN_DEFINITIONS_H
#define __SCREEN_DEFINITIONS_H

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

/* -------- this contains the ids for the 8 bit picts */
/* the 16 bit versions are these ids + 10000 */
/* the 32 bit versions are these ids + 20000 */
enum {
	INTRO_SCREEN_BASE= 1000,
	MAIN_MENU_BASE= 1100,
	PROLOGUE_SCREEN_BASE= 1200,
	EPILOGUE_SCREEN_BASE= 1300,
	CREDIT_SCREEN_BASE= 1400,
	CHAPTER_SCREEN_BASE= 1500,
	COMPUTER_INTERFACE_BASE= 1600,
	INTERFACE_PANEL_BASE= 1700,
	FINAL_SCREEN_BASE= 1800
};

#endif

