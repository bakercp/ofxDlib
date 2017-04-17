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
    img = ofxDlib::toGrayscale(img);

    ofPixels blurred_img;

    // Blur the image.
    dlib::gaussian_blur(img, blurred_img, 3);

    // Now find the horizontal and vertical gradient images.
    ofFloatPixels horz_gradient;
    ofFloatPixels vert_gradient;
    ofPixels edge_image;

    dlib::sobel_edge_detector(blurred_img, horz_gradient, vert_gradient);

    // now we do the non-maximum edge suppression step so that our edges are nice and thin
    dlib::suppress_non_maximum_edges(horz_gradient, vert_gradient, edge_image);

    // Now we would like to see what our images look like.  So let's upload them
    // to ofTextures.
    imgTexture.loadData(img);
    blurTexture.loadData(blurred_img);
    edgeTexture.loadData(edge_image);

    // We can also easily display the edge_image as a heatmap or using the jet color
    // scheme like so.

    ofPixels heat_edge_image = dlib::heatmap(edge_image);
    ofPixels jet_edge_image = dlib::jet(edge_image);

    heatEdgeTexture.loadData(ofxDlib::toOf(heat_edge_image));
    jetEdgeTexture.loadData(ofxDlib::toOf(jet_edge_image));


    //image_window win_hot(heatmap(edge_image));
    // auto heatMap = dlib::heatmap(edge_image);

    // auto heatMap = ofxDlib::toOf(dlib::heatmap(edge_image));
    //dlib::image_window win_jet(jet(edge_image));
    // auto jetImg = dlib::jet(edge_image);


    // Finally, note that you can access the elements of an image using the normal [row][column]
    // operator like so:
    //  std::cout << horz_gradient[0][3] << std::endl;
    //  std::cout << "number of rows in image:    " << horz_gradient.nr() << std::endl;
    //  std::cout << "number of columns in image: " << horz_gradient.nc() << std::endl;

}


void ofApp::draw()
{
    imgTexture.draw(0, 0);
    blurTexture.draw(imgTexture.getWidth(), 0);
    edgeTexture.draw(0, imgTexture.getHeight());

}

