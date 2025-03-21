#ifndef _VEC3__H
#define _VEC3__H

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
#include <cfloat>
#include <cmath>

const GLfloat kThreshhold = FLT_MIN * 10.0;

struct vec4 {
    GLfloat _d[4];

    vec4() {}

    vec4(const GLfloat& x, const GLfloat& y, const GLfloat& z, const GLfloat& w) {
        _d[0] = x;
        _d[1] = y;
        _d[2] = z;
        _d[3] = w;
    }

    GLfloat* p() { return _d; }

    const GLfloat* p() const { return _d; }

    GLfloat operator[](unsigned i) const { return _d[i]; }

    GLfloat& operator[](unsigned i) { return _d[i]; }
};

struct vec3 : public vec4 {

    vec3() {}

    vec3(const GLfloat& x, const GLfloat& y, const GLfloat& z) : vec4(x, y, z, 0.0) {}

    vec3(const GLfloat* f) : vec4(f[0], f[1], f[2], 0.0) {}

    bool operator==(const vec3& v) const {
        return (std::abs(_d[0] - v[0]) + std::abs(_d[1] - v[1]) + std::abs(_d[2] - v[2])) < kThreshhold;
    }

    GLfloat dot(const vec3& v) const { return _d[0] * v[0] + _d[1] * v[1] + _d[2] * v[2]; }

    vec3 cross(const vec3& v) const {
        return vec3(_d[1] * v[2] - _d[2] * v[1], _d[2] * v[0] - _d[0] * v[2], _d[0] * v[1] - _d[1] * v[0]);
    }

    vec3 operator*(const GLfloat& f) const { return vec3(_d[0] * f, _d[1] * f, _d[2] * f); }

    vec3 operator-(const vec3& v) const { return vec3(_d[0] - v[0], _d[1] - v[1], _d[2] - v[2]); }

    vec3 operator+(const vec3& v) const { return vec3(_d[0] + v[0], _d[1] + v[1], _d[2] + v[2]); }

    GLfloat length() const { return std::sqrt(dot(*this)); }

    vec3 norm() const { return (*this) * (1.0 / length()); }

    void copy(GLfloat* v) {
        v[0] = _d[0];
        v[1] = _d[1];
        v[2] = _d[2];
    }
};

struct vertex3 : public vec4 {

    vertex3() {}

    vertex3(const GLfloat& x, const GLfloat& y, const GLfloat& z) : vec4(x, y, z, 1.0) {}

    vertex3(const GLfloat* f) : vec4(f[0], f[1], f[2], 1.0) {}

    vec3 operator-(const vertex3& v) const { return vec3(_d[0] - v[0], _d[1] - v[1], _d[2] - v[2]); }
};

struct vertex2 : public vec4 {

    vertex2() {}

    vertex2(const vec4& v) : vec4(v) {}

    vertex2(const GLfloat& x, const GLfloat& y) : vec4(x, y, 0.0, 1.0) {}

    vertex2(const GLfloat* f) : vec4(f[0], f[1], 0.0, 1.0) {}
};

struct mat4 {

    GLfloat _d[4][4];

    mat4(GLenum em) { glGetFloatv(em, &(_d[0][0])); }

    vec4 operator*(const vec4& v) const {
        return vec4(_d[0][0] * v[0] + _d[0][1] * v[1] + _d[0][1] * v[2] + _d[0][1] * v[3],
                    _d[1][0] * v[0] + _d[1][1] * v[1] + _d[1][1] * v[2] + _d[1][1] * v[3],
                    _d[2][0] * v[0] + _d[2][1] * v[1] + _d[2][1] * v[2] + _d[2][1] * v[3],
                    _d[3][0] * v[0] + _d[3][1] * v[1] + _d[3][1] * v[2] + _d[3][1] * v[3]);
    }

    void glSet(GLenum em) {
        GLenum old;
        glGetIntegerv(GL_MATRIX_MODE, (GLint*)&old);
        glPushAttrib(GL_MATRIX_MODE);
        glMatrixMode(em);
        glLoadMatrixf(&(_d[0][0]));
        glMatrixMode(old);
    }
};

#endif
