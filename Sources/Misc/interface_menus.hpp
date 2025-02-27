#ifndef __INTERFACE_MENUS_H
#define __INTERFACE_MENUS_H

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

enum { /* Menus available during the game */
    mGame  = 128,
    iPause = 1,
    iSave,
    iRevert,
    iCloseGame,
    iQuitGame
};

enum { /* Menu interface... */
    mInterface = 129,
    iNewGame   = 1,
    iLoadGame,
    iGatherGame,
    iJoinGame,
    iPreferences,
    iReplayLastFilm,
    iSaveLastFilm,
    iReplaySavedFilm,
    iCredits,
    iQuit,
    iCenterButton,
    iPlaySingletonLevel,
    iAbout
};

/* This is the menu with nothing in the title, so that it doesn't show up */
/* when the menu bar is drawn atexit.. */
enum {
    mFakeEmptyMenu = 130
};

#endif
