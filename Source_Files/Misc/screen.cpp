/*
SCREEN.C
Saturday, August 21, 1993 11:55:26 AM

Saturday, December 18, 1993 2:17:05 PM
	removed visible_color_table and cinematic_fade_test, halfway through implementing the new
	startup CLUT munger.
Friday, August 12, 1994 6:59:29 PM
	more changes for 16-bit.
Friday, September 9, 1994 12:13:04 PM (alain)
	modified to get window from a resource. set fore and back color before using copybits.

Jan 30, 2000 (Loren Petrich):
	Did some typecasts
	Suppressed "process_screen_click", because it conflicts with routine of same name
		in shell.c
	Surrounded quadruple_screen with "extern "C"

Feb 1, 2000 (Loren Petrich):
	Added drop shadow to fps display in update_fps_display()
	Changed font size there to 14 (more readable)

Feb. 4, 2000 (Loren Petrich):
	Changed halt() to assert(false) for better debugging

Feb. 5, 2000 (Loren Petrich):
	Suppressed WHITE_SCREEN_BETWEEN_FRAMES so as to get that smeared look in non-rendered places

Feb 13, 2000 (Loren Petrich):
	Added screendump capability: dump_screen()

Feb 18, 2000 (Loren Petrich):
	Added chase-cam support

Feb 21, 2000 (Loren Petrich):
	Turned fps drop shadow into a black halo
	Added crosshair support

Feb 24, 2000 (Loren Petrich):
	Made crosshairs invisible when using the terminal or the overhead map

Feb 25, 2000 (Loren Petrich):
	Added tunnel-vision-mode support

Feb 26, 2000 (Loren Petrich):
	Shrunk [I forget what I had wanted to write]

Mar 5, 2000 (Loren Petrich):
	Added reset_screen() function,
	for the purpose of resetting its state when starting a game

Mar 12, 2000 (Loren Petrich):
	Added OpenGL stuff

Mar 22, 2000 (Loren Petrich):
	Improved field-of-view management so that the FOV would be set properly
	when one enters a level.

Apr 19, 2000 (Loren Petrich):
	Suppressed Valkyrie code
	
Apr 30, 2000 (Loren Petrich):
	Added reloading of view context when reverting, so that OpenGL won't look funny when one
	changes a level.

May 2, 2000 (Loren Petrich):
	Added piping of overhead map and terminal graphics through OpenGL (optional)

May 21, 2000 (Loren Petrich):
	Disabled sound stopping for the terminal and overhead map

May 22, 2000 (Loren Petrich):
	Added suppression of overhead map if selected with <view map="false">...</view>
	
	Added setting of target field of view; also making initialize_world_view always called

May 28, 2000 (Loren Petrich):
	Added support for rendering the HUD

Jun 15, 2000 (Loren Petrich):
	Added support for Chris Pruett's Pfhortran

Jul 2, 2000 (Loren Petrich):
	The HUD is now always buffered, and the terminal is always at 100%

Jul 3, 2000 (Loren Petrich):
	OpenGL gets switched off if one tries to start in 256-color mode.

Jul 5, 2000 (Loren Petrich):
	Partially completed the task of generalizing the view-control setup so that it can be generalized
	to higher resolutions. This involved putting all the buffer-size and viewport calculation into
	render_screen(), and of getting as much resizing as possible into render_screen().
	The unavoidable exceptions are resized to 640*480, because Bungie's original resizers
	are inadequate for the task ahead.

Jul 6, 2000 (Loren Petrich):
	Got HUD's position generalized
	Fixed dump_screen so that it works properly in higher resolutions
	Added bigger screen-size values and increased the maximum size accordingly

Jul 8, 2000 (Loren Petrich):
	Modified render_screen() so that the two kinds of OpenGL rendering, for the main view
	and for the map view, are distinct, and that OpenGL for map view would be handled properly
	(text displayed in it, etc.)

Jul 17, 2000 (Loren Petrich):
	Added a view-effect reset to enter_screen() because initialize_view_data() no longer has it,
	in order to make teleport view-stretching work correctly.

Aug 3, 2000 (Loren Petrich):
	Removed all OGL_Map routines, because their contents are now handled in OverheadMapRenderer.c
	and subclass routines

Aug 9, 2000 (Loren Petrich):
	Removed OGL_StartMain() and OGL_EndMain() as redundant

Sept 9, 2000 (Loren Petrich):
	Added a kludge to make OpenGL mode well-behaved in 32-bit (millions of colors) screen depth
	-- reset the screen depth to 16-bit and then back to 32-bit.

Nov 29, 2000 (Loren Petrich):
	Added making view-folding effect optional

Dec 2, 2000 (Loren Petrich):
	Added support for hiding and re-showing the app

Dec 23, 2000
	Had a lot of trouble trying to get DrawSprocket to work properly.
	Also, moved stuff shared with screen_sdl.cpp into screen_shared.cpp
	
Dec 29, 2000 (Loren Petrich):
	Added stuff for doing screen messages
*/

/*
//we expect Entries to leave the device color table untouched, and it doesn�t
*/

#include "macintosh_cseries.h"
#include "my32bqd.h"
// #include "valkyrie.h"

#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "world.h"
#include "map.h"
#include "render.h"
#include "shell.h"
#include "interface.h"
#include "player.h"
#include "overhead_map.h"
#include "fades.h"
#include "game_window.h"
#include "screen.h"
#include "preferences.h"
#include "computer_interface.h"

// LP addition: crosshairs support
#include "Crosshairs.h"
// LP addition: OpenGL support
// OGL_Map.h removed as unnecessary
#include "OGL_Render.h"

// LP addition: view control
#include "ViewControl.h"
#include <DrawSprocket.h>

//CP addition: scripting support
#include "scripting.h"

#ifdef env68k
#pragma segment screen
#endif

#ifdef DEBUG
//#define CYBERMAXX
// #define WHITE_SCREEN_BETWEEN_FRAMES
//#define DIRECT_SCREEN_TEST
#endif

#ifdef env68k
#define EXTERNAL
#endif

#ifdef envppc
#define EXTERNAL
#endif

/* ---------- constants */

#ifdef DIRECT_SCREEN_TEST
#define WORLD_H 160
#define WORLD_V 80
#else
#define WORLD_H 0
#define WORLD_V 0
#endif

#include "screen_shared.cpp"

#define DESIRED_SCREEN_WIDTH 640
#define DESIRED_SCREEN_HEIGHT 480

/* standard screen width is twice height */
// LP change: to what's in the definitions below
#define MAXIMUM_WORLD_WIDTH 1024
#define MAXIMUM_WORLD_HEIGHT 768
// #define MAXIMUM_WORLD_WIDTH 640
// #define MAXIMUM_WORLD_HEIGHT 480

#define DEFAULT_WORLD_WIDTH 640
#define DEFAULT_WORLD_HEIGHT 320

#define windGAME_WINDOW 128
#define windBACKDROP_WINDOW 129

#ifdef envppc
#define EXTRA_MEMORY (100*KILO)
#else
#define EXTRA_MEMORY 0
#endif

#define FREE_MEMORY_FOR_16BIT (4*MEG+EXTRA_MEMORY)
#define FREE_MEMORY_FOR_32BIT (9*MEG+EXTRA_MEMORY)
#define FREE_MEMORY_FOR_FULL_SCREEN (3*MEG+EXTRA_MEMORY)

#define clutMARATHON8_CLUT_ID 5002

typedef ReqListRec *ReqListPtr;

/* ---------- structures */

#define _EVERY_OTHER_LINE_BIT 0x8000
#define _SIXTEEN_BIT_BIT 0x4000
#define _THIRTYTWO_BIT_BIT 0x2000

struct copy_screen_data
{
	pixel8 *source, *destination;
	short width, height, bytes_per_row;
	short source_slop, destination_slop;
	short flags;
};

/* ---------- globals */

GDHandle world_device; /* the device we�re running on */
WindowPtr screen_window; /* a window covering our entire device */
WindowPtr backdrop_window; /* a window covering all devices */

// LP note: world_pixels is the rendering buffer
// for the main view, the overhead map, and the terminals.
// The HUD has a separate buffer.
// It is initialized to NULL so as to allow its initing to be lazy.
GWorldPtr world_pixels = NULL;

static bool enough_memory_for_16bit, enough_memory_for_32bit, enough_memory_for_full_screen;

//static bool restore_depth_on_exit; /* otherwise restore CLUT */
//static short restore_depth, restore_flags; /* for SetDepth on exit */
static GDSpec restore_spec, resolution_restore_spec;

// LP addition: a GWorld for the Heads-Up Display
// (if not allocated, then it's the null pointer).
// Also the currently-requested rendering area when doing in piped-through-OpenGL mode
GWorldPtr HUD_Buffer = NULL;
static Rect HUD_SourceRect = {0, 0, 160, 640};
static Rect HUD_DestRect;

// Flag for making 32-bit-OGL screen resetting done only once.
static bool ScreenFix_OGL32 = false;

// DrawSprocket activity:
static bool DSp_Present = false;
static bool DSp_Inited = false;

// Current DrawSprocket context (NULL means none active)
static DSpContextReference DSp_Ctxt = NULL;


/* ---------- private prototypes */

// LP change: the source and destination rects will be very variable, in general.
// Also indicating whether to use high or low resolution (the terminal and the ovhd map are always hi-rez)
static void update_screen(Rect& source, Rect& destination, bool hi_rez);
// static void update_screen(void);

void calculate_destination_frame(short size, bool high_resolution, Rect *frame);
static void calculate_source_frame(short size, bool high_resolution, Rect *frame);
static void calculate_adjusted_source_frame(struct screen_mode_data *mode, Rect *frame);

