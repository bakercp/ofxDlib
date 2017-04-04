#!/bin/bash


rm -rf  ../../../scripts/apothecary/apothecary/build/dlib/
../../../scripts/apothecary/apothecary/./apothecary -v -j16 -d ../libs update ofxDlib

