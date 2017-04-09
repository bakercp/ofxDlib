//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Set up a timing mechanism for benchmarking.
    auto start = std::chrono::system_clock::now();

    // Load the network data.
    dlib::deserialize(ofToDataPath("mmod_human_face_detector.dat", true)) >> net;

    dlib::matrix<dlib::rgb_pixel> img;

    dlib::load_image(img, ofToDataPath("Crowd.jpg", true));

    image.setFromPixels(ofxDlib::copy(img));

    double detScale = 1;

    // Make the image larger so we can detect small faces.
    while (img.size() < 2400 * 2400)
    {
        dlib::pyramid_up(img);
        detScale *= 2;
    }

    // Note that you can process a bunch of images in a std::vector at once and
    // it runs much faster, since this will form mini-batches of images and
    // therefore get better parallelism out of your GPU hardware.  However, all
    // the images must be the same size.  To avoid this requirement on images
    // being the same size we process them individually in this example.
    dets = net(img);

    // Finish benchmarking.
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "Finished computation at " << std::ctime(&end_time);
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s";
    std::cout << std::endl;

    // 16 seconds on MacBook Pro (15-inch, Mid 2012), no CUDA support.
    // 1.2 seconds on i7 7700 + Nvidia 1080, CUDA support + MKL libs.

    std::cout << "Found " << dets.size() << " faces." << std::endl;


    for (auto& det: dets)
    {
        ofxDlib::scale(det, 1.0f / detScale);
    }
}


void ofApp::draw()
{
    ofBackground(0);
    ofNoFill();
    ofSetColor(ofColor::white);

    image.draw(0, 0);

    for (auto& r: dets)
    {
        std::stringstream ss;
        ss << "Confidence: " << r.detection_confidence << std::endl;
        ss << "Ignore: " << r.ignore;

        ofRectangle rect = ofxDlib::toOf(r);

        ofDrawRectangle(rect);
        ofPopMatrix();
        ofDrawBitmapString(ss.str(), rect.getCenter());
    }
}

