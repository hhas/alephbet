#ifndef __MOUSE_H
#define __MOUSE_H

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

#include "world.h"

void enter_mouse(short type);
fixed_yaw_pitch pull_mouselook_delta();
void exit_mouse(short type);
void mouse_idle(short type);
void recenter_mouse(void);

// ZZZ: stuff of various hackiness levels to pretend mouse buttons are keys
void mouse_buttons_become_keypresses(Uint8* ioKeyMap);
void mouse_scroll(bool up);
void mouse_moved(int delta_x, int delta_y);

#define NUM_SDL_REAL_MOUSE_BUTTONS 5
#define NUM_SDL_MOUSE_BUTTONS 7   		  // two scroll-wheel buttons
#define AO_SCANCODE_BASE_MOUSE_BUTTON 400 // this is button 1's pseudo-keysym
#define AO_SCANCODE_MOUSESCROLL_UP 405    // stored as mouse button 6
#define AO_SCANCODE_MOUSESCROLL_DOWN 406  // stored as mouse button 7

#endif
