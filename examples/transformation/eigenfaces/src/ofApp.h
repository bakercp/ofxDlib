//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;

    // The analysis.
    ofxDlib::PCA<float> pca;

    // The mean image we calculate.
    ofTexture meanTexture;

    // The eigen images.
    std::vector<ofTexture> eigenTextures;

    // Computed image.
    ofTexture computedTexture;

    // Our for image combinations.
    std::vector<float> weights;
    bool recomputeImage = true;

    // The data describing our images.
    std::size_t imageWidth = 0;
    std::size_t imageHeight = 0;
    ofPixelFormat imageFormat = OF_PIXELS_UNKNOWN;

    enum
    {
        // The total number of eigen images we will calculate.
        MAX_PRINCIPAL_COMPONENTS = 0,

        MIN_WEIGHT = -127,
        MAX_WEIGHT = 127,
        SLIDER_HEIGHT = 24
    };

};
