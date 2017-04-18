//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Here we declare an image object that can store rgb pixels.
    ofPixels img;

    // Now load the image file into our image.
    ofLoadImage(img, "Puppy.jpg");

    // Convert the image to grayscale.
    ofPixels gray = ofxDlib::toGrayscale(img);

    // ofxDlib includes wrappers to translate ofPixels into of dlib::generic_image.

    // Declare our result image (note that it has not yet been allocated, but
    // will be within the dlib::gaussian_blur function below).
    ofPixels blurred_img;

    // Blur the image.
    dlib::gaussian_blur(gray, blurred_img, 3);

    // Now find the horizontal and vertical gradient images.
    ofFloatPixels horz_gradient;
    ofFloatPixels vert_gradient;
    ofPixels edge_image;

    dlib::sobel_edge_detector(blurred_img, horz_gradient, vert_gradient);

    // Now we do the non-maximum edge suppression step so that our edges are
    // nice and thin.
    dlib::suppress_non_maximum_edges(horz_gradient, vert_gradient, edge_image);

    // We can also easily display the edge_image as a heatmap or using the jet
    // color scheme like so.
    dlib::matrix<dlib::rgb_pixel> heat_edge_matrix = dlib::heatmap(edge_image);

    // Here we use create the heat_edge_image.
    // This is a shallow copy and shares the same underlying data with
    // heat_edge_matrix. Thus, it is important that heat_edge_matrix does not
    // go out of scope before heat_edge_image.
    ofPixels heat_edge_image = ofxDlib::toOf(heat_edge_matrix);

    std::cout << "The same underlying data? " <<  (dlib::image_data(heat_edge_matrix) == dlib::image_data(heat_edge_image)) << std::endl;

    // Since dlib::heatmap and similar objects return dlib::matrix_exp, our
    // toOf helper functions correctly resolve the expression without an
    // additional assignment to a dlib::matrix before passing to ofxDlib::toOf.
    //
    // Thus, ofxDlib::toOf will preserve the speedy templated expression system
    // in dlib when possible.
    ofPixels jet_edge_image = ofxDlib::toOf(dlib::jet(edge_image));
    ofPixels random_edge_image = ofxDlib::toOf(dlib::randomly_color_image(edge_image));

    // Upload the data to the textures.
    imgTexture.loadData(img);
    grayTexture.loadData(gray);
    blurTexture.loadData(blurred_img);
    edgeTexture.loadData(edge_image);
    heatEdgeTexture.loadData(heat_edge_image);
    jetEdgeTexture.loadData(jet_edge_image);
    randomEdgeTexture.loadData(random_edge_image);

}


void ofApp::draw()
{
    int w = imgTexture.getWidth();
    int h = imgTexture.getHeight();

    grayTexture.draw(0, 0);
    blurTexture.draw(w, 0);
    edgeTexture.draw(0, h);

    ofPushMatrix();
    ofTranslate(w, h);
    ofScale(0.5);
    heatEdgeTexture.draw(0, 0);
    jetEdgeTexture.draw(w, 0);
    randomEdgeTexture.draw(0, h);
    imgTexture.draw(w, h);
    ofPopMatrix();
}