static GDHandle find_and_initialize_world_device(long area, short depth);
static void restore_world_device(void);
static bool parse_device(GDHandle device, short depth, bool *color, bool *needs_to_change);

extern "C" {
void quadruple_screen(struct copy_screen_data *data);
}

static void update_fps_display(GrafPtr port);

// LP addition: display the current position
static void DisplayPosition(GrafPtr port);

// Also display the current messages
static void DisplayMessages(GrafPtr port);

static void calculate_screen_options(void);

static void ClearScreen();

// Draws HUD in software mode
void DrawHUD(Rect& SourceRect, Rect& DestRect);

// DrawSprocket handlers:

// Checks on the DrawSprocket's presence and returns whether it is active;
// it inits DSp if necessary
static bool DSp_Check();

// For debugging DSp errors
static bool DSp_HandleError(char *Description, OSStatus ErrorCode);

// Changes the monitor's resolution;
// returns the success of doing so
static bool DSp_ChangeResolution(GDHandle Device, short BitDepth, short Width, short Height);


/* ---------- code */
void initialize_screen(
	struct screen_mode_data *mode)
{
	OSErr error;
	Rect bounds;
	GrafPtr old_port;
	RgnHandle gray_region= GetGrayRgn();

	if (!screen_initialized)
	{
		/* Calculate the screen options-> 16, 32, full? */
		calculate_screen_options();
#ifdef OBSOLETE
		enough_memory_for_16bit= (FreeMem()>FREE_MEMORY_FOR_16BIT) ? true : false;
		enough_memory_for_32bit= (FreeMem()>FREE_MEMORY_FOR_32BIT) ? true : false;
		enough_memory_for_full_screen= (FreeMem()>FREE_MEMORY_FOR_FULL_SCREEN) ? true : false;
#endif
	}
	
	if (mode->bit_depth==32 && !enough_memory_for_32bit) mode->bit_depth= 16;
	if (mode->bit_depth==16 && !enough_memory_for_16bit) mode->bit_depth= 8;
	interface_bit_depth= bit_depth= mode->bit_depth;
	switch (mode->acceleration)
	{
		case _valkyrie_acceleration:
			mode->bit_depth= bit_depth= 16;
			mode->high_resolution= false;
			interface_bit_depth= 8;
			break;
	}

	/* beg, borrow or steal an n-bit device */
	graphics_preferences->device_spec.bit_depth= interface_bit_depth;
	world_device= BestDevice(&graphics_preferences->device_spec);
	if (!world_device&&interface_bit_depth!=8)
	{
		graphics_preferences->device_spec.bit_depth= 8;
		world_device= BestDevice(&graphics_preferences->device_spec);
		if (world_device) mode->bit_depth= bit_depth= interface_bit_depth= 8;
	}
	if (!world_device) alert_user(fatalError, strERRORS, badMonitor, -1);

#ifdef OBSOLETE
	/* beg, borrow or steal an n-bit device */
	world_device= find_and_initialize_world_device(DESIRED_SCREEN_WIDTH*DESIRED_SCREEN_HEIGHT, interface_bit_depth);
	if (!world_device&&interface_bit_depth!=8)
	{
		world_device= find_and_initialize_world_device(DESIRED_SCREEN_WIDTH*DESIRED_SCREEN_HEIGHT, 8);
		if (world_device) mode->bit_depth= bit_depth= interface_bit_depth= 8;
	}
	if (!world_device) alert_user(fatalError, strERRORS, badMonitor, -1);
#endif
	
	// When this is working, uncomment it and also change appropriately
	// do_preferences() in interface_macintosh.cpp
	/*
	// Use the DrawSprocket to change the display resolution
	if (graphics_preferences->screen_mode.fullscreen)
	{
		short msize = screen_mode.size;
		assert(msize >= 0 && msize < NUMBER_OF_VIEW_SIZES);
		const ViewSizeData& VS = ViewSizes[msize];
		DSp_ChangeResolution(world_device, graphics_preferences->device_spec.bit_depth,
			VS.OverallWidth, VS.OverallHeight);
	}
	else if (DSp_Ctxt)
	{
		DSp_HandleError("Init: Release",DSpContext_Release(DSp_Ctxt));
		DSp_Ctxt = NULL;
	}
	*/
	
	if (!screen_initialized)
	{
		graphics_preferences->device_spec.width= DESIRED_SCREEN_WIDTH;
		graphics_preferences->device_spec.height= DESIRED_SCREEN_HEIGHT;
#if 0
		BuildGDSpec(&resolution_restore_spec, world_device);
		SetResolutionGDSpec(&graphics_preferences->device_spec);
#endif
		
		/* get rid of the menu bar */
		myHideMenuBar(GetMainDevice());
		
		uncorrected_color_table= new color_table;
		world_color_table= new color_table;
		visible_color_table= new color_table;
		interface_color_table= new color_table;
		assert(uncorrected_color_table && world_color_table && visible_color_table && interface_color_table);

		backdrop_window= (WindowPtr) NewPtr(sizeof(CWindowRecord));
		assert(backdrop_window);
		backdrop_window= GetNewCWindow(windBACKDROP_WINDOW, backdrop_window, (WindowPtr) -1);
		assert(backdrop_window);
		MoveWindow(backdrop_window, (**gray_region).rgnBBox.left, (**gray_region).rgnBBox.top, false);
		SizeWindow(backdrop_window, RECTANGLE_WIDTH(&(**gray_region).rgnBBox), RECTANGLE_HEIGHT(&(**gray_region).rgnBBox), true);
		ShowWindow(backdrop_window);

		screen_window= (WindowPtr) NewPtr(sizeof(CWindowRecord));
		assert(screen_window);
		screen_window= GetNewCWindow(windGAME_WINDOW, screen_window, (WindowPtr) -1);
		assert(screen_window);
		SetWRefCon(screen_window, refSCREEN_WINDOW);
		ShowWindow(screen_window);

		/* allocate the bitmap_definition structure for our GWorld (it is reinitialized every frame */
		world_pixels_structure= (struct bitmap_definition *) NewPtr(sizeof(struct bitmap_definition)+sizeof(pixel8 *)*MAXIMUM_WORLD_HEIGHT);
		assert(world_pixels_structure);
		
		/* allocate and initialize our view_data structure; we don�t call initialize_view_data
			anymore (change_screen_mode does that) */
		world_view= (struct view_data *) NewPtr(sizeof(struct view_data));
		assert(world_view);
		world_view->field_of_view= NORMAL_FIELD_OF_VIEW; /* degrees (was 74 for a long, long time) */
		world_view->target_field_of_view= NORMAL_FIELD_OF_VIEW; // LP addition: for no change in FOV
		world_view->overhead_map_scale= DEFAULT_OVERHEAD_MAP_SCALE;
		world_view->overhead_map_active= false;
		world_view->terminal_mode_active= false;
		world_view->horizontal_scale= 1, world_view->vertical_scale= 1;
		// LP addition:
		world_view->tunnel_vision_active = false;
		
		/* make sure everything gets cleaned up after we leave */
		atexit(restore_world_device);
	
		world_pixels= (GWorldPtr) NULL;
	}
	else
	{
		unload_all_collections();
	}
	
	if (!screen_initialized || MatchGDSpec(&restore_spec)!=MatchGDSpec(&graphics_preferences->device_spec))
	{
		if (screen_initialized)
		{
			GetPort(&old_port);
			SetPort(screen_window);
			PaintRect(&screen_window->portRect);
			SetPort(old_port);
			
			SetDepthGDSpec(&restore_spec);
		}
		BuildGDSpec(&restore_spec, world_device);
	}
	SetDepthGDSpec(&graphics_preferences->device_spec);

	MoveWindow(screen_window, (*world_device)->gdRect.left, (*world_device)->gdRect.top, false);
	SizeWindow(screen_window, RECTANGLE_WIDTH(&(*world_device)->gdRect), RECTANGLE_HEIGHT(&(*world_device)->gdRect), true);

	{
		Point origin;
		
		origin.h= - (RECTANGLE_WIDTH(&(*world_device)->gdRect)-DESIRED_SCREEN_WIDTH)/2;
		origin.v= - (RECTANGLE_HEIGHT(&(*world_device)->gdRect)-DESIRED_SCREEN_HEIGHT)/2;
		if (origin.v>0) origin.v= 0;
		
		GetPort(&old_port);
		SetPort(screen_window);
		SetOrigin(origin.h, origin.v);
		SetPort(old_port);
	}
	
	/* allocate and initialize our GWorld */
	// LP change: doing a 640*480 allocation as a sensible starting point
	calculate_destination_frame(_full_screen, true, &bounds);
	error= world_pixels ? myUpdateGWorld(&world_pixels, 0, &bounds, (CTabHandle) NULL, (GDHandle) NULL, 0) :
		myNewGWorld(&world_pixels, 0, &bounds, (CTabHandle) NULL, (GDHandle) NULL, 0);
	if (error!=noErr) alert_user(fatalError, strERRORS, outOfMemory, error);
	/*
	calculate_destination_frame(mode->size, mode->high_resolution, &bounds);
	error= screen_initialized ? myUpdateGWorld(&world_pixels, 0, &bounds, (CTabHandle) NULL, (GDHandle) NULL, 0) :
		myNewGWorld(&world_pixels, 0, &bounds, (CTabHandle) NULL, (GDHandle) NULL, 0);
	if (error!=noErr) alert_user(fatalError, strERRORS, outOfMemory, error);
	*/
	
	change_screen_mode(mode, false);
	
	screen_initialized= true;
	return;
}


