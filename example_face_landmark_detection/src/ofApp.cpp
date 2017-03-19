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
    // loading the model from the shape_predictor_68_face_landmarks.dat file you gave
    // as a command line argument.
    dlib::shape_predictor sp;
    
    dlib::deserialize(ofToDataPath("shape_predictor_68_face_landmarks.dat", true)) >> sp;

    dlib::array2d<dlib::rgb_pixel> img;

    dlib::load_image(img, ofToDataPath("Crowd.jpg", true));

    // Make the image larger so we can detect small faces.
    dlib::pyramid_up(img);

    // Now tell the face detector to give us a list of bounding boxes
    // around all the faces in the image.
    std::vector<dlib::rectangle> dets = detector(img);

    std::cout << "Number of faces detected: " << dets.size() << std::endl;

    // Now we will go ask the shape_predictor to tell us the pose of
    // each face we detected.
    std::vector<dlib::full_object_detection> shapes;

    for (auto j = 0; j < dets.size(); ++j)
    {
        dlib::full_object_detection shape = sp(img, dets[j]);

        std::cout << "number of parts: "<< shape.num_parts() << std::endl;
        std::cout << "pixel position of first part:  " << shape.part(0) << std::endl;
        std::cout << "pixel position of second part: " << shape.part(1) << std::endl;
        // You get the idea, you can get all the face part locations if
        // you want them.  Here we just store them in shapes so we can
        // put them on the screen.
        shapes.push_back(shape);
    }

    // Now let's view our face poses on the screen.
//    win.clear_overlay();
//    win.set_image(img);
//    win.add_overlay(render_face_detections(shapes));

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
        ofSaveImage(p, ofGetTimestampString() + "_" + std::to_string(ofRandom(1)) + ".jpg");
    }

//    for (auto& chip: face_chips)
//    {
//        dlib::serial
//    }

}


void ofApp::draw()
{
    ofBackground(0);
    ofNoFill();
    ofSetColor(ofColor::white);

//    ofPushMatrix();
//    ofScale(0.5);

    texture.draw(0, 0);

    for (auto& r: rectangles)
    {
        std::stringstream ss;
        ss << "Confidence: " << r.detection_confidence << std::endl;
        ss << "Ignore: " << r.ignore;

        ofRectangle rect = toOf(r);

//        rect.x *= scale;
//        rect.y *= scale;
//        rect.width *= scale;
//        rect.height *= scale;

        ofDrawRectangle(rect);
        ofPopMatrix();
        ofDrawBitmapString(ss.str(), rect.getCenter());
    }

//    ofPopMatrix();
}

