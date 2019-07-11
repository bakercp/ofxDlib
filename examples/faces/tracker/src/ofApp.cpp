//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    // Set up the detector settings.
    ofxDlib::FaceTracker::Settings settings;

    // By default the detector uses the standard dlib HOG detector.
    // It can also use the more accurate (and computationally expensive)
    // MMOD detector. The MMOD detector benefits greatly from hardware
    // acceleration (e.g. MKL, CUDA, etc).
    //
    settings.detectorType = ofxDlib::FaceDetector::Type::FACE_DETECTOR_MMOD;

    // We can scale the image DOWN to increase detection speed.
    // But this makes smaller faces more difficult to detect.
    //
    // settings.inputScale = 0.75;

    // We can scale the image UP to detect smaller faces. But this will result
    // in slower detection speeds.
    //
//     settings.inputScale = 1.5;

    // By default, scaling interpolation is done using a
    // OF_INTERPOLATE_NEAREST_NEIGHBOR, which is very fast. In some cases
    // it might be useful to use more complex interpolation, like
    // OF_INTERPOLATE_BILINEAR. This will be slower.
    //
    // settings.inputScaleInterpolation = OF_INTERPOLATE_BILINEAR;

    // If you know what region of the image will contain faces, you can set
    // region of interest (ROI). This will increase the speed by reducing the
    // number of pixels that need to be processed.
//    settings.inputROI = ofRectangle(200, 200, 400, 400);

//    settings.gpuDevice = 1;

    // Set up the detector.
    tracker.setup(settings);

    // Add trackers event listeners.
    trackBeginListener = tracker.trackBegin.newListener(this, &ofApp::onTrackBegin);
    trackUpdateListener = tracker.trackUpdate.newListener(this, &ofApp::onTrackUpdate);
    trackEndListener = tracker.trackEnd.newListener(this, &ofApp::onTrackEnd);

    // Set up the video input.
    // video.setup(1280, 720);
    video.load("LateLate.mp4");
    video.play();
    video.setPosition(0.25);
}


void ofApp::update()
{
    video.update();

    if (video.isFrameNew())
    {
        tracker.track(video);
    }
}


void ofApp::draw()
{
//    const auto* t = tracker.t();
//    if (t)
//    {
//        std::size_t droppedInputs =  t->droppedInputsCount();
//        std::size_t droppedOutputs =  t->droppedOutputsCount();

//        std::cout << "IN: " << droppedInputs << " OUT: " << droppedOutputs << std::endl;
//    }

//    std::size_t droppedInputsCount() const
//    {
//        std::unique_lock<std::mutex> lock(_mutex);
//        return _droppedInputsCount;
//    }

//    /// \returns the number of outputs that were not recevied.
//    std::size_t droppedOutputsCount() const



    // Draw the input video.
    ofSetColor(ofColor::white);
    video.draw(0, 0);

    // Draw the ROI.
    ofSetColor(ofColor::white, 127);
    ofxDlib::draw(tracker.settings().inputROI, "ROI");

    // Draw the tracks.
    // ofSetColor(ofColor::yellow);
    ofxDlib::draw(tracker.tracker());

    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()) + " FPS", 20, 20);
}


void ofApp::onTrackBegin(ofxDlib::FaceTrackerEventArgs& evt)
{
    std::cout << "Track begin: " << evt.label << std::endl;
}


void ofApp::onTrackUpdate(ofxDlib::FaceTrackerEventArgs& evt)
{
    std::cout << "Track update: " << evt.label << std::endl;
}


void ofApp::onTrackEnd(ofxDlib::FaceTrackerEventArgs& evt)
{
    std::cout << "Track end: " << evt.label << std::endl;
}
