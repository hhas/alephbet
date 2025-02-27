#ifndef _RENDER_RASTERIZER_CLASS_
#define _RENDER_RASTERIZER_CLASS_

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
 *  Rendering Clipping/Rasterization Class
 *  Defines a class for doing rasterization from the prepared lists of objects;
 *  from render.c
 */

#include "Rasterizer.hpp"
#include "RenderPlaceObjs.hpp"
#include "RenderSortPoly.hpp"
#include "render.hpp"
#include "world.hpp"
#include <vector>

/* ---------- flagged world points */

struct flagged_world_point2d /* for floors */
{
    // LP change: made this more long-distance friendly
    int32 x, y;
    uint16 flags; /* _clip_left, _clip_right, _clip_top, _clip_bottom are valid */
};

struct flagged_world_point3d /* for ceilings */
{
    // LP change: made this more long-distance friendly
    int32 x, y;
    world_distance z;
    uint16 flags; /* _clip_left, _clip_right, _clip_top, _clip_bottom are valid */
};

/* ---------- vertical surface definition */

/* it’s not worth putting this into the side_data structure, although the transfer mode should
    be in the side_texture_definition structure */
struct vertical_surface_data {
    short lightsource_index;
    _fixed ambient_delta; /* a delta to the lightsource’s intensity, then pinned to [0,FIXED_ONE] */

    world_distance length;
    world_distance h0, h1, hmax; /* h0<h1; hmax<=h1 and is the height where this wall side meets the ceiling */
    // LP change: made this more long-distance friendly
    long_vector2d p0, p1; /* will transform into left, right points on the screen (respectively) */

    struct side_texture_definition* texture_definition;
    short transfer_mode;
};

typedef enum {
    kDiffuse,
    kGlow
} RenderStep;

class RenderRasterizerClass {
  protected:

    // Auxiliary data and routines:
    virtual void render_tree(RenderStep renderStep);
    virtual void render_node(sorted_node_data* node, bool SeeThruLiquids, RenderStep renderStep);
    virtual void store_endpoint(endpoint_data* endpoint, long_vector2d& p);

    // LP change: indicate whether the void is present on one side;
    // useful for suppressing semitransparency to the void
    virtual void render_node_floor_or_ceiling(clipping_window_data* window, polygon_data* polygon,
                                              horizontal_surface_data* surface, bool void_present, bool ceil,
                                              RenderStep renderStep);
    virtual void render_node_side(clipping_window_data* window, vertical_surface_data* surface, bool void_present,
                                  RenderStep renderStep);

    // LP change: add "other side of media" flag, to indicate that the sprite will be rendered
    // on the opposite side of the liquid surface from the viewpoint, instead of the same side.
    virtual void render_node_object(render_object_data* object, bool other_side_of_media, RenderStep renderStep);

    // LP changes for better long-distance support

    short xy_clip_horizontal_polygon(flagged_world_point2d* vertices, short vertex_count, long_vector2d* line,
                                     uint16 flag);

    void xy_clip_flagged_world_points(flagged_world_point2d* p0, flagged_world_point2d* p1,
                                      flagged_world_point2d* clipped, long_vector2d* line);

    short z_clip_horizontal_polygon(flagged_world_point2d* vertices, short vertex_count, long_vector2d* line,
                                    world_distance height, uint16 flag);

    void z_clip_flagged_world_points(flagged_world_point2d* p0, flagged_world_point2d* p1, world_distance height,
                                     flagged_world_point2d* clipped, long_vector2d* line);

    short xz_clip_vertical_polygon(flagged_world_point3d* vertices, short vertex_count, long_vector2d* line,
                                   uint16 flag);

    void xz_clip_flagged_world_points(flagged_world_point3d* p0, flagged_world_point3d* p1,
                                      flagged_world_point3d* clipped, long_vector2d* line);

    short xy_clip_line(flagged_world_point2d* posts, short vertex_count, long_vector2d* line, uint16 flag);

  public:

    // Pointers to view and sorted polygons
    view_data* view;
    RenderSortPolyClass* RSPtr;
    RasterizerClass* RasPtr;

    virtual void render_tree();

    virtual bool renders_viewer_sprites_in_tree() { return false; }

    // Inits everything
    RenderRasterizerClass();
};


#endif
