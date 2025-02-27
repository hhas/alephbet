#ifndef _CSERIES
#define _CSERIES

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define VERSION "unknown version"
#endif

#include <SDL.h>
#include <SDL_endian.h>
#include <string>
#include <time.h>

#define DEBUG


/*
 *  Endianess definitions
 */

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define ALEPHBET_LITTLE_ENDIAN 1
#else
#undef ALEPHBET_LITTLE_ENDIAN
#endif


constexpr bool PlatformIsLittleEndian() noexcept {
#ifdef ALEPHBET_LITTLE_ENDIAN
    return true;
#else
    return false;
#endif // end ALEPHBET_LITTLE_ENDIAN
}

/*
 *  Data types with specific bit width
 */

#include "cstypes.hpp"

/*
 *  Emulation of MacOS data types and definitions
 */

#if defined(__APPLE__) && defined(__MACH__)
// if we're on the right platform, we can use the real thing (and get headers for functions we might want to use)
#include <CoreFoundation/CoreFoundation.h>
#else
typedef int OSErr;

struct Rect {
    int16 top, left;
    int16 bottom, right;
};

const int noErr = 0;
#endif

constexpr Rect MakeRect(int16 top, int16 left, int16 bottom, int16 right) { return {top, left, bottom, right}; }

constexpr Rect MakeRect(SDL_Rect r) { return {int16(r.y), int16(r.x), int16(r.y + r.h), int16(r.x + r.w)}; }

struct RGBColor {
    uint16 red, green, blue;
};

const int kFontIDMonaco  = 4;
const int kFontIDCourier = 22;

/*
 *  Include CSeries headers
 */

#include "csalerts.hpp"
#include "cscluts.hpp"
#include "csdialogs.hpp"
#include "csfonts.hpp"
#include "csmacros.hpp"
#include "csmisc.hpp"
#include "cspaths.hpp"
#include "cspixels.hpp"
#include "csstrings.hpp"
#include "cstypes.hpp"


#endif