// LP change: force reload of view context
void ReloadViewContext()
{
	// Cribbed from enter_screen
	switch (screen_mode.acceleration)
	{
		// LP addition: doing OpenGL if present;
		// otherwise, switching OpenGL off (kludge for having it not appear)
		case _opengl_acceleration:
			if (screen_mode.bit_depth > 8)
				OGL_StartRun((CGrafPtr)screen_window);
			else screen_mode.acceleration = _no_acceleration;
			break;
		
		/*
		case _valkyrie_acceleration:
			valkyrie_begin();
			break;
		*/
	}
}


void enter_screen(
	void)
{
	GrafPtr old_port;
	
	GetPort(&old_port);
	SetPort(screen_window);
	PaintRect(&screen_window->portRect);
	SetPort(old_port);

	if (world_view->overhead_map_active) set_overhead_map_status(false);
	if (world_view->terminal_mode_active) set_terminal_status(false);
	
	// LP change: adding this view-effect resetting here
	// since initialize_world_view() no longer resets it.
	world_view->effect = NONE;
	
	// Kludge to stop annoying 32-bit-mode flickering -- set to 16-bit, then to 32-bit again
	if ((screen_mode.acceleration == _opengl_acceleration) && screen_mode.bit_depth == 32)
	{
		// Be sure to do this only once...
		if (!ScreenFix_OGL32)
		{
			GDSpec& DevSpec = graphics_preferences->device_spec;
			DevSpec.bit_depth = 16;
			SetDepthGDSpec(&DevSpec);
			DevSpec.bit_depth = 32;
			SetDepthGDSpec(&DevSpec);
			ScreenFix_OGL32 = true;
		}
	}

	// Of course, do this...
	change_screen_mode(&screen_mode, true);
	
	switch (screen_mode.acceleration)
	{
		// LP addition: doing OpenGL if present
		// otherwise, switching OpenGL off (kludge for having it not appear)
		case _opengl_acceleration:
			if (screen_mode.bit_depth > 8)
				OGL_StartRun((CGrafPtr)screen_window);
			else screen_mode.acceleration = _no_acceleration;
			break;
		
		/*
		case _valkyrie_acceleration:
			valkyrie_begin();
			break;
		*/
	}

	return;
}

void exit_screen(
	void)
{

	// LP addition: doing OpenGL if present
	OGL_StopRun();
	
	// Deallocate the drawing buffer
	/*
	if (world_pixels)
	{
		myDisposeGWorld(world_pixels);
		world_pixels = NULL;
	}
	*/
	
	/*
	switch (screen_mode.acceleration)
	{
		case _valkyrie_acceleration:
			change_screen_mode(&screen_mode, false);
			valkyrie_end();
			break;
	}
	*/
	
	return;
}


// LP addition: added stuff to keep the sounds going if desired
/*
static bool stop_sounds = true;

void change_screen_mode_keep_sounds(
	struct screen_mode_data *mode,
	bool redraw)
{
	stop_sounds = false;
	change_screen_mode(mode,redraw);
	stop_sounds = true;
}
*/

void change_screen_mode(
	struct screen_mode_data *mode,
	bool redraw)
{
	// LP: stripped it down to a minimum
	
	// Get the screen mode here
	screen_mode = *mode;
	
	// "Redraw" means clear the screen
	if (redraw) ClearScreen();
	
// LP change: beginning of obsolete part
#if 0
	short width, height;
	Rect bounds;
	GrafPtr old_port;
	OSErr error;

//	if (mode->high_resolution && mode->size==_full_screen && !enough_memory_for_full_screen)
//	{
//		mode->high_resolution= false;
//	}
	/*
	switch (mode->acceleration)
	{
		case _valkyrie_acceleration:
			if (!world_view->overhead_map_active && !world_view->terminal_mode_active)
			{
				mode->high_resolution= false;
			}
			break;
	}
	*/
	/* This makes sure that the terminal and map use their own resolutions */
	world_view->overhead_map_active= false;
	world_view->terminal_mode_active= false;

	if (redraw)
	{
		GetPort(&old_port);
		SetPort(screen_window);
		calculate_destination_frame(mode->size==_full_screen ? _full_screen : _100_percent, true, &bounds);
		PaintRect(&bounds);
		SetPort(old_port);
	}
	
	/*
	switch (mode->acceleration)
	{
		case _valkyrie_acceleration:
			calculate_destination_frame(mode->size, mode->high_resolution, &bounds);
			OffsetRect(&bounds, -screen_window->portRect.left, -screen_window->portRect.top);
			valkyrie_initialize(world_device, true, &bounds, 0xfe);
			if (redraw) valkyrie_erase_graphic_key_frame(0xfe);
			break;
	}
	*/
	
	/* save parameters */
	screen_mode= *mode;
	
	// LP addition: whether one wants to stop sounds... actually, never
	/*
	if (stop_sounds)
		free_and_unlock_memory(); *//* do our best to give UpdateGWorld memory */
	
	/* adjust the size of our GWorld based on mode->size and mode->resolution */
	calculate_adjusted_source_frame(mode, &bounds);
	error= myUpdateGWorld(&world_pixels, 0, &bounds, (CTabHandle) NULL, (GDHandle) NULL, 0);
	if (error!=noErr) alert_user(fatalError, strERRORS, outOfMemory, error);
	
	calculate_source_frame(mode->size, mode->high_resolution, &bounds);
	width= RECTANGLE_WIDTH(&bounds), height= RECTANGLE_HEIGHT(&bounds);

	/* adjust and initialize the world_view structure */
#ifdef CYBERMAXX
	world_view->screen_width= width;
	world_view->screen_height= height/2;
	world_view->standard_screen_width= 2*height;
#else
	world_view->screen_width= width;
	world_view->screen_height= height;
	world_view->standard_screen_width= 2*height;
#endif
	initialize_view_data(world_view);
// LP change: end of obsolete part
#endif
	
	frame_count= frame_index= 0;

	return;
}

