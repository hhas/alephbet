#ifndef __RENDER_H
#define __RENDER_H

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

#include "scottish_textures.hpp"
#include "textures.hpp"
#include "world.hpp"
// Stuff to control the view
#include "ViewControl.hpp"

/* ---------- constants */

/* the distance behind which we are not required to draw objects */
#define MINIMUM_OBJECT_DISTANCE ((short)(WORLD_ONE / 20))

// LP change: suppressed going/leaving states, because of alternative way of
// adjusting the FOV.
enum /* render effects */
{
    _render_effect_fold_in,
    _render_effect_fold_out,
    // _render_effect_going_fisheye,
    // _render_effect_leaving_fisheye,
    _render_effect_explosion,
    // LP additions:
    // _render_effect_going_tunnel,
    // _render_effect_leaving_tunnel
};

enum /* shading tables */
{
    _shading_normal,     /* to black */
    _shading_infravision /* false color */
};

// LP change: using accessors instead
#define TUNNEL_VISION_FIELD_OF_VIEW View_FOV_TunnelVision()
#define NORMAL_FIELD_OF_VIEW        View_FOV_Normal()
#define EXTRAVISION_FIELD_OF_VIEW   View_FOV_ExtraVision()

/* ---------- structures */

struct definition_header {
    short tag;
    short clip_left, clip_right;
};

struct view_data {
    // LP change: specifying current and target field-of-view as floats;
    // one changes the field of view by setting a new target then adjusting
    // the current FOV toward it
    float field_of_view;
    float target_field_of_view;
    short standard_screen_width;       /* this is *not* the width of the projected image (see initialize_view_data() in
                                          RENDER.C */
    short screen_width, screen_height; /* dimensions of the projected image */
    short horizontal_scale, vertical_scale;

    short half_screen_width, half_screen_height;
    short world_to_screen_x, world_to_screen_y;
    short dtanpitch; /* world_to_screen*tan(pitch) */
    angle half_cone; /* often ==field_of_view/2 (when screen_width==standard_screen_width) */
    angle half_vertical_cone;

    long_vector2d left_edge, right_edge; // view cone edges as world directions

    short ticks_elapsed;
    uint32 tick_count; /* for effects and transfer modes */
    float heartbeat_fraction;
    short origin_polygon_index;
    angle yaw, pitch, roll;
    fixed_angle virtual_yaw, virtual_pitch;
    world_point3d origin;
    _fixed maximum_depth_intensity; /* in fixed units */

    short shading_mode;

    short effect, effect_phase;
    short real_world_to_screen_x, real_world_to_screen_y;

    bool overhead_map_active;
    short overhead_map_scale;

    bool under_media_boundary;
    short under_media_index;

    bool terminal_mode_active;

    // LP addition: this indicates whether to show weapons-in-hand display;
    // this is on in first-person, off in third-person
    bool show_weapons_in_hand;

    // LP: Indicates whether or not tunnel vision is active
    bool tunnel_vision_active;

    // LP addition: value of yaw used by landscapes; this is so that the center
    // can stay stationary
    angle landscape_yaw;

    // whether to mimic software renderer when looking up/down
    bool mimic_sw_perspective;
};

/* ---------- render flags */

#define TEST_STATE_FLAG(i, f)   TEST_RENDER_FLAG(i, f)
#define SET_STATE_FLAG(i, f, v) SET_RENDER_FLAG(i, f)

#define TEST_RENDER_FLAG(index, flag) (render_flags[index] & (flag))
#define SET_RENDER_FLAG(index, flag)  render_flags[index] |= (flag)

#define RENDER_FLAGS_BUFFER_SIZE \
    MAX(MAX(MAXIMUM_ENDPOINTS_PER_MAP, MAXIMUM_LINES_PER_MAP), MAX(MAXIMUM_SIDES_PER_MAP, MAXIMUM_POLYGONS_PER_MAP))

// #define RENDER_FLAGS_BUFFER_SIZE (8*KILO)
enum /* render flags */
{
    _polygon_is_visible_bit,            /* some part of this polygon is horizontally in the view cone */
    _endpoint_has_been_visited_bit,     /* we’ve already tried to cast a ray out at this endpoint */
    _endpoint_is_visible_bit,           /* this endpoint is horizontally in the view cone */
    _side_is_visible_bit,               /* this side was crossed while building the tree and should be drawn */
    _line_has_clip_data_bit,            /* this line has a valid clip entry */
    _endpoint_has_clip_data_bit,        /* this endpoint has a valid clip entry */
    _endpoint_has_been_transformed_bit, /* this endpoint has been transformed into screen-space */
    NUMBER_OF_RENDER_FLAGS,             /* should be <=16 */

    _polygon_is_visible            = 1 << _polygon_is_visible_bit,
    _endpoint_has_been_visited     = 1 << _endpoint_has_been_visited_bit,
    _endpoint_is_visible           = 1 << _endpoint_is_visible_bit,
    _side_is_visible               = 1 << _side_is_visible_bit,
    _line_has_clip_data            = 1 << _line_has_clip_data_bit,
    _endpoint_has_clip_data        = 1 << _endpoint_has_clip_data_bit,
    _endpoint_has_been_transformed = 1 << _endpoint_has_been_transformed_bit
};

/* ---------- globals */

extern vector<uint16> RenderFlagList;
#define render_flags (RenderFlagList.data())

// extern uint16 *render_flags;

/* ---------- prototypes/RENDER.C */

void allocate_render_memory(void);

void initialize_view_data(struct view_data* view, bool ignore_preferences = false);
void render_view(struct view_data* view, struct bitmap_definition* software_render_dest /*ignored under OpenGL*/);

void start_render_effect(struct view_data* view, short effect);

void check_m1_exploration(void);


/* ----------- prototypes/SCREEN.C */
void render_overhead_map(struct view_data* view);
void render_computer_interface(struct view_data* view);

// LP: definitions moved up here because they are referred to
// outside of render.c, where they are defined.

void instantiate_rectangle_transfer_mode(view_data* view, rectangle_definition* rectangle, short transfer_mode,
                                         _fixed transfer_phase);

void instantiate_polygon_transfer_mode(view_data* view, polygon_definition* polygon, short transfer_mode,
                                       bool horizontal);


// In overhead_map.cpp:

void ResetOverheadMap();

#endif
