#ifndef MODEL_RENDERER
#define MODEL_RENDERER

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
	Renders 3D-model objects;
	it can render either with or without a Z-buffer;
	without a Z-buffer, it depth-sorts polygons
*/

#include "csmacros.hpp"  // need obj_clear
#include "Model3D.hpp"

struct ModelRenderShader
{
	unsigned int Flags;
	
	// This callback takes only one argument, whatever data is appropriate
	// (the pointer after it)
	void (* TextureCallback)(void *);
	void *TextureCallbackData;
	
	// This callback takes not only whatever data is appropriate
	// (the pointer after it),
	// but also the number of vertices, the normals, the vertex positions,
	// and the vertex-lighting color values, in that order.
	// The normals and the positions are in model coordinates.
	void (* LightingCallback)(void *, size_t, GLfloat *, GLfloat *, GLfloat *);
	void *LightingCallbackData;
	
	ModelRenderShader() {obj_clear(*this);}
};

struct IndexedCentroidDepth
{
	// sort from farthest to nearest
	bool operator<(const IndexedCentroidDepth& other) const {
		return depth > other.depth;
	}

	GLfloat depth;
	unsigned short index;
};

class ModelRenderer
{
	// Kept here to avoid unnecessary re-allocation
	vector<IndexedCentroidDepth> IndexedCentroidDepths;
	vector<GLushort> SortedVertIndices;
	vector<GLfloat> ExtLightColors;
	
	void SetupRenderPass(Model3D& Model, ModelRenderShader& Shader);
	
public:
	
	// Needed for depth-sorting the model triangles by centroid;
	// it is in model coordinates.
	GLfloat ViewDirection[3];
	
	// External lighting now done with a shader callback
		
	// Render flags:
	enum {
		Textured	= 0x0001,
		Colored		= 0x0002,
		ExtLight	= 0x0004,	// Whether to to use the external-light colors
		EL_SemiTpt	= 0x0008	// Whether the external-light colors include semitransparency
	};
	
	// Does the actual rendering; args:
	// A 3D model (of course!)
	// Array of shaders to be used for multipass rendering
	// How many shaders in that array to use
	// How many shaders are assumed to be separably renderable when a Z-buffer is present;
	//   these are assumed to be all-or-nothing, and are always the first shaders.
	//   Semitransparent shaders are nonseparable.
	// Whether a Z-buffer is present; without it, no shaders are rendered separately.
	void Render(Model3D& Model, ModelRenderShader *Shaders, int NumShaders,
		int NumSeparableShaders, bool Use_Z_Buffer);
	
	// In case one wants to start over again with these persistent arrays
	void Clear();
};


#endif
