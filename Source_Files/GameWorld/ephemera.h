#ifndef EPHEMERA_H
#define EPHEMERA_H

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

#include <cstdint>

#include "map.h"
#include "shape_descriptors.h"
#include "world.h"

// object owner flags are unused, so we can re-use them to specify behavior
enum {
	_ephemera_end_when_animation_loops = 0x0004
};

void allocate_ephemera_storage(int max_ephemera);
void init_ephemera(int16_t polygon_count);
int16_t new_ephemera(const world_point3d& origin, int16_t polygon_index, shape_descriptor shape, angle facing);
void remove_ephemera(int16_t ephemera_index);

object_data* get_ephemera_data(int16_t ephemera_index);
int16_t get_polygon_ephemera(int16_t polygon_index);

void remove_ephemera_from_polygon(int16_t ephemera_index);
void add_ephemera_to_polygon(int16_t ephemera_index, int16_t polygon_index);

void set_ephemera_shape(int16_t ephemera_index, shape_descriptor shape);

void note_ephemera_polygon_rendered(int16_t polygon_index);

void update_ephemera();

#endif