void render_screen(
	short ticks_elapsed)
{
	PixMapHandle pixels;

	/* make whatever changes are necessary to the world_view structure based on whichever player
		is frontmost */
	world_view->ticks_elapsed= ticks_elapsed;
	world_view->tick_count= dynamic_world->tick_count;
	world_view->yaw= current_player->facing;
	world_view->pitch= current_player->elevation;
	world_view->maximum_depth_intensity= current_player->weapon_intensity;
	world_view->shading_mode= current_player->infravision_duration ? _shading_infravision : _shading_normal;

	// LP change: always do this, no matter what
	// Moved downward after resizing
	// initialize_view_data(world_view);
	/*
	if (current_player->extravision_duration)
	{
		// LP idiot-proofing
		if (world_view->field_of_view>=EXTRAVISION_FIELD_OF_VIEW && world_view->effect!=_render_effect_going_fisheye)
		{
			world_view->field_of_view= EXTRAVISION_FIELD_OF_VIEW;
			initialize_view_data(world_view);
		}
	}
	else
	{
		// LP idiot-proofing and tunnel-vision support 
		if (world_view->field_of_view<=NORMAL_FIELD_OF_VIEW && world_view->effect!=_render_effect_leaving_fisheye
			&& world_view->effect!=_render_effect_going_tunnel && world_view->effect!=_render_effect_leaving_tunnel)
		{
			// Tunnel-vision support
			world_view->field_of_view= world_view->tunnel_vision_active ? TUNNEL_VISION_FIELD_OF_VIEW : NORMAL_FIELD_OF_VIEW;
			// world_view->field_of_view= NORMAL_FIELD_OF_VIEW;
			initialize_view_data(world_view);
		}
	}
	*/
	
	// LP change: suppress the overhead map if desired
	if (PLAYER_HAS_MAP_OPEN(current_player) && View_MapActive())
	{
		if (!world_view->overhead_map_active) set_overhead_map_status(true);
	}
	else
	{
		if (world_view->overhead_map_active) set_overhead_map_status(false);
	}

	if(player_in_terminal_mode(current_player_index))
	{
		if (!world_view->terminal_mode_active) set_terminal_status(true);
	} else {
		if (world_view->terminal_mode_active) set_terminal_status(false);
	}

	// LP change: set rendering-window bounds for the current sort of display to render
	// A Rect is, in order, top, left, bottom, right
	screen_mode_data *mode = &screen_mode;
	Rect ScreenRect = screen_window->portRect;
	Rect BufferRect, ViewRect;
	bool HighResolution;
	
	short msize = mode->size;
	assert(msize >= 0 && msize < NUMBER_OF_VIEW_SIZES);
	const ViewSizeData& VS = ViewSizes[msize];
	
	// Rectangle where the view is to go (must not overlap the HUD)
	short OverallWidth = VS.OverallWidth;
	short OverallHeight = VS.OverallHeight - (VS.ShowHUD ? 160 : 0);
	short BufferWidth, BufferHeight;
	
	// Offsets for placement in the screen
	short ScreenOffsetWidth = ((RECTANGLE_WIDTH(&ScreenRect) - VS.OverallWidth) >> 1) + ScreenRect.left;
	short ScreenOffsetHeight = ((RECTANGLE_HEIGHT(&ScreenRect) - VS.OverallHeight) >> 1) + ScreenRect.top;
	
	// HUD location
	Rect HUD_DestRect;
	short HUD_Offset = (OverallWidth - 640) >> 1;
	SetRect(&HUD_DestRect, HUD_Offset, OverallHeight, HUD_Offset + 640, VS.OverallHeight);
	OffsetRect(&HUD_DestRect, ScreenOffsetWidth, ScreenOffsetHeight);
	
	bool ChangedSize = false;
	
	// Each kind of display needs its own size
	if (world_view->terminal_mode_active)
	{
		// Standard terminal size
		BufferWidth = 640;
		BufferHeight = 320;
		HighResolution = true;		
	}
	else if (world_view->overhead_map_active)
	{
		// Fill the available space
		BufferWidth = OverallWidth;
		BufferHeight = OverallHeight;
		HighResolution = true;		
	}
	else
	{
		BufferWidth = VS.MainWidth;
		BufferHeight = VS.MainHeight;
		HighResolution = mode->high_resolution;
	}
	
	if (BufferWidth != PrevBufferWidth)
	{
		ChangedSize = true;
		PrevBufferWidth = BufferWidth;
	}
	if (BufferHeight != PrevBufferHeight)
	{
		ChangedSize = true;
		PrevBufferHeight = BufferHeight;
	}
	
	// Do the buffer/viewport rectangle setup:
	
	// First, the destination rectangle (viewport to be drawn in)
	SetRect(&ViewRect,0,0,BufferWidth,BufferHeight);
	short OffsetWidth = (OverallWidth - BufferWidth) >> 1;
	short OffsetHeight = (OverallHeight - BufferHeight) >> 1;
	OffsetRect(&ViewRect, OffsetWidth+ScreenOffsetWidth, OffsetHeight+ScreenOffsetHeight);
	
	if (OffsetWidth != PrevOffsetWidth)
	{
		ChangedSize = true;
		PrevOffsetWidth = OffsetWidth;
	}
	if (OffsetHeight != PrevOffsetHeight)
	{
		ChangedSize = true;
		PrevOffsetHeight = OffsetHeight;
	}
	
	// Now the buffer rectangle; be sure to shrink it as appropriate
	if (!HighResolution)
	{
		BufferWidth >>= 1;
		BufferHeight >>= 1;
	}
	SetRect(&BufferRect,0,0,BufferWidth,BufferHeight);
	
	// Set up view data appropriately (cribbed from change_screen_mode)
	world_view->screen_width= BufferWidth;
	world_view->screen_height= BufferHeight;
	world_view->standard_screen_width= 2*BufferHeight;	
	initialize_view_data(world_view);
	
	if (world_pixels)
	{
		// Check on the drawing buffer's size
		if (RECTANGLE_WIDTH(&world_pixels->portRect) != BufferWidth) ChangedSize = true;
		if (RECTANGLE_HEIGHT(&world_pixels->portRect) != BufferHeight) ChangedSize = true;
	}
	else ChangedSize = true;
	
	if (ChangedSize)
	{
		ClearScreen();
		if (VS.ShowHUD) draw_interface();
		
		// Reallocate the drawing buffer
		short error = world_pixels ?
			myUpdateGWorld(&world_pixels, 0, &BufferRect, (CTabHandle) NULL, (GDHandle) NULL, 0) :
				myNewGWorld(&world_pixels, 0, &BufferRect, (CTabHandle) NULL, (GDHandle) NULL, 0);
		if (error!=noErr) alert_user(fatalError, strERRORS, outOfMemory, error);
	}
	
	// Be sure that the main view is buffered...
	OGL_SetWindow(ScreenRect,ViewRect, true);
	
	// LP: Resizing must come *before* locking the GWorld's pixels
	myLockPixels(world_pixels);
	pixels= myGetGWorldPixMap(world_pixels);
	
	switch (screen_mode.acceleration)
	{
		/*
		case _valkyrie_acceleration:
			if (!world_view->overhead_map_active && !world_view->terminal_mode_active)
			{
				valkyrie_initialize_invisible_video_buffer(world_pixels_structure);
				break;
			}
		*/
			/* if we�re using the overhead map, fall through to no acceleration */
		// LP change: might want to init OpenGL rendering here
		case _opengl_acceleration:
			if (!world_view->overhead_map_active && !world_view->terminal_mode_active)
			{
				break;
			}
		case _no_acceleration:
			world_pixels_structure->width= world_view->screen_width;
			world_pixels_structure->height= world_view->screen_height;
			world_pixels_structure->flags= 0;
/* #ifdef DIRECT_SCREEN_TEST
			pixels= (*world_device)->gdPMap; //((CGrafPtr)screen_window)->portPixMap;
			world_pixels_structure->bytes_per_row= (*pixels)->rowBytes&0x3fff;
			world_pixels_structure->row_addresses[0]= ((pixel8 *)(*pixels)->baseAddr) + WORLD_H +
				WORLD_V*world_pixels_structure->bytes_per_row;
			*//* blanking the screen is not supported in direct-to-screen mode */
// #else /* !DIRECT_SCREEN_TEST */
			world_pixels_structure->bytes_per_row= (*pixels)->rowBytes&0x3fff;
			world_pixels_structure->row_addresses[0]= (pixel8 *) myGetPixBaseAddr(world_pixels);
// #ifdef WHITE_SCREEN_BETWEEN_FRAMES
// LP change: will use OpenGL option for this
			if (TEST_FLAG(Get_OGL_ConfigureData().Flags,OGL_Flag_VoidColor))
			{
				/* The terminal stuff only draws when necessary */
				if(!world_view->terminal_mode_active)
				{
					GDHandle old_device;
					CGrafPtr old_port;
					
					myGetGWorld(&old_port, &old_device);
					mySetGWorld(world_pixels, (GDHandle) NULL);
					RGBColor SavedColor;
					GetBackColor(&SavedColor);
					RGBBackColor(&Get_OGL_ConfigureData().VoidColor);
					EraseRect(&world_pixels->portRect);
					RGBBackColor(&SavedColor);
					mySetGWorld(old_port, old_device);
				}
			}
// #endif /* WHITE_SCREEN_BETWEEN_FRAMES */
// #endif /* else DIRECT_SCREEN_TEST */
			precalculate_bitmap_row_addresses(world_pixels_structure);
			break;
		
		default:
			// LP change:
			assert(false);
			// halt();
	}

	// assert(world_view->screen_height<=MAXIMUM_WORLD_HEIGHT);

// LP change: Cybermaxx support -- this indicates how to do binocular vision
	/* render left and right images on alternating scan lines */
/*
	{
		world_point3d left_origin, right_origin;
		short left_polygon_index, right_polygon_index;
		angle left_angle, right_angle;

		get_binocular_vision_origins(current_player_index, &left_origin, &left_polygon_index, &left_angle,
			&right_origin, &right_polygon_index, &right_angle);
		
		world_pixels_structure->bytes_per_row*= 2;
		precalculate_bitmap_row_addresses(world_pixels_structure);
		world_view->origin= left_origin;
		world_view->origin_polygon_index= left_polygon_index;
		world_view->yaw= left_angle;
		render_view(world_view, world_pixels_structure);
		
		world_pixels_structure->row_addresses[0]+= world_pixels_structure->bytes_per_row/2;
		precalculate_bitmap_row_addresses(world_pixels_structure);
		world_view->origin= right_origin;
		world_view->origin_polygon_index= right_polygon_index;
		world_view->yaw= right_angle;
		render_view(world_view, world_pixels_structure);
	}
*/
	world_view->origin= current_player->camera_location;
	world_view->origin_polygon_index= current_player->camera_polygon_index;
	// LP addition: chase-cam support
	//CP Change: add script-based camera control
	if (!script_Camera_Active())
	{
		// LP addition: chase-cam support
		world_view->show_weapons_in_hand =
			!ChaseCam_GetPosition(world_view->origin,world_view->origin_polygon_index,world_view->yaw,world_view->pitch);
	}
	
	// Is map to be drawn with OpenGL?
	if (OGL_IsActive() && world_view->overhead_map_active)
		OGL_MapActive = (TEST_FLAG(Get_OGL_ConfigureData().Flags,OGL_Flag_Map) != 0);
	else
		OGL_MapActive = false;
	
	render_view(world_view, world_pixels_structure);
	
	switch (screen_mode.acceleration)
	{
		/*
		case _valkyrie_acceleration:
			if (!world_view->overhead_map_active && !world_view->terminal_mode_active)
			{
				valkyrie_switch_to_invisible_video_buffer();
				break;
			}
			// if we�re using the overhead map, fall through to no acceleration
		*/
			// LP change: OpenGL acceleration included with no acceleration here
		case _opengl_acceleration:
		case _no_acceleration:
			update_fps_display((GrafPtr)world_pixels);
			// LP additions: display position and messages and show crosshairs
			if (!world_view->terminal_mode_active)
				DisplayPosition((GrafPtr)world_pixels);
			DisplayMessages((GrafPtr)world_pixels);
			
			// Don't show the crosshairs when either the overhead map or the terminal is active
			if (!world_view->overhead_map_active && !world_view->terminal_mode_active)
				if (Crosshairs_IsActive())
					if (!OGL_RenderCrosshairs())
						Crosshairs_Render((GrafPtr)world_pixels);
// #ifndef DIRECT_SCREEN_TEST
			// LP change: put in OpenGL buffer swapping when the main view or the overhead map
			// are being rendered in OpenGL.
			// Otherwise, if OpenGL is active, then blit the software rendering to the screen.
			bool OGL_WasUsed = false;
			if ((OGL_MapActive || !world_view->overhead_map_active) && !world_view->terminal_mode_active)
			{
				// Main or map view already rendered
				OGL_WasUsed = OGL_SwapBuffers();
			}
			else
			{
				// Copy 2D rendering to screen
				// Paint on top without any buffering if piping through OpenGL had been selected
				OGL_WasUsed = OGL_Copy2D(world_pixels,world_pixels->portRect,world_pixels->portRect,false,false);
			}
			if (!OGL_WasUsed) update_screen(BufferRect,ViewRect,HighResolution);
			if (HUD_RenderRequest)
			{
				if (OGL_Get2D())
				{
					// This horrid-looking resizing does manage to get the HUD to work properly...
					struct DownwardOffsetSet
					{
						short Top, Bottom;
					};
					const DownwardOffsetSet OGL_DownwardOffsets[NUMBER_OF_VIEW_SIZES] =
					{
						{160, 480},		//  _320_160_HUD
						{160, 480},		//  _480_240_HUD
						{160, 480},		//  _640_320_HUD
						{160, 480},		//  _640_480
						{190, 600},		//  _800_400_HUD
						{190, 600},		//  _800_600
						{232, 768},		// _1024_512_HUD
						{232, 768},		// _1024_768
					};
					const DownwardOffsetSet& Set = OGL_DownwardOffsets[msize];
					HUD_DestRect.top = Set.Top;
					HUD_DestRect.bottom = Set.Bottom;
					// Paint on top without any buffering
					OGL_SetWindow(ScreenRect,HUD_DestRect,false);
					OGL_Copy2D(HUD_Buffer,HUD_SourceRect,HUD_DestRect,false,false);
				}
				else
					DrawHUD(HUD_SourceRect,HUD_DestRect);
				HUD_RenderRequest = false;
			}
// #endif
			break;
		
		default:
			// LP change:
			assert(false);
			// halt();
	}

	myUnlockPixels(world_pixels);

	return;
}
/*
void process_screen_click(
	EventRecord *event)
{
	(void) (event);
	return;
}
*/
void change_interface_clut(
	struct color_table *color_table)
{
	obj_copy(*interface_color_table, *color_table);

