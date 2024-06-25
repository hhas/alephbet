#ifndef _RASTERIZER_SHADER__H
#define _RASTERIZER_SHADER__H

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
#include "Rasterizer_OGL.h"
#include <memory>

#ifdef HAVE_OPENGL

class FBOSwapper;
class Rasterizer_Shader_Class : public Rasterizer_OGL_Class {
	friend class RenderRasterize_Shader;
	
protected:
	std::unique_ptr<FBOSwapper> swapper;
	bool smear_the_void;
	short view_width;
	short view_height;

public:

	Rasterizer_Shader_Class();
	~Rasterizer_Shader_Class();

	virtual void SetView(view_data& View);
	virtual void setupGL();
	virtual void Begin();
	virtual void End();

};

#endif
#endif