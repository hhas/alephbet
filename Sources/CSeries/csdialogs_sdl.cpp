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
 *  The purpose of this file is to provide a few easy-to-implement dialog-item
 *  modification and query functions, for greater source compatibility between
 *  SDL and traditional Mac versions.
 */

#include "cseries.hpp"
#include "sdl_dialogs.hpp"
#include "sdl_widgets.hpp"

void modify_control_enabled(DialogPtr inDialog, short inWhichItem, short inChangeEnable) {

    assert(inDialog != NULL);

    widget* theWidget = inDialog->get_widget_by_id(inWhichItem);

    assert(theWidget != NULL);

    if (inChangeEnable != NONE)
        theWidget->set_enabled(inChangeEnable == CONTROL_ACTIVE ? true : false);
}

/*************************************************************************************************
 *
 * Function: get_selection_control_value
 * Purpose:  given a dialog and an item number, extract the value of the control
 *
 *************************************************************************************************/
// Works only on w_select (and subclasses).
short get_selection_control_value(DialogPtr dialog, short which_control) {
    assert(dialog != NULL);

    w_select* theSelectionWidget = dynamic_cast<w_select*>(dialog->get_widget_by_id(which_control));

    assert(theSelectionWidget != NULL);

    return theSelectionWidget->get_selection() + 1;
}

void copy_cstring_to_static_text(DialogPtr dialog, short item, const char* cstring) {
    assert(dialog != NULL);

    w_static_text* theStaticText = dynamic_cast<w_static_text*>(dialog->get_widget_by_id(item));

    assert(theStaticText != NULL);

    theStaticText->set_text(cstring);
}