	return;
}

/* builds color tables suitable for SetEntries (in either bit depth) and makes the screen look
	like we expect it to */
void change_screen_clut(
	struct color_table *color_table)
{
	if (interface_bit_depth==8 && bit_depth==8)
	{
		obj_copy(*uncorrected_color_table, *color_table);
		obj_copy(*interface_color_table, *color_table);
	}
	
	if (bit_depth==16 || bit_depth==32)
	{
		build_direct_color_table(uncorrected_color_table, bit_depth);
		if (interface_bit_depth!=8)
		{
			obj_copy(*interface_color_table, *uncorrected_color_table);
		}
	}

	gamma_correct_color_table(uncorrected_color_table, world_color_table, screen_mode.gamma_level);
	obj_copy(*visible_color_table, *world_color_table);

	/* switch to our device, stuff in our corrected color table */
	assert_world_color_table(interface_color_table, world_color_table);

	/* and finally, make sure the ctSeeds of our GWorld and our GDevice are synchronized */
	{
		Rect bounds;
		OSErr error;
	
		// LP change: doing a 640*480 allocation as a sensible starting point
		calculate_destination_frame(_full_screen, true, &bounds);
		// calculate_adjusted_source_frame(&screen_mode, &bounds);
		error= myUpdateGWorld(&world_pixels, 0, &bounds, (CTabHandle) NULL, (GDHandle) NULL, 0);
		if (error!=noErr) alert_user(fatalError, strERRORS, outOfMemory, error);
	}
	return;
}

void build_direct_color_table(
	struct color_table *color_table,
	short bit_depth)
{
	struct rgb_color *color;
	short maximum_component;
	short i;
	
	switch (bit_depth)
	{
		case 16: maximum_component= PIXEL16_MAXIMUM_COMPONENT; break;
		case 32: maximum_component= PIXEL32_MAXIMUM_COMPONENT; break;
		default:
			// LP change:
			assert(false);
			// halt();
	}
	
	color_table->color_count= maximum_component+1;
	for (i= 0, color= color_table->colors; i<=maximum_component; ++i, ++color)
	{
		color->red= color->green= color->blue= (i*0xffff)/maximum_component;
	}
	
	return;
}

void render_computer_interface(
	struct view_data *view)
{
	GWorldPtr old_gworld;
	GDHandle old_device;
	struct view_terminal_data data;

	GetGWorld(&old_gworld, &old_device);
	SetGWorld(world_pixels, (GDHandle) NULL);
	
	// LP change: calculate the view dimensions directly from the GWorld's boundaries
	Rect& WPRect = ((CGrafPtr)world_pixels)->portRect;
	data.left = WPRect.left;
	data.right = WPRect.right;
	data.top = WPRect.top;
	data.bottom = WPRect.bottom;
	data.vertical_offset = 0;
	
	/*
	data.left= 0; data.right= view->screen_width;
	data.top= 0; data.bottom= view->screen_height;
	
	switch (screen_mode.size)
	{
			case _50_percent:
			case _75_percent:
			// LP change: moved "full screen" up here, because this is supposed to be like 100%
			// in all cases
			case _full_screen:
				// LP change:
				assert(false);
				// halt();
				break;
				
			case _100_percent:
				data.vertical_offset= 0;
				break;
			case _full_screen:
				data.vertical_offset= (view->screen_height-DEFAULT_WORLD_HEIGHT)/2;
				break;
	}
	*/
	
	_render_computer_interface(&data);

	RGBForeColor(&rgb_black);
	PenSize(1, 1);
	TextFont(0);
	TextFace(normal);
	TextSize(0);
	SetGWorld(old_gworld, old_device);
	
	return;
}

void render_overhead_map(
	struct view_data *view)
{
	GWorldPtr old_gworld;
	GDHandle old_device;
	struct overhead_map_data overhead_data;

	GetGWorld(&old_gworld, &old_device);
	SetGWorld(world_pixels, (GDHandle) NULL);
	
	PaintRect(&world_pixels->portRect);
	
	// LP change: calculate the view dimensions directly from the GWorld's boundaries
	Rect& WPRect = ((CGrafPtr)world_pixels)->portRect;
	overhead_data.left = WPRect.left;
	overhead_data.top = WPRect.top;
	overhead_data.half_width = (overhead_data.width = RECTANGLE_WIDTH(&WPRect)) >> 1;
	overhead_data.half_height = (overhead_data.height = RECTANGLE_HEIGHT(&WPRect)) >> 1;

	overhead_data.scale= view->overhead_map_scale;
	overhead_data.mode= _rendering_game_map;
	overhead_data.origin.x= view->origin.x;
	overhead_data.origin.y= view->origin.y;
	/*
	overhead_data.half_width= view->half_screen_width;
	overhead_data.half_height= view->half_screen_height;
	overhead_data.width= view->screen_width;
	overhead_data.height= view->screen_height;
	overhead_data.top= overhead_data.left= 0;
	*/
	
	_render_overhead_map(&overhead_data);

	RGBForeColor(&rgb_black);
	PenSize(1, 1);
	TextFont(0);
	TextFace(normal);
	TextSize(0);
	SetGWorld(old_gworld, old_device);
	
	return;
}

// Accessors for the screen's GDevice and GrafPort;
// this abstraction is used for DrawSprocket support
GDHandle GetWorldDevice()
{
	if (DSp_Check() && (DSp_Ctxt != NULL))
	{
		DisplayIDType DisplayID;
		GDHandle DC_Handle;
		assert(DSp_HandleError("Getting Display ID",DSpContext_GetDisplayID(DSp_Ctxt,&DisplayID)));
		DMGetGDeviceByDisplayID(DisplayID,&DC_Handle,true);
		return DC_Handle;
	}
	
	return world_device;
}
CGrafPtr GetScreenGrafPort() {
	if (DSp_Check() && (DSp_Ctxt != NULL))
	{
		CGrafPtr DC_Buffer;
		assert(DSp_HandleError("Getting GrafPtr",DSpContext_GetFrontBuffer(DSp_Ctxt,&DC_Buffer)));
		return DC_Buffer;
	}
	return (CGrafPtr)screen_window;
}


bool machine_supports_16bit(
	GDSpecPtr spec)
{
	GDSpec test_spec;
	bool found_16bit_device= true;

	/* Make sure that enough_memory_for_16bit is valid */
	calculate_screen_options();
	
	test_spec= *spec;
	test_spec.bit_depth= 16;
	if (!HasDepthGDSpec(&test_spec)) /* automatically searches for grayscale devices */
	{
		found_16bit_device= false;
	}
	
	return found_16bit_device && enough_memory_for_16bit;
}

bool machine_supports_32bit(
	GDSpecPtr spec)
{
	GDSpec test_spec;
	bool found_32bit_device= true;

	/* Make sure that enough_memory_for_16bit is valid */
	calculate_screen_options();

	test_spec= *spec;
	test_spec.bit_depth= 32;
	if (!HasDepthGDSpec(&test_spec)) /* automatically searches for grayscale devices */
	{
		found_32bit_device= false;
	}
	
	return found_32bit_device && enough_memory_for_32bit;
}

short hardware_acceleration_code(
	GDSpecPtr spec)
{
	short acceleration_code= _no_acceleration;
	
	calculate_screen_options();
	
	/*
	if (machine_has_valkyrie(spec) && enough_memory_for_16bit)
	{
		acceleration_code= _valkyrie_acceleration;
	}
	*/
	
	return acceleration_code;
}

void update_screen_window(
	WindowPtr window,
	EventRecord *event)
{
	(void) (window,event);
		
	draw_interface();
	change_screen_mode(&screen_mode, true);
	assert_world_color_table(interface_color_table, world_color_table);
	
	return;
}

void activate_screen_window(
	WindowPtr window,
	EventRecord *event,
	bool active)
{
	(void) (window,event,active);
	
	return;
}

/* LowLevelSetEntries bypasses the Color Manager and goes directly to the hardware.  this means
	QuickDraw doesn�t fuck up during RGBForeColor and RGBBackColor. */
