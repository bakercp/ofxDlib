#!/bin/bash
set -e

# openFrameorks information.

echo "Calling ${BASH_SOURCE[0]} from `pwd`"

# This script assumes it is in the openFrameworks/addons/THE_ADDON/scripts dir.
if ! [ -z ${OF_ROOT+x} ]; then
  export OF_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../../../" && pwd )"
  echo "Setting OF_ROOT to: ${OF_ROOT}"
else
  echo "OF_ROOT is already set to: ${OF_ROOT}"
fi
OF_ADDONS_PATH=$OF_ROOT/addons
OF_SCRIPTS_PATH=$OF_ROOT/scripts
OF_APOTHECARY_PATH=$OF_SCRIPTS_PATH/apothecary

# Addon information.
ADDON_NAME="$(basename $( cd "$( dirname "${BASH_SOURCE[0]}" )/../" && pwd ))"
ADDON_PATH=$OF_ADDONS_PATH/$ADDON_NAME
ADDON_SCRIPTS_PATH=$ADDON_PATH/scripts
# Get the OS type.
OS_TYPE=`${ADDON_SCRIPTS_PATH}/shared/ostype.sh`

echo "openFrameworks Config"
echo "====================="
echo "           OF_ROOT: ${OF_ROOT}"
echo "    OF_ADDONS_PATH: ${OF_ADDONS_PATH}"
echo "   OF_SCRIPTS_PATH: ${OF_SCRIPTS_PATH}"
echo "OF_APOTHECARY_PATH: ${OF_APOTHECARY_PATH}"
echo ""
echo "MACHINE CONFIG"
echo "=============="
echo "           OS_TYPE: ${OS_TYPE}"
echo ""
echo "ADDON CONFIG"
echo "============"
echo "        ADDON_NAME: ${ADDON_NAME}"
echo "        ADDON_PATH: ${ADDON_PATH}"
echo "ADDON_SCRIPTS_PATH: ${ADDON_SCRIPTS_PATH}"
echo ""

echo "Building for ${OS_TYPE} ..."

# Install any apothecary dependencies.
if [ -f $ADDON_SCRIPTS_PATH/dependencies/$OS_TYPE/install.sh ] ; then
  echo "Installing ${ADDON_NAME} dependencies for ${OS_TYPE} ..."
  /bin/bash $ADDON_SCRIPTS_PATH/dependencies/$TARGET_OS/install.sh
else
  echo "No special ${ADDON_NAME} dependencies required for ${OS_TYPE}."
fi

# Install or update apothecary.
echo "Installing apothecary ..."
/bin/bash $ADDON_SCRIPTS_PATH/apothecary/install.sh

# Clean any prior builds.
echo "Cleaning prior apothecary builds for ${ADDON_NAME} ..."
/bin/bash $OF_APOTHECARY_PATH/apothecary/apothecary -v clean $ADDON_NAME

# Build using apothcary
echo "Building ${ADDON_NAME} libraries for ${OS_TYPE} ..."
/bin/bash $OF_APOTHECARY_PATH/apothecary/apothecary -v -j16 -d $ADDON_PATH/libs update $ADDON_NAME

echo "Build of ${ADDON_NAME} complete for ${OS_TYPE}."
