//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


class Rectangle
{
public:
    /// \brief The size of the rectangle.
    glm::vec2 size;

    /// \brief The scaled size.
    glm::vec2 scaledSize;

    /// \brief The center of the rectangle.
    glm::vec2 position;

    /// \brief The velocity of the rectangle.
    glm::vec2 velocity;

    /// \brief The color of the rectangle.
    ofColor color;

    /// \brief The id of the rectangle.
    int id = 0;

    /// \brief The angular offset for scaling.
    float angularOffsetRad = 0;

    float offsetMagnitude = 0;

    /// \brief The minimum scale.
    float minScale = 1;
};


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;

    ofxDlib::SmoothTracker<ofRectangle> rectangleTracker;

    std::vector<Rectangle> rectangles;

};
