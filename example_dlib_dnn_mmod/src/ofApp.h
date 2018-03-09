//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
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


// The first thing we do is define our CNN.  The CNN is going to be evaluated
// convolutionally over an entire image pyramid.  Think of it like a normal
// sliding window classifier.  This means you need to define a CNN that can look
// at some part of an image and decide if it is an object of interest.  In this
// example I've defined a CNN with a receptive field of a little over 50x50
// pixels.  This is reasonable for face detection since you can clearly tell if
// a 50x50 image contains a face.  Other applications may benefit from CNNs with
// different architectures.
//
// In this example our CNN begins with 3 downsampling layers.  These layers will
// reduce the size of the image by 8x and output a feature map with
// 32 dimensions.  Then we will pass that through 4 more convolutional layers to
// get the final output of the network.  The last layer has only 1 channel and
// the values in that last channel are large when the network thinks it has
// found an object at a particular location.


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
    void draw() override;


    // A custom class for associating images and their detection rectangles.
    struct TestImage
    {
        // Our image.
        ofImage image;

        // Our face detection bounding boxes.
        std::vector<ofRectangle> faceRects;
    };

    // These will be used for our slideshow.
    std::vector<TestImage> testImages;
    std::size_t currentImage = 0;
    uint64_t nextImage = 0;
};
