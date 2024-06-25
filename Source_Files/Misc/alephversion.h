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


#define A1_DISPLAY_NAME "Aleph Bet"
#define A1_DISPLAY_VERSION "0.9.0"
#define A1_DISPLAY_DATE_VERSION "2024-06-03"
#define A1_DATE_VERSION "20240603"

#ifdef _WIN32
#define WIN_VERSION_STRING 0,2024,6,3 // <-- don't forget to update that for windows releases
#define A1_DISPLAY_PLATFORM "Windows"
#define A1_UPDATE_PLATFORM "windows"
#elif defined (__APPLE__) && defined(__MACH__)
#ifdef MAC_APP_STORE
#define A1_DISPLAY_PLATFORM "Mac OS X (App Store)"
#define A1_UPDATE_PLATFORM "macappstore"
#else
#define A1_DISPLAY_PLATFORM "macOS"
#define A1_UPDATE_PLATFORM "macosx"
#endif
#elif defined (linux)
#define A1_DISPLAY_PLATFORM "Linux"
#define A1_UPDATE_PLATFORM "source"
#elif defined (__NetBSD__)
#define A1_DISPLAY_PLATFORM "NetBSD"
#define A1_UPDATE_PLATFORM "source"
#elif defined (__OpenBSD__)
#define A1_DISPLAY_PLATFORM "OpenBSD"
#define A1_UPDATE_PLATFORM "source"
#else
#define A1_DISPLAY_PLATFORM "Unknown"
#define A1_UPDATE_PLATFORM "source"
#endif

#ifndef A1_VERSION_STRING
#define A1_VERSION_STRING A1_DISPLAY_PLATFORM " " A1_DISPLAY_DATE_VERSION " " A1_DISPLAY_VERSION
#endif

#define A1_HOMEPAGE_URL "https://aleph-bet-marathon.github.io/"
#define A1_UPDATE_URL "https://aleph-bet-marathon.github.io/update_check/" A1_UPDATE_PLATFORM ".txt"
#define A1_METASERVER_HOST "metaserver.lhowon.org"
#define A1_METASERVER_LOGIN_URL "https://metaserver.lhowon.org/metaclient/login"
#define A1_METASERVER_SIGNUP_URL "https://metaserver.lhowon.org/metaclient/signup"
#define A1_METASERVER_SETTINGS_URL "https://metaserver.lhowon.org/metaclient/settings"
#define A1_LEADERBOARD_URL "https://stats.lhowon.org/"
#define A1_STATSERVER_ADD_URL "https://stats.lhowon.org/statclient/add"

#endif // ALEPHVERSION_H
