#ifndef __TEXTURES_H
#define __TEXTURES_H

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

#include    "cseries.h"

#include <vector>

/* ---------- structures */

enum /* bitmap flags */
{
	_COLUMN_ORDER_BIT= 0x8000,
	_TRANSPARENT_BIT= 0x4000,
	_PATCHED_BIT= 0x2000, // the bitmap should take precedent over MML
};

struct bitmap_definition
{
	int16 width, height; /* in pixels */
	int16 bytes_per_row; /* if ==NONE this is a transparent RLE shape */
	
	int16 flags; /* [column_order.1] [unused.15] */
	int16 bit_depth; /* should always be ==8 */
	
	int16 unused[8];
	
	pixel8 *row_addresses[1];
};
const int SIZEOF_bitmap_definition = 30;

// A [bitmap_definition][row pointer array] buffer (can be empty)
class bitmap_definition_buffer
{
private:
	std::vector<byte> buf;
	
public:
	bitmap_definition_buffer() {}
	
	explicit bitmap_definition_buffer(int row_count /*>= 1*/)
		: buf(sizeof(bitmap_definition) + (assert(row_count >= 1), row_count - 1) * sizeof(pixel8*)) {}
	
	bool empty() const { return buf.empty(); }
	int row_count() const { return empty() ? 0 : 1 + (buf.size() - sizeof(bitmap_definition)) / sizeof(pixel8*); }
	
	bitmap_definition* get() { return empty() ? nullptr : reinterpret_cast<bitmap_definition*>(buf.data()); }
	bitmap_definition const* get() const { return empty() ? nullptr : reinterpret_cast<const bitmap_definition*>(buf.data()); }
	
	void clear() { buf.clear(); }
};

/* ---------- prototypes/TEXTURES.C */

/* assumes pixel data follows bitmap_definition structure immediately */
pixel8 *calculate_bitmap_origin(struct bitmap_definition *bitmap);

/* initialize bytes_per_row, height and row_address[0] before calling */
void precalculate_bitmap_row_addresses(struct bitmap_definition *texture);

void map_bytes(byte *buffer, byte *table, int32 size);
void remap_bitmap(struct bitmap_definition *bitmap,	pixel8 *table);

#endif

