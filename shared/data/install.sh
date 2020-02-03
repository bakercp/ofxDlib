#!/usr/bin/env bash
set -e

if [ -z "$1" ]; then
    echo "Usage: install_data_for_examples <path_to_addon>"
else
    # Form the shared data path.
    addon_shared_data_path=${1}/shared/data

    dirs=${1}/example*
    for dir in $dirs
    do
    echo "Process ${dir}"

    required_data=${dir}/bin/data/data.txt

    if [ -f "$required_data" ]; then
        #echo ${required_data}
        # For the example data path.
        example_data_path=$(dirname ${required_data})

        # The || [ -n "$line" ]; is to help when the last line isn't a new line char.
        while read line || [ -n "$line" ];
        do
        # Make sure the data doesn't start with a comment hash #
        # Make sure that it isn't am empty line.
        if [ "${line:0:1}" != "#"  ] && [ -n "${line// }" ]; then
            # Turn the line into an array (space delimited).
            tokens=($line)
            # Get the first token -- the source location.
            data_source="${tokens[0]}"
            data_destination="${tokens[1]}"

            # remove trailing whitespace
            #data_source="$(echo -e "${data_source}" | sed -e 's/[[:space:]]*$//')"
            data_destination="$(echo -e "${data_destination}" | sed -e 's/[[:space:]]*$//')"

            src="${addon_shared_data_path}/${data_source}"
            dst="${example_data_path}/${data_destination}"

            if [ "${data_destination}" == "" ]; then
                # copy folder contents into /data folder
                cp -r "${src}/." "${example_data_path}/"
            else
                if [ -d "${src}" ]; then
                    echo "  copy directory ${data_source}"
                    cp -r $src $dst
                elif [ -f "${src}" ]; then
                    echo "  copy file ${data_source}"
                    cp $src $dst
                else
                    echo "  ${addon_shared_data_path}/${data_source} does not exist. Did you install the data?"
                fi
            fi
        fi
        done < $required_data
    else
        echo "  data.txt does not exist, skip copy"
    fi

        echo ""
    done
fi
