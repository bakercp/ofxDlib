#!/bin/bash


APOTHECARY_DIR=../../../scripts/apothecary/


if ! [ -f $APOTHECARY_DIR/apothecary/apothecary ] ; then
  echo "Apothecary not installed, getting."
  git clone https://github.com/openframeworks/apothecary.git $APOTHECARY_DIR/
else
  echo "- Exists: Pulling"
  pushd $APOTHECARY_DIR/apothecary > /dev/null
  git pull
  popd > /dev/null
fi



rm -rf  ../../../scripts/apothecary/apothecary/build/dlib/
../../../scripts/apothecary/apothecary/./apothecary -v -j16 -d ../libs update ofxDlib

