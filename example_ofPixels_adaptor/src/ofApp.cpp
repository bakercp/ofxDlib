//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"




void ofApp::setup()
{
    // Here we declare an unallocated ofPixels instance.
    ofPixels pix;

    // Now load the image file into our pixels. ofLoadImage allocates the memory
    // and loads the pixel data.
    ofLoadImage(pix, "Puppy.jpg");

    // ofxDlib includes wrappers to translate ofPixels into of
    // dlib::generic_image. Here we declare our result image
    // (note that it has not yet been allocated, but will be within the
    // dlib::gaussian_blur function below).
    ofPixels pixBlurred;

    // Now we wrap the unallocated pixBlurred in an wrapper that will treat it
    // as an ofPixels object with a pixel type of OF_PIXELS_RGB. It will take
    // care of allocation, etc for us.
    auto wrappedBlurredPix = ofxDlib::toDlib<dlib::rgb_pixel>(pixBlurred);

    // Now we will pass the allocated ofPixels (`pix`) and the unallocated
    // wrapped ofPixels (wrappedBlurredPix) to the dlib::gaussian_blur
    // function.
    dlib::gaussian_blur(pix, wrappedBlurredPix, 3);

    // This time, we want dlib to treat the output as grayscale.
    ofPixels pixBlurredGrayscale;

    // Now we wrap the unallocated pixBlurred in an wrapper that will treat it
    // as an ofPixels object with a pixel type of OF_PIXELS_GRAY. It will take
    // care of allocation, etc for us.
    auto wrappedPixBlurredGrayscale = ofxDlib::toDlib<unsigned char>(pixBlurredGrayscale);

    // Now we will pass the allocated ofPixels (`pix`) and the unallocated
    // wrapped ofPixels (wrappedBlurredPix) to the dlib::gaussian_blur
    // function.
    dlib::gaussian_blur(pix, wrappedPixBlurredGrayscale, 3);

    pixTexture.loadData(pix);
    pixBlurredTexture.loadData(pixBlurred);
    pixBlurredGrayscaleTexture.loadData(pixBlurredGrayscale);
}


void ofApp::draw()
{
    float w = pixTexture.getWidth();
    float h = pixTexture.getHeight();

    ofPushMatrix();
    ofTranslate(0, 0);
    pixTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("pix", 14, 14);

    ofTranslate(w, 0);
    pixBlurredTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("pixBlurred", 14, 14);

    ofTranslate(w, 0);
    pixBlurredGrayscaleTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("pixBlurredGrayscale", 14, 14);
    ofPopMatrix();
}

