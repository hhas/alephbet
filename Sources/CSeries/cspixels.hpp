#ifndef _CSERIES_PIXELS_
#define _CSERIES_PIXELS_

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

typedef uint8 pixel8;
typedef uint16 pixel16;
typedef uint32 pixel32;

#define PIXEL8_MAXIMUM_COLORS      256
#define PIXEL16_MAXIMUM_COMPONENT  31
#define PIXEL32_MAXIMUM_COMPONENT  255
#define NUMBER_OF_COLOR_COMPONENTS 3

/*
    note that the combiner macros expect input values in the range
        0x0000 through 0xFFFF
    while the extractor macros return output values in the ranges
        0x00 through 0x1F (in the 16-bit case)
        0x00 through 0xFF (in the 32-bit case)
 */

#define RGBCOLOR_TO_PIXEL16(r, g, b) (((r) >> 1 & 0x7C00) | ((g) >> 6 & 0x03E0) | ((b) >> 11 & 0x001F))
#define RED16(p)                     ((p) >> 10 & 0x1F)
#define GREEN16(p)                   ((p) >> 5 & 0x1F)
#define BLUE16(p)                    ((p) & 0x1F)

#define RGBCOLOR_TO_PIXEL32(r, g, b) (((r) << 8 & 0x00FF'0000) | ((g) & 0x0'0000'FF00) | ((b) >> 8 & 0x0000'00FF))
#define RED32(p)                     ((p) >> 16 & 0xFF)
#define GREEN32(p)                   ((p) >> 8 & 0xFF)
#define BLUE32(p)                    ((p) & 0xFF)

#endif
