#!/bin/bash

source 

# Check to see if apothecary is already installed.
if ! [ -f $OF_APOTHECARY_PATH/apothecary/apothecary ] ; then
  echo "Apothecary not installed, installing latest version."
  git clone https://github.com/openframeworks/apothecary.git $OF_APOTHECARY_PATH/
else
  pushd $OF_APOTHECARY_PATH/ > /dev/null
  if git rev-parse --is-inside-work-tree ; then
      echo "Apothecary is under git control, updating."
      git pull https://github.com/danzeeeman/apothecary android-r15
      # git pull origin master
  else
      echo "Apothecary is not under git control, so it may not be up-to-date."
  fi
  popd > /dev/null
fi

# Install any apothecary dependencies.
if [ -f $OF_APOTHECARY_PATH/scripts/$TARGET/install.sh ] ; then
  echo "Installing apothecary dependencies."
  $OF_APOTHECARY_PATH/scripts/$TARGET/install.sh
else
  echo "No additional apothecary dependencies to install."
fi

echo "Done installing apothecary."
