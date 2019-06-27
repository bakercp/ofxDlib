//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;

    // The video grabber.
    ofVideoGrabber video;

    // The face detector.
    ofxDlib::FaceDetector detector;

    // The last detections.
    std::vector<ofxDlib::ObjectDetection> detections;

};
