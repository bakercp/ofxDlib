//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Load the net.
    deserialize(ofToDataPath("mmod_network.dat", true)) >> net;

    // Initialize the camera.
    grabber.setup(640, 480);
}


void ofApp::update()
{
    grabber.update();
    
    if (grabber.isFrameNew())
    {
        dlib::matrix<dlib::rgb_pixel> img = dlib::mat(ofxDlib::toDlib<dlib::rgb_pixel>(grabber.getPixels()));
        auto dets = net(img);
        
        std::cout << dets.size() << std::endl;
        
        detections.clear();
        for (auto&& d : dets)
            detections.push_back(ofxDlib::toOf(d));

    }
}

    
void ofApp::draw()
{
    ofFill();
    ofSetColor(255);
    grabber.draw(0, 0);
    
    ofSetColor(255, 255, 0);
    ofNoFill();
    for (auto rect: detections)
        ofDrawRectangle(rect);
}
