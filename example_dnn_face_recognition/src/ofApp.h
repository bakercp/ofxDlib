//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


//
// From http://dlib.net/dnn_face_recognition_ex.cpp.html
//
// This is an example illustrating the use of the deep learning tools from the dlib C++
// Library.  In it, we will show how to do face recognition.  This example uses the
// pretrained dlib_face_recognition_resnet_model_v1 model which is freely available from
// the dlib web site.  This model has a 99.38% accuracy on the standard LFW face
// recognition benchmark, which is comparable to other state-of-the-art methods for face
// recognition as of February 2017.
//
// In this example, we will use dlib to do face clustering.  Included in the examples
// folder is an image, bald_guys.jpg, which contains a bunch of photos of action movie
// stars Vin Diesel, The Rock, Jason Statham, and Bruce Willis.   We will use dlib to
// automatically find their faces in the image and then to automatically determine how
// many people there are (4 in this case) as well as which faces belong to each person.
//
// Finally, this example uses a network with the loss_metric loss.  Therefore, if you want
// to learn how to train your own models, or to get a general introduction to this loss
// layer, you should read the dnn_metric_learning_ex.cpp and
// dnn_metric_learning_on_images_ex.cpp examples.
//


#include "ofMain.h"
#include "ofxDlib.h"
#include "DNN.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;
    void keyPressed(int key) override;

    anet_type net;

    dlib::shape_predictor sp;
    dlib::frontal_face_detector detector;

//    ofImage img;
    std::vector<ofRectangle> detectedFaces;
    
//    anet_type net;
//    dlib::softmax<anet_type::subnet_type> snet;
//    std::vector<string> labels;
//    dlib::rand rnd;


};
