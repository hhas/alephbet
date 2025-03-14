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

#include "InfoTree.hpp"
#include "dynamic_limits.hpp"
#include "effects.hpp"
#include "ephemera.hpp"
#include "flood_map.hpp"
#include "map.hpp"
#include "monsters.hpp"
#include "projectiles.hpp"
#include <string.h>

// original defaults
static uint16 m2_dynamic_limits[NUMBER_OF_DYNAMIC_LIMITS] = {
        384,  // Objects (every possible kind)
        220,  // NPC's
        20,   // Paths for NPC's to follow (determines how many may be active)
        32,   // Projectiles
        64,   // Currently-active effects (blood splatters, explosions, etc.)
        1024, // Number of objects to render (was really 72, but
              // doesn't affect film playback)
        16,   // Local collision buffer (target visibility, NPC-NPC collisions, etc.)
        64,   // Global collision buffer (projectiles with other objects)
        4096, // Ephemeral objects (render effects)
        256,  // Garbage objects (corpses) across the whole map
        10,   // Garbage objects (corpses) in a single polygon
};

// expanded defaults up to 1.0
static uint16 a1_1_0_dynamic_limits[NUMBER_OF_DYNAMIC_LIMITS] = {
        1024, // Objects (every possible kind)
        512,  // NPC's
        128,  // Paths for NPC's to follow (determines how many may be active)
        128,  // Projectiles
        128,  // Currently-active effects (blood splatters, explosions, etc.)
        1024, // Number of objects to render
        64,   // Local collision buffer (target visibility, NPC-NPC collisions, etc.)
        256,  // Global collision buffer (projectiles with other objects)
        4096, // Ephemeral objects (render effects)
        256,  // Garbage objects (corpses) across the whole map
        10,   // Garbage objects (corpses) in a single polygon
};

// 1.1 reverts paths for classic scenario compatibility
static uint16 a1_1_1_dynamic_limits[NUMBER_OF_DYNAMIC_LIMITS] = {
        1024, // Objects (every possible kind)
        512,  // NPC's
        20,   // Paths for NPC's to follow (determines how many may be active)
        128,  // Projectiles
        128,  // Currently-active effects (blood splatters, explosions, etc.)
        1024, // Number of objects to render
        64,   // Local collision buffer (target visibility, NPC-NPC collisions, etc.)
        256,  // Global collision buffer (projectiles with other objects)
        4096, // Ephemeral objects (render effects)
        256,  // Garbage objects (corpses) across the whole map
        10,   // Garbage objects (corpses) in a single polygon
};

static std::vector<uint16> dynamic_limits(NUMBER_OF_DYNAMIC_LIMITS);

static bool dynamic_limits_loaded = false;

void reset_dynamic_limits() {
    if (film_profile.increased_dynamic_limits_1_1) {
        dynamic_limits.assign(a1_1_1_dynamic_limits, a1_1_1_dynamic_limits + NUMBER_OF_DYNAMIC_LIMITS);
    } else if (film_profile.increased_dynamic_limits_1_0) {
        dynamic_limits.assign(a1_1_0_dynamic_limits, a1_1_0_dynamic_limits + NUMBER_OF_DYNAMIC_LIMITS);
    } else {
        dynamic_limits.assign(m2_dynamic_limits, m2_dynamic_limits + NUMBER_OF_DYNAMIC_LIMITS);
    }

    dynamic_limits_loaded = true;
}

void reset_mml_dynamic_limits() { reset_dynamic_limits(); }

void parse_limit_value(const InfoTree& root, std::string child, int type) {
    for (const InfoTree& limit : root.children_named(child))
        limit.read_attr_bounded<uint16>("value", dynamic_limits[type], 0, 32'767);
}

void parse_mml_dynamic_limits(const InfoTree& root) {
    parse_limit_value(root, "objects", _dynamic_limit_objects);
    parse_limit_value(root, "monsters", _dynamic_limit_monsters);
    parse_limit_value(root, "paths", _dynamic_limit_paths);
    parse_limit_value(root, "projectiles", _dynamic_limit_projectiles);
    parse_limit_value(root, "effects", _dynamic_limit_effects);
    parse_limit_value(root, "rendered", _dynamic_limit_rendered);
    parse_limit_value(root, "local_collision", _dynamic_limit_local_collision);
    parse_limit_value(root, "global_collision", _dynamic_limit_global_collision);
    parse_limit_value(root, "ephemera", _dynamic_limit_ephemera);
    parse_limit_value(root, "garbage", _dynamic_limit_garbage);
    parse_limit_value(root, "garbage_per_polygon", _dynamic_limit_garbage_per_polygon);

    // Resize the arrays of objects, monsters, effects, and projectiles
    EffectList.resize(MAXIMUM_EFFECTS_PER_MAP);
    ObjectList.resize(MAXIMUM_OBJECTS_PER_MAP);
    MonsterList.resize(MAXIMUM_MONSTERS_PER_MAP);
    ProjectileList.resize(MAXIMUM_PROJECTILES_PER_MAP);

    // Resize the array of paths also
    allocate_pathfinding_memory();

    allocate_ephemera_storage(dynamic_limits[_dynamic_limit_ephemera]);
}

// Accessor
uint16 get_dynamic_limit(int which) {
    if (dynamic_limits_loaded) {
        return dynamic_limits[which];
    } else {
        return a1_1_1_dynamic_limits[which];
    }
}
