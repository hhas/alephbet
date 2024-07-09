#!/usr/bin/env bash

set -e

function render() {
    file=$1
    w=$2
    h=$3
    margin=$4
    if [ "$margin" = "" ]; then
        margin=2.2
    fi
    if [ $file -ot logo.pov -o $file -ot $0 ]; then
        povray logo.pov +W$w +H$h +O$file -D +UA +A0.05 Declare=MARGIN=$margin File_Gamma=sRGB
    fi
}

# Windows icon
for f in 16 32 48 128 256; do
    render win$f.png $f $f
done
convert win{16,32,48,128,256}.png ../../Resources/Windows/alephbet.ico

if which iconutil &>/dev/null; then
    mkdir -p AlephBet.iconset
    for f in 16 32 128 256 512; do
        render AlephBet.iconset/icon_${f}x${f}.png $f $f
        render AlephBet.iconset/icon_${f}x${f}@2x.png $((f*2)) $((f*2))
    done
    iconutil -c icns AlephBet.iconset
    mv AlephBet.icns ../../PBProjects/AlephBet.icns
fi
