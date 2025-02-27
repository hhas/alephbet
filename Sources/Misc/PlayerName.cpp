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

#include "cseries.hpp"

#include "PlayerName.hpp"
#include "TextStrings.hpp"
#include "InfoTree.hpp"
#include <string.h>

static char PlayerName[256];


// Get that name
const char *GetPlayerName() {return PlayerName;}


void reset_mml_player_name()
{
	// no reset
}

void parse_mml_player_name(const InfoTree& root)
{
	boost::optional<std::string> name_opt;
	if ((name_opt = root.get_value_optional<std::string>()))
		DeUTF8_C(name_opt->c_str(), name_opt->size(), PlayerName, 255);
}
