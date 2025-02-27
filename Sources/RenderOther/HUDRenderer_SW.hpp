#ifndef _HUD_RENDERER_SW_H_
#define _HUD_RENDERER_SW_H_

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
 *  HUD rendering using graphics functions from screen_drawing
 */

#include "HUDRenderer.hpp"

class HUD_SW_Class : public HUD_Class
{
public:
	HUD_SW_Class() {}
	~HUD_SW_Class() {}

protected:
	void update_motion_sensor(short time_elapsed);
	void render_motion_sensor(short time_elapsed);
	void draw_or_erase_unclipped_shape(short x, short y, shape_descriptor shape, bool draw);
	void draw_entity_blip(point2d *location, shape_descriptor shape);

	void DrawShape(shape_descriptor shape, screen_rectangle *dest, screen_rectangle *src);
	void DrawShapeAtXY(shape_descriptor shape, short x, short y, bool transparency = false);
	void DrawText(const char *text, screen_rectangle *dest, short flags, short font_id, short text_color);
	void FillRect(screen_rectangle *r, short color_index);
	void FrameRect(screen_rectangle *r, short color_index);

	void DrawTexture(shape_descriptor shape, short texture_type, short x, short y, int size);

	void SetClipPlane(int x, int y, int c_x, int c_y, int radius) {}
	void DisableClipPlane(void) {}

	int TextWidth(const char* text, short font_id) override;
};

#endif
