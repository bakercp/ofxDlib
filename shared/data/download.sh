#!/usr/bin/env bash
set -e

THIS_PATH=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

/usr/bin/env bash ${THIS_PATH}/mnist.sh
/usr/bin/env bash ${THIS_PATH}/dlib.sh
/usr/bin/env bash ${THIS_PATH}/media.sh
