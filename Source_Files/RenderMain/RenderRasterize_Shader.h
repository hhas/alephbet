#ifndef _RENDERRASTERIZE_SHADER__H
#define _RENDERRASTERIZE_SHADER__H

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

#include "cseries.h"
#include "map.h"
#include "RenderRasterize.h"
#include "OGL_FBO.h"
#include "OGL_Textures.h"
#include "Rasterizer_Shader.h"

#include <memory>

#ifdef HAVE_OPENGL

class Blur;
class RenderRasterize_Shader : public RenderRasterizerClass {

	std::unique_ptr<Blur> blur;
	Rasterizer_Shader_Class *RasPtr;
	
	int objectCount;
	world_distance objectY;
	float weaponFlare;
	float selfLuminosity;
	
	long_vector2d leftmost_clip, rightmost_clip;

protected:
	virtual void render_node(sorted_node_data *node, bool SeeThruLiquids, RenderStep renderStep);	
	virtual void store_endpoint(endpoint_data *endpoint, long_vector2d& p);

	virtual void render_node_floor_or_ceiling(
		  clipping_window_data *window, polygon_data *polygon, horizontal_surface_data *surface,
		  bool void_present, bool ceil, RenderStep renderStep);
	virtual void render_node_side(
		  clipping_window_data *window, vertical_surface_data *surface,
		  bool void_present, RenderStep renderStep);

	virtual void render_node_object(render_object_data *object, bool other_side_of_media, RenderStep renderStep);
	
	virtual void clip_to_window(clipping_window_data *win);
	virtual void _render_node_object_helper(render_object_data *object, RenderStep renderStep);

    void render_viewer_sprite_layer(RenderStep renderStep);
    void render_viewer_sprite(rectangle_definition& RenderRectangle, RenderStep renderStep);
	
public:

	RenderRasterize_Shader();
	~RenderRasterize_Shader();

	virtual void setupGL(Rasterizer_Shader_Class& Rasterizer);

	virtual void render_tree(void);
        bool renders_viewer_sprites_in_tree() { return true; }

	std::unique_ptr<TextureManager> setupWallTexture(const shape_descriptor& Texture, short transferMode, float pulsate, float wobble, float intensity, float offset, RenderStep renderStep);
	std::unique_ptr<TextureManager> setupSpriteTexture(const rectangle_definition& rect, short type, float offset, RenderStep renderStep);
};

#endif
#endif
