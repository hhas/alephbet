#ifndef _CSERIES_TYPES_
#define _CSERIES_TYPES_

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

#include <limits.h>
#ifdef HAVE_CONFIG_H // pick up HAVE_OPENGL
#include "config.h"
#endif

// IR note: consts in headers are slow and eat TOC space.
//const int NONE = -1;
enum {
	NONE = -1,
	UNONE = 65535
};

// Integer types with specific bit size
#include <SDL_types.h>
#include <time.h>	// for time_t
typedef Uint8 uint8;
typedef Sint8 int8;
typedef Uint16 uint16;
typedef Sint16 int16;
typedef Uint32 uint32;
typedef Sint32 int32;
typedef time_t TimeType;

// Minimum and maximum values for these types
#ifndef INT16_MAX
#define INT16_MAX 32767
#endif
#ifndef UINT16_MAX
#define UINT16_MAX 65535
#endif
#ifndef INT16_MIN
#define INT16_MIN (-INT16_MAX-1)
#endif
#ifndef INT32_MAX
#define INT32_MAX 2147483647
#endif
#ifndef INT32_MIN
#define INT32_MIN (-INT32_MAX-1)
#endif

// Fixed point (16.16) type
// LP: changed to _fixed to get around MSVC namespace conflict
typedef int32 _fixed;

#define FIXED_FRACTIONAL_BITS 16
#define INTEGER_TO_FIXED(i) ((_fixed)(i)<<FIXED_FRACTIONAL_BITS)
#define FIXED_INTEGERAL_PART(f) ((f)>>FIXED_FRACTIONAL_BITS)

#define FIXED_ONE		(1L<<FIXED_FRACTIONAL_BITS)
#define FIXED_ONE_HALF	(1L<<(FIXED_FRACTIONAL_BITS-1))

// Binary powers
const int MEG = 0x100000;
const int KILO = 0x400L;

// Construct four-character-code
#define FOUR_CHARS_TO_INT(a,b,c,d) (((uint32)(a) << 24) | ((uint32)(b) << 16) | ((uint32)(c) << 8) | (uint32)(d))

// Hmmm, this should be removed one day...
typedef uint8 byte;

// Make it compile on systems without OpenGL
#ifndef HAVE_OPENGL
#define GLfloat float
#endif

#endif
