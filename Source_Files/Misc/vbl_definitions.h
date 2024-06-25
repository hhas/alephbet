#ifndef __VBL_DEFINITIONS_H
#define __VBL_DEFINITIONS_H

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

#include "player.h"

#define MAXIMUM_QUEUE_SIZE           512

typedef struct action_queue /* 8 bytes */
{
	int16 read_index, write_index;
	uint32 *buffer;
} ActionQueue;

struct recording_header
{
	int32 length;
	int16 num_players;
	int16 level_number;
	uint32 map_checksum;
	int16 version;
	struct player_start_data starts[MAXIMUM_NUMBER_OF_PLAYERS];
	struct game_data game_information;
};
const int SIZEOF_recording_header = 352;

struct replay_private_data {
	bool valid;
	struct recording_header header;
	int16 replay_speed;
	bool game_is_being_replayed;
	bool game_is_being_recorded;
	bool have_read_last_chunk;
	ActionQueue *recording_queues;
	
	// fileref recording_file_refnum;
	char *fsread_buffer;
	char *location_in_cache;
	int32 bytes_in_cache;
	
	int32 film_resource_offset;
	char *resource_data;
	int32 resource_data_size;
};

/* ----- globals */
extern struct replay_private_data replay;

/* ------ prototypes */
#ifndef DEBUG
	#define get_player_recording_queue(x) (replay.recording_queues+(x))
#else
ActionQueue *get_player_recording_queue(short player_index);
#endif

typedef void *timer_task_proc;

timer_task_proc install_timer_task(short tasks_per_second, bool (*func)(void));
void remove_timer_task(timer_task_proc proc);

#endif
