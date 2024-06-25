#ifndef NETWORK_DISTRIBUTION_TYPES_H
#define NETWORK_DISTRIBUTION_TYPES_H

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
 *  Centralized location for distribution types (for NetDistributeInformation,
 *  NetAddDistributionFunction, etc.) helps avoid potential conflicts.
 */

enum {
        kOriginalNetworkAudioDistributionTypeID = 0,    // for compatibility with older versions
        kNewNetworkAudioDistributionTypeID = 1          // new-style realtime network audio data
};

#endif // NETWORK_DISTRIBUTION_TYPES_H
