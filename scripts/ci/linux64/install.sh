#!/bin/bash

echo "Installing ... "
echo `pwd`

SCRIPTS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}/../../" )" && pwd )"

echo $SCRIPTS_DIR

# Installing ...
# /home/travis/build/bakercp/ofxDlib
# /bin/bash: install_dlib.sh: No such file or directory

/bin/bash $SCRIPTS_DIR/install_dlib.sh
