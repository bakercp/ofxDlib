#!/usr/bin/env bash

THIS_PATH=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
THIS_NAME=$(basename "${BASH_SOURCE[0]%.*}")
INSTALL_PATH=${THIS_PATH}/${THIS_NAME}
mkdir -p ${INSTALL_PATH}

echo "Installing ${THIS_NAME} data ..."

BASE_URL="http://yann.lecun.com/exdb/mnist"
FILES=(
  "train-images-idx3-ubyte"
  "train-labels-idx1-ubyte"
  "t10k-images-idx3-ubyte"
  "t10k-labels-idx1-ubyte"
)

for file in "${FILES[@]}"
do
  if ! [ -f ${INSTALL_PATH}/${file} ]; then
    curl -L --progress-bar ${BASE_URL}/${file}.gz | gunzip > ${INSTALL_PATH}/${file}
  fi
  echo "✔ ${INSTALL_PATH}/${file}"
done
