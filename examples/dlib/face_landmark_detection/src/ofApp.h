//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


// This example program shows how to find frontal human faces in an image and
// estimate their pose.  The pose takes the form of 68 landmarks.  These are
// points on the face such as the corners of the mouth, along the eyebrows, on
// the eyes, and so forth.

// The face detector we use is made using the classic Histogram of Oriented
// Gradients (HOG) feature combined with a linear classifier, an image pyramid,
// and sliding window detection scheme.  The pose estimator was created by
// using dlib's implementation of the paper:
//    One Millisecond Face Alignment with an Ensemble of Regression Trees by
//    Vahid Kazemi and Josephine Sullivan, CVPR 2014
// and was trained on the iBUG 300-W face landmark dataset (see
// https://ibug.doc.ic.ac.uk/resources/facial-point-annotations/):
//    C. Sagonas, E. Antonakos, G, Tzimiropoulos, S. Zafeiriou, M. Pantic.
//    300 faces In-the-wild challenge: Database and results.
//    Image and Vision Computing (IMAVIS), Special Issue on Facial Landmark Localisation "In-The-Wild". 2016.
// You can get the trained model file from:
// http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2.
// Note that the license for the iBUG 300-W dataset excludes commercial use.
// So you should contact Imperial College London to find out if it's OK for
// you to use this model file in a commercial product.

// Also, note that you can train your own models using dlib's machine learning
// tools.  See train_shape_predictor_ex.cpp to see an example.


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;

    ofImage image;

    std::vector<dlib::full_object_detection> shapes;

    std::vector<ofImage> faceChips;

};
