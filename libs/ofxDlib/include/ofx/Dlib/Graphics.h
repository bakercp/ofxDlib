//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "ofx/Dlib/FaceDetector.h"


namespace ofx {
namespace Dlib {


inline void draw(const ofRectangle& rectangle, const std::string& label, float labelFillWidth = 1.0)
{
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(2);
    ofDrawRectangle(rectangle);
    ofFill();
    ofDrawRectangle(rectangle.getLeft(),
                    rectangle.getBottom(),
                    rectangle.getWidth(),
                    20);

    ofNoFill();
    ofDrawRectangle(rectangle.getLeft(),
                    rectangle.getBottom(),
                    rectangle.getWidth(),
                    20);

    // Get an inverted color for the text.
    ofSetColor(ofGetStyle().color.getInverted());
    ofDrawBitmapString(label,
                       rectangle.getLeft() + 6,
                       rectangle.getBottom() + 14);

    ofPopStyle();
}

inline void draw(const ObjectDetection& detection)
{
    draw(detection.rectangle, "Confidence: " + ofToString(detection.confidence, 2));
}


template <typename Type>
inline void draw(const std::vector<Type>& collection)
{
    for (auto&& item: collection)
        draw(item);
}


} } // namespace ofx::Dlib
