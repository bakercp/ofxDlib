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


    ofPixels pixels;
    ofTexture texture;

    std::vector<dlib::mmod_rect> rectangles;

    float scale = 1;
};
