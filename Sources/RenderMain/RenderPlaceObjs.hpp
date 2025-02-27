#ifndef _RENDER_PLACE_OBJECTS_CLASS_
#define _RENDER_PLACE_OBJECTS_CLASS_

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

/*
 *  Rendering Object-Placement Class
 *  Defines a class for placing inhabitants in appropriate rendering order;
 *  from render.c. Works from RenderSortPoly stuff.
 */

#include "RenderSortPoly.hpp"
#include "interface.hpp"
#include "render.hpp"
#include "world.hpp"
#include <vector>

/* ---------- render objects */

struct render_object_data {
    struct sorted_node_data* node;                 /* node we are being drawn inside */
    struct clipping_window_data* clipping_windows; /* our privately calculated clipping window */

    struct render_object_data* next_object; /* the next object in this chain */

    struct rectangle_definition rectangle;

    int16 ymedia;
};

class RenderPlaceObjsClass {
    struct span_data;

    void initialize_render_object_list();

    render_object_data* build_render_object(object_data* object, _fixed floor_intensity, _fixed ceiling_intensity,
                                            float Opacity, long_point3d* origin, long_point3d* rel_origin);

    void sort_render_object_into_tree(render_object_data* new_render_object, const span_data& span);

    span_data build_base_node_list(const render_object_data* render_object, short origin_polygon_index);

    void build_aggregate_render_object_clipping_window(render_object_data* render_object, const span_data& span);

    bool add_object_to_sorted_nodes(object_data* object, _fixed floor_intensity, _fixed ceiling_intensity,
                                    float Opacity);

    shape_information_data* rescale_shape_information(shape_information_data* unscaled, shape_information_data* scaled,
                                                      uint16 flags);

  public:

    // LP additions: growable list of render objects; these are all the inhabitants
    // Length changed in build_render_object()
    // keep SortedNodes in sync
    vector<render_object_data> RenderObjects;

    // Pointers to view and calculated visibility tree and sorted polygons
    view_data* view;
    RenderVisTreeClass* RVPtr;
    RenderSortPolyClass* RSPtr;

    void build_render_object_list();

    // Inits everything
    RenderPlaceObjsClass();
};

#endif
