#ifndef _OGL_SUBST_TEXTURE_DEF_
#define _OGL_SUBST_TEXTURE_DEF_

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
 *  This contains the definitions of all the OpenGL substitute textures
 *  for the walls and the sprites
 */

#include "OGL_Texture_Def.hpp"


#ifdef HAVE_OPENGL


// Options for wall textures and sprites
struct OGL_TextureOptions: public OGL_TextureOptionsBase
{
	bool VoidVisible;		// Can see the void through texture if semitransparent
	short TileRatioExp;		// Tile replacement walls to 2^n x 2^n WU
	
	// Parameters for mapping substitute sprites (inhabitants, weapons in hand)
	
	OGL_TextureOptions():
		VoidVisible(false),TileRatioExp{0} {}
};


// Get the texture options that are currently set
OGL_TextureOptions *OGL_GetTextureOptions(short Collection, short CLUT, short Bitmap);

// for managing the texture loading and unloading;
int OGL_CountTextures(short Collection);
void OGL_LoadTextures(short Collection);
void OGL_UnloadTextures(short Collection);

class InfoTree;
void parse_mml_opengl_texture(const InfoTree& root);
void reset_mml_opengl_texture();
void parse_mml_opengl_txtr_clear(const InfoTree& root);

#endif // def HAVE_OPENGL

#endif
