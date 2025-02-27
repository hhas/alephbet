#ifndef __SHAPE_DESCRIPTORS_H
#define __SHAPE_DESCRIPTORS_H

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

/* ---------- types */

typedef uint16 shape_descriptor; /* [clut.3] [collection.5] [shape.8] */

#define DESCRIPTOR_SHAPE_BITS      8
#define DESCRIPTOR_COLLECTION_BITS 5
#define DESCRIPTOR_CLUT_BITS       3

#define MAXIMUM_COLLECTIONS           (1 << DESCRIPTOR_COLLECTION_BITS)
#define MAXIMUM_SHAPES_PER_COLLECTION (1 << DESCRIPTOR_SHAPE_BITS)
#define MAXIMUM_CLUTS_PER_COLLECTION  (1 << DESCRIPTOR_CLUT_BITS)

/* ---------- collections */

enum /* collection numbers */
{
    _collection_interface,       // 0
    _collection_weapons_in_hand, // 1
    _collection_juggernaut,      // 2
    _collection_tick,            // 3
    _collection_rocket,          // 4 -- LP: also known as "Explosion Effects"
    _collection_hunter,          // 5
    _collection_player,          // 6
    _collection_items,           // 7
    _collection_trooper,         // 8
    _collection_fighter,         // 9
    _collection_defender,        // 10
    _collection_yeti,            // 11
    _collection_civilian,        // 12
    _collection_civilian_fusion, // 13 -- LP: formerly _collection_madd
    _collection_enforcer,        // 14
    _collection_hummer,          // 15
    _collection_compiler,        // 16
    _collection_walls1,          // 17 -- LP: Lh'owon water
    _collection_walls2,          // 18 -- LP: Lh'owon lava
    _collection_walls3,          // 19 -- LP: Lh'owon sewage
    _collection_walls4,          // 20 -- LP: Jjaro
    _collection_walls5,          // 21 -- LP: Pfhor
    _collection_scenery1,        // 22 -- LP: Lh'owon water
    _collection_scenery2,        // 23 -- LP: Lh'owon lava
    _collection_scenery3,        // 24 -- LP: Lh'owon sewage
    _collection_scenery4,        // 25 pathways -- LP: Jjaro
    _collection_scenery5,        // 26 alien -- LP: Pfhor
    _collection_landscape1,      // 27 day -- LP: Lh'owon day
    _collection_landscape2,      // 28 night -- LP: Lh'owon night
    _collection_landscape3,      // 29 moon -- LP: Lh'owon moon
    _collection_landscape4,      // 30 -- LP: outer space
    _collection_cyborg,          // 31

    NUMBER_OF_COLLECTIONS
};

/* ---------- macros */

#define GET_DESCRIPTOR_SHAPE(d) ((d) & (uint16)(MAXIMUM_SHAPES_PER_COLLECTION - 1))
#define GET_DESCRIPTOR_COLLECTION(d) \
    (((d) >> DESCRIPTOR_SHAPE_BITS) & (uint16)((1 << (DESCRIPTOR_COLLECTION_BITS + DESCRIPTOR_CLUT_BITS)) - 1))
#define BUILD_DESCRIPTOR(collection, shape) (((collection) << DESCRIPTOR_SHAPE_BITS) | (shape))

#define BUILD_COLLECTION(collection, clut) ((collection) | (uint16)((clut) << DESCRIPTOR_COLLECTION_BITS))
#define GET_COLLECTION_CLUT(collection) \
    (((collection) >> DESCRIPTOR_COLLECTION_BITS) & (uint16)(MAXIMUM_CLUTS_PER_COLLECTION - 1))
#define GET_COLLECTION(collection) ((collection) & (MAXIMUM_COLLECTIONS - 1))

#endif
