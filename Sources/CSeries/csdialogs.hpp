#ifndef _CSERIES_DIALOGS_
#define _CSERIES_DIALOGS_

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

#include <string> /* Prefix header doesn't do this? */
#include <vector>

#define iOK					1

class dialog;

typedef	dialog*	DialogPtr;

#define	CONTROL_INACTIVE	0
#define	CONTROL_ACTIVE		1

extern void copy_cstring_to_static_text(DialogPtr dialog, short item, const char* cstring);

extern void modify_control_enabled(
	DialogPtr dlg,
	short item,
	short hilite);

extern short get_selection_control_value(
        DialogPtr dialog,
        short which_control);
        

#endif//_CSERIES_DIALOGS_
