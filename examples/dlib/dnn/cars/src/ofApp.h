//
// Copyright (c) 2017 Dario Longobardi
//
// License:	MIT
//

//
// From dnn_mmod_find_cars_ex.cpp 
//

#pragma once


#include "ofMain.h"
#include "ofxDlib.h"

using namespace dlib;

// The rear view vehicle detector network
template <long num_filters, typename SUBNET> using con5d = con<num_filters,5,5,2,2,SUBNET>;
template <long num_filters, typename SUBNET> using con5  = con<num_filters,5,5,1,1,SUBNET>;
template <typename SUBNET> using downsampler  = relu<affine<con5d<32, relu<affine<con5d<32, relu<affine<con5d<16,SUBNET>>>>>>>>>;
template <typename SUBNET> using rcon5  = relu<affine<con5<55,SUBNET>>>;
using net_type = loss_mmod<con<1,9,9,1,1,rcon5<rcon5<rcon5<downsampler<input_rgb_image_pyramid<pyramid_down<6>>>>>>>>;


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;
    ofImage image;
    std::vector<ofRectangle> rectof;
};
