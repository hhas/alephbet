#ifndef _CSERIES_CLUTS_
#define _CSERIES_CLUTS_

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

// Need this here
#include "cstypes.hpp"

class LoadedResource;
struct RGBColor;

typedef struct rgb_color {
    uint16 red;
    uint16 green;
    uint16 blue;
} rgb_color;

typedef struct color_table {
    short color_count;
    rgb_color colors[256];
} color_table;

extern void build_color_table(color_table* table, LoadedResource& clut);

enum {
    gray15Percent,
    windowHighlight,

    NUM_SYSTEM_COLORS
};

extern RGBColor rgb_black;
extern RGBColor rgb_white;
extern RGBColor system_colors[NUM_SYSTEM_COLORS];

#endif
