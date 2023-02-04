#!/bin/sh

FONT="Iosevka,'Iosevka Term','Cascadia Code','JetBrains Mono','Fira Code',monospace"

for pic in $(find ./static/files -name '*.dot' -print); do
    SVG_NAME=".$(echo $pic | cut -d'.' -f2)"

    # light mode
    dot $pic -Tsvg -Gfontname="$FONT" -Nfontname="$FONT" -Efontname="$FONT" > ${SVG_NAME}_light.svg

    # dark mode
    dot $pic -Tsvg -Gfontname="$FONT" -Nfontname="$FONT" -Efontname="$FONT" -Gbgcolor="none" -Gcolor="white" -Gfontcolor="white" -Nfillcolor="none" -Ncolor="white" -Nfontcolor="white" -Efillcolor="none" -Ecolor="white" -Efontcolor="white" > ${SVG_NAME}_dark.svg
done;