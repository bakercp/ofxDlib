//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Set up the detector settings.
    ofxDlib::FaceDetector::Settings settings;

    // By default the detector uses the standard dlib HOG detector.
    // It can also use the more accurate (and computationally expensive)
    // MMOD detector. The MMOD detector benefits greatly from hardware
    // acceleration (e.g. MKL, CUDA, etc).
    //
    // settings.detectorType = FaceDetector::Type::FACE_DETECTOR_MMOD;

    // We can scale the image DOWN to increase detection speed.
    // But this makes smaller faces more difficult to detect.
    //
    // settings.inputScale = 0.5;

    // We can scale the image UP to detect smaller faces. But this will result
    // in slower detection speeds.
    //
    // settings.inputScale = 1.5;

    // By default, scaling interpolation is done using a
    // OF_INTERPOLATE_NEAREST_NEIGHBOR, which is very fast. In some cases
    // it might be useful to use more complex interpolation, like
    // OF_INTERPOLATE_BILINEAR. This will be slower.
    //
    // settings.inputScaleInterpolation = OF_INTERPOLATE_BILINEAR;

    // If you know what region of the image will contain faces, you can set
    // region of interest (ROI). This will increase the speed by reducing the
    // number of pixels that need to be processed.
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
        detections = detector.detect(video);
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
