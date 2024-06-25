#ifndef _OVERHEAD_MAP_SDL_CLASS_
#define _OVERHEAD_MAP_SDL_CLASS_

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
 *  OverheadMap_SDL.h -- Subclass of OverheadMapClass for rendering with SDL
 */

#include "OverheadMapRenderer.h"


class OverheadMap_SDL_Class : public OverheadMapClass {
protected:
	void draw_polygon(
		short vertex_count,
		short *vertices,
		rgb_color &color);

	void draw_line(
		short *vertices,
		rgb_color &color,
		short pen_size);

	void draw_thing(
		world_point2d &center,
		rgb_color &color,
		short shape,
		short radius);

	void draw_player(
		world_point2d &center,
		angle facing,
		rgb_color &color,
		short shrink,
		short front,
		short rear,
		short rear_theta);

	void draw_text(
		world_point2d &location,
		rgb_color &color,
		char *text,
		FontSpecifier& FontData,
		// FontDataStruct &FontData,
		short justify);

	void set_path_drawing(rgb_color &color);

	void draw_path(
		short step,	// 0: first point
		world_point2d &location);

private:
	uint32 path_pixel;
	world_point2d path_point;
};

#endif
