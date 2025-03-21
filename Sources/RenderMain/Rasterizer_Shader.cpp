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

#include "OGL_Headers.hpp"

#include <iostream>

#include "Rasterizer_Shader.hpp"

#include "AnimatedTextures.hpp"
#include "ChaseCam.hpp"
#include "OGL_FBO.hpp"
#include "OGL_Faders.hpp"
#include "OGL_Shader.hpp"
#include "OGL_Textures.hpp"
#include "fades.hpp"
#include "lightsource.hpp"
#include "media.hpp"
#include "player.hpp"
#include "preferences.hpp"
#include "screen.hpp"
#include "weapons.hpp"

#ifdef HAVE_OPENGL

#define MAXIMUM_VERTICES_PER_WORLD_POLYGON (MAXIMUM_VERTICES_PER_POLYGON + 4)

const float FixedAngleToDegrees = 360.0 / (float(FIXED_ONE) * float(FULL_CIRCLE));

const GLdouble kViewBaseMatrix[16] = {0, 0, -1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};

const GLdouble kViewBaseMatrixInverse[16] = {0, 1, 0, 0, 0, 0, 1, 0, -1, 0, 0, 0, 0, 0, 0, 1};

Rasterizer_Shader_Class::Rasterizer_Shader_Class()  = default;
Rasterizer_Shader_Class::~Rasterizer_Shader_Class() = default;

void Rasterizer_Shader_Class::SetView(view_data& view) {
    OGL_SetView(view);

    if (view.screen_width != view_width || view.screen_height != view_height) {
        view_width  = view.screen_width;
        view_height = view.screen_height;
        swapper.reset();
        swapper.reset(new FBOSwapper(view_width * MainScreenPixelScale(), view_height * MainScreenPixelScale(), false));
    }

    float aspect  = view.screen_width / float(view.screen_height);
    float deg2rad = 8.0 * atan(1.0) / 360.0;
    float xtan, ytan;
    if (View_FOV_FixHorizontalNotVertical()) {
        xtan = tan(view.field_of_view * deg2rad / 2.0);
        ytan = xtan / aspect;
    } else {
        ytan = tan(view.field_of_view * deg2rad / 2.0) / 2.0;
        xtan = ytan * aspect;
    }

    // Adjust for view distortion during teleport effect
    ytan *= view.real_world_to_screen_y / double(view.world_to_screen_y);
    xtan *= view.real_world_to_screen_x / double(view.world_to_screen_x);

    double yaw   = view.virtual_yaw * FixedAngleToDegrees;
    double pitch = view.virtual_pitch * FixedAngleToDegrees;
    pitch        = (pitch > 180.0 ? pitch - 360.0 : pitch);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float nearVal = 64.0;
    float farVal  = 128.0 * 1024.0;
    float x       = xtan * nearVal;
    float y       = ytan * nearVal;
    float yoff    = view.mimic_sw_perspective ? tan(pitch * deg2rad) * nearVal : 0;
    glFrustum(-x, x, -y + yoff, y + yoff, nearVal, farVal);

    glMatrixMode(GL_MODELVIEW);

    // setup a rotation matrix for the landscape texture shader
    // this aligns the landscapes to the center of the screen for standard
    // pitch ranges, so that they don't need to be stretched

    glLoadIdentity();
    glTranslated(view.origin.x, view.origin.y, view.origin.z);
    glRotated(yaw, 0.0, 0.0, 1.0);
    glRotated(-pitch, 0.0, 1.0, 0.0);
    glMultMatrixd(kViewBaseMatrixInverse);

    GLfloat landscapeInverseMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, landscapeInverseMatrix);

    Shader* s;

    s = Shader::get(Shader::S_Landscape);
    s->enable();
    s->setMatrix4(Shader::U_LandscapeInverseMatrix, landscapeInverseMatrix);

    s = Shader::get(Shader::S_LandscapeBloom);
    s->enable();
    s->setMatrix4(Shader::U_LandscapeInverseMatrix, landscapeInverseMatrix);

    Shader::disable();

    // setup the normal view matrix

    glLoadMatrixd(kViewBaseMatrix);
    if (!view.mimic_sw_perspective)
        glRotated(pitch, 0.0, 1.0, 0.0);
    //	apperently 'roll' is not what i think it is
    //	rubicon sets it to some strange value
    //	double roll = view.roll * 360.0 / float(NUMBER_OF_ANGLES);
    //	glRotated(roll, 1.0, 0.0, 0.0);
    glRotated(-yaw, 0.0, 0.0, 1.0);
    glTranslated(-view.origin.x, -view.origin.y, -view.origin.z);
}

void Rasterizer_Shader_Class::setupGL() {
    view_width  = 0;
    view_height = 0;
    swapper.reset();

    smear_the_void                   = false;
    OGL_ConfigureData& ConfigureData = Get_OGL_ConfigureData();
    if (!TEST_FLAG(ConfigureData.Flags, OGL_Flag_VoidColor))
        smear_the_void = true;
}

void Rasterizer_Shader_Class::Begin() {
    Rasterizer_OGL_Class::Begin();
    swapper->activate();
    if (smear_the_void)
        swapper->current_contents().draw_full();
}

void Rasterizer_Shader_Class::End() {
    swapper->deactivate();
    swapper->swap();

    float gamma_adj = get_actual_gamma_adjust(graphics_preferences->screen_mode.gamma_level);
    if (gamma_adj < 0.99f || gamma_adj > 1.01f) {
        Shader* s = Shader::get(Shader::S_Gamma);
        s->enable();
        s->setFloat(Shader::U_GammaAdjust, gamma_adj);
    }
    swapper->draw();
    Shader::disable();

    SetForeground();
    glColor3f(0, 0, 0);
    OGL_RenderFrame(0, 0, view_width, view_height, 1);

    Rasterizer_OGL_Class::End();
}

#endif
