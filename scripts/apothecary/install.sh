#!/bin/bash

# This script assumes it is in the openFrameworks/addons/THE_ADDON/scripts/apothecary dir.
OF_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../../../../" && pwd )"

# This is the default location for apothecary.
APOTHECARY_DIR=$OF_ROOT/scripts/apothecary

# Check to see if apothecary is already installed.
if ! [ -f $APOTHECARY_DIR/apothecary/apothecary ] ; then
  echo "Apothecary not installed, installing latest version."
  git clone https://github.com/openframeworks/apothecary.git $APOTHECARY_DIR/
else
  pushd $APOTHECARY_DIR/ > /dev/null
  if git rev-parse --is-inside-work-tree ; then
      echo "Apothecary is under git control, updating."
      git pull origin master
  else
      echo "Apothecary is not under git control, so it may not be up-to-date."
  fi
  popd > /dev/null
fi

# Move into the apothecary directory.
pushd $APOTHECARY_DIR/apothecary/ > /dev/null
# Determine target os.
TARGET_OS=`./ostype.sh`
# Install apthecary requirements.
popd > /dev/null

# Install any dependencies.
if [ -f $APOTHECARY_DIR/scripts/$TARGET_OS/install.sh ] ; then
  echo "Installing apothecary dependencies."
  $APOTHECARY_DIR/scripts/$TARGET_OS/install.sh
else
  echo "No additional apthecary dependencies to install."
fi

echo "Done installing apothecary."
