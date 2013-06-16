#!/bin/bash

rm -rf debian
rm -rf rpmpackage
rm -rf desktop
rm -f LICENSE rpm.sh debian.sh

make
./packagemonkey --dir "/home/motters/develop/packagemonkey" -l "gpl3" -e "Bob Mottram (4096 bits) <bob@robotics.uk.to>" --brief "Tool to make packaging easier" --desc "Making packing easier than it previously was." --homepage "https://github.com/fuzzgun/packagemonkey.git" --section "utils" --version "0.10" --categories "Utility"
