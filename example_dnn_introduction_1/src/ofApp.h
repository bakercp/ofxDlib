//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//

//
// From http://dlib.net/dnn_introduction_ex.cpp.html
//
// This is an example illustrating the use of the deep learning tools from the
// dlib C++ Library.  In it, we will train the venerable LeNet convolutional
// neural network to recognize hand written digits.  The network will take as
// input a small image and classify it as one of the 10 numeric digits between
// 0 and 9.

// The specific network we will run is from the paper
//     LeCun, Yann, et al. "Gradient-based learning applied to document recognition."
//     Proceedings of the IEEE 86.11 (1998): 2278-2324.
// except that we replace the sigmoid non-linearities with rectified linear units.

// These tools will use CUDA and cuDNN to drastically accelerate network
// training and testing. Without CUDA / cuDNN, the program will still run but
// will be much slower to execute.


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;

};
