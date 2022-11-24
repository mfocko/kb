#!/bin/bash

# remove preexisting ZIPs
find ./static/files -name '*.zip' -exec rm {} \;

for relative_path in $(find ./static/files -name '.zipit' -print); do
    relative_path=$(dirname $relative_path)
    base=$(basename $relative_path)
    pushd $relative_path/..

    echo "PWD: $PWD"

    all_files=$(find $base/** ! -name '.zipit' -print)
    zip -9 $base.zip $all_files
    mv $base.zip $base/$base.zip

    popd
done;