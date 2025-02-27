#ifndef _TEXT_STRINGS_
#define _TEXT_STRINGS_

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

/*
 *  These functions replace the getting of MacOS STR# resources, and are called
 *  in much the same fashion. They're referenced by a resource ID (a string
 *  set) and a zero-based index (individual strings).
 */

#include <stddef.h>

// Set up a string in the repository; a repeated call will replace an old string
void TS_PutCString(short ID, short Index, const char *String);

// Returns a pointer to a string;
// if the ID and the index do not point to a valid string,
// this function will then return NULL
const char *TS_GetCString(short ID, short Index);

// Checks on the presence of a string set
bool TS_IsPresent(short ID);

// Count the strings (contiguous from index zero)
size_t TS_CountStrings(short ID);

// Deletes a string, should one ever want to do that
void TS_DeleteString(short ID, short Index);

// Deletes the stringset with some ID
void TS_DeleteStringSet(short ID);

// Deletes all of the stringsets
void TS_DeleteAllStrings();

// Write output as a C string;
// Returns how many characters resulted.
// Needs at least (OutMaxLen + 1) characters allocated.
size_t DeUTF8_C(const char *InString, size_t InLen, char *OutString, size_t OutMaxLen);

class InfoTree;
void parse_mml_stringset(const InfoTree& root);
void reset_mml_stringset();

#endif
