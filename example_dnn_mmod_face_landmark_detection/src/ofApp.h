//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


template <long num_filters, typename SUBNET>
using con5d = dlib::con<num_filters,5,5,2,2,SUBNET>;

template <long num_filters, typename SUBNET>
using con5  = dlib::con<num_filters,5,5,1,1,SUBNET>;

template <typename SUBNET>
using downsampler  = dlib::relu<dlib::affine<con5d<32, dlib::relu<dlib::affine<con5d<32, dlib::relu<dlib::affine<con5d<16,SUBNET>>>>>>>>>;

template <typename SUBNET>
using rcon5 = dlib::relu<dlib::affine<con5<45,SUBNET>>>;

using net_type = dlib::loss_mmod<dlib::con<1,9,9,1,1,rcon5<rcon5<rcon5<downsampler<dlib::input_rgb_image_pyramid<dlib::pyramid_down<6>>>>>>>>;


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;

    net_type net;

    ofImage image;

    std::vector<dlib::mmod_rect> dets;

};
