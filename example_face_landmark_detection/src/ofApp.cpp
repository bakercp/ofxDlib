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

    // And we also need a shape_predictor.  This is the tool that will predict face
    // landmark positions given an image and face bounding box.  Here we are just
    // loading the model from the shape_predictor_68_face_landmarks.dat.
    dlib::shape_predictor sp;
    
    dlib::deserialize(ofToDataPath("shape_predictor_68_face_landmarks.dat", true)) >> sp;

    dlib::array2d<dlib::rgb_pixel> img;

    dlib::load_image(img, ofToDataPath("Crowd.jpg", true));

    // Make the image larger so we can detect small faces.
    dlib::pyramid_up(img);

    ofPixels pixels;
    toOf(img, pixels);
    image.setFromPixels(pixels);

    // Now tell the face detector to give us a list of bounding boxes
    // around all the faces in the image.
    dets = detector(img);

    std::cout << "Number of faces detected: " << dets.size() << std::endl;
    // Now we will go ask the shape_predictor to tell us the pose of
    // each face we detected.

    for (auto j = 0; j < dets.size(); ++j)
    {
        dlib::full_object_detection shape = sp(img, dets[j]);
        // std::cout << "number of parts: "<< shape.num_parts() << std::endl;
        // std::cout << "pixel position of first part:  " << shape.part(0) << std::endl;
        // std::cout << "pixel position of second part: " << shape.part(1) << std::endl;
        // You get the idea, you can get all the face part locations if
        // you want them.  Here we just store them in shapes so we can
        // put them on the screen.
        shapes.push_back(shape);
    }

    // We can also extract copies of each face that are cropped, rotated upright,
    // and scaled to a standard size as shown here:
    dlib::array<dlib::array2d<dlib::rgb_pixel>> face_chips;

    dlib::extract_image_chips(img,
                              dlib::get_face_chip_details(shapes),
                              face_chips);

    for (auto& f: face_chips)
    {
        ofPixels p;
        toOf(f, p);
        facechips.push_back(ofImage(p));
    }

}


void ofApp::draw()
{
    ofBackground(0);
    ofNoFill();
    ofSetColor(ofColor::white);

    image.draw(0, 0);

    for (auto& r: dets)
    {
        ofSetColor(ofColor::red);
        ofDrawRectangle(toOf(r));
    }

    for (auto& shape: shapes)
    {
        ofSetColor(ofColor::yellow);
        ofDrawRectangle(toOf(shape.get_rect()));
        for (std::size_t i = 0; i < shape.num_parts(); ++i)
        {
            ofDrawCircle(toOf(shape.part(i)), 5);
        }
    }
}