void animate_screen_clut(
	struct color_table *color_table,
	bool full_screen)
{
	CTabHandle macintosh_color_table= build_macintosh_color_table(color_table);
	
	if (macintosh_color_table)
	{
		GDHandle old_device;

		HLock((Handle)macintosh_color_table);	
		old_device= GetGDevice();
		SetGDevice(world_device);
		switch (screen_mode.acceleration)
		{
			/*
			case _valkyrie_acceleration:
				if (!full_screen)
				{
					valkyrie_change_video_clut(macintosh_color_table);
					break;
				}
				// if we�re doing full-screen fall through to LowLevelSetEntries
			*/
			
			// LP change: added OpenGL behavior
			case _opengl_acceleration:
			case _no_acceleration:
				LowLevelSetEntries(0, (*macintosh_color_table)->ctSize, (*macintosh_color_table)->ctTable);
				break;
		}
		
		DisposeHandle((Handle)macintosh_color_table);
		SetGDevice(old_device);
	}
	
	return;
}

void assert_world_color_table(
	struct color_table *interface_color_table,
	struct color_table *world_color_table)
{
	if (interface_bit_depth==8)
	{
		CTabHandle macintosh_color_table= build_macintosh_color_table(interface_color_table);
		
		if (macintosh_color_table)
		{
			GDHandle old_device;
			
			HLock((Handle)macintosh_color_table);
			old_device= GetGDevice();
			SetGDevice(world_device);
			SetEntries(0, (*macintosh_color_table)->ctSize, (*macintosh_color_table)->ctTable);
			SetGDevice(old_device);
			
			DisposeHandle((Handle)macintosh_color_table);
		}
	}
	
	if (world_color_table) animate_screen_clut(world_color_table, false);
	
	return;
}

void darken_world_window(
	void)
{
	GrafPtr old_port;
	Rect bounds;
	
	GetPort(&old_port);
	SetPort(screen_window);
	PenPat(&qd.gray);
	PenMode(srcOr);
	calculate_destination_frame(screen_mode.size, screen_mode.high_resolution, &bounds);
	PaintRect(&bounds);
	PenMode(srcCopy);
	PenPat(&qd.black);
	SetPort(old_port);
	
	return;
}

void validate_world_window(
	void)
{
	GrafPtr old_port;
	Rect bounds;
	
	GetPort(&old_port);
	SetPort(screen_window);
	calculate_destination_frame(screen_mode.size, screen_mode.high_resolution, &bounds);
	ValidRect(&bounds);
	SetPort(old_port);

	switch (screen_mode.acceleration)
	{
		/*
		case _valkyrie_acceleration:
			valkyrie_erase_graphic_key_frame(0xfe);
			break;
		*/
	}
	
	return;
}



/* ---------- private code */

// LP addition: routine for displaying text
static void DisplayText(short BaseX, short BaseY, char *Text)
{
	// OpenGL version:
	// activate only in the main view, and also if OpenGL is being used for the overhead map
	if((OGL_MapActive || !world_view->overhead_map_active) && !world_view->terminal_mode_active)
		if (OGL_RenderText(BaseX, BaseY, Text)) return;
	
	// C to Pascal
	Str255 PasText;
	
	int Len = MIN(strlen(Text),255);
	PasText[0] = Len;
	memcpy(PasText+1,Text,Len);
	
	// LP change: added drop-shadow rendering
	// Changed it to a black halo
	
	RGBForeColor(&rgb_black);
	MoveTo(BaseX+1,BaseY+1);
	DrawString(PasText);
	// Changed back to drop shadow only for performance reasons
	/*
	MoveTo(BaseX+1,BaseY);
	DrawString(PasText);
	MoveTo(BaseX+1,BaseY-1);
	DrawString(PasText);
	MoveTo(BaseX,BaseY+1);
	DrawString(PasText);
	MoveTo(BaseX,BaseY-1);
	DrawString(PasText);
	MoveTo(BaseX-1,BaseY+1);
	DrawString(PasText);
	MoveTo(BaseX-1,BaseY);
	DrawString(PasText);
	MoveTo(BaseX-1,BaseY-1);
	DrawString(PasText);
	*/
	
	RGBForeColor(&rgb_white);
	MoveTo(BaseX,BaseY);
	DrawString(PasText);
}


static void update_fps_display(
	GrafPtr port)
{
	if (displaying_fps)
	{
		long ticks= TickCount();
		GrafPtr old_port;
		char fps[256];
		
		frame_ticks[frame_index]= ticks;
		frame_index= (frame_index+1)%FRAME_SAMPLE_SIZE;
		if (frame_count<FRAME_SAMPLE_SIZE)
		{
			frame_count+= 1;
			strcpy(fps, "--");
		}
		else
		{
			sprintf(fps, "%3.2ffps", (FRAME_SAMPLE_SIZE*60)/(float)(ticks-frame_ticks[frame_index]));
		}

		GetPort(&old_port);
		SetPort(port);
		// Now using font-definition abstraction
		FontSpecifier& Font = GetOnScreenFont();
		Font.Use();
		// The line spacing is a generalization of "5" for larger fonts
		short Offset = Font.LineSpacing / 3;
		// LP: Changed font size from 9 to this more readable value
		// TextSize(12);
		// LP: No desire to change this at the moment
		// TextFont(kFontIDMonaco);
		// LP change: moved the rendering out
		DisplayText(port->portRect.left+Offset,port->portRect.bottom-Offset,fps);
		RGBForeColor(&rgb_black);
		/*
		MoveTo(5, port->portRect.bottom-5);
		RGBForeColor(&rgb_white);
		DrawString(fps);
		RGBForeColor(&rgb_black);
		*/
		SetPort(old_port);
	}
	else
	{
		frame_count= frame_index= 0;
	}
	
	return;
}


static void DisplayPosition(GrafPtr port)
{
	if (!ShowPosition) return;
	
	// Push
	GrafPtr old_port;
	GetPort(&old_port);
	SetPort(port);
	
	FontSpecifier& Font = GetOnScreenFont();
	Font.Use();
	// TextSize(12);
	// TextFont(kFontIDMonaco);
	
	short LineSpacing = Font.LineSpacing;
	short X = port->portRect.left + LineSpacing/3;
	short Y = port->portRect.top + LineSpacing;
	const float FLOAT_WORLD_ONE = float(WORLD_ONE);
	const float AngleConvert = 360/float(FULL_CIRCLE);
	sprintf(temporary, "X       = %8.3f",world_view->origin.x/FLOAT_WORLD_ONE);
	DisplayText(X,Y,temporary);
	Y += LineSpacing;
	sprintf(temporary, "Y       = %8.3f",world_view->origin.y/FLOAT_WORLD_ONE);
	DisplayText(X,Y,temporary);
	Y += LineSpacing;
	sprintf(temporary, "Z       = %8.3f",world_view->origin.z/FLOAT_WORLD_ONE);
	DisplayText(X,Y,temporary);
	Y += LineSpacing;
	sprintf(temporary, "Polygon = %8d",world_view->origin_polygon_index);
	DisplayText(X,Y,temporary);
	Y += LineSpacing;
	short Angle = world_view->yaw;
	if (Angle > HALF_CIRCLE) Angle -= FULL_CIRCLE;
	sprintf(temporary, "Yaw     = %8.3f",AngleConvert*Angle);
	DisplayText(X,Y,temporary);
	Y += LineSpacing;
	Angle = world_view->pitch;
	if (Angle > HALF_CIRCLE) Angle -= FULL_CIRCLE;
	sprintf(temporary, "Pitch   = %8.3f",AngleConvert*Angle);
	DisplayText(X,Y,temporary);
	
	RGBForeColor(&rgb_black);
	
	// Pop
	SetPort(old_port);
}

static void DisplayMessages(GrafPtr port)
{	
	// Push
	GrafPtr old_port;
	GetPort(&old_port);
	SetPort(port);
	
	FontSpecifier& Font = GetOnScreenFont();
	Font.Use();
	// TextSize(12);
	// TextFont(kFontIDMonaco);
	
	short LineSpacing = Font.LineSpacing;
	short X = port->portRect.left + LineSpacing/3;
	short Y = port->portRect.top + LineSpacing;
	if (ShowPosition) Y += 6*LineSpacing;	// Make room for the position data
	for (int k=0; k<NumScreenMessages; k++)
	{
		int Which = (MostRecentMessage+NumScreenMessages-k) % NumScreenMessages;
		while (Which < 0)
			Which += NumScreenMessages;
		ScreenMessage& Message = Messages[Which];
		if (Message.TimeRemaining <= 0) continue;
		Message.TimeRemaining--;
		
		DisplayText(X,Y,Message.Text);
		Y += LineSpacing;
	}
	
	RGBForeColor(&rgb_black);
	
	// Pop
	SetPort(old_port);
}


static void restore_world_device(
	void)
{
	GrafPtr old_port;

	switch (screen_mode.acceleration)
	{
		/*
		case _valkyrie_acceleration:
			valkyrie_restore();
			break;
		*/
	}
	
	GetPort(&old_port);
	SetPort(screen_window);
	PaintRect(&screen_window->portRect);
	SetPort(old_port);
	
	myShowMenuBar();

	/* put our device back the way we found it */
	SetDepthGDSpec(&restore_spec);

#if 0
	if (resolution_restore_spec.width)
	{
		SetResolutionGDSpec(&resolution_restore_spec);
	}
#endif

	myDisposeGWorld(world_pixels);
	
	CloseWindow(screen_window);
	CloseWindow(backdrop_window);
	
	return;
}

// LP changes: moved sizing and resolution outside of this function,
// because they can be very variable
/* pixels are already locked, etc. */
/*
static void update_screen(
	void)
	*/
