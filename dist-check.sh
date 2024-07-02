#!/usr/bin/env bash

set -e

if ! which cargo >/dev/null; then
    echo "$0 uses Rust to perform some checks, and thus requires cargo to be installed."
    echo ""
    echo "See <https://www.rust-lang.org/learn/get-started> for installation instructions."
    exit 1
fi

cd "$(dirname $0)"

# -----------------------------------------------------------------------------

echo "Checking about screen fodder."

if ! test Source_Files/Misc/a1-authors.h -nt authors-update.lua \
|| ! test Source_Files/Misc/a1-authors.h -nt A1-AUTHORS \
|| ! test Source_Files/Misc/ab-authors.h -nt authors-update.lua \
|| ! test Source_Files/Misc/ab-authors.h -nt AB-AUTHORS; then
    echo "About screen out of date. Please run authors-update.lua and rebuild."
    exit 1
fi

displaydate="$(date "+%Y-%m-%d")"
shortdate="$(echo $displaydate | tr -d -)"
if [ "$(grep '^#define AB_DATE_DISPLAY_VERSION' Source_Files/Misc/alephversion.h | sed -e 's/.*"\(.*\)"/\1/g' )" != "$displaydate" -o "$(grep '^#define AB_DATE_VERSION' Source_Files/Misc/alephversion.h | sed -e 's/.*"\(.*\)"/\1/g' )" != "$shortdate" ]; then
    echo "Dates in Source_Files/Misc/alephversion.h need to be updated. Please update them and rebuild."
    echo "  (Today's date is $displaydate)"
    exit 1
fi

# -----------------------------------------------------------------------------

echo "Checking copyright notices."
cargo run -q --release --manifest-path=copyright-check/Cargo.toml -- \
    --work "Aleph Bet" \
    --years "1994-2024" \
    --authors "Bungie Inc., the Aleph One developers, and the Aleph Bet developers" \
    configure.ac \
    Makefile.am \
    README.md \
    docs/README.txt \
    --exclude '*.o' \
    --exclude 'AStream.*' \
    --exclude 'BStream.*' \
    --exclude 'HTTP.*' \
    --exclude 'IMG_savepng.*' \
    --exclude 'SDL_ffmpeg.*' \
    --exclude 'SDL_rwops_zzip.*' \
    --exclude 'Statistics.*' \
    --exclude '/Source_Files/CSeries/confpaths.h' \
    --exclude '/Source_Files/Lua/l*' \
    --exclude '/Source_Files/Lua/COPYRIGHT' \
    --exclude '/Source_Files/Lua/README' \
    --include '/Source_Files/Lua/language_definition.h' \
    --include '/Source_Files/Lua/lua_*.cpp' \
    --include '/Source_Files/Lua/lua_*.h' \
    --exclude '/Source_Files/Misc/a1-authors.h' \
    --exclude '/Source_Files/Misc/ab-authors.h' \
    --exclude '/Source_Files/Misc/AlephSansMono-Bold.h' \
    --exclude '/Source_Files/Misc/CourierPrime*.h' \
    --exclude '/Source_Files/Misc/ProFontAO.h' \
    --exclude '/Source_Files/Misc/*.xpm' \
    --exclude '/Source_Files/Misc/powered_by_aleph*.h' \
    --exclude '/Source_Files/Network/PortForward.*' \
    --exclude '/Source_Files/Network/SDL_netx.*' \
    --exclude '/Source_Files/Network/SSLP_*' \
    --exclude '/Source_Files/RenderMain/Shaders' \
    --exclude '/Source_Files/RenderOther/sdl_resize.*' \
    --exclude '/Source_Files/TCPMess/*' \
    --include '/Source_Files/TCPMess/Makefile.am' \
    --exclude '*.Po' \
    --exclude '*.o' \
    --exclude '*.a' \
    --exclude 'Source_Files/alephbet' \
    --dir Source_Files

# -----------------------------------------------------------------------------

echo "All dist checks passed!"