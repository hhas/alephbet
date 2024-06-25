#ifndef _RENDER_SORT_POLYGONS_CLASS_
#define _RENDER_SORT_POLYGONS_CLASS_

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
 *  Rendering Polygon-Sorting Class
 *  Defines a class for sorting the polygons into appropriate depth order;
 *  from render.c. Works from RenderVisTree stuff.
 */

#include <vector>
#include "world.h"
#include "render.h"
#include "RenderVisTree.h"


/* ---------- sorted nodes */

struct sorted_node_data
{
	short polygon_index;
	
	struct render_object_data *interior_objects;
	struct render_object_data *exterior_objects;
	
	struct clipping_window_data *clipping_windows;
};


class RenderSortPolyClass
{
	// Auxiliary data and routines:

	void initialize_sorted_render_tree();
	
	clipping_window_data *build_clipping_windows(node_data *ChainBegin);

	void calculate_vertical_clip_data(line_clip_data **accumulated_line_clips,
		size_t accumulated_line_clip_count, clipping_window_data *window, short x0, short x1);
		
public:
	
	 /* converts map polygon indexes to sorted nodes (only valid if _polygon_is_visible) */
	vector<sorted_node_data *> polygon_index_to_sorted_node;
	
	// LP additions: growable list of sorted nodes
	// Length changed in initialize_sorted_render_tree() and sort_render_tree()
	// When being built, the render objects are yet to be listed
	vector<sorted_node_data> SortedNodes;
	
	// LP addition: growable lists of accumulations of endpoint and line clips
	// used in build_clipping_windows()
	vector<endpoint_clip_data *> AccumulatedEndpointClips;
	vector<line_clip_data *> AccumulatedLineClips;

	// Pointers to view and calculated visibility tree
	view_data *view;
	RenderVisTreeClass *RVPtr;
	
	// Resizes all the objects defined inside;
	// the resizing is lazy
	void Resize(size_t NumPolygons);

	// Does the sorting
	void sort_render_tree();
  	
  	// Inits everything
 	RenderSortPolyClass();
};

// Historical note: cause of too-many-transparent-line errors
// LP addition: node-alias growable list
// Only used in sort_render_tree()
// Suppressed as unnecessary because of node_data polygon-sorted-tree structure
// static GrowableList<node_data *> NodeAliases(32);


#endif
