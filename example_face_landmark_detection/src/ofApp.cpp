//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Set up a timing mechanism for benchmarking.
    auto start = std::chrono::system_clock::now();

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

    ofPixels p;
    ofxDlib::toOf(img, p);
    image.setFromPixels(p);

    double detScale = 1;

    // Make the image larger so we can detect small faces.
    while (img.size() < 2400 * 2400)
    {
        dlib::pyramid_up(img);
        detScale *= 2;
    }

    // Now tell the face detector to give us a list of bounding boxes
    // around all the faces in the image.
    std::vector<dlib::rectangle> dets = detector(img);

    // Finish benchmarking.
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "Finished computation at " << std::ctime(&end_time);
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s";
    std::cout << std::endl;

    std::cout << "Number of faces detected: " << dets.size() << std::endl;
    // Now we will go ask the shape_predictor to tell us the pose of
    // each face we detected.

    for (std::size_t j = 0; j < dets.size(); ++j)
    {
        dlib::full_object_detection shape = sp(img, dets[j]);
        shapes.push_back(shape);
    }

    // We can also extract copies of each face that are cropped, rotated upright,
    // and scaled to a standard size as shown here:

    std::vector<dlib::chip_details> chipDetails = dlib::get_face_chip_details(shapes);

    dlib::array<dlib::array2d<dlib::rgb_pixel>> face_chips;

    dlib::extract_image_chips(img,
                              chipDetails,
                              face_chips);

    for (auto& f: face_chips)
    {
        ofPixels p;
        ofxDlib::toOf(f, p);
        p.resize(p.getWidth() / detScale, p.getHeight() / detScale);
        faceChips.push_back(ofImage(p));
    }

    for (auto& shape: shapes)
    {
        ofxDlib::scale(shape, 1.0f / detScale);
    }
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
            ofDrawCircle(ofxDlib::toOf(shape.part(i)), 5);
        }
    }

    float x = 0;
    float y = 0;

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

