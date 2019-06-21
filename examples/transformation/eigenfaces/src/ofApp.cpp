//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    std::string pcaDataPath = "pca.dat";
    std::string imagesDataPath = "images.json";

    ofJson imagesJson = ofLoadJson(imagesDataPath);

    if (!pca.load(pcaDataPath) || imagesJson.empty())
    {
        // Load our image database. Many additional parameters can be used
        // to filter our image collection.
        ofxDlib::ImageCollection<float> images("images/");

        // Create our vector.
        std::vector<ofFloatPixels> augmentedImages = images.pixels();

        // Add a flipped version of each image.
        for (auto img: images.pixels())
        {
            img.mirror(false, true);
            augmentedImages.push_back(img);
        }

        // Load the image collection into a matrix where each row represents
        // one image and each column and the columns represent pixel data.
        dlib::matrix<float> imageData = ofxDlib::to_rows(augmentedImages);


        // Train the PCA with the image data.
        pca.train(imageData);

        // Save the PCA data.
        pca.save(pcaDataPath);

        // Save the data.
        imagesJson["width"] = images.width();
        imagesJson["height"] = images.height();
        imagesJson["pixel_format"] = images.pixelFormat();
        ofSaveJson(imagesDataPath, imagesJson);
    }

    imageWidth = imagesJson["width"].get<std::size_t>();
    imageHeight = imagesJson["height"].get<std::size_t>();
    imageFormat = imagesJson["pixel_format"].get<ofPixelFormat>();

    meanTexture = ofxDlib::toTexture(pca.mean(),
                                     imageWidth,
                                     imageHeight,
                                     imageFormat, true);

    for (std::size_t row = 0; row < pca.eigenvectors().nr(); ++row)
    {
        dlib::matrix<float> eigenface = dlib::rowm(pca.eigenvectors(), row);
        eigenTextures.push_back(ofxDlib::toTexture(eigenface,
                                                   imageWidth,
                                                   imageHeight,
                                                   imageFormat, true));
    }
}


void ofApp::update()
{
}


void ofApp::draw()
{
    meanTexture.draw(0, 0);

    std::size_t x = meanTexture.getWidth();
    std::size_t y = 0;

    for (auto& t: eigenTextures)
    {
        std::cout << x << ", " << y << std::endl;
        t.draw(x, y);
        if ((x + t.getWidth() * 2) > ofGetWidth())
        {
            x = 0;
            y += t.getHeight();
        }
        else x += t.getWidth();
    }
}


void ofApp::keyPressed(int key)
{

}
