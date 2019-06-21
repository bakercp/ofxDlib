//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // We need a face detector. We will use this to get bounding boxes for
    // each face in an image.
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

    // Allocate some pixels.
    ofPixels pixels;

    // Load an image.
    ofLoadImage(pixels, "people.jpg");

    // Make the image bigger by a factor of two.  This is useful since
    // the face detector looks for faces that are about 80 by 80 pixels
    // or larger.  Therefore, if you want to find faces that are smaller
    // than that then you need to upsample the image as we do here by
    // calling pyramid_up().  So this will allow it to detect faces that
    // are at least 40 by 40 pixels in size.  We could call pyramid_up()
    // again to find even smaller faces, but note that every time we
    // upsample the image we make the detector run slower since it must
    // process a larger image.
    dlib::pyramid_up(pixels);

    // Now tell the face detector to give us a list of bounding boxes
    // around all the faces in the image.
    faceRects = detector(pixels);

    // Set the image from the pixels.
    image.setFromPixels(pixels);

}


void ofApp::draw()
{
    ofBackground(0);
    ofNoFill();
    ofSetColor(ofColor::white);

    image.draw(0, 0);

    for (auto& faceRect: faceRects)
    {
        ofSetColor(ofColor::yellow);
        ofDrawRectangle(ofxDlib::toOf(faceRect));
    }

    ofDrawBitmapStringHighlight("Num. faces detected: " + ofToString(faceRects.size()), 14, 20);
}
