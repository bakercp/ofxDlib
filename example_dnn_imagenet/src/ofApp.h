//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


//
// From http://dlib.net/dnn_imagenet_ex.cpp.html
//
// This example shows how to classify an image into one of the 1000 imagenet
// categories using the deep learning tools from the dlib C++ Library.  We will
// use the pretrained ResNet34 model available on the dlib website.
//
// The ResNet34 architecture is from the paper Deep Residual Learning for Image
// Recognition by He, Zhang, Ren, and Sun.  The model file that comes with dlib
// was trained using the dnn_imagenet_train_ex.cpp program on a Titan X for
// about 2 weeks.  This pretrained model has a top5 error of 7.572% on the 2012
// imagenet validation dataset.
//


#include "ofMain.h"
#include "ofxDlib.h"
#include "Resnet34.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;
    void keyPressed(int key) override;

    anet_type net;
    std::vector<string> labels;
    dlib::softmax<anet_type::subnet_type> snet;
    dlib::rand rnd;

    ofTexture currentImage;
    std::size_t currentImageIndex = 0;
    std::vector<std::filesystem::path> imagePaths;

    bool loadNext = true;

};
