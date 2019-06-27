//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


using namespace ofxDlib;


void ofApp::setup()
{
    // Set up the detector settings.
    FaceDetector::Settings settings;

    // By default the detector uses the standard dlib HOG detector.
    // It can also use the more accurate (and computationally expensive)
    // MMOD detector. The MMOD detector benefits greatly from hardware
    // acceleration (e.g. MKL, CUDA, etc).
    //
    // settings.detectorType = FaceDetector::Type::FACE_DETECTOR_MMOD;

    // We scale the image down to increase detection speed.
    settings.inputScale = 0.5;

    // We set a region of interest (ROI) to increase detection speed.
    settings.inputROI = ofRectangle(200, 200, 800, 400);

    // Set up the detector.
    detector.setup(settings);

    // Set up the video input.
    video.setup(1280, 720);
}


void ofApp::update()
{
    video.update();

    if (video.isFrameNew())
    {
        detections = detector.detect(video.getPixels());
    }
}


void ofApp::draw()
{
    // Draw the input video.
    ofSetColor(ofColor::white);
    video.draw(0, 0);

    // Draw the ROI.
    ofSetColor(ofColor::white, 127);
    ofxDlib::draw(detector.settings().inputROI, "ROI");

    // Draw the detections.
    ofSetColor(ofColor::yellow);
    ofxDlib::draw(detections);
}
