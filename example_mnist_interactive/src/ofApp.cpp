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
    if (needsPrediction)
    {
        ofPixels pixels;
        drawingArea.readToPixels(pixels);
        predictedLabels = net(dlib::mat(dlib::of_pixels_<unsigned char>(pixels)));

//        );
//// As you can see, a network will greedily assign things given to its
//// constructor to the layers inside itself.  The assignment is done in the
//// order the layers are defined, but it will skip layers where the
//// assignment doesn't make sense.

//// Now let's print the details of the pnet to the screen and inspect it.

//// Now that we know the index numbers for each layer, we can access them
//// individually using layer<index>(pnet).  For example, to access the output
//// tensor for the first prelu layer we can say:
//layer<3>(pnet).get_output();
//// Or to print the prelu parameter for layer 7 we can say:
//cout << "prelu param: "<< layer<7>(pnet).layer_details().get_initial_param_value() << endl;


//        auto v = net.subnet<fc>().get_output();

    }

}


void ofApp::draw()
{
    ofSetColor(255);
    brush.draw(ofGetMouseX() - brushRadius, ofGetMouseY() - brushRadius, brushRadius * 2, brushRadius * 2);



    float xOffset = 0;
    float yOffset = 0;
    float w = 28;
    float h = 28;
//    auto maxPredictionLabel = std::distance(predictedLabels.begin(),
//                                            std::max_element(predictedLabels.begin(),
//                                                             predictedLabels.end()));
    float barWidth = 100;
    for (auto& entry: mnistTrainingData)
    {
        unsigned long label = entry.first;

        std::size_t index = ofGetFrameNum() % entry.second.size();

        ofSetColor(255);
        entry.second[index].draw(xOffset, yOffset, w, h);
//        if (label == maxPredictionLabel) ofSetColor(255, 255, 0);
//        else ofSetColor(255);

//        ofDrawRectangle(xOffset, yOffset, predictedLabels)

        yOffset += h;
    }

}


void ofApp::keyPressed(int key)
{
    if (key == 'c')
    {

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
    auto convertToTextures = [](const decltype(training_images)& images,
                                const decltype(training_labels)& labels,
                                decltype(mnistTrainingData)& textureMap) {
        for (std::size_t i = 0; i < images.size(); ++i)
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
    drawingArea.allocate(28, 28, 1);

    // Reserve some space for the predictions.
    predictedLabels.resize(10);
}
