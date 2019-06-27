//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    glm::vec2 center = { ofGetWidth() / 2, ofGetHeight() / 2 };

    for (int i = 0; i < 10; ++i)
    {
        Rectangle rectangle;
        rectangle.size = { ofRandom(30, 100), ofRandom(30, 100) };
        rectangle.scaledSize = rectangle.size;
        rectangle.position = center;
        rectangle.velocity = { ofRandom(-2, 2), ofRandom(-2, 2) };
        rectangle.color = ofColor::fromHsb(ofRandom(255), 255, 255);
        rectangle.id = i;
        rectangle.angularOffsetRad = ofRandom(0, glm::pi<float>());
        rectangle.offsetMagnitude = ofRandom(0.1, 1);
        rectangle.minScale = ofRandom(.1, 1);
        rectangles.push_back(rectangle);
    }

    ofSetBackgroundColor(0);
}


void ofApp::update()
{
    std::vector<ofRectangle> rects;

    for (auto& r: rectangles)
    {
        float scale = ofMap(std::sin(r.offsetMagnitude * ofGetElapsedTimef() + r.angularOffsetRad), -1, 1, r.minScale, 1);

        r.scaledSize = r.size * scale;

        r.position += r.velocity;

        // Check right and left walls.
        if (r.position.x < 0 || r.position.x > ofGetWidth())
            r.velocity.x *= -1;

        // Check top and bottom walls.
        if (r.position.y < 0 || r.position.y > ofGetHeight())
            r.velocity.y *= -1;

        ofRectangle z(r.position - r.scaledSize / 2,
                      r.position + r.scaledSize / 2);

        rects.push_back(z);
    }

    rectangleTracker.track(rects);
}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    ofFill();
    for (auto& r: rectangles)
    {
        ofSetColor(r.color);
        ofDrawRectangle(ofRectangle(r.position - r.scaledSize / 2,
                                    r.position + r.scaledSize / 2));
    }

    // Draw the original ids.
    for (auto& r: rectangles)
        ofDrawBitmapStringHighlight(ofToString(r.id), r.position);


    // Draw the tracked ids.
    ofNoFill();
    ofSetColor(255);
    for (auto& label: rectangleTracker.currentLabels())
    {
        auto r = rectangleTracker.getCurrent(label);
        auto l = rectangleTracker.getAge(label);

        ofDrawRectangle(r);
        ofDrawBitmapStringHighlight(ofToString(l) + "\n" + ofToString(label), r.position);
    }
}
