#ifndef _CROSSHAIRS
#define _CROSSHAIRS

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

#include "cseries.h"  // need RGBColor

struct SDL_Surface;

enum {
	CHShape_RealCrosshairs,
	CHShape_Circle
};

struct CrosshairData
{
     RGBColor Color;
	short Thickness;
	short FromCenter;
	short Length;
	short Shape;
	float Opacity;
	float GLColorsPreCalc[4];
	bool PreCalced;
};

// True for OK, false for cancel
// the structure will not be changed if this was canceled
// Implemented in PlayerDialogs.c
bool Configure_Crosshairs(CrosshairData &Data);

// Gotten from preferences
// Implemented in preferences.c
CrosshairData& GetCrosshairData();

// All these functions return the crosshairs' state (true: active; false: inactive)
bool Crosshairs_IsActive();
bool Crosshairs_SetActive(bool NewState);

bool Crosshairs_Render(SDL_Surface *s);

#endif
