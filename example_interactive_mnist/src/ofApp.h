//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
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

    void loadData();

    ofxDlib::Network::LeNet net;

    ofFbo drawingArea;
    ofTexture brush;
    float brushRadius = 20;
    bool needsPrediction = false;
    bool needsClear = true;

    std::size_t predictedLabel = 0;
    std::vector<float> lastLayer;
//    std::l
    
    
    // All MNIST data (training and testing) in a map.
    std::map<unsigned long, std::vector<ofTexture>> mnistTrainingData;
    std::map<unsigned long, std::vector<ofTexture>> mnistTestingData;

    enum
    {
        MNIST_WIDTH = 28,
        MNIST_HEIGHT = 28,
        DRAWING_SCALE = 10
    };
};
