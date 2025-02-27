#ifndef _XML_LEVEL_SCRIPT_
#define _XML_LEVEL_SCRIPT_
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
 *  Support for XML scripts in map files
 *  The reason for a separate object is that it will be necessary to execute
 *  certain commands only on certain levels.
 */

#include "FileHandler.hpp"

// Loads all those in resource 128 in a map file (or some appropriate equivalent)
void LoadLevelScripts(FileSpecifier& MapFile);

// Runs a script for some level; loads Pfhortran,
// runs level-specific MML...
void ResetLevelScript();
void RunLevelScript(int LevelIndex);
void RunScriptChunks();

// Intended to be run at the end of a game
void RunEndScript();

// Intended for restoring old parameter values, because MML sets values at a variety
// of different places, and it may be easier to simply set stuff back to defaults
// by including those defaults in the script.
void RunRestorationScript();

// Finds the level movie and the end movie, to be used in show_movie()
// The first is for some level,
// while the second is for the end of a game
void FindLevelMovie(short index);

// Gets the pointer of a movie to play at a level, as a pointer to the file specifier.
// A NULL pointer means no movie to play.
// Its arg is the playback size, which will not be changed if not specified explicitly.
FileSpecifier *GetLevelMovie(float& PlaybackSize);

// handle new embedded MML and Lua scripts
void SetMMLS(uint8* data, size_t length);
void SetLUAS(uint8* data, size_t length);
uint8* GetMMLS(size_t& length);
uint8* GetLUAS(size_t& length);

// For selecting the end-of-game screens --
// what fake level index for them, and how many to display
// (resource numbers increasing in sequence) 
extern short EndScreenIndex;
extern short NumEndScreens;

class InfoTree;
void parse_mml_default_levels(const InfoTree& root);
void reset_mml_default_levels();

#endif
