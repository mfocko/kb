#!/bin/sh

# remove preexisting ZIPs
find ./static/files -name '*.zip' -exec rm {} \;

for relative_path in $(find ./static/files -name '.zipit' -print); do
    relative_path=$(dirname $relative_path)
    base=$(basename $relative_path)
    cd $relative_path/..

    all_files=$(find $base/** ! -name '.zipit' -print)
    zip -9 $base.zip $all_files
    mv $base.zip $base/$base.zip

    cd -
done;