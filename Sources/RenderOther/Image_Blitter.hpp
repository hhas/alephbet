#ifndef _IMAGE_BLITTER_
#define _IMAGE_BLITTER_

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
 *  Implements images for 2D UI
 */

#include "ImageLoader.hpp"
#include "cseries.hpp"

#include <set>
#include <vector>

struct Image_Rect {
    float x = 0, y = 0, w = 0, h = 0;

    Image_Rect() = default;

    explicit constexpr Image_Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

    /*implicit*/ constexpr Image_Rect(SDL_Rect r) : x(r.x), y(r.y), w(r.w), h(r.h) {}
};

class Image_Blitter {
  public:

    Image_Blitter();

    bool Load(const ImageDescriptor& image);
    bool Load(int picture_resource);
    bool Load(const SDL_Surface& s);
    bool Load(const SDL_Surface& s, const SDL_Rect& src);
    virtual void Unload();
    bool Loaded();

    void Rescale(float width, float height);
    float Width();
    float Height();
    int UnscaledWidth();
    int UnscaledHeight();

    virtual void Draw(SDL_Surface* dst_surface, const Image_Rect& dst) { Draw(dst_surface, dst, crop_rect); }

    virtual void Draw(SDL_Surface* dst_surface, const Image_Rect& dst, const Image_Rect& src);

    virtual ~Image_Blitter();

    // tint the output image -- (1, 1, 1, 1) is untinted
    float tint_color_r, tint_color_g, tint_color_b, tint_color_a;

    // rotate the output image about the center of destination rect
    // (in degrees clockwise)
    float rotation;

    // set default cropping rectangle
    Image_Rect crop_rect;

  protected:

    SDL_Surface* m_surface;
    SDL_Surface* m_disp_surface;
    SDL_Surface* m_scaled_surface;
    Image_Rect m_src, m_scaled_src;
};

#endif
