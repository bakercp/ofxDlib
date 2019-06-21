//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
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

    ofTexture imgTexture;
    ofTexture blurredTexture;
    ofTexture rgbBlurredTexture;
    ofTexture horzGradientTexture;
    ofTexture vertGradientTexture;
    ofTexture edgeTexture;
    ofTexture heatEdgeTexture;
    ofTexture jetEdgeTexture;
    ofTexture randomEdgeTexture;

};
