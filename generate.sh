#!/bin/bash

# Generates packaging for packagemonkey

rm -rf debian rpmpackage archpackage puppypackage ebuildpackage
rm -f rpm.sh debian.sh arch.sh puppy.sh ebuild.sh

./packagemonkey -n "Packagemonkey" --cmd --dir "." -l "gpl3" -e "Bob Mottram (4096 bits) <bob@robotics.uk.to>" --brief "Tool to make packaging easier" --desc "Creating installable packages for Linux distros has always been a bit of a black art.  There is a lot of documentation out there and it's often voluminous and not especially easy to interpret.  When doing packaging by hand there are also many ways to go wrong.  Packagemonkey is intended to make packaging simpler, and enable the creation of multiple different types of package." --homepage "https://github.com/fuzzgun/packagemonkey" --section "utils" --version "1.00" --categories "Utility/ConsoleOnly" --compile "-lz" --dependsdeb "build-essential, lintian"
