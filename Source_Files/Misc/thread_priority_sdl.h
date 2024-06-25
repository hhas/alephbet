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
#ifndef THREAD_PRIORITY_H
#define	THREAD_PRIORITY_H

struct SDL_Thread;

// Should bump up the specified thread's priority quite a bit, or else
// (if that's impossible) reduce the main thread's priority somewhat.
// The main thread's priority should not be further reduced by additional calls.
// This should by called by the main thread (so the priority reduction
// can be accomplished).
extern bool
BoostThreadPriority(SDL_Thread* inThread);

#endif // THREAD_PRIORITY_H
