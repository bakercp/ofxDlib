#!/bin/bash

# Get the OS type.
OS_TYPE=`./shared/ostype.sh`

# This script assumes it is in the openFrameworks/addons/THE_ADDON/scripts dir.
OF_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../../../" && pwd )"

# This is the addon name.
ADDON_NAME="$(basename $( cd "$( dirname "${BASH_SOURCE[0]}" )/../" && pwd ))"

# Install or update apothecary.
./apothecary/install.sh

# This is the default location for apothecary.
APOTHECARY_DIR=$OF_ROOT/scripts/apothecary

# Clean any prior builds.
$APOTHECARY_DIR/apothecary/./apothecary -v clean $ADDON_NAME

# Build using apothcary
$APOTHECARY_DIR/apothecary/./apothecary -v -j16 -d ../libs update $ADDON_NAME

