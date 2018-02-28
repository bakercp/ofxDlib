//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Here we declare an image object that can store rgb_pixels.  Note that in
    // dlib there is no explicit image object, just a 2D array and
    // various pixel types.
    dlib::array2d<dlib::rgb_pixel> img;

    // Now load the image file into our image.  If something is wrong then
    // load_image() will throw an exception.  Also, if you linked with libpng
    // and libjpeg then load_image() can load PNG and JPEG files in addition
    // to BMP files.
    dlib::load_image(img, ofToDataPath("Ada.png", true));

    // Now let's use some image functions.  First let's blur the image a little.
    dlib::array2d<unsigned char> blurred_img;
    dlib::gaussian_blur(img, blurred_img);

    dlib::array2d<dlib::rgb_pixel> rgb_blurred_img;
    dlib::gaussian_blur(img, rgb_blurred_img, 4);

    // Now find the horizontal and vertical gradient images.
    dlib::array2d<short> horz_gradient, vert_gradient;
    dlib::array2d<unsigned char> edge_image;
    dlib::sobel_edge_detector(blurred_img, horz_gradient, vert_gradient);

    // now we do the non-maximum edge suppression step so that our edges are nice and thin
    dlib::suppress_non_maximum_edges(horz_gradient, vert_gradient, edge_image);

    // Now we would like to see what our images look like.
    //
    // We can convert the images to ofPixels_<...> using ofxDlib::toOf(...).
    // We then load them into textures in order to view them.
    imgTexture.loadData(ofxDlib::toOf(img));
    blurredTexture.loadData(ofxDlib::toOf(blurred_img));
    rgbBlurredTexture.loadData(ofxDlib::toOf(rgb_blurred_img));

    ofShortPixels horzGradPixels = ofxDlib::toOf(horz_gradient);
    ofShortPixels vertGradPixels = ofxDlib::toOf(vert_gradient);

    horzGradientTexture.loadData(horzGradPixels);
    vertGradientTexture.loadData(vertGradPixels);
    edgeTexture.loadData(ofxDlib::toOf(edge_image));

    // We can also easily display the edge_image as a heatmap or using the
    // jet color scheme like so.
    heatEdgeTexture.loadData(ofxDlib::toOf(dlib::heatmap(edge_image)));
    jetEdgeTexture.loadData(ofxDlib::toOf(dlib::jet(edge_image)));
    randomEdgeTexture.loadData(ofxDlib::toOf(dlib::randomly_color_image(edge_image)));
}


void ofApp::draw()
{
    float w = imgTexture.getWidth();
    float h = imgTexture.getHeight();

    ofColor fg(255);
    ofColor bg(0, 200);
    float x = 10;
    float y = 20;

    ofPushMatrix();
    ofTranslate(0, 0);
    imgTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Original Image", x, y, bg);

    ofTranslate(w, 0);
    blurredTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Blurred Gray Image 1x", x, y, bg);

    ofTranslate(w, 0);
    rgbBlurredTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Blurred RGB Image 4x", x, y, bg);

    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(0, h);
    horzGradientTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Horz. Gradient", x, y, bg);

    ofTranslate(w, 0);
    vertGradientTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Vert. Gradient", x, y, bg);

    ofTranslate(w, 0);
    randomEdgeTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Random Edge Image", x, y, bg);
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(0, h + h);
    edgeTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Normal Edge Image", x, y, bg);

    ofTranslate(w, 0);
    heatEdgeTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Heat Edge Image", x, y, bg);

    ofTranslate(w, 0);
    jetEdgeTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Jet Edge Image", x, y, bg);
    ofPopMatrix();

}

