//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"




void ofApp::setup()
{
    grabber.setup(640, 480);
}


void ofApp::update()
{
    grabber.update();

    if (grabber.isFrameNew())
    {
        ofPixels pixels;
        dlib::gaussian_blur(grabber.getPixels(), pixels, 3);
        blurredImage.setFromPixels(pixels);
    }
}


void ofApp::draw()
{
    grabber.draw(0, 0);
    blurredImage.draw(grabber.getWidth(), 0);
}

