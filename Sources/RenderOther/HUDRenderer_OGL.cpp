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

#include "HUDRenderer_OGL.hpp"

#ifdef HAVE_OPENGL

#include "FontHandler.hpp"

#include "game_window.hpp"
#include "images.hpp"
#include "render.hpp"
#include "scottish_textures.hpp"
#include "screen_definitions.hpp"

#include "OGL_Blitter.hpp"
#include "OGL_Setup.hpp"
#include "OGL_Textures.hpp"
#include "Shape_Blitter.hpp"

#ifdef HAVE_OPENGL
#include "OGL_Headers.hpp"
#include "OGL_Render.hpp"
#endif

#include <math.h>

extern bool MotionSensorActive;


// Rendering object
static HUD_OGL_Class HUD_OGL;


static OGL_Blitter HUD_Blitter;         // HUD backdrop storage
static bool hud_pict_not_found = false; // HUD backdrop picture not found, don't try again to load it

extern int LuaTexturePaletteSize();

void OGL_DrawHUD(Rect& dest, short time_elapsed) {
    // Load static HUD picture if necessary
    if (!HUD_Blitter.Loaded() && !hud_pict_not_found) {
        if (!HUD_Blitter.Load(INTERFACE_PANEL_BASE)) {
            hud_pict_not_found = true;
        }
    }

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_FOG);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Draw static HUD picture
    if (HUD_Blitter.Loaded() && !LuaTexturePaletteSize()) {
        SDL_Rect hud_dest      = {dest.left, dest.top, dest.right - dest.left, dest.bottom - dest.top};
        HUD_Blitter.nearFilter = TxtrTypeInfoList[OGL_Txtr_HUD].NearFilter;
        HUD_Blitter.Draw(hud_dest);
    } else {
        glColor3ub(0, 0, 0);
        OGL_RenderRect(dest.left, dest.top, dest.right - dest.left, dest.bottom - dest.top);
    }

    GLdouble x_scale = (dest.right - dest.left) / 640.0;
    GLdouble y_scale = (dest.bottom - dest.top) / 160.0;
    glMatrixMode(GL_MODELVIEW);
    glTranslated(dest.left, dest.top - (320.0 * y_scale), 0.0);
    glScaled(x_scale, y_scale, 1.0);

    // Add dynamic elements (redraw everything)
    mark_weapon_display_as_dirty();
    mark_ammo_display_as_dirty();
    mark_shield_display_as_dirty();
    mark_oxygen_display_as_dirty();
    mark_player_inventory_as_dirty(current_player_index, NONE);
    HUD_OGL.update_everything(time_elapsed);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopAttrib();
}

/*
 *  Update motion sensor
 */

void HUD_OGL_Class::update_motion_sensor(short time_elapsed) {
    if (!(GET_GAME_OPTIONS() & _motion_sensor_does_not_work) && MotionSensorActive) {
        render_motion_sensor(time_elapsed);
    }
}

/*
 *  Draw shapes
 */

void HUD_OGL_Class::DrawShape(shape_descriptor shape, screen_rectangle* dest, screen_rectangle* src) {
    // Set up texture
    TextureManager TMgr;
    TMgr.ShapeDesc = shape;
    get_shape_bitmap_and_shading_table(shape, &TMgr.Texture, &TMgr.ShadingTables, _shading_normal);
    TMgr.IsShadeless  = true;
    TMgr.TransferMode = _shadeless_transfer;
    TMgr.TextureType  = OGL_Txtr_HUD;
    if (!TMgr.Setup())
        return;

    // Get dimensions
    int orig_width = TMgr.Texture->width, orig_height = TMgr.Texture->height;
    int x = dest->left, y = dest->top;
    int width = dest->right - dest->left, height = dest->bottom - dest->top;
    GLdouble U_Scale  = TMgr.U_Scale * (src->right - src->left) / orig_width;
    GLdouble V_Scale  = TMgr.V_Scale * (src->bottom - src->top) / orig_height;
    GLdouble U_Offset = TMgr.U_Offset + TMgr.U_Scale * src->left / orig_width;
    GLdouble V_Offset = TMgr.V_Offset + TMgr.V_Scale * src->top / orig_height;

    // Draw shape
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    TMgr.SetupTextureMatrix();
    TMgr.RenderNormal();

    OGL_RenderTexturedRect(x, y, width, height, U_Offset, V_Offset, U_Offset + U_Scale, V_Offset + V_Scale);

    TMgr.RestoreTextureMatrix();
}

