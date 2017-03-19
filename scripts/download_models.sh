#!/bin/bash

# Download dlib models and media.

echo "Downloading models and media .."

ADDON_PATH=$( cd $(dirname $0)/.. ; pwd -P )
SCRIPTS_PATH=$ADDON_PATH/scripts

MODELS_PATH=$ADDON_PATH/models

dlib_model_base_url="http://dlib.net/files"
dlib_model_compressed_suffix=".bz2"
dlib_models=(
  "dlib_face_recognition_resnet_model_v1.dat"
  "mmod_dog_hipsterizer.dat"
  "mmod_human_face_detector.dat"
  "resnet34_1000_imagenet_classifier.dnn"
  "shape_predictor_68_face_landmarks.dat"
)

for dlib_model in "${dlib_models[@]}"
do
  dlib_model_compressed=$dlib_model$dlib_model_compressed_suffix
  dlib_model_compressed_path=$MODELS_PATH/$dlib_model_compressed
  dlib_model_path=$MODELS_PATH/$dlib_model

  echo "Downloading: $dlib_model"

  if ! [ -f $dlib_model_path ] && ! [ -f $dlib_model_compressed_path ] ; then
    curl -L -o $dlib_model_compressed_path --progress-bar $dlib_model_base_url/$dlib_model_compressed
  else
    echo "- Exists: Skipping download $model"
  fi

  if ! [ -f $dlib_model_path ] ; then
    echo "Decompressing $dlib_model_compressed_path"
    bzip2 -d $dlib_model_compressed_path
  else
    echo "- Exists: Skipping decompression $model"
  fi

  echo ""
done

echo ""
echo "Installing example models ..."

for required_models in `ls $ADDON_PATH/example*/bin/data/required_models.txt`
do
  while read model || [ -n "$model" ];
  do
    cp -v $MODELS_PATH/$model $(dirname $required_models)/
  done < $required_models
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
    url=${tokens[0]}
    destination=${tokens[1]}
    echo "Downloading $url ..."
    if [ -z "$destination" ];
    then
      curl -L -O --progress-bar $url
    else
      curl -L -o $destination --progress-bar $url
    fi
  done < $required_media
  popd
  echo ""
done
