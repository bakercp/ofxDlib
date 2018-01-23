//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


// From http://dlib.net/dnn_mmod_face_detection_ex.cpp.html

// This example shows how to run a CNN based face detector using dlib.  The
// example loads a pretrained model and uses it to find faces in images.  The
// CNN model is much more accurate than the HOG based model shown in the
// face_detection_ex.cpp example, but takes much more computational power to
// run, and is meant to be executed on a GPU to attain reasonable speed.  For
// example, on a NVIDIA Titan X GPU, this example program processes images at
// about the same speed as face_detection_ex.cpp.

// Also, users who are just learning about dlib's deep learning API should read
// the dnn_introduction_ex.cpp and dnn_introduction2_ex.cpp examples to learn
// how the API works.  For an introduction to the object detection method you
// should read dnn_mmod_ex.cpp


// TRAINING THE MODEL
//     Finally, users interested in how the face detector was trained should
//     read the dnn_mmod_ex.cpp example program.  It should be noted that the
//     face detector used in this example uses a bigger training dataset and
//     larger CNN architecture than what is shown in dnn_mmod_ex.cpp, but
//     otherwise training is the same.  If you compare the net_type statements
//     in this file and dnn_mmod_ex.cpp you will see that they are very similar
//     except that the number of parameters has been increased.

//     Additionally, the following training parameters were different during
//     training: The following lines in dnn_mmod_ex.cpp were changed from
//         mmod_options options(face_boxes_train, 40,40);
//         trainer.set_iterations_without_progress_threshold(300);
//     to the following when training the model used in this example:
//         mmod_options options(face_boxes_train, 80,80);
//         trainer.set_iterations_without_progress_threshold(8000);

//     Also, the random_cropper was left at its default settings,  So we didn't
//     call these functions:
//         cropper.set_chip_dims(200, 200);
//         cropper.set_min_object_size(40,40);

//     The training data used to create the model is also available at
//     http://dlib.net/files/data/dlib_face_detection_dataset-2016-09-30.tar.gz


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


template <long num_filters, typename SUBNET>
using con5d = dlib::con<num_filters,5,5,2,2,SUBNET>;

template <long num_filters, typename SUBNET>
using con5 = dlib::con<num_filters,5,5,1,1,SUBNET>;

template <typename SUBNET>
using downsampler = dlib::relu<dlib::affine<con5d<32, dlib::relu<dlib::affine<con5d<32, dlib::relu<dlib::affine<con5d<16,SUBNET>>>>>>>>>;

template <typename SUBNET>
using rcon5  = dlib::relu<dlib::affine<con5<45,SUBNET>>>;

using net_type = dlib::loss_mmod<dlib::con<1,9,9,1,1,rcon5<rcon5<rcon5<downsampler<dlib::input_rgb_image_pyramid<dlib::pyramid_down<6>>>>>>>>;


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;

    ofImage image;

    std::vector<dlib::mmod_rect> dets;

};
