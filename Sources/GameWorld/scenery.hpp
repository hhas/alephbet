#ifndef __SCENERY_H
#define __SCENERY_H

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

#include "world.hpp"

/* ---------- prototypes/SCENERY.C */

void initialize_scenery(void);

short new_scenery(struct object_location* location, short scenery_type);

void animate_scenery(void);

// ghs: allow Lua to add and delete scenery
void deanimate_scenery(short object_index);
void randomize_scenery_shape(short object_index);

void randomize_scenery_shapes(void);

void get_scenery_dimensions(short scenery_type, world_distance* radius, world_distance* height);
void damage_scenery(short object_index);

bool get_scenery_collection(short scenery_type, short& collection);
bool get_damaged_scenery_collection(short scenery_type, short& collection);

class InfoTree;
void parse_mml_scenery(const InfoTree& root);
void reset_mml_scenery();

#endif
