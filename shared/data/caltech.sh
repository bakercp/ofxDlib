#!/usr/bin/env bash
# set -e


# echo "Download Caltech256 Image data"
# mkdir -p $DATA_PATH/caltech
# pushd $DATA_PATH/caltech > /dev/null
#
# caltech_256_data=256_ObjectCategories
# caltech_256_data_base_url="http://www.vision.caltech.edu/Image_Datasets/Caltech256/"
# caltech_256_data_compressed_suffix=".tar"
# caltech_256_data_compressed=$caltech_256_data$caltech_256_data_compressed_suffix
#
# if ! [ -f $caltech_256_data ] && ! [ -f $caltech_256_data_compressed ]; then
#     curl -L -O --progress-bar http://www.vision.caltech.edu/Image_Datasets/Caltech256/256_ObjectCategories.tar
# fi
#
# if ! [ -d $caltech_256_data ] ; then
#   echo "Decompressing $caltech_256_data_compressed"
#   tar xf $caltech_256_data_compressed
# else
#   echo "- Exists: Skipping decompression $caltech_256_data_compressed"
# fi
#
# sample_size=10
# echo ""
# echo "Create a sample of the Caltech256 Image data with sample size ${sample_size}"
# caltech_256_Sample_data=${caltech_256_data}_Sample
# caltech_256_Sample_Flat_data=${caltech_256_data}_Sample_Flat
#
# if ! [ -d $caltech_256_Sample_data ] || [ -d $caltech_256_Sample_Flat_data]; then
#   mkdir -p $caltech_256_Sample_Flat_data
#
#   for class_name_path in $caltech_256_data/* ; do
#     class_name=`basename $class_name_path`
#     class_name_no_number=${class_name:4}
#     sample_class_name_path=$caltech_256_Sample_data/$class_name
#     samples_remaining=$sample_size
#     mkdir -p $sample_class_name_path
#     for class_image_name in $class_name_path/* ; do
#       image_name=`basename $class_image_name`
#       image_name_no_number=${image_name:4}
#       flat_image_name=${class_name_no_number}-${image_name_no_number}
#       if (( samples_remaining <= -1 )); then
#         break
#       else
#         cp -v ${class_image_name} ${sample_class_name_path}/
#         cp -v ${class_image_name} ${caltech_256_Sample_Flat_data}/$flat_image_name
#
#         #cp -v ${class_image_name} ${sample_class_name_path}/
#       fi
#       samples_remaining=$((--samples_remaining))
#     done
#   done
# else
#   echo "- Exists: Skipping decompression $caltech_256_Sample_data"
# fi
#
# popd > /dev/null

