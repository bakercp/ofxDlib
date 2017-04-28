#!/bin/bash

# Get the OS type.
OS_TYPE=`./shared/ostype.sh`

# This script assumes it is in the openFrameworks/addons/THE_ADDON/scripts dir.
OF_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../../../" && pwd )"

# This is the addon name.
ADDON_NAME="$(basename $( cd "$( dirname "${BASH_SOURCE[0]}" )/../" && pwd ))"

# Download the precompiled library.
curl -L --progress https://github.com/bakercp/$ADDON_NAME/releases/download/libs-master/$OS_TYPE.zip | tar -xf-  -C ../libs/

# Install data if needed.
if [ -f data/install.sh ] ; then
  data/./install.sh
fi

# Install any system dependencies if they exist.
if [ -f dependencies/$OS_TYPE/install.sh ] ; then
  dependencies/$OS_TYPE/install.sh
fi