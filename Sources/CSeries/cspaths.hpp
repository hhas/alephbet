#ifndef _CSERIES_PATHS_
#define _CSERIES_PATHS_

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
 *  Provides OS-specific directory paths and application info
 */

#include "cstypes.hpp"
#include <string>

typedef enum {
    kPathLocalData,
    kPathDefaultData,
    kPathLegacyData,
    kPathBundleData,
    kPathLogs,
    kPathPreferences,
    kPathLegacyPreferences,
    kPathScreenshots,
    kPathSavedGames,
    kPathQuickSaves,
    kPathImageCache,
    kPathRecordings
} CSPathType;

std::string get_data_path(CSPathType type);
char get_path_list_separator();

std::string get_application_name();
std::string get_application_identifier();

#endif
