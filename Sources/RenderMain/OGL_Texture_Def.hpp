#ifndef _OGL_TEXTURE_DEF_
#define _OGL_TEXTURE_DEF_

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
 *  This contains the "base" definitions of the OpenGL textures, which are used
 *  both for wall/sprite substitutions and for model skins.
 */


#include <vector>

#include "ImageLoader.hpp"
#include "shape_descriptors.hpp"

#ifdef HAVE_OPENGL


/*
    Since Apple OpenGL currently does not support indexed-color images in direct-color
    rendering, it's necessary to keep track of all possible images separately, and this means
    not only all possible color tables, but also infravision and silhouette images.
    OpenGL 1.2 will change all of that, however :-)
*/

enum {
    // The bitmap sets for the different color tables do not need to be listed
    INFRAVISION_BITMAP_SET = MAXIMUM_CLUTS_PER_COLLECTION,
    SILHOUETTE_BITMAP_SET,
    INFRAVISION_BITMAP_CLUTSPECIFIC,
    SILHOUETTE_BITMAP_CLUTSPECIFIC = 2 * MAXIMUM_CLUTS_PER_COLLECTION + 2,
    NUMBER_OF_OPENGL_BITMAP_SETS   = 3 * MAXIMUM_CLUTS_PER_COLLECTION + 2
};

enum {
    // The definitions for clut variants (used in texture and model MML)
    ALL_CLUT_VARIANTS   = -1,
    CLUT_VARIANT_NORMAL = 0,
    CLUT_VARIANT_INFRAVISION,
    CLUT_VARIANT_SILHOUETTE,
    NUMBER_OF_CLUT_VARIANTS
};

// Check for infravision or silhouette special CLUTs
static inline bool IsInfravisionTable(short CLUT) {
    return (CLUT == INFRAVISION_BITMAP_SET
            || (CLUT >= INFRAVISION_BITMAP_CLUTSPECIFIC
                && CLUT < INFRAVISION_BITMAP_CLUTSPECIFIC + MAXIMUM_CLUTS_PER_COLLECTION));
}

static inline bool IsSilhouetteTable(short CLUT) {
    return (CLUT == SILHOUETTE_BITMAP_SET
            || (CLUT >= SILHOUETTE_BITMAP_CLUTSPECIFIC
                && CLUT < SILHOUETTE_BITMAP_CLUTSPECIFIC + MAXIMUM_CLUTS_PER_COLLECTION));
}

// If the color-table value has this value, it means all color tables:
const int ALL_CLUTS = -1;

// Here are the texture-opacity types.
// Opacity is the value of the alpha channel, sometimes called transparency
enum {
    OGL_OpacType_Crisp, // The default: crisp edges, complete opacity
    OGL_OpacType_Flat,  // Fuzzy edges, but with flat opacity
    OGL_OpacType_Avg,   // Fuzzy edges, and opacity = average(color channel values)
    OGL_OpacType_Max,   // Fuzzy edges, and opacity = max(color channel values)
    OGL_NUMBER_OF_OPACITY_TYPES
};

// Here are the texture-blend types
enum {
    OGL_BlendType_Crossfade, // The default: crossfade from background to texture value
    OGL_BlendType_Add,       // Add texture value to background
    OGL_BlendType_Crossfade_Premult,
    OGL_BlendType_Add_Premult,
    OGL_NUMBER_OF_BLEND_TYPES,
    OGL_FIRST_PREMULT_ALPHA = OGL_BlendType_Crossfade_Premult
};

// Shared options for wall/sprite textures and for skins
struct OGL_TextureOptionsBase {
    short OpacityType;  // Which type of opacity to use?
    float OpacityScale; // How much to scale the opacity
    float OpacityShift; // How much to shift the opacity
    bool Substitution;  // Is this a substitute texture?

    // Names of files to load; these will be extended ones with directory specifications
    // <dirname>/<dirname>/<filename>
    FileSpecifier NormalColors, NormalMask, GlowColors, GlowMask, OffsetMap;

    // the image is premultiplied
    bool NormalIsPremultiplied, GlowIsPremultiplied;

    // hints passed into loadfromfile, in case file dimensions are POT
    short actual_height, actual_width;

    // what kind of texture this is, for texture quality purposes
    short Type;

    // Normal and glow-mapped images
    ImageDescriptor NormalImg, GlowImg, OffsetImg;

    // Normal and glow blending
    short NormalBlend, GlowBlend;

    // Bloom controls, for normal and glow images
    float BloomScale;
    float BloomShift;
    float GlowBloomScale;
    float GlowBloomShift;
    float LandscapeBloom;

    // Glow modulated using max of normal lighting intensity and this value
    float MinGlowIntensity;

    // For convenience
    void Load();
    void Unload();

    virtual int GetMaxSize();

    OGL_TextureOptionsBase()
        : OpacityType(OGL_OpacType_Crisp), OpacityScale(1), OpacityShift(0), NormalBlend(OGL_BlendType_Crossfade),
          GlowBlend(OGL_BlendType_Crossfade), Substitution(false), NormalIsPremultiplied(false),
          GlowIsPremultiplied(false), actual_height(0), actual_width(0), Type(-1), BloomScale(0), BloomShift(0),
          GlowBloomScale(1), GlowBloomShift(0), LandscapeBloom(0.5), MinGlowIntensity(1) {}
};

#endif

#endif
