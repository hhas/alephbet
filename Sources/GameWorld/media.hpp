#ifndef __MEDIA_H
#define __MEDIA_H

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

#include <vector>
#include "map.hpp"

/* ---------- constants */

// #define MAXIMUM_MEDIAS_PER_MAP 16

// LP addition: added JjaroGoo support
enum /* media types */
{
	_media_water,
	_media_lava,
	_media_goo,
	_media_sewage,
	_media_jjaro,
	NUMBER_OF_MEDIA_TYPES
};

enum /* media flags */
{
	_media_sound_obstructed_by_floor, // this media makes no sound when under the floor
	
	NUMBER_OF_MEDIA_FLAGS /* <= 16 */
};

#define MEDIA_SOUND_OBSTRUCTED_BY_FLOOR(m) TEST_FLAG16((m)->flags, _media_sound_obstructed_by_floor)

#define SET_MEDIA_SOUND_OBSTRUCTED_BY_FLOOR(m, v) SET_FLAG16((m)->flags, _media_sound_obstructed_by_floor, (v))

enum /* media detonation types */
{
	_small_media_detonation_effect,
	_medium_media_detonation_effect,
	_large_media_detonation_effect,
	_large_media_emergence_effect,
	NUMBER_OF_MEDIA_DETONATION_TYPES
};

enum /* media sounds */
{
	_media_snd_feet_entering,
	_media_snd_feet_leaving,
	_media_snd_head_entering,
	_media_snd_head_leaving,
	_media_snd_splashing,
	_media_snd_ambient_over,
	_media_snd_ambient_under,
	_media_snd_platform_entering,
	_media_snd_platform_leaving,
	
	NUMBER_OF_MEDIA_SOUNDS
};

/* ---------- macros */

#define UNDER_MEDIA(m, z) ((z)<=(m)->height)

/* ---------- structures */

struct media_data /* 32 bytes */
{
	int16 type;
	uint16 flags;

	/* this light is not used as a real light; instead, the intensity of this light is used to
		determine the height of the media: height= low + (high-low)*intensity ... this sounds
		gross, but it makes media heights as flexible as light intensities; clearly discontinuous
		light functions (e.g., strobes) should not be used */
	int16 light_index;

	/* this is the maximum external velocity due to current; acceleration is 1/32nd of this */
	angle current_direction;
	world_distance current_magnitude;
	
	world_distance low, high;
	
	world_point2d origin;
	world_distance height;

	_fixed minimum_light_intensity;
	shape_descriptor texture;
	int16 transfer_mode;
	
	int16 unused[2];
};
const int SIZEOF_media_data = 32;

/* --------- globals */


// Turned the list of lights into a variable array;
// took over their maximum number as how many of them

extern vector<media_data> MediaList;
#define medias (MediaList.data())
#define MAXIMUM_MEDIAS_PER_MAP (MediaList.size())

// extern struct media_data *medias;

/* --------- prototypes/MEDIA.C */

size_t new_media(struct media_data *data);

void update_medias(void);

void get_media_detonation_effect(short media_index, short type, short *detonation_effect);
short get_media_sound(short media_index, short type);
short get_media_submerged_fade_effect(short media_index);
struct damage_definition *get_media_damage(short media_index, _fixed scale);
bool get_media_collection(short media_index, short& collection);

// LP addition: media dangerous?
bool IsMediaDangerous(short media_type);

bool media_in_environment(short media_type, short environment_code);

media_data *get_media_data(
	const size_t media_index);

// LP addition: count number of media types used,
// for better Infinity compatibility when saving games
size_t count_number_of_medias_used();

// LP: routines for packing and unpacking the data from streams of bytes

uint8 *unpack_media_data(uint8 *Stream, media_data* Objects, size_t Count);
uint8 *pack_media_data(uint8 *Stream, media_data* Objects, size_t Count);

class InfoTree;
void parse_mml_liquids(const InfoTree& root);
void reset_mml_liquids();

#endif

