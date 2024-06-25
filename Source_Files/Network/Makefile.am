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

noinst_LIBRARIES = libnetwork.a

SUBDIRS = Metaserver

libnetwork_a_SOURCES = ConnectPool.h network.h network_capabilities.h		  \
  network_data_formats.h network_dialog_widgets_sdl.h network_dialogs.h		  \
  network_distribution_types.h network_games.h network_lookup_sdl.h			  \
  network_messages.h network_private.h network_star.h NetworkGameProtocol.h	  \
  RingGameProtocol.h SDL_netx.h SSLP_API.h SSLP_Protocol.h StarGameProtocol.h \
  Update.h HTTP.h PortForward.h \
  \
  ConnectPool.cpp network.cpp network_capabilities.cpp						  \
  network_data_formats.cpp network_dialogs.cpp network_dialog_widgets_sdl.cpp \
  network_games.cpp network_lookup_sdl.cpp network_messages.cpp				  \
  network_star_hub.cpp network_star_spoke.cpp network_udp.cpp				  \
  RingGameProtocol.cpp SDL_netx.cpp SSLP_limited.cpp StarGameProtocol.cpp	  \
  Update.cpp HTTP.cpp PortForward.cpp

EXTRA_libnetwork_a_SOURCES = network_dummy.cpp

AM_CPPFLAGS = -I$(top_srcdir)/Source_Files/CSeries -I$(top_srcdir)/Source_Files/Files \
  -I$(top_srcdir)/Source_Files/GameWorld -I$(top_srcdir)/Source_Files/Input \
  -I$(top_srcdir)/Source_Files/Network/Metaserver \
  -I$(top_srcdir)/Source_Files/Misc -I$(top_srcdir)/Source_Files/ModelView \
  -I$(top_srcdir)/Source_Files/RenderMain \
  -I$(top_srcdir)/Source_Files/RenderOther -I$(top_srcdir)/Source_Files/Sound \
  -I$(top_srcdir)/Source_Files/TCPMess -I$(top_srcdir)/Source_Files/XML \
  -I$(top_srcdir)/Source_Files/Lua -I$(top_srcdir)/Source_Files
