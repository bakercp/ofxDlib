//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//

//
// From http://dlib.net/3d_point_cloud_ex.cpp.html
//
// This is a simple example of displaying 3D point clouds on the screen using
// dlib's random number generation.
//

#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;

    ofEasyCam camera;
    ofMesh mesh;

};