static void update_screen(Rect& source, Rect& destination, bool hi_rez)
{
	/*
	Rect source, destination;
	
	calculate_source_frame(screen_mode.size, screen_mode.high_resolution, &source);
	calculate_destination_frame(screen_mode.size, screen_mode.high_resolution, &destination);
	*/
	
	if (hi_rez)
	// if (screen_mode.high_resolution)
	{
		GrafPtr old_port;
		RGBColor old_forecolor, old_backcolor;
		
		GetPort(&old_port);
		SetPort(screen_window);

		GetForeColor(&old_forecolor);
		GetBackColor(&old_backcolor);
		RGBForeColor(&rgb_black);
		RGBBackColor(&rgb_white);
		
		CopyBits((BitMapPtr)*world_pixels->portPixMap, &screen_window->portBits,
			&source, &destination, srcCopy, (RgnHandle) NULL);
		
		RGBForeColor(&old_forecolor);
		RGBBackColor(&old_backcolor);
		SetPort(old_port);
	}
	else
	{
		int8 mode;
		short pelsize= bit_depth>>3;
		struct copy_screen_data data;
		short source_rowBytes, destination_rowBytes, source_width, destination_width;
		PixMapHandle screen_pixmap= (*world_device)->gdPMap;
		
		source_rowBytes= (*myGetGWorldPixMap(world_pixels))->rowBytes&0x3fff;
		destination_rowBytes= (*screen_pixmap)->rowBytes&0x3fff;
		source_width= RECTANGLE_WIDTH(&source);
		destination_width= RECTANGLE_WIDTH(&destination);
		
		data.source= (unsigned char *)myGetPixBaseAddr(world_pixels);
		data.source_slop= source_rowBytes-source_width*pelsize;
		
		data.destination= (unsigned char *)(*screen_pixmap)->baseAddr + (destination.top-screen_window->portRect.top)*destination_rowBytes +
			(destination.left-screen_window->portRect.left)*pelsize;
		data.destination_slop= destination_rowBytes-destination_width*pelsize;

		data.bytes_per_row= destination_rowBytes;
		data.height= RECTANGLE_HEIGHT(&source);

		assert(!((long)data.source&3));
		assert(!((long)data.destination&3));
		assert(!(data.destination_slop&3));
		assert(!(data.source_slop&3));
		assert(!(data.bytes_per_row&3));

		data.flags= 0;
		data.destination_slop+= destination_rowBytes;
#ifdef env68k
		if (screen_mode.draw_every_other_line) data.flags|= _EVERY_OTHER_LINE_BIT;
#endif
		switch (bit_depth)
		{
			case 8:
				data.width= source_width>>2;
				break;
			case 16:
				data.width= source_width>>1;
				data.flags|= _SIXTEEN_BIT_BIT;
				break;
			case 32:
				data.width= source_width;
				data.flags|= _THIRTYTWO_BIT_BIT;
				break;
			default:
				// LP change:
				assert(false);
				// halt();
		}

		mode= true32b;
		SwapMMUMode(&mode);
		quadruple_screen(&data);
		SwapMMUMode(&mode);
	}

	return;
}

/* This function is NOT void because both the computer interface editor and vulcan use it */
/*  to determine the size of their gworlds.. */
void calculate_destination_frame(
	short size,
	bool high_resolution,
	Rect *frame)
{
	(void) (high_resolution);
	
	/* calculate destination frame */
	switch (size)
	{
		case _full_screen: /* ow */
			SetRect(frame, 0, 0, DESIRED_SCREEN_WIDTH, DESIRED_SCREEN_HEIGHT);
			break;
		case _100_percent:
			SetRect(frame, 0, 0, DEFAULT_WORLD_WIDTH, DEFAULT_WORLD_HEIGHT);
			break;
		case _75_percent:
			SetRect(frame, 0, 0, 3*DEFAULT_WORLD_WIDTH/4, 3*DEFAULT_WORLD_HEIGHT/4);
			break;
		case _50_percent:
			SetRect(frame, 0, 0, DEFAULT_WORLD_WIDTH/2, DEFAULT_WORLD_HEIGHT/2);
			break;
	}
	
	if (size!=_full_screen)
	{
		OffsetRect(frame, WORLD_H+((DEFAULT_WORLD_WIDTH-frame->right)>>1), 
			WORLD_V+((DEFAULT_WORLD_HEIGHT-frame->bottom)>>1));
	}
	
	return;
}

enum
{
	BITS_PER_CACHE_LINE= 6,
	CACHE_LINE_SIZE= (1<<BITS_PER_CACHE_LINE) // ppc601
};

static void calculate_adjusted_source_frame(
	struct screen_mode_data *mode,
	Rect *frame)
{
	calculate_source_frame(mode->size, mode->high_resolution, frame);

#ifdef OBSOLETE
	short width;

#ifdef envppc
#if 0
	// calculate width in bytes
	width= RECTANGLE_WIDTH(frame);
	switch (mode->bit_depth)
	{
		case 8: break;
		case 16: width<<= 1; break;
		case 32: width<<= 2; break;
		default:
			// LP change:
			assert(false);
			// halt();
	}

	// assure that our width is an odd-multiple of our cache line size
	if (width&(CACHE_LINE_SIZE-1)) width= (width&~(CACHE_LINE_SIZE-1)) + CACHE_LINE_SIZE;
	if (!((width>>BITS_PER_CACHE_LINE)&1)) width+= CACHE_LINE_SIZE;
	
	// restore width in pixels
	switch (mode->bit_depth)
	{
		case 8: break;
		case 16: width>>= 1; break;
		case 32: width>>= 2; break;
		default:
			// LP change:
			assert(false);
			// halt();
	}
#else
	if (mode->bit_depth==8) width= 704;
#endif
	frame->right= frame->left + width;
#endif
#endif

	return;
}

static void calculate_source_frame(
	short size,
	bool high_resolution,
	Rect *frame)
{
	calculate_destination_frame(size, high_resolution, frame);
	if (!high_resolution)
	{
		frame->right-= (RECTANGLE_WIDTH(frame)>>1);
		frame->bottom-= (RECTANGLE_HEIGHT(frame)>>1);
	}
	OffsetRect(frame, (*world_device)->gdRect.left-frame->left,
		(*world_device)->gdRect.top-frame->top);
	
	return;
}

static void calculate_screen_options(
	void)
{
	static bool screen_options_initialized= false;
	
	if(!screen_options_initialized)
	{
		enough_memory_for_16bit= (FreeMem()>FREE_MEMORY_FOR_16BIT) ? true : false;
		enough_memory_for_32bit= (FreeMem()>FREE_MEMORY_FOR_32BIT) ? true : false;
		enough_memory_for_full_screen= (FreeMem()>FREE_MEMORY_FOR_FULL_SCREEN) ? true : false;
		screen_options_initialized= true;
	}
}


#ifndef EXTERNAL
void quadruple_screen(
	struct copy_screen_data *data)
{
	register short bytes_per_row= data->bytes_per_row;
	register long *read= (long *) data->source;
	register long *write1= (long *) data->destination;
	register long *write2= (long *) ((byte *)data->destination + bytes_per_row);
	register unsigned long in, out1, out2;
	register short count;
	short rows_left;

	for (rows_left=data->height;rows_left>0;--rows_left)
	{
		if (data->flags&_THIRTYTWO_BIT_BIT)
		{
			for (count= data->width; count>0; --count)
			{
				in= *read++;
				*write1++= *write2++= *write1++= *write2++= in;
			}
		}
		else
		{
			if (data->flags&_SIXTEEN_BIT_BIT)
			{
				for (count=data->width;count>0;--count)
				{
					in= *read++;
					*write1++= *write2++= (in&0xffff0000) | (in>>16);
					*write1++= *write2++= (in&0x0000ffff) | (in<<16);
				}
			}
			else
			{
				if (data->flags&_EVERY_OTHER_LINE_BIT)
				{
					for (count=data->width;count>0;--count)
					{
						in= *read++;
						*write1++= ((unsigned long)(((word)(in&0xff)) | ((word)(in<<8)))) | (unsigned long)((((in>>8)&0xff) | (in&0xff00))<<16);
						in>>= 16;
						*write1++= ((unsigned long)(((word)(in&0xff)) | ((word)(in<<8)))) | (unsigned long)((((in>>8)&0xff) | (in&0xff00))<<16);
					}
				}
				else
				{
					for (count=data->width;count>0;--count)
					{
						in= *read++;
						out2= ((unsigned long)(((word)(in&0xff)) | ((word)(in<<8)))) | (unsigned long)((((in>>8)&0xff) | (in&0xff00))<<16);
						in>>= 16;
						out1= ((unsigned long)(((word)(in&0xff)) | ((word)(in<<8)))) | (unsigned long)((((in>>8)&0xff) | (in&0xff00))<<16);
						
						*write1++= out1, *write2++= out1;
						*write1++= out2, *write2++= out2;
					}
				}
			}
		}
		
		(byte*)read+= data->source_slop;
		(byte*)write1+= data->destination_slop;
		(byte*)write2+= data->destination_slop;
	}
	
	return;
}
#endif


