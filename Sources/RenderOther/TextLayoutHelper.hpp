#ifndef TEXTLAYOUTHELPER_H
#define TEXTLAYOUTHELPER_H

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
 *  Additional comment added by the original author (Woody Zenfell):
 *
 *  The code in this file is licensed to you under the GNU GPL. As the
 *  copyright holder, however, I reserve the right to use this code as I see
 *  fit, without being bound by the GPL's terms.  This exemption is not
 *  intended to apply to modified versions of this file - if I were to use a
 *  modified version, I would be a licensee of whomever modified it, and thus
 *  must observe the GPL terms.
 *
 *  TextLayoutHelper assists with laying out a set of non-overlapping
 *  rectangles.  It's not as smart or as general as it could be, but it works.
 */

// should eventually use list and some other sort mechanism, probably, for cheaper insertions.
#include <vector>

using std::vector;

class TextLayoutHelper {
  public:

    TextLayoutHelper();
    ~TextLayoutHelper();

    // Remove all reservations
    void removeAllReservations();

    // Reserve a place for the rectangle given.  Returns the smallest bottom that will not overlap previous
    // reservations.
    int reserveSpaceFor(int inLeft, unsigned int inWidth, int inLowestBottom, unsigned int inHeight);

  protected:

    struct ReservationEnd;
    struct Reservation;

    typedef vector<ReservationEnd> CollectionOfReservationEnds;

    CollectionOfReservationEnds mReservationEnds;
};

struct TextLayoutHelper::ReservationEnd {
    int mHorizontalCoordinate;
    TextLayoutHelper::Reservation* mReservation;
    bool mStartOfReservation;
};

struct TextLayoutHelper::Reservation {
    int mBottom;
    int mTop;
};

#endif // TEXTLAYOUTHELPER_H
