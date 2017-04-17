//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//

//
// From http://dlib.net/3d_point_cloud_ex.cpp.html
//
// This is an example illustrating the use of the perspective_window tool
// in the dlib C++ Library.  It is a simple tool for displaying 3D point
// clouds on the screen.
//

#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;

    std::vector<ofTexture> textures;

};
