//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//

//
// From http://dlib.net/image_ex.cpp.html
//
// This example illustrates some basic dlib image manipulation functions as but
// does so using native openFrameworks types.
//

#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;

    ofTexture pixTexture;
    ofTexture pixBlurredTexture;
    ofTexture pixBlurredGrayscaleTexture;

};
