## Process this file with automake to produce Makefile.in 

SUBDIRS = CSeries Files FFmpeg GameWorld Input Lua Misc	\
   ModelView Network RenderMain RenderOther Sound TCPMess XML

bin_PROGRAMS = alephbet

alephbet_SOURCES = shell.h \
  \
  shell.cpp shell_misc.cpp shell_options.h shell_options.cpp main.cpp

alephbet_LDADD = \
  CSeries/libcseries.a Files/libfiles.a FFmpeg/libffmpeg.a \
  GameWorld/libgameworld.a Input/libinput.a Lua/liba1lua.a Misc/libmisc.a \
  ModelView/libmodelview.a Network/libnetwork.a Network/Metaserver/libmetaserver.a \
  RenderMain/librendermain.a RenderOther/librenderother.a Sound/libsound.a \
  XML/libxml.a \
  \
  CSeries/libcseries.a Files/libfiles.a FFmpeg/libffmpeg.a \
  GameWorld/libgameworld.a Input/libinput.a Lua/liba1lua.a Misc/libmisc.a \
  ModelView/libmodelview.a Network/libnetwork.a Network/Metaserver/libmetaserver.a \
  RenderMain/librendermain.a RenderOther/librenderother.a Sound/libsound.a \
  TCPMess/libtcpmess.a XML/libxml.a

AM_CPPFLAGS = -I$(top_srcdir)/Source_Files/CSeries -I$(top_srcdir)/Source_Files/Files \
  -I$(top_srcdir)/Source_Files/GameWorld -I$(top_srcdir)/Source_Files/Input \
  -I$(top_srcdir)/Source_Files/Lua -I$(top_srcdir)/Source_Files/Misc \
  -I$(top_srcdir)/Source_Files/ModelView -I$(top_srcdir)/Source_Files/Network \
  -I$(top_srcdir)/Source_Files/Network/Metaserver \
  -I$(top_srcdir)/Source_Files/FFmpeg -I$(top_srcdir)/Source_Files/RenderMain \
  -I$(top_srcdir)/Source_Files/RenderOther -I$(top_srcdir)/Source_Files/Sound \
  -I$(top_srcdir)/Source_Files/XML -I$(top_srcdir)/Source_Files/TCPMess

AlephBet_LDADD = $(alephbet_LDADD) alephbet-resources.o
AlephBet_SOURCES = $(alephbet_SOURCES)

if MAKE_WINDOWS
BUILD_YEAR = `echo $(VERSION) | cut -c 1-4`
BUILD_MONTH = `echo $(VERSION) | cut -c 5-6 | sed -e s/^0//`
BUILD_DAY = `echo $(VERSION) | cut -c 7-8 | sed -e s/^0//`
WIN_VERSION_STRING= 0, $(BUILD_YEAR), $(BUILD_MONTH), $(BUILD_DAY)
A1_DISPLAY_VERSION= `grep '^\#define A1_DISPLAY_VERSION' $(srcdir)/Misc/alephversion.h | sed -e 's/\(.*\"\)\(.*\)\(\"\)/\2/g' | tr -d '\n'`
endif

alephbet-resources.o: $(top_srcdir)/Resources/Windows/alephbet.rc Misc/alephversion.h
	sed -e "s/WIN_VERSION_STRING/$(WIN_VERSION_STRING)/g" -e "s/A1_DISPLAY_VERSION/$(A1_DISPLAY_VERSION)/g" $(top_srcdir)/Resources/Windows/alephbet.rc | @WINDRES@ -I$(top_srcdir)/Resources/Windows -o alephbet-resources.o
