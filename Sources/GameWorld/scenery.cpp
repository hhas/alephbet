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

#include <stdlib.h>
#include <string.h>
#include <vector>

#include "InfoTree.hpp"
#include "cseries.hpp"
#include "effects.hpp"
#include "flood_map.hpp"
#include "interface.hpp"
#include "map.hpp"
#include "monsters.hpp"
#include "platforms.hpp"
#include "player.hpp"
#include "projectiles.hpp"
#include "render.hpp"
#include "scenery.hpp"

/* ---------- constants */

enum {
    MAXIMUM_ANIMATED_SCENERY_OBJECTS = 20
};

/* ---------- globals */

#include "scenery_definitions.hpp"


// LP change: growable list of animated scenery objects
static vector<short> AnimatedSceneryObjects;


/* ---------- private prototypes */

struct scenery_definition* get_scenery_definition(short scenery_type);

/* ---------- code */

void initialize_scenery(void) {
    // Will reserve some space for them
    AnimatedSceneryObjects.reserve(32);
}

/* returns object index if successful, NONE otherwise */
short new_scenery(struct object_location* location, short scenery_type) {
    short object_index = NONE;

    // This get function will do the bounds checking
    struct scenery_definition* definition = get_scenery_definition(scenery_type);
    if (!definition)
        return NONE;

    object_index = new_map_object(location, definition->shape);
    if (object_index != NONE) {
        struct object_data* object = get_object_data(object_index);

        SET_OBJECT_OWNER(object, _object_is_scenery);
        SET_OBJECT_SOLIDITY(object, (definition->flags & _scenery_is_solid) ? true : false);
        object->permutation = scenery_type;
    }

    return object_index;
}

void animate_scenery(void) {
    for (unsigned i = 0; i < AnimatedSceneryObjects.size(); ++i) animate_object(AnimatedSceneryObjects[i]);
}

void deanimate_scenery(short object_index) {
    std::vector<short>::iterator it;
    for (it = AnimatedSceneryObjects.begin(); it != AnimatedSceneryObjects.end() && *it != object_index; it++);

    if (it != AnimatedSceneryObjects.end())
        AnimatedSceneryObjects.erase(it);
}

void randomize_scenery_shape(short object_index) {
    object_data* object            = get_object_data(object_index);
    scenery_definition* definition = get_scenery_definition(object->permutation);
    if (!definition)
        return;

    if (!randomize_object_sequence(object_index, definition->shape)) {
        AnimatedSceneryObjects.push_back(object_index);
    }
}

void randomize_scenery_shapes(void) {
    struct object_data* object;
    short object_index;

    AnimatedSceneryObjects.clear();

    for (object_index = 0, object = objects; object_index < MAXIMUM_OBJECTS_PER_MAP; ++object_index, ++object) {
        if (SLOT_IS_USED(object) && GET_OBJECT_OWNER(object) == _object_is_scenery) {
            struct scenery_definition* definition = get_scenery_definition(object->permutation);
            if (!definition)
                continue;

            if (!randomize_object_sequence(object_index, definition->shape)) {
                AnimatedSceneryObjects.push_back(object_index);
            }
        }
    }
}

void get_scenery_dimensions(short scenery_type, world_distance* radius, world_distance* height) {
    struct scenery_definition* definition = get_scenery_definition(scenery_type);
    if (!definition) {
        // Fallback
        *radius = 0;
        *height = 0;
        return;
    }

    *radius = definition->radius;
    *height = definition->height;
}

void damage_scenery(short object_index) {
    struct object_data* object            = get_object_data(object_index);
    struct scenery_definition* definition = get_scenery_definition(object->permutation);
    if (!definition)
        return;

    if (definition->flags & _scenery_can_be_destroyed) {
        object->shape    = definition->destroyed_shape;
        object->sequence = 0;

        if (film_profile.fix_destroy_scenery_random_frame) {
            randomize_object_sequence(object_index, object->shape);
        }

        // LP addition: don't create a destruction effect if the effect type is NONE
        if (definition->destroyed_effect != NONE)
            new_effect(&object->location, object->polygon, definition->destroyed_effect, object->facing);
        SET_OBJECT_OWNER(object, _object_is_normal);
    }
}

bool get_scenery_collection(short scenery_type, short& collection) {
    struct scenery_definition* definition = get_scenery_definition(scenery_type);
    if (!definition)
        return false;

    collection = GET_DESCRIPTOR_COLLECTION(definition->shape);
    return true;
}

bool get_damaged_scenery_collection(short scenery_type, short& collection) {
    struct scenery_definition* definition = get_scenery_definition(scenery_type);
    if (!definition || !(definition->flags & _scenery_can_be_destroyed))
        return false;

    collection = GET_DESCRIPTOR_COLLECTION(definition->destroyed_shape);
    return true;
}

/* ---------- private code */

struct scenery_definition* get_scenery_definition(short scenery_type) {
    return GetMemberWithBounds(scenery_definitions, scenery_type, NUMBER_OF_SCENERY_DEFINITIONS);
}

// UGH: level MML loads *after* game_wad has instantiated map scenery,
// so, if we see scenery solidity change, go through and reset all the
// scenery object solidity

bool ok_to_reset_scenery_solidity = false;

static void reset_scenery_solidity() {
    if (!ok_to_reset_scenery_solidity)
        return;

    for (int i = 0; i < MAXIMUM_OBJECTS_PER_MAP; ++i) {
        object_data* object = &objects[i];
        if (SLOT_IS_USED(object) && GET_OBJECT_OWNER(object) == _object_is_scenery) {
            scenery_definition* definition = get_scenery_definition(object->permutation);
            if (!definition)
                break;

            SET_OBJECT_SOLIDITY(object, definition->flags & _scenery_is_solid);
        }
    }
}


struct scenery_definition* original_scenery_definitions = NULL;

void reset_mml_scenery() {
    if (original_scenery_definitions) {
        for (unsigned i = 0; i < NUMBER_OF_SCENERY_DEFINITIONS; i++)
            scenery_definitions[i] = original_scenery_definitions[i];
        free(original_scenery_definitions);
        original_scenery_definitions = NULL;
    }
}

void parse_mml_scenery(const InfoTree& root) {
    // back up old values first
    if (!original_scenery_definitions) {
        original_scenery_definitions
                = (struct scenery_definition*)malloc(sizeof(struct scenery_definition) * NUMBER_OF_SCENERY_DEFINITIONS);
        assert(original_scenery_definitions);
        for (unsigned i = 0; i < NUMBER_OF_SCENERY_DEFINITIONS; i++)
            original_scenery_definitions[i] = scenery_definitions[i];
    }

    for (const InfoTree& object : root.children_named("object")) {
        int16 index;
        if (!object.read_indexed("index", index, NUMBER_OF_SCENERY_DEFINITIONS))
            continue;
        scenery_definition& def = scenery_definitions[index];

        object.read_attr("flags", def.flags);
        object.read_attr("radius", def.radius);
        object.read_attr("height", def.height);
        object.read_indexed("destruction", def.destroyed_effect, NUMBER_OF_EFFECT_TYPES, true);

        for (const InfoTree& child : object.children_named("normal"))
            for (const InfoTree& shape : child.children_named("shape")) shape.read_shape(def.shape);
        for (const InfoTree& child : object.children_named("destroyed"))
            for (const InfoTree& shape : child.children_named("shape")) shape.read_shape(def.destroyed_shape);
    }

    reset_scenery_solidity();
}
