#!/bin/bash

# This is the default location for apothecary.
APOTHECARY_DIR=../../../scripts/apothecary/

# Check to see if apothecary is already installed.
if ! [ -f $APOTHECARY_DIR/apothecary/apothecary ] ; then
  echo "Apothecary not installed, installing."
  git clone https://github.com/openframeworks/apothecary.git $APOTHECARY_DIR/
else
  echo "Apothecay is installed, attempting to pull the latest version."
  pushd $APOTHECARY_DIR/apothecary > /dev/null
  git pull
  popd > /dev/null
fi

# Cleaning any previous dlib builds.
rm -rf  ../../../scripts/apothecary/apothecary/build/dlib/

# Executing the Apothecary build script for dlib.
../../../scripts/apothecary/apothecary/./apothecary -v -j16 -d ../libs update ofxDlib

