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

noinst_LIBRARIES = libfiles.a

if BUILD_ZZIP_RWOPS
ZZIP_SRCS = SDL_rwops_zzip.c
else
ZZIP_SRCS = 
endif

libfiles_a_SOURCES = AStream.h crc.h extensions.h FileHandler.h		\
  find_files.h game_wad.h Packing.h resource_manager.h			\
  SDL_rwops_ostream.h SDL_rwops_zzip.h tags.h wad.h wad_prefs.h		\
  WadImageCache.h                                                       \
									\
  AStream.cpp crc.cpp FileHandler.cpp find_files_sdl.cpp game_wad.cpp	\
  import_definitions.cpp Packing.cpp preprocess_map_sdl.cpp		\
  preprocess_map_shared.cpp resource_manager.cpp SDL_rwops_ostream.cpp  \
  $(ZZIP_SRCS) wad.cpp wad_prefs.cpp wad_sdl.cpp WadImageCache.cpp

EXTRA_libfiles_a_SOURCES = SDL_rwops_zzip.c

AM_CPPFLAGS = -I$(top_srcdir)/Source_Files/CSeries -I$(top_srcdir)/Source_Files/GameWorld \
  -I$(top_srcdir)/Source_Files/Input -I$(top_srcdir)/Source_Files/Misc \
  -I$(top_srcdir)/Source_Files/ModelView -I$(top_srcdir)/Source_Files/Network -I$(top_srcdir)/Source_Files/Network/Metaserver -I$(top_srcdir)/Source_Files/TCPMess \
  -I$(top_srcdir)/Source_Files/RenderMain \
  -I$(top_srcdir)/Source_Files/RenderOther -I$(top_srcdir)/Source_Files/Sound \
  -I$(top_srcdir)/Source_Files/XML -I$(top_srcdir)/Source_Files
