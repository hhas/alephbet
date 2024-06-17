
## Process this file with automake to produce Makefile.in

SUBDIRS = Source_Files tools data

# Requires automake 1.5
AUTOMAKE_OPTIONS = 1.5 foreign dist-bzip2 no-dist-gzip

EXTRA_DIST = docs/MML.html docs/Lua.html docs/Lua_HUD.html		\
  docs/README.txt Resources/Windows/alephbet.ico		        \
  Resources/Windows/alephbet.rc						\
  PBProjects/AlephBet.xcodeproj/project.pbxproj				\
  PBProjects/en.lproj/InfoPlist.strings PBProjects/FilmIcon.icns	\
  PBProjects/ImagesIcon.icns PBProjects/MapIcon.icns			\
  PBProjects/MMLIcon.icns PBProjects/MusakIcon.icns			\
  PBProjects/PhysIcon.icns			                        \
  PBProjects/SaveIcon.icns PBProjects/ShapesIcon.icns			\
  PBProjects/SoundsIcon.icns		\
  PBProjects/AlephBet.icns				\
  PBProjects/config.h PBProjects/confpaths.h	\
  data/AlephSansMono-Bold.ttf data/AlephSansMonoLicense.txt		\
  data/ProFontAO.ttf data/ProFontAOLicense.txt		\
  docs/alephbet.6 examples/lua/Cheats.lua A1-THANKS README.md		\
  data/powered-by-alephbet.svg						\
  PBProjects/Info-AlephBet-Xcode4.plist\
  flatpak/alephbet.desktop flatpak/alephbet.png \
  flatpak/org.bungie.source.AlephBet.yml				  \
  flatpak/run-nodata.sh flatpak/run.sh \
  flatpak/shared/alephbet.yml flatpak/shared/boost.yml flatpak/shared/glu.yml \
  flatpak/shared/miniupnpc.yml flatpak/shared/zzip.yml

man_MANS = docs/alephbet.6

dist-hook: AlephBet.spec
	cp AlephBet.spec $(distdir)

release: dist windist

# Rule to build RPM distribution package
rpm: $(PACKAGE)-$(VERSION).tar.gz
	rpmbuild -ta --clean $(PACKAGE)-$(VERSION).tar.gz

# Rules to build Windows zipfiles
WINZIP=$(PACKAGE)-$(VERSION)-Win.zip

winzip: $(WINZIP)

windist: winzip

# Windows directories


define cp_no_svn
mkdir -p "$(2)"
tar -c --exclude='.svn' --exclude='.git' --exclude='Makefile' --exclude='Makefile.*' -C "$(1)" . | tar -x -C "$(2)"
endef

define windist_common
unix2dos -n $(srcdir)/THANKS "$(1)/THANKS.txt"
unix2dos -n $(srcdir)/COPYING "$(1)/COPYING.txt"
unix2dos -n $(srcdir)/README "$(1)/README.txt"
mkdir "$(1)/docs"
cp $(srcdir)/docs/Lua.html "$(1)/docs/Lua.html"
cp $(srcdir)/docs/Lua_HUD.html "$(1)/docs/Lua_HUD.html"
cp $(srcdir)/docs/MML.html "$(1)/docs/MML.html"
mkdir "$(1)/Extras"
cp $(srcdir)/data/Software_Transparent_Liquids.mml "$(1)/Extras"
cp $(srcdir)/data/Carnage_Messages.mml "$(1)/Extras"
unix2dos -n $(srcdir)/examples/lua/Cheats.lua "$(1)/Extras/Cheats.lua"
$(call cp_no_svn,$(srcdir)/Resources/Library Licenses,$(1)/docs/Library Licenses)
endef

# Windows version

WINDISTDIR=$(PACKAGE)-$(VERSION)
$(WINZIP): Source_Files/AlephBet.exe
	mkdir "$(WINDISTDIR)"
	$(call windist_common,$(WINDISTDIR))
	cp Source_Files/AlephBet.exe $(WINDISTDIR)/
	unix2dos -n $(srcdir)/INSTALL.Windows "$(WINDISTDIR)/docs/INSTALL.Windows.txt"
	cp $(srcdir)/data/Transparent_Sprites.mml $(WINDISTDIR)/Extras
	cp $(srcdir)/data/Transparent_Liquids.mml $(WINDISTDIR)/Extras
	$(call cp_no_svn,$(srcdir)/data/default_theme,$(WINDISTDIR)/Plugins/Default_Theme)
	rm -f $(WINZIP)
	zip -r $(WINZIP) $(WINDISTDIR)
	rm -rf "$(WINDISTDIR)"

# flatpaks
A1_FLATPAK=AlephBet-$(VERSION).flatpak
a1-flatpak: $(A1_FLATPAK)

flatpaks: a1-flatpak

$(A1_FLATPAK):
	flatpak-builder --repo=flatpak/repo --force-clean flatpak/AlephBet.build $(srcdir)/flatpak/org.bungie.source.AlephBet.yml
	flatpak build-bundle flatpak/repo $@ org.bungie.source.AlephBet

FORCE:
