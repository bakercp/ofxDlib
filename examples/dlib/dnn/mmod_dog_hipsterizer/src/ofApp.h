//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


// From: http://dlib.net/dnn_mmod_dog_hipsterizer.cpp.html

// This example shows how to run a CNN based dog face detector using dlib.  The
// example loads a pretrained model and uses it to find dog faces in images.
// We also use the dlib::shape_predictor to find the location of the eyes and
// nose and then draw glasses and a mustache onto each dog found :)


// Users who are just learning about dlib's deep learning API should read the
// dnn_introduction_ex.cpp and dnn_introduction2_ex.cpp examples to learn how
// the API works.  For an introduction to the object detection method you
// should read dnn_mmod_ex.cpp



// TRAINING THE MODEL
//     Finally, users interested in how the dog face detector was trained should
//     read the dnn_mmod_ex.cpp example program.  It should be noted that the
//     dog face detector used in this example uses a bigger training dataset and
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
//     http://dlib.net/files/data/CU_dogs_fully_labeled.tar.gz

//     Lastly, the shape_predictor was trained with default settings except we
//     used the following non-default settings: cascade depth=20, tree
//     depth=5, padding=0.2


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"

using namespace std;
using namespace dlib;

// ----------------------------------------------------------------------------------------

template <long num_filters, typename SUBNET> using con5d = con<num_filters,5,5,2,2,SUBNET>;
template <long num_filters, typename SUBNET> using con5  = con<num_filters,5,5,1,1,SUBNET>;

template <typename SUBNET> using downsampler  = relu<affine<con5d<32, relu<affine<con5d<32, relu<affine<con5d<16,SUBNET>>>>>>>>>;
template <typename SUBNET> using rcon5  = relu<affine<con5<45,SUBNET>>>;

using net_type = loss_mmod<con<1,9,9,1,1,rcon5<rcon5<rcon5<downsampler<input_rgb_image_pyramid<pyramid_down<6>>>>>>>>;

// ----------------------------------------------------------------------------------------


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;

    struct ColoredLine
    {
        ofPoint p0;
        ofPoint p1;
        ofColor color;
    };


    // Our image.
    ofImage image;

    // Our face detection bounding boxes.
    std::vector<ColoredLine> lines;

};
