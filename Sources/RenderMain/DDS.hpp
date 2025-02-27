#ifndef _DIRECTDRAWSURFACE_H_
#define _DIRECTDRAWSURFACE_H_

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

#include "cstypes.hpp"

#ifndef __DDRAW_INCLUDED__

#define DDSD_CAPS        0x0000'0001
#define DDSD_HEIGHT      0x0000'0002
#define DDSD_WIDTH       0x0000'0004
#define DDSD_PITCH       0x0000'0008
#define DDSD_PIXELFORMAT 0x0000'1000
#define DDSD_MIPMAPCOUNT 0x0000'2000
#define DDSD_LINEARSIZE  0x0000'8000
#define DDSD_DEPTH       0x0080'0000

#define DDPF_ALPHAPIXELS 0x0000'0001
#define DDPF_FOURCC      0x0000'0004
#define DDPF_RGB         0x0000'0040

#define DDSCAPS_COMPLEX 0x0000'0008
#define DDSCAPS_TEXTURE 0x0000'1000
#define DDSCAPS_MIPMAP  0x0040'0000

#define DDSCAPS2_CUBEMAP 0x0000'0200
#define DDSCAPS2_VOLUME  0x0020'0000

struct DDSURFACEDESC2 {
    uint32 dwSize;
    uint32 dwFlags;
    uint32 dwHeight;
    uint32 dwWidth;
    uint32 dwPitchOrLinearSize;
    uint32 dwDepth;
    uint32 dwMipMapCount;
    uint32 dwReserved1[11];

    struct {
        uint32 dwSize;
        uint32 dwFlags;
        uint32 dwFourCC;
        uint32 dwRGBBitCount;
        uint32 dwRBitMask;
        uint32 dwGBitMask;
        uint32 dwBBitMask;
        uint32 dwRGBAlphaBitMask;
    } ddpfPixelFormat;

    struct {
        uint32 dwCaps1;
        uint32 dwCaps2;
        uint32 Reserved[2];
    } ddsCaps;

    uint32 dwReserved2;
};

#endif
#endif
