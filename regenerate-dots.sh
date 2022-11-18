#!/bin/bash

FONT="Iosevka,'Iosevka Term','Cascadia Code','JetBrains Mono','Fira Code',monospace"

for pic in $(find ./static/files -name '*.dot' -print); do
    SVG_NAME=".$(echo $pic | cut -d'.' -f2).svg"
    dot $pic -Tsvg -Gfontname="$FONT" -Nfontname="$FONT" -Efontname="$FONT" > $SVG_NAME
done;