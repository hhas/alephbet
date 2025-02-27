#ifndef _OVERHEAD_MAP_OPENGL_CLASS_
#define _OVERHEAD_MAP_OPENGL_CLASS_

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
 *  Overhead-Map OpenGL Class
 *  Subclass of OverheadMapClass for doing rendering in OpenGL
 */

#include <vector>
#include "OverheadMapRenderer.hpp"


class OverheadMap_OGL_Class: public OverheadMapClass
{
	void begin_overall();
	void end_overall();
	
	void begin_polygons();
	
	void draw_polygon(
		short vertex_count,
		short *vertices,
		rgb_color& color);
	
	void end_polygons();

	void DrawCachedPolygons();
	
	void begin_lines();

	void draw_line(
		short *vertices,
		rgb_color& color,
		short pen_size);

	void end_lines();	// Needed for flushing cached lines
	
	void DrawCachedLines();
	
	void draw_thing(
		world_point2d& center,
		rgb_color& color,
		short shape,
		short radius);
	
	void draw_player(
		world_point2d& center,
		angle facing,
		rgb_color& color,
		short shrink,
		short front,
		short rear,
		short rear_theta);
	
	// Text justification: 0=left, 1=center
	void draw_text(
		world_point2d& location,
		rgb_color& colorr,
		char *text,
		FontSpecifier& FontData,
		short justify);
	
	void set_path_drawing(rgb_color& color);
	void draw_path(
		short step,	// 0: first point
		world_point2d &location);
	
	void finish_path();
	
	// Cached polygons and their color
	vector<unsigned short> PolygonCache;
	rgb_color SavedColor;

	// Cached polygon lines and their width
	vector<world_point2d> LineCache;
	short SavedPenSize;
	
	// Cached lines For drawing monster paths
	vector<world_point2d> PathPoints;

public:
};

#endif
