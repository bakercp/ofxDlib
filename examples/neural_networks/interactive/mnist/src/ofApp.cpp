//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    ofBackground(0);

    // Load the program data.
    loadData();

    cout << "The pnet has " << net.num_layers << " layers in it." << endl;
    cout << net << endl;

    std::cout << "#layers: " << net.num_layers<< " #complayers: " << net.num_computational_layers << std::endl;

    auto& layer = dlib::layer<ofxDlib::LeNet5::tag_10_con_1, 1>(net);
    auto& layer_details = layer.layer_details();
    auto& layer_output = layer.get_output();
    auto& layer_parameters = layer_details.get_layer_params();

    auto filter_nr = layer_details.nr();
    auto filter_nc = layer_details.nc();
    auto filter_k = 1;
    auto filter_ns = layer_details.num_filters();

    for (std::size_t i = 0; i < filter_ns; ++i)
    {
        std::size_t offset = i * filter_k * filter_nr * filter_nc;
        dlib::alias_tensor a(1, 1, filter_nr, filter_nc);
//        dlib::matrix<float, 5, 5> m = dlib::mat(a(layer_params, offset));
        dlib::matrix<float, 5, 5> m = dlib::mat(a(layer_parameters, offset));
        std::cout << i << ": " << ofToString(std::vector<float>(m.begin(), m.end())) << std::endl;

        auto mm = std::minmax(m.begin(), m.end());
        std::cout << "---- " << *mm.first << "," << *mm.second << std::endl;
        ofFloatPixels p = ofxDlib::toOf(m);
        ofxDlib::map(p, *mm.first, *mm.second);

        layer11Kernels.push_back(ofTexture());
        layer11Kernels.back().loadData(p);
        layer11Kernels.back().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    }
    
    
    
//    auto& layer = dlib::layer<ofxDlib::Network::tlnet8>(net);
//    auto& layer_details = layer.layer_details();
//    auto& layer_parameters = layer_details.get_layer_params();
//
//    auto nr = layer_details.nr();
//    auto nc = layer_details.nc();
//    auto k = 1;
//    auto nf = layer_details.num_filters();
//
//    for (std::size_t i = 0; i < nf; ++i)
//    {
//        std::size_t offset = i * k * nr * nc;
//        dlib::alias_tensor a(1, 1, nr, nc);
//        //        dlib::matrix<float, 5, 5> m = dlib::mat(a(layer_params, offset));
//        dlib::matrix<float, 5, 5> m = dlib::mat(a(layer_parameters, offset));
//        std::cout << i << ": " << ofToString(std::vector<float>(m.begin(), m.end())) << std::endl;
//
//        auto mm = std::minmax_element(m.begin(), m.end());
//        std::cout << "---- " << *mm.first << "," << *mm.second << std::endl;
//        std::cout << std::endl;
//        ofFloatPixels p = ofxDlib::toOf(m);
//        ofxDlib::map(p, *mm.first, *mm.second);
//
//        layer11Kernels.push_back(ofTexture());
//        layer11Kernels.back().loadData(p);
//        layer11Kernels.back().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
//    }
////    n_*k_*nr_*nc_
//
////    dlib::alias_tensor
//
}
//
//
void ofApp::update()
{
    if (needsPrediction)
    {
        if (needsClear)
        {
            drawingArea.begin();
            ofClear(0);
            drawingArea.end();
            needsClear = false;
        }

        ofPixels pixels;
        drawingArea.readToPixels(pixels);
        pixels = ofxDlib::toGrayscale(pixels);
        pixels.resize(MNIST_WIDTH, MNIST_HEIGHT);
        auto dlib_img = ofxDlib::toDlib<unsigned char>(pixels);
        dlib::matrix<unsigned char> dlib_mat = dlib::mat(dlib_img);

        predictedLabel = net(dlib_mat);


//        dlib::resizable_tensor p = dlib::layer<11>(net).layer_details().
//
//        dlib::alias_tensor_instance t = dlib::layer<1>(net).layer_details().get_weights();
        dlib::resizable_tensor t = dlib::layer<1>(net).get_output();

//        dlib::resizable_tensor t0 = dlib::layer<11>(net).layer_details().get_layer_params();

//        std::cout << "lp: " << ofToString(std::vector<float>(t0.begin(), t0.end())) << std::endl;

        lastLayer = std::vector<float>(t.begin(), t.end());


        using namespace ofx::Dlib;
        using namespace ofx::Dlib::LeNet5;
  
        
//        layer11Convolutions = GetLayerOutputsAsTextures<tag_7_con_0, 14, 14>(net);
        layer11Convolutions = layerOutputsToTextures<tag_9_relu_3, 28, 28>(net);
//        layer11Convolutions = GetLayerOutputsAsTextures<tag_10_con_1, 28, 28>(net);

        
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
    
    {
    ofPushMatrix();
    ofTranslate(ofGetMouseX(), ofGetMouseY());
    float yy = 0;
    float v = 30;
    for (auto& t: layer11Kernels)
    {
        ofSetColor(255);
        t.draw(0, yy, v, v);
        yy += (v * 1.1);
    }
    
    ofPopMatrix();
    }
    
    
    {
        ofPushMatrix();
        ofTranslate(ofGetMouseX() + 50, ofGetMouseY());
        float yy = 0;
        float v = 56;
        for (auto& t: layer11Convolutions)
        {
            ofSetColor(255);
            t.draw(0, yy, v, v);
            yy += (v * 1.1);
        }
        
        ofPopMatrix();
    }

    
}


void ofApp::keyPressed(int key)
{
    if (key == 'c')
    {
        needsClear = true;
        needsPrediction = true;
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
    dlib::deserialize(ofToDataPath("mnist_network_tagged.dat", true)) >> net;

//    ofxDlib::Network::LeNet _net;
//    dlib::deserialize(ofToDataPath("mnist_network.dat", true)) >> _net;
//
//    net = _net;
//
//    _net = net;

    
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

    drawingArea.begin();
    ofClear(0);
    drawingArea.end();

    // Make sure it's not empty to start.
    lastLayer.resize(10);
}
