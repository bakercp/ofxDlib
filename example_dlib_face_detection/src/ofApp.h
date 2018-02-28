//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


// From: http://dlib.net/face_detection_ex.cpp.html

// This face detector is made using the now classic Histogram of Oriented
// Gradients (HOG) feature combined with a linear classifier, an image pyramid,
// and sliding window detection scheme.  This type of object detector is fairly
// general and capable of detecting many types of semi-rigid objects in
// addition to human faces.  Therefore, if you are interested in making your
// own object detectors then read the fhog_object_detector_ex.cpp example
// program.  It shows how to use the machine learning tools which were used to
// create dlib's face detector.

// Finally, note that the face detector is fastest when compiled with at least
// SSE2 instructions enabled.

#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;

    // Our image.
    ofImage image;

    // Our face detection bounding boxes.
    std::vector<dlib::rectangle> faceRects;

};
