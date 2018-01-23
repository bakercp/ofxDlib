#!/usr/bin/env bash

THIS_PATH=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
THIS_NAME=$(basename "${BASH_SOURCE[0]%.*}")
INSTALL_PATH=${THIS_PATH}/${THIS_NAME}
mkdir -p ${INSTALL_PATH}

echo "Installing ${THIS_NAME} data ..."

if ! [ -d ${INSTALL_PATH}/examples ]; then
  git clone --depth=1 https://github.com/davisking/dlib.git .tmp/
  rm .tmp/examples/*.{h,cpp,txt}
  mv .tmp/examples ${INSTALL_PATH}/
  rm -rf .tmp/
fi
echo "✅ ${INSTALL_PATH}/examples"


INSTALL_PATH=${INSTALL_PATH}/models
mkdir -p ${INSTALL_PATH}
# BASE_URL="http://dlib.net/files"
BASE_URL="https://github.com/bakercp/ofxDlib/releases/download/models/"
FILES=(
  #"dlib_face_recognition_resnet_model_v1.dat.bz2"
  #"dlib_face_recognition_resnet_model_v1_lfw_test_scripts.tar.bz2"
  #"dlib_kitti_submission_mmodCNN_basic7convModel.tar.bz2"
  #"imagenet2015_validation_images.txt.bz2"
  "mmod_dog_hipsterizer.dat"
  "mmod_front_and_rear_end_vehicle_detector.dat"
  "mmod_human_face_detector.dat"
  "mmod_rear_end_vehicle_detector.dat"
  "resnet34_1000_imagenet_classifier.dnn"
  "semantic_segmentation_voc2012net.dnn"
  "shape_predictor_5_face_landmarks.dat"
  "shape_predictor_68_face_landmarks.dat"
)

for file in "${FILES[@]}"
do
  if ! [ -f ${INSTALL_PATH}/${file} ]; then
    curl -L --progress-bar ${BASE_URL}/${file}.bz2 | bunzip2 > ${INSTALL_PATH}/${file}
  fi
  echo "✅ ${INSTALL_PATH}/${file}"
done
