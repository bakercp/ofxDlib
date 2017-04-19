#!/bin/bash

echo "Installing ... "
echo `pwd`

SCRIPTS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../../" && pwd )"

echo $SCRIPTS_DIR

# Installing ...
# /home/travis/build/bakercp/ofxDlib
# /bin/bash: install_dlib.sh: No such file or directory

pushd $SCRIPTS_DIR/ >> /dev/null

/bin/bash install_dlib.sh

popd >> /dev/null
