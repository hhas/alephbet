#ifndef __SHAPE_DEFINITIONS_H
#define __SHAPE_DEFINITIONS_H

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

#include "shape_descriptors.hpp"

struct collection_definition;

/* ---------- structures */

struct collection_header /* 32 bytes on disk */
{
    int16 status;
    uint16 flags;

    int32 offset, length;
    int32 offset16, length16;

    // LP: handles to pointers
    collection_definition* collection;
    std::vector<byte> shading_tables;
};

const int SIZEOF_collection_header = 32;

/* ---------- globals */

static struct collection_header collection_headers[MAXIMUM_COLLECTIONS];

#endif
