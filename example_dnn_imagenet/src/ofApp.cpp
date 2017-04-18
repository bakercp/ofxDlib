//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Load the resnet DNN.
    dlib::deserialize(ofToDataPath("resnet34_1000_imagenet_classifier.dnn", true)) >> net >> labels;

    // Make a network with softmax as the final layer.  We don't have to do this
    // if we just want to output the single best prediction, since the anet_type
    // already does this.  But if we instead want to get the probability of each
    // class as output we need to replace the last layer of the network with a
    // softmax layer, which we do as follows:
    dlib::softmax<anet_type::subnet_type> snet;
    snet.subnet() = net.subnet();

    // Load paths to a bunch of images.
    for (auto& p: std::filesystem::directory_iterator(ofToDataPath("256_ObjectCategories_Sample_Flat", true)))
    {
        imagePaths.push_back(p);
    }
}


void ofApp::update()
{
    if (loadNext)
    {
        dlib::array<dlib::matrix<dlib::rgb_pixel>> images;

        dlib::matrix<dlib::rgb_pixel> img;

        dlib::load_image(img, imagePaths[currentImageIndex].string());

        const int num_crops = 16;

        // Grab 16 random crops from the image.  We will run all of them through the
        // network and average the results.
        randomly_crop_images(img, images, rnd, num_crops);

        // p(i) == the probability the image contains object of class i.
        dlib::matrix<float,1,1000> p = dlib::sum_rows(dlib::mat(snet(images.begin(), images.end()))) / num_crops;

        // Print the 5 most probable labels
        for (int k = 0; k < 5; ++k)
        {
            unsigned long predicted_label = dlib::index_of_max(p);

            std::cout << p(predicted_label) << ": " << labels[predicted_label] << endl;

            p(predicted_label) = 0; // set it to zero so we find the next with index_of_max.
        }

        currentImage.loadData(ofxDlib::toOf(img));
        currentImageIndex = (currentImageIndex + 1) % imagePaths.size();

        loadNext = false;
    }
}


void ofApp::draw()
{
    currentImage.draw(0, 0);
}


void ofApp::keyPressed(int key)
{
    loadNext = true;
}
