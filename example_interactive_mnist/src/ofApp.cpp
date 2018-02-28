//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    ofBackground(0);

    // Load the program data.
    loadData();

//    cout << "The pnet has " << net.num_layers << " layers in it." << endl;
//    cout << net << endl;


}


void ofApp::update()
{
    if (needsClear)
    {
        drawingArea.begin();
        ofClear(0);
        drawingArea.end();
        needsClear = false;
    }

    if (needsPrediction)
    {
        ofPixels pixels;
        drawingArea.readToPixels(pixels);
        pixels = ofxDlib::toGrayscale(pixels);
        pixels.resize(MNIST_WIDTH, MNIST_HEIGHT);
        auto dlib_img = ofxDlib::toDlib<unsigned char>(pixels);
        dlib::matrix<unsigned char> dlib_mat = dlib::mat(dlib_img);

        predictedLabel = net(dlib_mat);
        
        dlib::resizable_tensor t = dlib::layer<1>(net).get_output();


        dlib::resizable_tensor t = dlib::layer<1>(net).get_output();

        
        //        std::cout << "size: " << t.size() << std::endl;
        //        std::cout << "num_samples: " << t.num_samples() << std::endl;
        //        std::cout << "nr: " << t.nr() << std::endl;
        //        std::cout << "nc: " << t.nc() << std::endl;
        //        std::cout << "k: " << t.k() << std::endl;

        
        
        lastLayer = std::vector<float>(t.begin(), t.end());

        needsPrediction = false;
    }
}


void ofApp::draw()
{
    ofSetColor(255);

    ofRectangle drawingAreaDisplay(10,
                                   10,
                                   drawingArea.getWidth(),
                                   drawingArea.getHeight());

    float x = ofGetMouseX() - drawingAreaDisplay.x;
    float y = ofGetMouseY() - drawingAreaDisplay.y;

    if (ofGetMousePressed())
    {
        drawingArea.begin();
        ofSetColor(255);
        ofFill();
        ofDrawCircle(x, y, brushRadius);
        drawingArea.end();
        needsPrediction = true;
    }
    else if (drawingAreaDisplay.inside(ofGetMouseX(), ofGetMouseY()))
    {
        ofSetColor(255, 80);
        ofFill();
        ofDrawCircle(x + drawingAreaDisplay.x,
                     y + drawingAreaDisplay.y, brushRadius);
    }
    
    ofSetColor(255);
    ofFill();
    
    ofPushMatrix();
    ofTranslate(0, 0);
    drawingArea.draw(drawingAreaDisplay);
    ofPopMatrix();
    
    ofSetColor(255, 127);
    ofNoFill();
    ofDrawRectangle(drawingAreaDisplay);
    
    
    ofPushMatrix();
    ofTranslate(drawingAreaDisplay.x + drawingAreaDisplay.width,
                drawingAreaDisplay.y);
    
    float xOffset = 0;
    float yOffset = 0;

    float barWidth = 100;
    for (auto& entry: mnistTrainingData)
    {
        unsigned long label = entry.first;

        std::size_t index = ofGetFrameNum() % entry.second.size();

        ofSetColor(255);
        ofFill();
        entry.second[index].draw(xOffset, yOffset, MNIST_WIDTH, MNIST_HEIGHT);
        
        if (label == predictedLabel) ofSetColor(255, 255, 0);
        else ofSetColor(255);
        
        float value = ofMap(lastLayer[label], 0, 30, 0, 100, true);
        
        if (fabs(value) > 0.01)
        {
            ofDrawRectangle(MNIST_WIDTH + xOffset, yOffset, value, MNIST_HEIGHT);
        }

        yOffset += MNIST_HEIGHT;
    }

    ofPopMatrix();
}


void ofApp::keyPressed(int key)
{
    if (key == 'c')
    {
        needsClear = true;
    }
    else if (key == '-')
    {
        brushRadius = std::max(1.0f, brushRadius - 1.0f);
    }
    else if (key == '=')
    {
        brushRadius = std::min(50.0f, brushRadius + 1.0f);
    }
}


void ofApp::loadData()
{
    // Load trained network data.
    dlib::deserialize(ofToDataPath("mnist_network.dat", true)) >> net;

    // MNIST is broken into two parts, a training set of 60000 images and a test set of
    // 10000 images.  Each image is labeled so that we know what hand written digit is
    // depicted.  These next statements load the dataset into memory.
    std::vector<dlib::matrix<unsigned char>> training_images;
    std::vector<unsigned long> training_labels;
    std::vector<dlib::matrix<unsigned char>> testing_images;
    std::vector<unsigned long> testing_labels;

    load_mnist_dataset(ofToDataPath("mnist/", true),
                       training_images,
                       training_labels,
                       testing_images,
                       testing_labels);

    // Create a simple lambda function for converting the data from dlib::matrix
    // to ofTexture for viewing.

    const std::size_t maxImagesToLoad = 500;

    auto convertToTextures = [&](const decltype(training_images)& images,
                                 const decltype(training_labels)& labels,
                                 decltype(mnistTrainingData)& textureMap) {
        for (std::size_t i = 0; i < std::min(maxImagesToLoad, images.size()); ++i)
        {
            ofTexture tex;
            tex.loadData(ofxDlib::toOf(images[i]));
            unsigned long label = labels[i];
            auto iter = textureMap.find(label);
            if (iter != textureMap.end()) iter->second.push_back(tex);
            else textureMap[label] = { tex };
        }
    };

    convertToTextures(training_images, training_labels, mnistTrainingData);
    convertToTextures(testing_images, testing_labels, mnistTestingData);

    // Try to load a brush.
    if (!ofLoadImage(brush, "brush.png"))
    {
        // Create a brush.
        ofPixels brushPixels;
        std::size_t w = 128, h = 128;
        brushPixels.allocate(w, h, OF_PIXELS_RGBA);
        glm::vec2 center = { w / 2, h / 2 };
         for (std::size_t x = 0; x < w; ++x)
            for (std::size_t y = 0; y < h; ++y)
                brushPixels.setColor(x, y, ofColor(255, ofMap(glm::distance(glm::vec2(x, y), center), 0, center.x, 255, 0, true)));

        brush.loadData(brushPixels);
        ofSaveImage(brushPixels, "brush.png");
    }

    // Allocate a drawing area.
    drawingArea.allocate(MNIST_WIDTH * DRAWING_SCALE,
                         MNIST_HEIGHT * DRAWING_SCALE,
                         GL_RGBA);

    // Make sure it's not empty to start.
    lastLayer.resize(10);
}
