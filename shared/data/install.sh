#!/usr/bin/env bash
set -e

THIS_PATH=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

/usr/bin/env bash ${THIS_PATH}/mnist.sh
/usr/bin/env bash ${THIS_PATH}/dlib.sh
/usr/bin/env bash ${THIS_PATH}/media.sh

# function copy_shared_data_for_examples()
# {
#   if [ -z "$1" ]; then
#     echo "Usage: install_data_for_examples <path_to_addon>"
#     return 1
#   fi
#
#   # Form the shared data path.
#   addon_shared_data_path=${1}/shared/data
#
#   for required_data in ${1}/example*/bin/data/REQUIRED_DATA.txt
#   do
#     # For the example data path.
#     example_data_path=$(dirname ${required_data})
#
#     # The || [ -n "$line" ]; is to help when the last line isn't a new line char.
#     while read line || [ -n "$line" ];
#     do
#       # Make sure the data doesn't start with a comment hash #
#       # Make sure that it isn't am empty line.
#       if [ "${line:0:1}" != "#"  ] && [ -n "${line// }" ]; then
#         # Turn the line into an array (space delimited).
#         tokens=( "$line" )
#         # Get the first token -- the source location.
#         data_source=${tokens[0]}
#
#         if [ -e ${addon_shared_data_path}/${data_source} ]; then
#           rsync -Pqar ${addon_shared_data_path}/${data_source} ${example_data_path}
#         else
#           echo "${addon_shared_data_path}/${data_source} does not exist. Did you install the data?"
#         fi
#       fi
#     done < $required_data
#   done
#   return 0
# }
