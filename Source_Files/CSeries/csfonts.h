#ifndef _CSERIES_FONTS_
#define _CSERIES_FONTS_

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

#include "cstypes.h"
#include <string>

const int styleNormal = 0;
const int styleBold = 1;
const int styleItalic = 2;
const int styleUnderline = 4;
// const int styleOutline = 8; -- can't be used with TTF
const int styleShadow = 16;

struct TextSpec {
	int16 font;

	uint16 style;
	int16 size;
	int16 adjust_height;

	// paths to fonts
	std::string normal;
	std::string oblique;
	std::string bold;
	std::string bold_oblique;
};

#endif

