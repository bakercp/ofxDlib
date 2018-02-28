//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // We need a face detector. We will use this to get bounding boxes for
    // each face in an image.
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

    ofPixels pix;
    ofLoadImage(pix, "people.jpg");

    // Make the image larger so we can detect small faces.
    dlib::pyramid_up(pix);

    // Now tell the face detector to give us a list of bounding boxes
    // around all the faces in the image.
    std::vector<dlib::rectangle> dets = detector(pix);

    std::cout << "Number of faces detected: " << dets.size() << std::endl;

    // And we also need a shape_predictor.  This is the tool that will predict face
    // landmark positions given an image and face bounding box.  Here we are just
    // loading the model from the shape_predictor_68_face_landmarks.dat.
    dlib::shape_predictor sp;

    // Load the face landmark detector.
    dlib::deserialize(ofToDataPath("shape_predictor_68_face_landmarks.dat", true)) >> sp;

    // You can also try the 5 landmark detector, which is accurate and fast.
    // dlib::deserialize(ofToDataPath("shape_predictor_5_face_landmarks.dat", true)) >> sp;

    // Now we will go ask the shape_predictor to tell us the pose of
    // each face we detected.
    for (std::size_t j = 0; j < dets.size(); ++j)
    {
        dlib::full_object_detection shape = sp(pix, dets[j]);
        shapes.push_back(shape);
    }

    // We can also extract copies of each face that are cropped, rotated upright,
    // and scaled to a standard size as shown here:
    std::vector<dlib::chip_details> chipDetails = dlib::get_face_chip_details(shapes);

    // Create a collection of faces.
    dlib::array<ofPixels> face_chips;

    dlib::extract_image_chips(pix, chipDetails, face_chips);

    for (auto& f: face_chips)
    {
        faceChips.push_back(ofImage(f));
    }

    // Load the image intou our pixels.
    image.setFromPixels(pix);
}


void ofApp::draw()
{
    ofBackground(0);
    ofNoFill();
    ofSetColor(ofColor::white);

    ofPushMatrix();

    image.draw(0, 0);

    for (auto& shape: shapes)
    {
        ofSetColor(ofColor::yellow);
        ofDrawRectangle(ofxDlib::toOf(shape.get_rect()));

        for (std::size_t i = 0; i < shape.num_parts(); ++i)
        {
            ofDrawCircle(ofxDlib::toOf(shape.part(i)), 2);
        }
    }

    float x = 0;
    float y = 0;

    ofScale(0.5, 0.5);

    for (std::size_t i = 0; i < faceChips.size(); ++i)
    {
        auto& face = faceChips[i];

        if (i != 0 && x + face.getWidth() > ofGetWidth())
        {
            y += face.getHeight();
            x = 0;
        }

        ofSetColor(ofColor::white);
        face.draw(x, y);

        x += face.getWidth();
    }

    ofPopMatrix();
}

