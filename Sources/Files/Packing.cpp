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

#undef PACKED_DATA_IS_LITTLE_ENDIAN
#undef PACKED_DATA_IS_BIG_ENDIAN
#define PACKING_INTERNAL
#include "Packing.hpp"
#include "cseries.hpp"

// big endian

void StreamToValueBE(uint8*& Stream, uint16& Value) {
    // Must be unsigned, so they will be zero-extended
    uint16 Byte0 = uint16(*(Stream++));
    uint16 Byte1 = uint16(*(Stream++));

    Value = (Byte0 << 8) | Byte1;
}

void StreamToValueBE(uint8*& Stream, int16& Value) {
    uint16 UValue;
    StreamToValueBE(Stream, UValue);
    Value = int16(UValue);
}

void StreamToValueBE(uint8*& Stream, uint32& Value) {
    // Must be unsigned, so they will be zero-extended
    uint32 Byte0 = uint32(*(Stream++));
    uint32 Byte1 = uint32(*(Stream++));
    uint32 Byte2 = uint32(*(Stream++));
    uint32 Byte3 = uint32(*(Stream++));

    Value = (Byte0 << 24) | (Byte1 << 16) | (Byte2 << 8) | Byte3;
}

void StreamToValueBE(uint8*& Stream, int32& Value) {
    uint32 UValue;
    StreamToValueBE(Stream, UValue);
    Value = int32(UValue);
}

void ValueToStreamBE(uint8*& Stream, uint16 Value) {
    *(Stream++) = uint8(Value >> 8);
    *(Stream++) = uint8(Value);
}

void ValueToStreamBE(uint8*& Stream, int16 Value) { ValueToStreamBE(Stream, uint16(Value)); }

void ValueToStreamBE(uint8*& Stream, uint32 Value) {
    *(Stream++) = uint8(Value >> 24);
    *(Stream++) = uint8(Value >> 16);
    *(Stream++) = uint8(Value >> 8);
    *(Stream++) = uint8(Value);
}

void ValueToStreamBE(uint8*& Stream, int32 Value) { ValueToStreamBE(Stream, uint32(Value)); }

// little endian

void StreamToValueLE(uint8*& Stream, uint16& Value) {
    // Must be unsigned, so they will be zero-extended
    uint16 Byte0 = uint16(*(Stream++));
    uint16 Byte1 = uint16(*(Stream++));

    Value = (Byte1 << 8) | Byte0;
}

void StreamToValueLE(uint8*& Stream, int16& Value) {
    uint16 UValue;
    StreamToValueLE(Stream, UValue);
    Value = int16(UValue);
}

void StreamToValueLE(uint8*& Stream, uint32& Value) {
    // Must be unsigned, so they will be zero-extended
    uint32 Byte0 = uint32(*(Stream++));
    uint32 Byte1 = uint32(*(Stream++));
    uint32 Byte2 = uint32(*(Stream++));
    uint32 Byte3 = uint32(*(Stream++));

    Value = (Byte3 << 24) | (Byte2 << 16) | (Byte1 << 8) | Byte0;
}

void StreamToValueLE(uint8*& Stream, int32& Value) {
    uint32 UValue;
    StreamToValueLE(Stream, UValue);
    Value = int32(UValue);
}

void ValueToStreamLE(uint8*& Stream, uint16 Value) {
    *(Stream++) = uint8(Value);
    *(Stream++) = uint8(Value >> 8);
}

void ValueToStreamLE(uint8*& Stream, int16 Value) { ValueToStreamLE(Stream, uint16(Value)); }

void ValueToStreamLE(uint8*& Stream, uint32 Value) {
    *(Stream++) = uint8(Value);
    *(Stream++) = uint8(Value >> 8);
    *(Stream++) = uint8(Value >> 16);
    *(Stream++) = uint8(Value >> 24);
}

void ValueToStreamLE(uint8*& Stream, int32 Value) { ValueToStreamLE(Stream, uint32(Value)); }
