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

bool chat_input_mode = false;

#include "cseries.hpp"

#include "XML_ParseTreeRoot.hpp"
#include "interface.hpp"
#include "world.hpp"
#include "screen.hpp"
#include "map.hpp"
#include "shell.hpp"
#include "preferences.hpp"
#include "vbl.hpp"
#include "player.hpp"
#include "Music.hpp"
#include "items.hpp"
#include "TextStrings.hpp"
#include "InfoTree.hpp"

#include <ctype.h>


extern void process_new_item_for_reloading(short player_index, short item_type);
extern bool try_and_add_player_item(short player_index,	short type);
extern void mark_shield_display_as_dirty();
extern void mark_oxygen_display_as_dirty();
extern void accelerate_monster(short monster_index,	world_distance vertical_velocity, 
							   angle direction, world_distance velocity);
extern void update_interface(short time_elapsed);


/*
 *  Called regularly during event loops
 */

void global_idle_proc(void)
{
	Music::instance()->Idle();
	SoundManager::instance()->Idle();
}

/*
 *  Special version of malloc() used for level transitions, frees some
 *  memory if possible
 */

void *level_transition_malloc(
	size_t size)
{
	void *ptr= malloc(size);
	if (!ptr)
	{
		SoundManager::instance()->UnloadAllSounds();
		
		ptr= malloc(size);
		if (!ptr)
		{
			unload_all_collections();
			
			ptr= malloc(size);
		}
	}
	
	return ptr;
}
