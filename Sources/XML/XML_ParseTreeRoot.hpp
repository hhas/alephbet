#ifndef _XML_PARSE_TREE_ROOT_
#define _XML_PARSE_TREE_ROOT_

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
 *  This is the absolute root element, the one that contains the root elements
 *  of all the valid XML files. It also contains a method for setting up the
 *  parse tree, including that root element, of course.
 */

#include <stddef.h>

extern void ResetAllMMLValues(); // reset everything that's been changed to hard-coded defaults

class FileSpecifier;
extern bool ParseMMLFromFile(const FileSpecifier& filespec, bool load_menu_mml_only);
extern bool ParseMMLFromData(const char* buffer, size_t buflen);

#endif
