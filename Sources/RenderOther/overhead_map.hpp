#ifndef __OVERHEAD_MAP_H
#define __OVERHEAD_MAP_H

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

#define OVERHEAD_MAP_MINIMUM_SCALE 1
#define OVERHEAD_MAP_MAXIMUM_SCALE 4
#define DEFAULT_OVERHEAD_MAP_SCALE 3

enum /* modes */
{
    _rendering_saved_game_preview,
    _rendering_checkpoint_map,
    _rendering_game_map
};

struct overhead_map_data {
    short mode;
    short scale;
    world_point2d origin;
    short origin_polygon_index;
    short half_width, half_height;
    short width, height;
    short top, left;

    bool draw_everything;
};

void _render_overhead_map(struct overhead_map_data* data);

class InfoTree;
void parse_mml_overhead_map(const InfoTree& root);
void reset_mml_overhead_map();

#endif
