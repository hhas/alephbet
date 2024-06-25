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
    --exclude '/Source_Files/Lua/l*' \
    --exclude '/Source_Files/Lua/COPYRIGHT' \
    --exclude '/Source_Files/Lua/README' \
    --include '/Source_Files/Lua/language_definition.h' \
    --include '/Source_Files/Lua/lua_*' \
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
    --dir Source_Files

# -----------------------------------------------------------------------------

echo "All dist checks passed!"