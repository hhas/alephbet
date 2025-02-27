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

#include "OGL_Faders.hpp"
#include "OGL_Headers.hpp"
#include "OGL_Render.hpp"
#include "OGL_Setup.hpp"
#include "Random.hpp"
#include "cseries.hpp"
#include "fades.hpp"
#include "render.hpp"

#ifdef HAVE_OPENGL

// The randomizer for the flat-static color
static GM_Random FlatStaticRandom;

// Alternative: partially-transparent instead of the logic-op effect
static bool UseFlatStatic;
static uint16 FlatStaticColor[4];

// Fader stuff
bool OGL_FaderActive() {
    if (!OGL_IsActive())
        return false;

    OGL_ConfigureData& ConfigureData = Get_OGL_ConfigureData();
    return TEST_FLAG(ConfigureData.Flags, OGL_Flag_Fader);
}

static OGL_Fader FaderQueue[NUMBER_OF_FADER_QUEUE_ENTRIES];

OGL_Fader* GetOGL_FaderQueueEntry(int Index) {
    assert(Index >= 0 && Index < NUMBER_OF_FADER_QUEUE_ENTRIES);

    return FaderQueue + Index;
}

// Multiply a color by its alpha channel;
// make the results separate so as not to destroy the original values
inline void MultAlpha(GLfloat* InColor, GLfloat* OutColor) {
    for (int c = 0; c < 3; c++) OutColor[c] = InColor[c] * InColor[3];
    OutColor[3] = InColor[3];
}

// Take the complement of a color;
// make the results separate so as not to destroy the original values
inline void ComplementColor(GLfloat* InColor, GLfloat* OutColor) {
    for (int c = 0; c < 3; c++) OutColor[c] = 1 - InColor[c];
    OutColor[3] = InColor[3];
}

bool OGL_DoFades(float Left, float Top, float Right, float Bottom) {
    if (!OGL_FaderActive())
        return false;

    // Set up the vertices
    GLfloat Vertices[4][2];
    Vertices[0][0] = Left;
    Vertices[0][1] = Top;
    Vertices[1][0] = Right;
    Vertices[1][1] = Top;
    Vertices[2][0] = Right;
    Vertices[2][1] = Bottom;
    Vertices[3][0] = Left;
    Vertices[3][1] = Bottom;
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, Vertices[0]);

    // Do real blending
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    // Modified color:
    GLfloat BlendColor[4];

    for (int f = 0; f < NUMBER_OF_FADER_QUEUE_ENTRIES; f++) {
        OGL_Fader& Fader = FaderQueue[f];

        switch (Fader.Type) {
            case NONE:
                break;

            case _tint_fader_type:
                // The simplest kind: fade to the fader color.
                glColor4fv(Fader.Color);
                glDrawArrays(GL_POLYGON, 0, 4);
                break;

            case _randomize_fader_type:
                UseFlatStatic = TEST_FLAG(Get_OGL_ConfigureData().Flags, OGL_Flag_FlatStatic);
                if (UseFlatStatic) {
                    for (int c = 0; c < 3; c++) FlatStaticColor[c] = FlatStaticRandom.KISS() + FlatStaticRandom.LFIB4();
                    FlatStaticColor[3] = PIN(int(65'535 * Fader.Color[3] + 0.5), 0, 65'535);
                    glDisable(GL_ALPHA_TEST);
                    glEnable(GL_BLEND);
                    glColor4usv(FlatStaticColor);
                    glDrawArrays(GL_POLYGON, 0, 4);
                } else {
                    // Do random flipping of the lower bits of color values;
                    // the stronger the opacity (alpha), the more bits to flip.
                    glDisable(GL_BLEND);
                    MultAlpha(Fader.Color, BlendColor);
                    glColor3fv(BlendColor);
                    glEnable(GL_COLOR_LOGIC_OP);
                    glLogicOp(GL_XOR);
                    glDrawArrays(GL_POLYGON, 0, 4);
                    // Revert to defaults
                    glDisable(GL_COLOR_LOGIC_OP);
                    glEnable(GL_BLEND);
                }
                break;

            case _negate_fader_type:
                // This is only a partial approximation of the negation effect
                // Neither glBlendColorEXT nor glBlendEquationEXT is currently supported
                // in ATI Rage 128 AppleGL, which makes my life more difficult :-P
                MultAlpha(Fader.Color, BlendColor);
                glColor4fv(BlendColor);
                glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
                glDrawArrays(GL_POLYGON, 0, 4);
                // Revert to defaults
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;

            case _dodge_fader_type:
                ComplementColor(Fader.Color, BlendColor);
                MultAlpha(BlendColor, BlendColor);
                glColor4fv(BlendColor);
                glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
                glDrawArrays(GL_POLYGON, 0, 4);
                glBlendFunc(GL_DST_COLOR, GL_ONE);
                glDrawArrays(GL_POLYGON, 0, 4);
                // Revert to defaults
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;

            case _burn_fader_type:
                // Attempted to get that reversed-color effect at maximum intensity,
                // with it being only near maximum intensity
                // (MultAlpha + GL_SRC_ALPHA means opacity^2).
                MultAlpha(Fader.Color, BlendColor);
                glColor4fv(BlendColor);
                glBlendFunc(GL_DST_COLOR, GL_ONE);
                glDrawArrays(GL_POLYGON, 0, 4);
                ComplementColor(Fader.Color, BlendColor);
                MultAlpha(BlendColor, BlendColor);
                glColor4fv(BlendColor);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                glDrawArrays(GL_POLYGON, 0, 4);
                // Revert to defaults
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;

            case _soft_tint_fader_type:
                // Fade to the color multiplied by the fader color,
                // as if the scene was illuminated by light with that fader color.
                MultAlpha(Fader.Color, BlendColor);
                glColor4fv(BlendColor);
                glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
                glDrawArrays(GL_POLYGON, 0, 4);
                // Revert to defaults
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;
        }
    }
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    return true;
}

#endif // def HAVE_OPENGL