void HUD_OGL_Class::DrawShapeAtXY(shape_descriptor shape, short x, short y, bool transparency) {
    // Set up texture
    TextureManager TMgr;
    TMgr.ShapeDesc = shape;
    get_shape_bitmap_and_shading_table(shape, &TMgr.Texture, &TMgr.ShadingTables, _shading_normal);
    TMgr.IsShadeless  = true;
    TMgr.TransferMode = _shadeless_transfer;
    TMgr.TextureType  = OGL_Txtr_HUD;
    if (!TMgr.Setup())
        return;

    // Get dimensions
    int width = TMgr.Texture->width, height = TMgr.Texture->height;
    GLdouble U_Scale  = TMgr.U_Scale;
    GLdouble V_Scale  = TMgr.V_Scale;
    GLdouble U_Offset = TMgr.U_Offset;
    GLdouble V_Offset = TMgr.V_Offset;

    // Draw shape
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    if (transparency) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else
        glDisable(GL_BLEND);
    TMgr.SetupTextureMatrix();
    TMgr.RenderNormal();

    OGL_RenderTexturedRect(x, y, width, height, U_Offset, V_Offset, U_Offset + U_Scale, V_Offset + V_Scale);

    TMgr.RestoreTextureMatrix();
}

void HUD_OGL_Class::DrawTexture(shape_descriptor shape, short texture_type, short x, short y, int size) {
    Shape_Blitter b(GET_COLLECTION(GET_DESCRIPTOR_COLLECTION(shape)), GET_DESCRIPTOR_SHAPE(shape), texture_type,
                    GET_COLLECTION_CLUT(GET_DESCRIPTOR_COLLECTION(shape)));
    int w = b.Width();
    int h = b.Height();
    if (!w || !h)
        return;
    if (w >= h)
        b.Rescale(size, size * h / w);
    else
        b.Rescale(size * w / h, size);

    SDL_Rect r;
    r.w = b.Width();
    r.h = b.Height();
    r.x = x + (size - r.w) / 2;
    r.y = y + (size - r.h) / 2;
    b.OGL_Draw(r);
}

/*
 *  Draw text
 */

void HUD_OGL_Class::DrawText(const char* text, screen_rectangle* dest, short flags, short font_id, short text_color) {
    // Get color
    const rgb_color& c = get_interface_color(text_color);
    glColor3us(c.red, c.green, c.blue);

    // Get font information
    FontSpecifier& FontData = get_interface_font(font_id);

    FontData.NearFilter = TxtrTypeInfoList[OGL_Txtr_HUD].NearFilter;

    // Draw text
    FontData.OGL_DrawText(text, *dest, flags);
}

/*
 *  Fill rectangle
 */

void HUD_OGL_Class::FillRect(screen_rectangle* r, short color_index) {
    // Get color
    const rgb_color& c = get_interface_color(color_index);
    glColor3us(c.red, c.green, c.blue);

    // Draw rectangle
    OGL_RenderRect(r->left, r->top, r->right - r->left, r->bottom - r->top);
}

/*
 *  Frame rectangle
 */

void HUD_OGL_Class::FrameRect(screen_rectangle* r, short color_index) {
    // Get color
    const rgb_color& c = get_interface_color(color_index);
    glColor3us(c.red, c.green, c.blue);

    // Draw rectangle
    OGL_RenderFrame(r->left - 1, r->top - 1, r->right - r->left + 2, r->bottom - r->top + 2, 1);
}

/*
 *  Set clip plane for rendering a blip at (x, y) on the motion sensor.
 *  The plane gets attached tangential to the circle with the specified
 *  radius and center (this circle covers the entire motion sensor area).
 *  This should be a sufficient approximation to a circular clipping region
 *  for small blips.
 */

void HUD_OGL_Class::SetClipPlane(int x, int y, int c_x, int c_y, int radius) {
    GLdouble blip_dist = sqrt(static_cast<float>(x * x + y * y));
    if (blip_dist <= 2.0)
        return;
    GLdouble normal_x = x / blip_dist, normal_y = y / blip_dist;
    GLdouble tan_pt_x = c_x + normal_x * radius + 0.5, tan_pt_y = c_y + normal_y * radius + 0.5;

    glEnable(GL_CLIP_PLANE0);

    GLdouble eqn[4] = {-normal_x, -normal_y, 0, normal_x * tan_pt_x + normal_y * tan_pt_y};
    glClipPlane(GL_CLIP_PLANE0, eqn);
}

/*
 *  Disable clip plane
 */

void HUD_OGL_Class::DisableClipPlane(void) { glDisable(GL_CLIP_PLANE0); }

int HUD_OGL_Class::TextWidth(const char* text, short font_id) { return get_interface_font(font_id).TextWidth(text); }

#define MESSAGE_AREA_X_OFFSET -9
#define MESSAGE_AREA_Y_OFFSET -5

void HUD_OGL_Class::draw_message_area(short) {
    {
        screen_rectangle* player_name_rect = get_interface_rectangle(_player_name_rect);
        DrawShapeAtXY(BUILD_DESCRIPTOR(_collection_interface, _network_panel),
                      player_name_rect->left + MESSAGE_AREA_X_OFFSET, player_name_rect->top + MESSAGE_AREA_Y_OFFSET);
        draw_player_name();
    }
}


#endif // def HAVE_OPENGL
