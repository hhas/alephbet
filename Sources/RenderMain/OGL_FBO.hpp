#ifndef _OGL_FBO_
#define _OGL_FBO_

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
 *  Framebuffer Object utilities
 */

#include "cseries.hpp"

#ifdef HAVE_OPENGL

#include "OGL_Headers.hpp"
#include <vector>

class FBO {

  private:

    GLuint _fbo;
    GLuint _depthBuffer;
    GLuint _fboTarget;
    static std::vector<FBO*> active_chain;

  public:

    GLuint _w;
    GLuint _h;
    bool _srgb;
    GLuint texID;

    FBO(GLuint w, GLuint h, bool srgb = false);
    ~FBO();

    void activate(bool clear = false, GLuint fboTarget = GL_FRAMEBUFFER_EXT);
    void deactivate();

    void draw();
    void prepare_drawing_mode(bool blend = false);
    void reset_drawing_mode();
    void draw_full(bool blend = false);

    static FBO* active_fbo();
};

class FBOSwapper {
  private:

    FBO first;
    FBO second;
    bool draw_to_first;
    bool active;
    bool clear_on_activate;

  public:

    FBOSwapper(GLuint w, GLuint h, bool srgb = false)
        : first(w, h, srgb), second(w, h, srgb), draw_to_first(true), active(false), clear_on_activate(true) {}

    void activate();
    void deactivate();
    void swap();

    void draw(bool blend = false);
    void filter(bool blend = false);

    void copy(FBO& other, bool srgb);

    void copy(FBO& other) { copy(other, first._srgb); }

    void blend(FBO& other, bool srgb);

    void blend(FBO& other) { blend(other, first._srgb); }

    void blend_multisample(FBO& other);

    FBO& current_contents() { return draw_to_first ? second : first; }
};


#endif // def HAVE_OPENGL

#endif
