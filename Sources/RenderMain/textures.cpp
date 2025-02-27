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

#include "textures.hpp"
#include "cseries.hpp"

#define MARATHON2

/* ---------- code */

pixel8* calculate_bitmap_origin(struct bitmap_definition* bitmap) {
    pixel8* origin;

    origin = (pixel8*)(((byte*)bitmap) + sizeof(struct bitmap_definition));
    if (bitmap->flags & _COLUMN_ORDER_BIT) {
        origin += bitmap->width * sizeof(pixel8*);
    } else {
        origin += bitmap->height * sizeof(pixel8*);
    }

    return origin;
}

void remap_bitmap(struct bitmap_definition* bitmap, pixel8* table) {
    short row, rows, columns;

    rows    = (bitmap->flags & _COLUMN_ORDER_BIT) ? bitmap->width : bitmap->height;
    columns = (bitmap->flags & _COLUMN_ORDER_BIT) ? bitmap->height : bitmap->width;

    if (bitmap->bytes_per_row != NONE) {
        for (row = 0; row < rows; ++row) { map_bytes(bitmap->row_addresses[row], table, sizeof(pixel8) * columns); }
    } else {
#ifdef MARATHON1
        short run_count;
        pixel8* pixels;

        pixels = bitmap->row_addresses[0];
        for (row = 0; row < rows; ++row) {
            // CB: this needs to be corrected to work properly on little-endian machines
            while (run_count = *((short*)pixels)++) {
                if (run_count > 0) {
                    map_bytes(pixels, table, run_count);
                    pixels += run_count;
                }
            }
        }
#endif

#ifdef MARATHON2
        pixel8* pixels;

        pixels = bitmap->row_addresses[0];
        for (row = 0; row < rows; ++row) {
            // CB: first/last are stored in big-endian order
            uint16 first  = *pixels++ << 8;
            first        |= *pixels++;
            uint16 last   = *pixels++ << 8;
            last         |= *pixels++;
            map_bytes(pixels, table, last - first);
            pixels += last - first;
        }
#endif
    };
}

/* must initialize bytes_per_row, height and row_address[0] */
void precalculate_bitmap_row_addresses(struct bitmap_definition* bitmap) {
    short row, rows, bytes_per_row;
    pixel8 *row_address, **table;

    rows = (bitmap->flags & _COLUMN_ORDER_BIT) ? bitmap->width : bitmap->height;

    row_address = bitmap->row_addresses[0];
    table       = bitmap->row_addresses;
    if ((bytes_per_row = bitmap->bytes_per_row) != NONE) {
        for (row = 0; row < rows; ++row) {
            *table++     = row_address;
            row_address += bytes_per_row;
        }
    } else {
#ifdef MARATHON1
        for (row = 0; row < rows; ++row) {
            short run_count;
            *table++ = row_address;

            // CB: this needs to be corrected to work properly on little-endian systems
            while (run_count = *((short*)row_address)++) {
                if (run_count > 0)
                    row_address += run_count;
            }
        }
#endif

#ifdef MARATHON2
        for (row = 0; row < rows; ++row) {
            *table++ = row_address;

            // CB: first/last are stored in big-endian order
            uint16 first  = *row_address++ << 8;
            first        |= *row_address++;
            uint16 last   = *row_address++ << 8;
            last         |= *row_address++;
            row_address  += last - first;
        }
#endif
    }
}

void map_bytes(byte* buffer, byte* table, int32 size) {
    while ((size -= 1) >= 0) {
        *buffer  = table[*buffer];
        buffer  += 1;
    }
}