// LP addition: routine for doing screendumps.
// Ought to return some error code.
void dump_screen()
{

	// Push the old graphics context
	GrafPtr old_port;
	GetPort(&old_port);
	SetPort(screen_window);
	
	RGBColor old_forecolor, old_backcolor;
	GetForeColor(&old_forecolor);
	GetBackColor(&old_backcolor);
	RGBForeColor(&rgb_black);
	RGBBackColor(&rgb_white);

	// Find out how the area that gets copied;
	// this is intended to be done as the game runs (including terminal mode)
	Rect DumpRect;

	short msize = screen_mode.size;
	assert(msize >= 0 && msize < NUMBER_OF_VIEW_SIZES);
	const ViewSizeData& VS = ViewSizes[msize];
	
	// Rectangle where the view is to go (must not overlap the HUD)
	short OverallWidth = VS.OverallWidth;
	short OverallHeight = VS.OverallHeight;
	
	// Offsets for placement in the screen
	Rect ScreenRect = screen_window->portRect;
	short ScreenOffsetWidth = ((RECTANGLE_WIDTH(&ScreenRect) - VS.OverallWidth) >> 1) + ScreenRect.left;
	short ScreenOffsetHeight = ((RECTANGLE_HEIGHT(&ScreenRect) - VS.OverallHeight) >> 1) + ScreenRect.top;
	
	SetRect(&DumpRect, 0, 0, OverallWidth, OverallHeight);
	OffsetRect(&DumpRect, ScreenOffsetWidth, ScreenOffsetHeight);
	
	// Just in case it's too big...
	DumpRect.left = MAX(DumpRect.left,ScreenRect.left);
	DumpRect.top = MAX(DumpRect.top,ScreenRect.top);
	DumpRect.right = MIN(DumpRect.right,ScreenRect.right);
	DumpRect.bottom = MIN(DumpRect.bottom,ScreenRect.bottom);
	
	// Now create a picture
	OpenCPicParams PicParams;
	PicParams.srcRect = DumpRect;
	PicParams.hRes = 0x00480000; // 72 dpi
	PicParams.vRes = 0x00480000; // 72 dpi
	PicParams.version = -2;
	PicParams.reserved1 = 0;
	PicParams.reserved2 = 0;
	PicHandle PicObject = OpenCPicture(&PicParams);

	ClipRect(&DumpRect);
	CopyBits(&screen_window->portBits, &screen_window->portBits,
		&DumpRect, &DumpRect, srcCopy, (RgnHandle) NULL);

	ClosePicture();
	
	// Pop back the old graphics context
	RGBForeColor(&old_forecolor);
	RGBBackColor(&old_backcolor);
	SetPort(old_port);

	// Decide on the file's name
	FSSpec Spec;
	// Get the app's FSSpec using cseries function
	get_my_fsspec(&Spec);
	
	// Now write out that picture
	short RefNum;
	OSErr err;
	
	int Indx = 1;
	while(true) {
		// Compose a suitable name
		psprintf(Spec.name,"Screenshot %0.4d",Indx++);
		// Check for the pre-existence of a file with that name
		// by trying to open it
		err = FSpOpenDF(&Spec, fsRdPerm, &RefNum);
		if (err == noErr)
		{
			FSClose(RefNum);
			continue;
		}
		// Now try to create it
		err = FSpCreate(&Spec,'ttxt','PICT',smSystemScript);
		if (err != noErr)
		{
			KillPicture(PicObject);
			return;
		}
		else break;
	}
	
	err = FSpOpenDF(&Spec, fsRdWrPerm, &RefNum);
	if (err != noErr) {
		KillPicture(PicObject);
		return;
	}
	
	// Write the header:
	long Zero = 0;
	long len;
	for (int z=0; z<512/sizeof(Zero); z++)
	{
		len = sizeof(Zero);
		err = FSWrite(RefNum,&len,&Zero);
		if (err != noErr) {
			FSClose(RefNum);
			KillPicture(PicObject);
			return;
		}
	}
	
	// Make that "snapshot" sound;
	// the resource number is from the "System" file
	Handle SoundHandle = GetResource('snd ',-16504);
	if (SoundHandle != NULL)
	{
		// Cribbed from Apple's QT Sound Manager docs
		HLock(SoundHandle);
		SndPlay(NULL, (SndListResource **)SoundHandle, true);
        HUnlock(SoundHandle);
		ReleaseResource(SoundHandle);
	}
	
	// Write the body:
	long PicLen = GetHandleSize(Handle(PicObject));
	len = PicLen;
	HLock(Handle(PicObject));
	err = FSWrite(RefNum,&len,*PicObject);
	HUnlock(Handle(PicObject));
		
	// All done!
	FSClose(RefNum);
	KillPicture(PicObject);
	return;
}


// Actually do the drawing, in software mode
void DrawHUD(Rect& SourceRect, Rect& DestRect)
{
	// Check if the HUD buffer is there
	assert(HUD_Buffer);
	
	// Code cribbed from draw_panels() in game_window_macintosh.c
	GrafPtr old_port;
	RGBColor old_forecolor, old_backcolor;
	
	GetPort(&old_port);
	SetPort(screen_window);
	GetForeColor(&old_forecolor);
	GetBackColor(&old_backcolor);
	RGBForeColor(&rgb_black);
	RGBBackColor(&rgb_white);
		
	/* Slam it to the screen. */
	CopyBits((BitMapPtr)*HUD_Buffer->portPixMap, &screen_window->portBits, //(BitMapPtr)*screen_pixmap,
		&SourceRect, &DestRect, srcCopy, (RgnHandle) NULL);	
	RGBForeColor(&old_forecolor);
	RGBBackColor(&old_backcolor);
	SetPort(old_port);
}


// LP: cribbed from change_screen_mode
void ClearScreen()
{
	GrafPtr old_port;
	GetPort(&old_port);
	SetPort(screen_window);
	PaintRect(&screen_window->portRect);
	SetPort(old_port);
}

// For switching to another process and returning (suspend/resume events)

void SuspendDisplay(EventRecord *EvPtr)
{
	// Switch resolution only if necessary
	if (restore_spec.bit_depth != graphics_preferences->device_spec.bit_depth)
		SetDepthGDSpec(&restore_spec);
	
	Boolean EvWasProcessed = false;
	if (DSp_Check()) DSpProcessEvent(EvPtr,&EvWasProcessed);
	
	HideWindow(screen_window);
	HideWindow(backdrop_window);
	SetCursor(&qd.arrow);
	myShowMenuBar();
	show_cursor();
}

void ResumeDisplay(EventRecord *EvPtr)
{
	Boolean EvWasProcessed = false;
	if (DSp_Check()) DSpProcessEvent(EvPtr,&EvWasProcessed);
	
	// The resolution may have changed when the app was switched out
	BuildGDSpec(&restore_spec, world_device);
	if (restore_spec.bit_depth != graphics_preferences->device_spec.bit_depth)
		SetDepthGDSpec(&graphics_preferences->device_spec);
	
	SetCursor(&qd.arrow);
	myHideMenuBar(GetMainDevice());
	ShowWindow(backdrop_window);
	SelectWindow(screen_window);
	ShowWindow(screen_window);
}

// Handling the resolution with the DrawSprocket

// Checks on the DrawSprocket's present and returns whether it is active;
// it inits DSp if necessary
bool DSp_Check()
{
	if (DSp_Inited) return DSp_Present;
	
	// Check on the weak linking...
	if((Ptr)DMGetDisplayIDByGDevice == (Ptr)kUnresolvedCFragSymbolAddress) return DSp_Present;
	if((Ptr)DSpStartup == (Ptr)kUnresolvedCFragSymbolAddress) return DSp_Present;
	
	// Set this because of having called this
	DSp_Inited = true;
	
	// Now start!
	return (DSp_Present = (DSpStartup() == noErr));
}


// For debugging convenience;
// returns 1 for success, 0 for failure
static bool DSp_HandleError(char *Description, OSStatus ErrorCode)
{
	if (ErrorCode != noErr) dprintf("DSp: %s Error: %d",Description,ErrorCode);
	return (ErrorCode == noErr);
}


// Changes the monitor's resolution and creates a DSp context for the new resolution
// and deletes the old one if necessary; returns the success of doing so
bool DSp_ChangeResolution(GDHandle Device, short BitDepth, short Width, short Height)
{
	if (!DSp_Check()) return false;
	
	DisplayIDType DisplayID;
	if (!DSp_HandleError("CR: Find Display ID",DMGetDisplayIDByGDevice(Device, &DisplayID, true))) return false;
	
	DSpContextAttributes TargetAttribs;
	
	// Cribbed from some of Apple's and Mark Szymczyk's sample code
	obj_clear(TargetAttribs);
	TargetAttribs.displayWidth = Width;
	TargetAttribs.displayHeight = Height;
	TargetAttribs.displayBestDepth = BitDepth;
	TargetAttribs.backBufferBestDepth = BitDepth;
	TargetAttribs.colorNeeds = kDSpColorNeeds_Require;
	TargetAttribs.displayDepthMask = kDSpDepthMask_All;
	TargetAttribs.backBufferDepthMask = kDSpDepthMask_All;
	TargetAttribs.pageCount = 1; // only the front buffer is needed
	
	DSpContextReference NewContext;
	
	if (!DSp_HandleError("CR: Find Context",DSpFindBestContextOnDisplayID(&TargetAttribs,&NewContext,DisplayID))) return false;
	if (NewContext == NULL) return false;
	if (!DSp_HandleError("CR: Reserve",DSpContext_Reserve(NewContext, &TargetAttribs))) return false;
	if (DSp_Ctxt)
	{
		// Switch and delete old context if there is one
		if (!DSp_HandleError("CR: Queue",DSpContext_Queue(DSp_Ctxt, NewContext, &TargetAttribs))) return false;
		if (!DSp_HandleError("CR: Switch",DSpContext_Switch(DSp_Ctxt, NewContext))) return false;
		if (!DSp_HandleError("CR: Release",DSpContext_Release(DSp_Ctxt))) return false;
	}
	if (!DSp_HandleError("CR: Activate",DSpContext_SetState(NewContext, kDSpContextState_Active))) return false;
	if (!DSp_HandleError("CR: Pause",DSpContext_SetState(NewContext, kDSpContextState_Paused))) return false;
	DSp_Ctxt = NewContext;
	
	return true;
}
