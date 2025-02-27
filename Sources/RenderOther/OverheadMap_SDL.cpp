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
 *  OverheadMap_SDL.cpp -- Subclass of OverheadMapClass for rendering with SDL
 */

#include "cseries.hpp"

#include "OverheadMap_SDL.hpp"
#include "map.hpp"
#include "screen_drawing.hpp"


// From screen_sdl.cpp
extern SDL_Surface *draw_surface;


/*
 *  Draw polygon
 */

void OverheadMap_SDL_Class::draw_polygon(short vertex_count, short *vertices, rgb_color& color)
{
	// Reallocate vertex array if necessary
	static world_point2d *vertex_array = NULL;
	static int max_vertices = 0;
	if (vertex_count > max_vertices) {
		delete[] vertex_array;
		vertex_array = new world_point2d[vertex_count];
		max_vertices = vertex_count;
	}

	// Copy vertex array
	for (int i=0; i<vertex_count; i++)
		vertex_array[i] = GetVertex(vertices[i]);

	// Get color
	uint32 pixel = SDL_MapRGB(draw_surface->format, color.red >> 8, color.green >> 8, color.blue >> 8);

	// Draw polygon
	::draw_polygon(draw_surface, vertex_array, vertex_count, pixel);
}


/*
 *  Draw line
 */

void OverheadMap_SDL_Class::draw_line(short *vertices, rgb_color &color, short pen_size)
{
	// Get start and end points
	world_point2d *v1 = &GetVertex(vertices[0]);
	world_point2d *v2 = &GetVertex(vertices[1]);

	// Get color
	uint32 pixel = SDL_MapRGB(draw_surface->format, color.red >> 8, color.green >> 8, color.blue >> 8);

	// Draw line
	::draw_line(draw_surface, v1, v2, pixel, pen_size);
}


/*
 *  Draw "thing" (object)
 */

void OverheadMap_SDL_Class::draw_thing(world_point2d &center, rgb_color &color, short shape, short radius)
{
	// Adjust object display so that objects get properly centered
	int raddown = int(0.75 * radius);
	int radup = int(1.5 * radius);
	SDL_Rect r = {center.x - raddown, center.y - raddown, radup, radup};

	// Get color
	uint32 pixel = SDL_MapRGB(draw_surface->format, color.red >> 8, color.green >> 8, color.blue >> 8);

	// Draw thing
	switch (shape) {
		case _rectangle_thing:
			SDL_FillRect(draw_surface, &r, pixel);
			break;
		case _circle_thing: {
			world_point2d circle[8] = {
				{int16(-0.75 * radius + center.x), int16(-0.30 * radius + center.y)},
				{int16(-0.75 * radius + center.x), int16(+0.30 * radius + center.y)},
				{int16(-0.30 * radius + center.x), int16(+0.75 * radius + center.y)},
				{int16(+0.30 * radius + center.x), int16(+0.75 * radius + center.y)},
				{int16(+0.75 * radius + center.x), int16(+0.30 * radius + center.y)},
				{int16(+0.75 * radius + center.x), int16(-0.30 * radius + center.y)},
				{int16(+0.30 * radius + center.x), int16(-0.75 * radius + center.y)},
				{int16(-0.30 * radius + center.x), int16(-0.75 * radius + center.y)}
			};
			for (int i=0; i<7; i++)
				::draw_line(draw_surface, circle + i, circle + i + 1, pixel, 2);
			::draw_line(draw_surface, circle + 7, circle, pixel, 2);
			break;
		}
	}
}


/*
 *  Draw player
 */

void OverheadMap_SDL_Class::draw_player(world_point2d &center, angle facing, rgb_color &color, short shrink, short front, short rear, short rear_theta)
{
	// Construct triangle vertices
	world_point2d triangle[3];
	triangle[0] = triangle[1] = triangle[2] = center;
	translate_point2d(triangle + 0, front >> shrink, facing);
	translate_point2d(triangle + 1, rear >> shrink, normalize_angle(facing + rear_theta));
	translate_point2d(triangle + 2, rear >> shrink, normalize_angle(facing - rear_theta));

	// Get color
	uint32 pixel = SDL_MapRGB(draw_surface->format, color.red >> 8, color.green >> 8, color.blue >> 8);

	// Draw triangle
	::draw_polygon(draw_surface, triangle, 3, pixel);
}


/*
 *  Draw text
 */

void OverheadMap_SDL_Class::draw_text(world_point2d &location, rgb_color &color, char *text, FontSpecifier& FontData, short justify)
{
	// Load font
	const font_info *font = FontData.Info;
	short style = FontData.Style;

	// Find left-side location
	int xpos = location.x;
	if (justify == _justify_center)
		xpos -= text_width(text, font, style) / 2;
	
	// Get color
	uint32 pixel = SDL_MapRGB(draw_surface->format, color.red >> 8, color.green >> 8, color.blue >> 8);

	// Draw text
	::draw_text(draw_surface, text, xpos, location.y, pixel, font, style);
}


/*
 *  Draw path
 */

void OverheadMap_SDL_Class::set_path_drawing(rgb_color &color)
{
	path_pixel = SDL_MapRGB(draw_surface->format, color.red >> 8, color.green >> 8, color.blue >> 8);
}

void OverheadMap_SDL_Class::draw_path(short step, world_point2d &location)
{
	if (step)
		::draw_line(draw_surface, &path_point, &location, path_pixel, 1);
	path_point = location;
}
