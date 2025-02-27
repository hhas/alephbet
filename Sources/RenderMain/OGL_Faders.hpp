#ifndef _OGL_FADERS_
#define _OGL_FADERS_

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


// Indicates whether OpenGL-rendering faders will be used
bool OGL_FaderActive();

// Which kinds of faders in the fader queue?
enum {
    FaderQueue_Liquid,
    FaderQueue_Other,
    NUMBER_OF_FADER_QUEUE_ENTRIES
};

// Fader data
struct OGL_Fader {
    // Which type of fade to do
    short Type;
    // The three color channels and a transparency channel
    float Color[4];

    OGL_Fader() : Type(NONE) {}
};

// Fader=queue accessor
OGL_Fader* GetOGL_FaderQueueEntry(int Index);

// Fader renderer; returns whether or not OpenGL faders were active.
bool OGL_DoFades(float Left, float Top, float Right, float Bottom);

#endif
