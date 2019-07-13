//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Set up the detector settings.
    ofxDlib::FaceTracker::Settings settings;

    // By default the detector uses the standard dlib HOG detector.
    // It can also use the more accurate (and computationally expensive)
    // MMOD detector. The MMOD detector benefits greatly from CUDA hardware
    // acceleration.
    //
    // Find a useful a discussion of the various methods here:
    // https://www.learnopencv.com/face-detection-opencv-dlib-and-deep-learning-c-python/
    //
    // settings.detectorType = ofxDlib::FaceDetector::Type::FACE_DETECTOR_MMOD;

    // If you are using the MMOD detector and have multiple GPUS, you can select
    // the gpu device to use. See also http://dlib.net/dlib/dnn/cuda_dlib.h.html
    //
    // settings.gpuDevice = 0;

    // We can scale the image DOWN to increase detection speed.
    // But this makes smaller faces more difficult to detect.
    //
    settings.inputScale = 0.5;

    // We can scale the image UP to detect smaller faces. But this may result
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
    //
    settings.inputROI = ofRectangle(200, 200, 800, 400);

    // By default, the tracker runs asynchronously in a background thread. It
    // emits tracker events from the main thread. If you want the tracking to
    // also run in the main thread, set this value to false.
    //
    // settings.async = true;

    // Set up the detector.
    tracker.setup(settings);

    // Add tracking event listeners.
    tracker.registerEvents(this);

    // Set up the video input.
    video.setup(1280, 720);
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
    // Draw the input video.
    ofSetColor(ofColor::white);
    video.draw(0, 0);

    // Draw the ROI.
    ofSetColor(ofColor::white, 127);
    ofxDlib::draw(tracker.settings().inputROI, "ROI");

    // Draw the tracks.
    ofSetColor(ofColor::yellow);
    ofxDlib::draw(tracker.tracks());

    ofDrawBitmapStringHighlight("    App: " + ofToString(ofGetFrameRate(), 2) + " FPS", 20, 20);
    ofDrawBitmapStringHighlight("Tracker: " + ofToString(tracker.fps(), 2) + " FPS", 20, 40);
}


void ofApp::onTrackBegin(ofxDlib::FaceTrackerEventArgs& evt)
{
    std::cout << "Track begin: " << evt.label << std::endl;
}


void ofApp::onTrackUpdate(ofxDlib::FaceTrackerEventArgs& evt)
{
    if (evt.lastSeen != 0)
        std::cout << "Track Update (Lost): " << evt.label << " " << evt.age << " " << evt.lastSeen << std::endl;
    else
        std::cout << "Track Update: " << evt.label << " " << evt.age << std::endl;
}


void ofApp::onTrackEnd(ofxDlib::FaceTrackerEventArgs& evt)
{
    std::cout << "Track end: " << evt.label << std::endl;
}
