#ifndef __ALEPHVERSION_H
#define __ALEPHVERSION_H

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

// Version info for Aleph Bet itself
#define AB_DISPLAY_NAME         "Aleph Bet"
#define AB_DISPLAY_VERSION      "0.9.0"
#define AB_CONTENT_VERSION      1
#define AB_DATE_VERSION         "20240716"
#define AB_DATE_DISPLAY_VERSION "2024-07-16"

// Info for the Aleph One version we're compatible with
#define A1_DISPLAY_VERSION "1.8.1"
#define A1_DATE_VERSION    "20240712" // used for Lua scripts and plugins

#ifdef _WIN32
#define WIN_VERSION_STRING  0, 9, 0, 0 // <-- don't forget to update that for windows releases
#define AB_DISPLAY_PLATFORM "Windows"
#define AB_UPDATE_PLATFORM  "windows"
#elif defined(__APPLE__) && defined(__MACH__)
#define AB_DISPLAY_PLATFORM "macOS"
#define AB_UPDATE_PLATFORM  "macosx"
#elif defined(linux)
#define AB_DISPLAY_PLATFORM "Linux"
#define AB_UPDATE_PLATFORM  "source"
#elif defined(__NetBSD__)
#define AB_DISPLAY_PLATFORM "NetBSD"
#define AB_UPDATE_PLATFORM  "source"
#elif defined(__OpenBSD__)
#define AB_DISPLAY_PLATFORM "OpenBSD"
#define AB_UPDATE_PLATFORM  "source"
#else
#define AB_DISPLAY_PLATFORM "Unknown"
#define AB_UPDATE_PLATFORM  "source"
#endif

#ifndef AB_VERSION_STRING
#define AB_VERSION_STRING AB_DISPLAY_PLATFORM " " AB_DISPLAY_VERSION
#endif

#define AB_HOMEPAGE_URL "https://aleph-bet-marathon.github.io/"
#define AB_UPDATE_URL   "https://aleph-bet-marathon.github.io/update_check/" AB_UPDATE_PLATFORM ".txt"

// For now, this is still the "Aleph One metaserver"
#define A1_METASERVER_HOST         "metaserver.lhowon.org"
#define A1_METASERVER_LOGIN_URL    "https://metaserver.lhowon.org/metaclient/login"
#define A1_METASERVER_SIGNUP_URL   "https://metaserver.lhowon.org/metaclient/signup"
#define A1_METASERVER_SETTINGS_URL "https://metaserver.lhowon.org/metaclient/settings"
#define A1_LEADERBOARD_URL         "https://stats.lhowon.org/"
#define A1_STATSERVER_ADD_URL      "https://stats.lhowon.org/statclient/add"

#endif // ALEPHVERSION_H
