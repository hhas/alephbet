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
 *  cscluts_sdl.cpp - CLUT handling, SDL implementation
 *
 *  Written in 2000 by Christian Bauer
 */

#include "FileHandler.hpp"
#include "cseries.hpp"
#include <SDL_endian.h>


// Global variables
RGBColor rgb_black = {0x0000, 0x0000, 0x0000};
RGBColor rgb_white = {0xffff, 0xffff, 0xffff};

RGBColor system_colors[NUM_SYSTEM_COLORS] = {
        {0x2666, 0x2666, 0x2666},
        {0xd999, 0xd999, 0xd999}
};

/*
 *  Convert Mac CLUT resource to color_table
 */

void build_color_table(color_table* table, LoadedResource& clut) {
    // Open stream to CLUT resource
    SDL_RWops* p = SDL_RWFromMem(clut.GetPointer(), (int)clut.GetLength());
    assert(p);

    // Check number of colors
    SDL_RWseek(p, 6, SEEK_CUR);
    table->color_count = std::min(SDL_ReadBE16(p) + 1, 256);

    // Convert color data
    rgb_color* dst = table->colors;
    for (int i = 0; i < table->color_count; i++) {
        SDL_RWseek(p, 2, SEEK_CUR);
        dst->red   = SDL_ReadBE16(p);
        dst->green = SDL_ReadBE16(p);
        dst->blue  = SDL_ReadBE16(p);
        dst++;
    }

    // Close stream
    SDL_RWclose(p);
}
