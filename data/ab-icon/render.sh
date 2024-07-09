#!/bin/sh

set -e

# Windows icon
for f in 16 32 48 128 256; do
    if [ win$f.png -ot logo.pov -o win$f.png -ot $0 ]; then
        povray logo.pov +W$f +H$f +Owin$f.png -D +UA +A0.05 Declare=MARGIN=2.2 File_Gamma=sRGB
    fi
done
convert win16.png win32.png win48.png win128.png win256.png ../../Resources/Windows/alephbet.ico
