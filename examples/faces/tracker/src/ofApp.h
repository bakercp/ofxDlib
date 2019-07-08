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

    void onTrackBegin(ofxDlib::FaceTrackerEventArgs& evt);
    void onTrackUpdate(ofxDlib::FaceTrackerEventArgs& evt);
    void onTrackEnd(ofxDlib::FaceTrackerEventArgs& evt);

    // The video grabber.
    ofVideoGrabber video;

    // The face detector.
    ofxDlib::FaceTracker tracker;

    ofEventListener trackEventListener;
    ofEventListener trackBeginListener;
    ofEventListener trackUpdateListener;
    ofEventListener trackEndListener;

    std::map<std::size_t, ofTexture> tracked;

};
