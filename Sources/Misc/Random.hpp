#ifndef _RANDOM_
#define _RANDOM_

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

/* Historical comment by the original author, Loren Petrich: */
/*
    It uses random-number generators created by Dr. George Marsaglia,
    and communicated to me by Joe McMahon (mcmahon@metalab.unc.edu).
    I've rewritten the code communicated to me into a C++ class,
    so that separate instances can be more easily handled.

    This is from code that is presumably GNU-compatible.
*/

#include "cstypes.hpp"

struct GM_Random {
    uint32 z, w, jsr, jcong, t[256], x, y;
    unsigned char c;

    uint32 znew() { return (z = 36'969 * (z & 65'535) + (z >> 16)); }

    uint32 wnew() { return (w = 18'000 * (w & 65'535) + (w >> 16)); }

    uint32 MWC() { return ((znew() << 16) + wnew()); }

    uint32 SHR3() {
        jsr = jsr ^ (jsr << 17);
        jsr = jsr ^ (jsr >> 13);
        jsr = jsr ^ (jsr << 5);
        return jsr;
    }

    uint32 CONG() { return (jcong = 69'069 * jcong + 1'234'567); }

    uint32 KISS() { return ((MWC() ^ CONG()) + SHR3()); }

    uint32 LFIB4() {
        t[c] = t[c] + t[c + 58] + t[c + 119] + t[c + 178];
        return t[++c];
    }

    uint32 SWB() {
        x          = t[c + 15];
        y          = t[c] + (x < y);
        t[c + 237] = x - y;
        return t[++c];
    }

    float UNI() { return (float)(KISS() * 2.328306e-10); }

    float VNI() { return (float)(((int32)KISS()) * 4.656613e-10); }

    /*
        Random seeds must be used to reset z, w, jsr, jcong and
        the table t[256].  Here is an example procedure, using KISS:
    */
    void SetTable() {
        for (int i = 0; i < 256; i++) t[i] = KISS();
    }

    /*
        Any one of KISS, MWC, LFIB4, SWB, SHR3, or CONG  can be used in
        an expression to provide a random 32-bit integer, while UNI
        provides a real in (0,1) and VNI a real in (-1,1).
    */

    GM_Random() : z(362'436'069), w(521'288'629), jsr(123'456'789), jcong(380'116'160), x(0), y(0), c(0) { SetTable(); }
};

#endif
