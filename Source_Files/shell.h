#ifndef __SHELL_H
#define __SHELL_H

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

#include "cstypes.h"
#include <string>

class FileSpecifier;
struct RGBColor;
struct SDL_Color;
struct SDL_Surface;

/* ---------- constants */

/* ---------- resources */

enum {
	strPROMPTS= 131,
	_save_game_prompt= 0,
	_save_replay_prompt,
	_select_replay_prompt,
	_default_prompt
};

enum class BobbingType
{
	none,
	camera_and_weapon,
	weapon_only
};

/* ---------- structures */

struct screen_mode_data
{
	short acceleration;
	
	bool high_resolution;
	bool fullscreen;
	bool draw_every_other_line;
	
	short bit_depth;  // currently 8 or 16
	short gamma_level;

	short width;
	short height;
	bool auto_resolution;
	bool high_dpi;
	bool hud;
	short hud_scale_level;
	short term_scale_level;
	bool fix_h_not_v;
	bool translucent_map;
	BobbingType bobbing_type;

	int fov; // 0 = use default (or MML/plugin)
};

#define NUMBER_OF_KEYS 21
#define NUMBER_UNUSED_KEYS 10

enum // input devices
{
	_keyboard_or_game_pad,
	_mouse_yaw_pitch
};

#define PREFERENCES_NAME_LENGTH 32

/* ---------- prototypes/SHELL.C [now shell_misc.cpp, shell_macintosh.cpp, shell_sdl.cpp] */

void global_idle_proc(void);

// Load the base MML scripts:
void LoadBaseMMLScripts(bool load_menu_mml_only);

// Application and directory info:
char *expand_symbolic_paths(char *dest, const char *src, int maxlen);
char *contract_symbolic_paths(char *dest, const char *src, int maxlen);

/* ---------- prototypes/SHAPES.C */

void initialize_shape_handler(void);

// ZZZ: this now works with RLE'd shapes, but needs extra storage.  Caller should
// be prepared to take a byte* if using an RLE shape (it will be set to NULL if
// shape is straight-coded); caller will need to free() that storage after freeing
// the SDL_Surface.
// If inIllumination is >= 0, it'd better be <= 1.  Shading tables are then used instead of the collection's CLUT.
// Among other effects (like being able to get darkened shapes), this lets player shapes be colorized according to
// team or player color.
// OK, yet another change... we now (optionally) take shape and collection separately, since there are too many
// low-level shapes in some collections to fit in the number of bits allotted.  If collection != NONE, it's taken
// as a collection and CLUT reference together; shape is (then) taken directly as a low-level shape index.
// If collection == NONE, shape is expected to convey information about all three elements (CLUT, collection,
// low-level shape index).
// Sigh, the extensions keep piling up... now we can also provide a quarter-sized surface from a shape.  It's hacky -
// the shape is shrunk by nearest-neighbor-style scaling (no smoothing), even at 16-bit and above, and it only works for RLE shapes.
SDL_Surface *get_shape_surface(int shape, int collection = NONE, byte** outPointerToPixelData = NULL, float inIllumination = -1.0f, bool inShrinkImage = false);

void open_shapes_file(FileSpecifier& File);

/* ---------- prototypes/SCREEN_DRAWING.C */

void _get_player_color(size_t color_index, RGBColor *color);
void _get_interface_color(size_t color_index, RGBColor *color);
void _get_player_color(size_t color_index, SDL_Color *color);
void _get_interface_color(size_t color_index, SDL_Color *color);


/* ---------- protoypes/INTERFACE_MACINTOSH.C */
void update_game_window(void);

/* ---------- prototypes/PREFERENCES.C */
void load_environment_from_preferences(void);

// LP: displays a text message on the screen in "printf" fashion
// Implemented in the "screen" routines
void screen_printf(const char *format, ...);

void main_event_loop(void);
void initialize_application(void);
void shutdown_application(void);
bool handle_open_document(const std::string& filename);

#endif
