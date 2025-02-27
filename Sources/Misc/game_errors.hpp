#ifndef __GAME_ERRORS_H
#define __GAME_ERRORS_H

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

enum { /* types */
    systemError,
    gameError,
    NUMBER_OF_TYPES
};

enum { /* Game Errors */
    errNone = 0,
    errMapFileNotSet,
    errIndexOutOfRange,
    errTooManyOpenFiles,
    errUnknownWadVersion,
    errWadIndexOutOfRange,
    errServerDied,
    errUnsyncOnLevelChange,
    NUMBER_OF_GAME_ERRORS
};

void set_game_error(short type, short error_code);
short get_game_error(short* type);
bool error_pending(void);
void clear_game_error(void);

// game_error system badly needs fixing (suggestion: use exceptions)
// for now, you can use this RAII class before calls where you don't care about
// the error, and it will restore the previous error when it is destroyed
class ScopedGameError {
  public:

    ScopedGameError() { _error = get_game_error(&_type); }

    ~ScopedGameError() { set_game_error(_type, _error); }

  private:

    short _error, _type;
};

#endif
