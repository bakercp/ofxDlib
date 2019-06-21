//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


// From: http://dlib.net/dnn_mmod_ex.cpp.html

// This example shows how to train a CNN based object detector using dlib's
// loss_mmod loss layer.  This loss layer implements the Max-Margin Object
// Detection loss as described in the paper:
//     Max-Margin Object Detection by Davis E. King (http://arxiv.org/abs/1502.00046).
// This is the same loss used by the popular SVM+HOG object detector in dlib
// (see fhog_object_detector_ex.cpp) except here we replace the HOG features
// with a CNN and train the entire detector end-to-end.  This allows us to make
// much more powerful detectors.

// It would be a good idea to become familiar with dlib's DNN tooling before
// reading this example.  So you should read dnn_introduction_ex.cpp and
// dnn_introduction2_ex.cpp before reading this example program.

// Just like in the fhog_object_detector_ex.cpp example, we are going to train
// a simple face detector based on the very small training dataset in the
// examples/faces folder.  As we will see, even with this small dataset the
// MMOD method is able to make a working face detector.  However, for real
// applications you should train with more data for an even better result.


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


using namespace std;
using namespace dlib;

// Let's begin the network definition by creating some network blocks.

// A 5x5 conv layer that does 2x downsampling
template <long num_filters, typename SUBNET> using con5d = con<num_filters,5,5,2,2,SUBNET>;
// A 3x3 conv layer that doesn't do any downsampling
template <long num_filters, typename SUBNET> using con3  = con<num_filters,3,3,1,1,SUBNET>;

// Now we can define the 8x downsampling block in terms of conv5d blocks.  We
// also use relu and batch normalization in the standard way.
template <typename SUBNET> using downsampler  = relu<bn_con<con5d<32, relu<bn_con<con5d<32, relu<bn_con<con5d<32,SUBNET>>>>>>>>>;

// The rest of the network will be 3x3 conv layers with batch normalization and
// relu.  So we define the 3x3 block we will use here.
template <typename SUBNET> using rcon3  = relu<bn_con<con3<32,SUBNET>>>;

// Finally, we define the entire network.   The special input_rgb_image_pyramid
// layer causes the network to operate over a spatial pyramid, making the detector
// scale invariant.
using net_type  = loss_mmod<con<1,6,6,1,1,rcon3<rcon3<rcon3<downsampler<input_rgb_image_pyramid<pyramid_down<6>>>>>>>>;

// ----------------------------------------------------------------------------------------


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;
    
    net_type net;

    ofVideoGrabber grabber;

    std::vector<ofRectangle> detections;

};
