//
// Copyright (c) 2017 Dario Longobardi
//
// License:	MIT
//

//
// From dnn_semantic_segmentation_ex.cpp 
//
/*
    Semantic segmentation using the PASCAL VOC2012 dataset.

    In segmentation, the task is to assign each pixel of an input image
    a label - for example, 'dog'.  Then, the idea is that neighboring
    pixels having the same label can be connected together to form a
    larger region, representing a complete (or partially occluded) dog.
    So technically, segmentation can be viewed as classification of
    individual pixels (using the relevant context in the input images),
    however the goal usually is to identify meaningful regions that
    represent complete entities of interest (such as dogs).

    Instructions how to run the example:
    1. Download the PASCAL VOC2012 data, and untar it somewhere.
       http://host.robots.ox.ac.uk/pascal/VOC/voc2012/VOCtrainval_11-May-2012.tar
    2. Build the dnn_semantic_segmentation_train_ex example program.
    3. Run:
       ./dnn_semantic_segmentation_train_ex /path/to/VOC2012
    4. Wait while the network is being trained.
    5. Build the dnn_semantic_segmentation_ex example program.
    6. Run:
       ./dnn_semantic_segmentation_ex /path/to/VOC2012-or-other-images

    An alternative to steps 2-4 above is to download a pre-trained network
    from here: http://dlib.net/files/semantic_segmentation_voc2012net.dnn

    It would be a good idea to become familiar with dlib's DNN tooling before reading this
    example.  So you should read dnn_introduction_ex.cpp and dnn_introduction2_ex.cpp
    before reading this example program.
*/


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"

class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;
    ofImage image;
    ofImage seg;
    ofImage seg_path;

    dlib::matrix<uint16_t> index_label_image;
    dlib::matrix<dlib::rgb_pixel> rgb_label_image;
    string classlabel;
};
