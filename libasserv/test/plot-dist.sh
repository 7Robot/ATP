#!/bin/bash

dest="/tmp/plot-$RANDOM"

build/test_ramp_dist $* > "$dest"

if [[ $? -ne 0 ]]; then
    rm -f "$dest"
    exit 1
fi

cp plot.template "$dest".sh
chmod +x "$dest".sh

echo set output \'"$dest".png\' >> "$dest".sh
echo plot \"$dest\" using 1:2 title \"position\", \
            \"$dest\" using 1:3 title \"vitesse\", \
            \"$dest\" using 1:4 title \"acceleration\" >> "$dest".sh

"$dest".sh

mirage "$dest".png

rm -f "$dest" "$dest".sh
