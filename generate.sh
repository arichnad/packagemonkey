#!/bin/bash

# Generates packaging

if [ -f /tmp/changelog ]; then
  rm /tmp/changelog
fi

if [ -f debian/changelog ]; then
  cp debian/changelog /tmp/changelog
fi

rm -rf debian rpmpackage archpackage puppypackage ebuildpackage
rm -f rpm.sh debian.sh arch.sh puppy.sh ebuild.sh

if [ -f /tmp/changelog ]; then
  mkdir debian
  mv /tmp/changelog debian/changelog
fi

./packagemonkey -n "Packagemonkey" --version "1.03" --cmd --dir "." -l "gpl3" -e "Bob Mottram (4096 bits) <bob@robotics.uk.to>" --brief "Tool to make packaging easier" --desc "Creating installable packages for Linux distros has always been a bit of a black art.  There is a lot of documentation out there and it's often voluminous and not especially easy to interpret.  When doing packaging by hand there are also many ways to go wrong.  Packagemonkey is intended to make packaging simpler, and enable the creation of multiple different types of package." --homepage "https://github.com/bashrc/packagemonkey" --repository "https://github.com/bashrc/packagemonkey.git" --section "utils" --categories "Utility/ConsoleOnly" --compile "-lz" --builddeb "zlib1g-dev" --dependsdeb "build-essential, lintian, zlib1g" --buildrpm "zlib-devel" --dependsrpm "zlib"
