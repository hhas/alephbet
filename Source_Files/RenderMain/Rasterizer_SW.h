#ifndef _RASTERIZER_SOFTWARE_CLASS_
#define _RASTERIZER_SOFTWARE_CLASS_

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

#include "Rasterizer.h"


class Rasterizer_SW_Class: public RasterizerClass
{
public:

	// Pointers to stuff used in scottish_textures:
	view_data *view;
	// Calling this one "screen" for scottish_textures convenience:
	bitmap_definition *screen;

	// Sets the rasterizer's view data;
	// be sure to call it before doing any rendering
	void SetView(view_data& View) {view = &View;}
	
	// Rendering calls
	// These are defined in scottish_textures.c (too great a name to change)
	
	void texture_horizontal_polygon(polygon_definition& textured_polygon);
	
	void texture_vertical_polygon(polygon_definition& textured_polygon);
	
	void texture_rectangle(rectangle_definition& textured_rectangle);
};


#endif
