## Process this file with automake to produce Makefile.in

# Aleph Bet is copyright ©1994-2024 Bungie Inc., the Aleph One developers, and
# the Aleph Bet developers.
#
# Aleph Bet is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# Aleph Bet is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <https://www.gnu.org/licenses/>.
#
# This license notice applies only to the Aleph Bet engine itself, and does
# not apply to Marathon, Marathon 2, or Marathon Infinity scenarios and
# assets, nor to elements of any third-party scenarios.

noinst_LIBRARIES = libmisc.a

if MAKE_WINDOWS
THREAD_PRIORITY = thread_priority_sdl_win32.cpp
else
THREAD_PRIORITY = thread_priority_sdl_posix.cpp
endif

libmisc_a_SOURCES = achievements.h ActionQueues.h alephversion.h binders.h CircularByteBuffer.h \
  CircularQueue.h Console.h DefaultStringSets.h game_errors.h \
  interface.h interface_menus.h key_definitions.h Logging.h \
  PlayerImage_sdl.h \
  PlayerName.h preference_dialogs.h preferences.h \
  preferences_widgets_sdl.h progress.h Random.h Scenario.h sdl_dialogs.h sdl_network.h \
  sdl_widgets.h shared_widgets.h thread_priority_sdl.h vbl_definitions.h vbl.h VecOps.h \
  WindowedNthElementFinder.h AlephSansMono-Bold.h powered_by_alephbet.h powered_by_alephbet_h.h \
  Statistics.h \
  \
  achievements.cpp ActionQueues.cpp CircularByteBuffer.cpp Console.cpp DefaultStringSets.cpp game_errors.cpp \
  interface.cpp \
  Logging.cpp PlayerImage_sdl.cpp PlayerName.cpp preferences.cpp \
  preference_dialogs.cpp preferences_widgets_sdl.cpp Scenario.cpp sdl_dialogs.cpp $(THREAD_PRIORITY) \
  sdl_widgets.cpp shared_widgets.cpp vbl.cpp \
  Statistics.cpp \
  ProFontAO.h CourierPrime.h CourierPrimeBold.h CourierPrimeItalic.h CourierPrimeBoldItalic.h

EXTRA_libmisc_a_SOURCES = alephbet.xpm alephbet32.xpm thread_priority_sdl_posix.cpp thread_priority_sdl_dummy.cpp thread_priority_sdl_win32.cpp thread_priority_sdl_macosx.cpp m1_achievements.lua steamshim_child.h steamshim_child.c

AM_CPPFLAGS = -I$(top_srcdir)/Source_Files/CSeries -I$(top_srcdir)/Source_Files/Files \
  -I$(top_srcdir)/Source_Files/GameWorld -I$(top_srcdir)/Source_Files/Input \
  -I$(top_srcdir)/Source_Files/Lua -I$(top_srcdir)/Source_Files/ModelView -I$(top_srcdir)/Source_Files/Network \
  -I$(top_srcdir)/Source_Files/Network/Metaserver \
  -I$(top_srcdir)/Source_Files/FFmpeg -I$(top_srcdir)/Source_Files/RenderMain \
  -I$(top_srcdir)/Source_Files/RenderOther -I$(top_srcdir)/Source_Files/Sound \
  -I$(top_srcdir)/Source_Files/TCPMess \
  -I$(top_srcdir)/Source_Files/XML \
  -I$(top_srcdir)/Source_Files
