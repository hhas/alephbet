#ifndef MARATHON_DYNAMIC_ENTITY_LIMITS
#define MARATHON_DYNAMIC_ENTITY_LIMITS

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

#include "cstypes.hpp"

// Limit types:
enum {
    _dynamic_limit_objects,             // Objects (every possible kind)
    _dynamic_limit_monsters,            // NPC's
    _dynamic_limit_paths,               // Paths for NPC's to follow (determines how many may be active)
    _dynamic_limit_projectiles,         // Projectiles
    _dynamic_limit_effects,             // Currently-active effects (blood splatters, explosions, etc.)
    _dynamic_limit_rendered,            // Number of objects to render
    _dynamic_limit_local_collision,     // [16] Local collision buffer (target visibility, NPC-NPC collisions, etc.)
    _dynamic_limit_global_collision,    // [64] Global collision buffer (projectiles with other objects)
    _dynamic_limit_ephemera,            // [1024] Ephemeral objects (render effects)
    _dynamic_limit_garbage,             // Garbage objects (corpses) across the whole map
    _dynamic_limit_garbage_per_polygon, // Garbage objects (corpses) within a single polygon
    NUMBER_OF_DYNAMIC_LIMITS
};


class InfoTree;
void parse_mml_dynamic_limits(const InfoTree& root);
void reset_mml_dynamic_limits();

// Accessor
uint16 get_dynamic_limit(int which);

// call this after changing the film profile but before loading MML
void reset_dynamic_limits();

#endif
