//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;

    ofImage image;

    std::vector<dlib::rectangle> dets;

    std::vector<dlib::full_object_detection> shapes;

    std::vector<ofImage> facechips;
};
