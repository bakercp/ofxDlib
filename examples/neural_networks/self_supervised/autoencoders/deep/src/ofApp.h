//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


// Auto encoders need:
//
// 1. Encoder
// 2. Decoder
// 3. Loss function.

// 1. Data de-noising.
// 2. Dimensionality reduction.


template<typename SUBNET>
using decoded_t = dlib::fc<784, dlib::sig<
                  dlib::fc<128, dlib::relu<
                  dlib::fc<64, dlib::relu<SUBNET>>>>>>;

template<typename SUBNET>
using encoded_t = dlib::fc<32, dlib::relu<
                  dlib::fc<128, dlib::relu<
                  dlib::fc<784, dlib::relu<SUBNET>>>>>>;

using input_t = dlib::input<dlib::matrix<float>>;

using autoencoder = dlib::loss_mean_squared_multioutput<decoded_t<encoded_t<input_t>>>;

using encoder = encoded_t<input_t>;
using decoder = decoded_t<input_t>;

class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;
    
    autoencoder net;

    uint64_t delay = 1000;
    uint64_t lastUpdate = 0;
    
    std::vector<dlib::matrix<float>> trainingImages;
    std::vector<dlib::matrix<float>> testingImages;

    std::vector<ofTexture> testingTex;
    std::vector<ofTexture> encodedTex;
    
    std::size_t testingImageOffset = 0;
};
