#ifndef _SCREEN_H_
#define _SCREEN_H_

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

#include <utility>
#include <vector>
#include <SDL.h>

struct Rect;

struct screen_mode_data;
namespace alephbet
{
	class Screen
	{
	public:
		static inline Screen* instance() {
			return &m_instance;
		}

		void Initialize(screen_mode_data* mode);
		const std::vector<std::pair<int, int> >& GetModes() { return m_modes; };
		int FindMode(int width, int height) {
			for (int i = 0; i < m_modes.size(); ++i)
			{
				if (m_modes[i].first == width &&
				    m_modes[i].second == height)
				{
					return i;
				}
			}
			return -1;
		}
		int ModeHeight(int mode) { return m_modes[mode].second; }
		int ModeWidth(int mode) { return m_modes[mode].first; }

		int height();
		int width();
		float pixel_scale();
		int window_height();
		int window_width();
		bool hud();
		bool lua_hud();
		bool openGL();
		bool fifty_percent();
		bool seventyfive_percent();
		SDL_Rect window_rect(); // 3D view + interface
		SDL_Rect view_rect(); // main 3D view
		SDL_Rect map_rect();
		SDL_Rect term_rect();
		SDL_Rect hud_rect();
		SDL_Rect OpenGLViewPort();

		void bound_screen(bool in_game = true);
		void bound_screen_to_rect(SDL_Rect &r, bool in_game = true);
		void scissor_screen_to_rect(SDL_Rect &r);
		void window_to_screen(int &x, int &y);
		
		SDL_Rect lua_clip_rect;
		SDL_Rect lua_view_rect;
		SDL_Rect lua_map_rect;
		SDL_Rect lua_term_rect;

		// TODO: the HUD should really draw messages / fps / input line itself
		Rect lua_text_margins;

	private:
		Screen() : m_initialized(false) { }
		static Screen m_instance;
		bool m_initialized;
		SDL_Rect m_viewport_rect;
		SDL_Rect m_ortho_rect;

		std::vector<std::pair<int, int> > m_modes;
	};
}

extern SDL_PixelFormat pixel_format_16;
extern SDL_PixelFormat pixel_format_32;

/* ---------- constants */

// Original screen-size definitions
enum /* screen sizes */
{
	_50_percent,
	_75_percent,
	_100_percent,
	_full_screen,
};

enum /* hardware acceleration codes */
{
	_no_acceleration,
	_opengl_acceleration
};

enum /* screen selection based on game state */
{
	_screentype_level,
	_screentype_menu,
	_screentype_chapter
};

/* ---------- missing from QUICKDRAW.H */

#define deviceIsGrayscale 0x0000
#define deviceIsColor 0x0001

/* ---------- structures */

/* ---------- globals */

extern struct color_table *world_color_table, *visible_color_table, *interface_color_table;

/* ---------- prototypes/SCREEN.C */

void initialize_gamma(void);

void change_screen_clut(struct color_table *color_table);
void change_interface_clut(struct color_table *color_table);
void animate_screen_clut(struct color_table *color_table, bool full_screen);

void build_direct_color_table(struct color_table *color_table, short bit_depth);

void start_teleporting_effect(bool out);
void start_extravision_effect(bool out);

void render_screen(short ticks_elapsed);

void toggle_overhead_map_display_status(void);

// Returns whether the size scale had been changed
bool zoom_overhead_map_out(void);
bool zoom_overhead_map_in(void);

bool map_is_translucent(void);

void enter_screen(void);
void exit_screen(void);

void validate_world_window(void);

void change_gamma_level(short gamma_level);

void assert_world_color_table(struct color_table *world_color_table, struct color_table *interface_color_table);

// LP change: added function for resetting the screen state when starting a game
void reset_screen();

// CP addition: added function to return the the game size
screen_mode_data *get_screen_mode(void);

// LP: when initing, ask whether to show the monitor-frequency dialog
//void initialize_screen(struct screen_mode_data *mode, bool ShowFreqDialog);
void change_screen_mode(struct screen_mode_data *mode, bool redraw, bool resize_hud = false);
void change_screen_mode(short screentype);

void toggle_fullscreen(bool fs);
void toggle_fullscreen();
void update_screen_window(void);
void clear_screen(bool update = true);

void calculate_destination_frame(short size, bool high_resolution, Rect *frame);

// LP addition: a routine for dumping the screen contents into a file.
// May need to be modified for pass-through video cards like the older 3dfx ones.
void dump_screen();

// For getting and setting tunnel-vision mode
bool GetTunnelVision();
bool SetTunnelVision(bool TunnelVisionOn);

// Request for drawing the HUD
void RequestDrawingHUD();
// Request for drawing the terminal
void RequestDrawingTerm();
// Request for drawing (or redrawing) a menu or intro screen
void draw_intro_screen();

// Corresponding with-and-without-HUD sizes for some view-size index,
// for the convenience of Pfhortran scripting;
// the purpose is to get a similar size of display with the HUD status possibly changed
short SizeWithHUD(short _size);
short SizeWithoutHUD(short _size);

// Displays a message on the screen for a second or so; may be good for debugging
void ShowMessage(char *Text);

/* SB: Custom Blizzard-style overlays */
#define MAXIMUM_NUMBER_OF_SCRIPT_HUD_ELEMENTS 6
bool IsScriptHUDNonlocal();
void SetScriptHUDNonlocal(bool nonlocal = true);
/* color is a terminal color */
void SetScriptHUDColor(int player, int idx, int color);
/* text == NULL or "" removes that HUD element
   to turn HUD elements off, set all elements NULL or "" */
void SetScriptHUDText(int player, int idx, const char* text);
/* icon == NULL turns the icon off
   someday I'll document the format */
bool SetScriptHUDIcon(int player, int idx, const char* icon, size_t length);
/* sets the icon for that HUD to a colored square (same colors as SetScriptHUDColor) */
void SetScriptHUDSquare(int player, int idx, int color);

bool MainScreenVisible();
int MainScreenLogicalWidth();
int MainScreenLogicalHeight();
int MainScreenWindowWidth();
int MainScreenWindowHeight();
int MainScreenPixelWidth();
int MainScreenPixelHeight();
float MainScreenPixelScale();
bool MainScreenIsOpenGL();
void MainScreenSwap();
void MainScreenCenterMouse();
SDL_Surface *MainScreenSurface();
void MainScreenUpdateRect(int x, int y, int w, int h);
void MainScreenUpdateRects(size_t count, const SDL_Rect *rects);

#endif
