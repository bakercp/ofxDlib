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
    // MMOD detector. The MMOD detector benefits greatly from hardware
    // acceleration (e.g. MKL, CUDA, etc).
    //
    // settings.detectorType = ofxDlib::FaceDetector::Type::FACE_DETECTOR_MMOD;

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

    //
    settings.trackerPersistence = 60;


    // Set up the detector.
    tracker.setup(settings);

    // Set up the video input.
    video.setup(1280, 720);
    trackBeginListener = tracker.trackBegin.newListener(this, &ofApp::onTrackBegin);
    trackUpdateListener = tracker.trackUpdate.newListener(this, &ofApp::onTrackUpdate);
    trackEndListener = tracker.trackEnd.newListener(this, &ofApp::onTrackEnd);
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

    ofSetColor(ofColor::white);

    float y = 0;
    for (auto&& t: tracked)
    {
        t.second.draw(0, y);
        y += t.second.getHeight();
    }
}


void ofApp::onTrackBegin(ofxDlib::FaceTrackerEventArgs& evt)
{
    std::cout << "Track begin: " << evt.label << std::endl;
    auto iter = tracked.find(evt.label);
    bool success = false;
    if (iter == tracked.end())
        std::tie(iter, success) = tracked.insert(std::make_pair(evt.label, ofTexture()));
    iter->second.loadData(evt.detection.faceChip());
}


void ofApp::onTrackUpdate(ofxDlib::FaceTrackerEventArgs& evt)
{
    std::cout << "Track update: " << evt.label << std::endl;
    auto iter = tracked.find(evt.label);
    bool success = false;
    if (iter == tracked.end())
        std::tie(iter, success) = tracked.insert(std::make_pair(evt.label, ofTexture()));
    iter->second.loadData(evt.detection.faceChip());
}


void ofApp::onTrackEnd(ofxDlib::FaceTrackerEventArgs& evt)
{
    std::cout << "Track end: " << evt.label << std::endl;
    auto iter = tracked.find(evt.label);
    if (iter != tracked.end())
        tracked.erase(iter);
    else
        std::cout << "NO IMAGE TRACKED FOR: " << evt.label << std::endl;
}
