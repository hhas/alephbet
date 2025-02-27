#ifndef _OGL_LOADSCREEN_
#define _OGL_LOADSCREEN_

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

#include "ImageLoader.hpp"
#include "OGL_Blitter.hpp"
#include "OGL_Headers.hpp"
#include "cseries.hpp"

#ifdef HAVE_OPENGL
class OGL_LoadScreen {
  public:

    static OGL_LoadScreen* instance();

    bool Start();
    void Stop();
    void Progress(const int percent);

    void Set(std::string Path, bool Stretch, bool Scale);
    void Set(std::string Path, bool Stretch, bool Scale, short X, short Y, short W, short H);
    void Clear();

    bool Use() { return use; }

    rgb_color* Colors() { return colors; }

  private:

    OGL_LoadScreen() : x(0), y(0), w(0), h(0), use(false), useProgress(false), percent(0) {}

    ~OGL_LoadScreen();

    std::string path;
    ImageDescriptor image;
    short x, y, w, h;

    OGL_Blitter blitter;
    SDL_Rect m_dst;
    double x_offset, y_offset, x_scale, y_scale;

    bool scale;
    bool stretch;

    bool use;
    bool useProgress;

    rgb_color colors[2];

    short percent;


    GLuint texture_ref;
};
#endif

#endif
