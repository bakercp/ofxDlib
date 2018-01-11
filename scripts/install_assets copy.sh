#!/bin/bash
set -e

# Download dlib models and media.

echo "Downloading models and data ..."
# openFrameworks information.
echo "Calling ${BASH_SOURCE[0]} from `pwd`"

# This script assumes it is in the openFrameworks/addons/THE_ADDON/scripts dir.
if [ -z ${OF_ROOT+x} ]; then
  OF_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../../.." && pwd )"
  echo "Setting OF_ROOT to: ${OF_ROOT}"
else
  echo "OF_ROOT is already set to: ${OF_ROOT}"
fi

OF_ADDONS_PATH=$OF_ROOT/addons
OF_SCRIPTS_PATH=$OF_ROOT/scripts
OF_APOTHECARY_PATH=$OF_SCRIPTS_PATH/apothecary

# Addon information.
if [ -z ${ADDON_NAME+x} ]; then
  ADDON_NAME="$(basename $( cd "$( dirname "${BASH_SOURCE[0]}" )/../" && pwd ))"
fi

ADDON_PATH=$OF_ADDONS_PATH/$ADDON_NAME
ADDON_SCRIPTS_PATH=$ADDON_PATH/scripts

SCRIPTS_PATH=$ADDON_PATH/scripts
OF_ADDON_MODELS_PATH=$ADDON_PATH/models
OF_ADDON_DATA_PATH=$ADDON_PATH/data
OF_ADDONS_PATH=$OF_ROOT/addons
OF_ADDON_SCRIPTS_PATH=$OF_ROOT/scripts
OF_APOTHECARY_PATH=$OF_SCRIPTS_PATH/apothecary

# Get the OS type.
TARGET=$(${ADDON_SCRIPTS_PATH}/shared/ostype.sh)

echo "openFrameworks Config"
echo "====================="
echo "           OF_ROOT: ${OF_ROOT}"
echo "    OF_ADDONS_PATH: ${OF_ADDONS_PATH}"
echo "   OF_SCRIPTS_PATH: ${OF_SCRIPTS_PATH}"
echo "OF_APOTHECARY_PATH: ${OF_APOTHECARY_PATH}"
echo ""
echo "MACHINE CONFIG"
echo "=============="
echo "           TARGET: ${TARGET}"
echo ""
echo "ADDON CONFIG"
echo "============"
echo "        ADDON_NAME: ${ADDON_NAME}"
echo "        ADDON_PATH: ${ADDON_PATH}"
echo "ADDON_SCRIPTS_PATH: ${ADDON_SCRIPTS_PATH}"
echo " ADDON_MODELS_PATH: ${ADDON_MODELS_PATH}"
echo "   ADDON_DATA_PATH: ${ADDON_DATA_PATH}"
echo ""

echo ""
echo "Installing example models ..."

for required_models in `ls $ADDON_PATH/example*/bin/data/required_models.txt`
do
  while read model || [ -n "$model" ];
  do
    echo $required_models
    rsync -Prvaq $OF_ADDON_MODELS_PATH/$model $(dirname $required_models)
  done < $required_models
  echo ""
done

echo ""
echo "Installing example data ..."

for required_data in `ls $ADDON_PATH/example*/bin/data/required_data.txt`
do
  while read data || [ -n "$data" ];
  do
    echo $required_data
    rsync -Prvaq $OF_ADDON_DATA_PATH/$data $(dirname $required_data)
  done < $required_data
  echo ""
done

echo ""
echo "Downloading example media ..."

for required_media in `ls $ADDON_PATH/example*/bin/data/required_media.txt`
do
  # Move into this directory. > /dev/null consumes the output.
  pushd $(dirname $required_media)/ > /dev/null

  # The || [ -n "$line" ]; is to help when the last line isn't a new line char.
  while read line || [ -n "$line" ];
  do
    tokens=($line)
    destination=${tokens[0]}
    url=${tokens[1]}

    if ! [ -f $destination ] ; then
      echo "Downloading $url"
      curl -L -o $destination --progress-bar $url
    else
      echo "- Exists: Skipping $destination"
    fi
  done < $required_media
  popd > /dev/null
  echo ""
done

