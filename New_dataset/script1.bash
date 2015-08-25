#!/bin/bash
function dirnametofilename() {
  for f in $*; do
    bn=$(basename "$f")
    ext="${bn##*.}"
    filepath=$(dirname "$f")
    dirname=$(basename "$filepath")
    mkdir -p "newdir/$filepath/positive"
    cp "$f" "newdir/$filepath/positive/$dirname$bn.$ext"
    #echo $bn
  done
}

dirnametofilename ./*/*
