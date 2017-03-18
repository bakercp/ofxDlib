#!/bin/sh

# Download dlib models.

base_url=http://dlib.net/files

model_path=../models

models=(
  "dlib_face_recognition_resnet_model_v1.dat.bz2"
  "imagenet2015_validation_images.txt.bz2"
  "mmod_dog_hipsterizer.dat.bz2"
  "mmod_human_face_detector.dat.bz2"
  "resnet34_1000_imagenet_classifier.dnn.bz2"
  "shape_predictor_68_face_landmarks.dat.bz2"
)

for model in "${models[@]}"
do

echo "Downloading $model"
curl -L -o $model_path/$model --progress-bar $base_url/$model

echo "Extracting $model"
bzip2 -d $model_path/$model

done

echo "Done"
