#ifndef __WAD_PREFS_H
#define __WAD_PREFS_H

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

#include "FileHandler.h"
#include "wad.h"

/* Open the file, and allocate whatever internal structures are necessary in the */
/*  preferences pointer.. */
bool w_open_preferences_file(char *PrefName, Typecode Type);

typedef void (*prefs_initializer)(void *prefs);
typedef bool (*prefs_validater)(void *prefs);

void *w_get_data_from_preferences(
	WadDataType tag,					/* Tag to read */
	size_t expected_size,				/* Data size */
	prefs_initializer initialize,	/* Call if I have to allocate it.. */
	prefs_validater validate);	/* Verify function-> fixes if bad and returns true */
	
void w_write_preferences_file(void);

/* ------ local structures */
/* This is the structure used internally */
struct preferences_info {
	preferences_info() : wad(NULL) {}

	FileSpecifier PrefsFile;
	struct wad_data *wad;
};

#endif
