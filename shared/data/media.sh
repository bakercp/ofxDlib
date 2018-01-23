#!/usr/bin/env bash

THIS_PATH=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
THIS_NAME=$(basename "${BASH_SOURCE[0]%.*}")
INSTALL_PATH=${THIS_PATH}/${THIS_NAME}
mkdir -p ${INSTALL_PATH}

echo "Installing ${THIS_NAME} data ..."

entries=(
  "https://upload.wikimedia.org/wikipedia/commons/b/be/July_4_crowd_at_Vienna_Metro_station.jpg Crowd.jpg"
  "https://upload.wikimedia.org/wikipedia/commons/9/90/RobertCornelius.jpg Selfie.jpg"
  "https://upload.wikimedia.org/wikipedia/commons/thumb/6/6e/Golde33443.jpg/160px-Golde33443.jpg Puppy.jpg"
)

for entry in "${entries[@]}"
do
  IFS=' ' read -r -a tokens <<< "${entry}"
  SRC=${tokens[0]}
  DST=${tokens[1]:-$(basename ${SRC})}
  if ! [ -f ${INSTALL_PATH}/${DST} ]; then
    curl -L --progress-bar ${SRC} > ${INSTALL_PATH}/${DST}
  fi
  echo "✅ ${INSTALL_PATH}/${DST}"
done
