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

#include "cseries.h"
#include "game_errors.h"

static short last_type= systemError;
static short last_error= 0;

void set_game_error(
	short type, 
	short error_code)
{
	assert(type>=0 && type<NUMBER_OF_TYPES);
	last_type= type;
	last_error= error_code;
#ifdef DEBUG
	if(type==gameError) assert(error_code>=0 && error_code<NUMBER_OF_GAME_ERRORS);
#endif
}

short get_game_error(
	short *type)
{
	if(type)
	{
		*type= last_type;
	}
	
	return last_error;
}

bool error_pending(
	void)
{
	return (last_error!=0);
}

void clear_game_error(
	void)
{
	last_error= 0;
	last_type= 0;
}
