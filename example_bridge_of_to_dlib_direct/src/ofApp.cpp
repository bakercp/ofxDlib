//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
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

    // Now double check to make sure that the pixel format was set to
    // OF_PIXELS_RGB when it was loaded.
    assert(pix.getPixelFormat() == OF_PIXELS_RGB);

    // By default, ofxDlib allows ofPixels (aka ofPixels_<unsigned char> to be
    // directly manipulated, but it will always assume that ofPixels have the
    // OF_PIXELS_RGB format. While very convenient, care must be taken when
    // using ofPixels directly with dlib.

    // Here we declare another unallocated ofPixels instance.
    ofPixels pixBlurred;

    // Now we will pass the allocated ofPixels (`pix`) and the unallocated
    // ofPixels (pixBlurred) directly to the dlib::gaussian_blur function.
    //
    // By default, dlib will _assume_ that `pix` has the OF_PIXELS_RGB because
    // it assumes that _all_ ofPixels have the OF_PIXELS_RGB format.
    //
    // dlib will then internally allocate the `ofPixels::allocate` function,
    // but it will _only_ be able to allocate OF_PIXELS_RGB data when working
    // directly with ofPixels.
    dlib::gaussian_blur(pix, pixBlurred, 3);

    // The key point: when reading or writing ofPixels directly, dlib will
    // assume that ofPixels _always_ have the OF_PIXELS_RGB format.

    pixTexture.loadData(pix);
    pixBlurredTexture.loadData(pixBlurred);

    // -------------------------------------------------------------------------

    // Here we declare an unallocated ofShortPixels instance.
    ofShortPixels shortPix;

    // Now load the image file into our pixels. ofLoadImage allocates the memory
    // and loads the pixel data.
    ofLoadImage(shortPix, "Puppy.jpg");

    // Before we use ofShortPixels directly with dlib, we need to make sure that
    // the data is in the OF_PIXELS_GRAY format.
    shortPix = ofxDlib::toGrayscale(shortPix);

    // Now double check to make sure that the pixel format was set to
    // OF_PIXELS_GRAY when it was loaded.
    assert(shortPix.getPixelFormat() == OF_PIXELS_GRAY);

    // By default, ofxDlib allows ofShortPixels (aka ofPixels_<unsigned short>
    // to be directly manipulated, but it will always assume that ofPixels have
    // the OF_PIXELS_GRAY format. While very convenient, care must be taken when
    // using ofShortPixels directly with dlib.

    // Here we declare another unallocated ofShortPixels instance.
    ofShortPixels shortPixBlurred;

    // Now we will pass the allocated ofPixels (`shortPix`) and the unallocated
    // ofShortPixels (shortPixBlurred) directly to the dlib::gaussian_blur
    // function.
    //
    // By default, dlib will _assume_ that `shortPix` has the OF_PIXELS_GRAY
    // because it assumes that _all_ ofShortPixels have the OF_PIXELS_GRAY
    // format.
    //
    // dlib will then internally allocate the `ofShortPixels::allocate`
    // function, but it will _only_ be able to allocate OF_PIXELS_GRAY data when
    // working directly with ofShortPixels.
    dlib::gaussian_blur(shortPix, shortPixBlurred, 3);

    // The key point: when reading or writing ofShortPixels directly, dlib will
    // assume that ofShortPixels _always_ have the OF_PIXELS_GRAY format.
    shortPixTexture.loadData(shortPix);
    shortPixBlurredTexture.loadData(shortPixBlurred);

    // -------------------------------------------------------------------------

    // Here we declare an unallocated ofFloatPixels instance.
    ofFloatPixels floatPix;

    // Now load the image file into our pixels. ofLoadImage allocates the memory
    // and loads the pixel data.
    ofLoadImage(floatPix, "Puppy.jpg");

    // Before we use ofFloatPixels directly with dlib, we need to make sure that
    // the data is in the OF_PIXELS_GRAY format.
    floatPix = ofxDlib::toGrayscale(floatPix);

    // Now double check to make sure that the pixel format was set to
    // OF_PIXELS_GRAY when it was loaded.
    assert(floatPix.getPixelFormat() == OF_PIXELS_GRAY);

    // By default, ofxDlib allows ofFloatPixels (aka ofPixels_<float> to be
    // directly manipulated, but it will always assume that ofPixels have the
    // OF_PIXELS_GRAY format. While very convenient, care must be taken when
    // using ofFloatPixels directly with dlib.

    // Here we declare another unallocated ofShortPixels instance.
    ofFloatPixels floatPixBlurred;

    // Now we will pass the allocated ofPixels (`floatPix`) and the unallocated
    // ofFloatPixels (floatPixBlurred) directly to the dlib::gaussian_blur
    // function.
    //
    // By default, dlib will _assume_ that `floatPix` has the OF_PIXELS_GRAY
    // because it assumes that _all_ ofFloatPixels have the OF_PIXELS_GRAY
    // format.
    //
    // dlib will then internally allocate the `ofFloatPixels::allocate`
    // function, but it will _only_ be able to allocate OF_PIXELS_GRAY data when
    // working directly with ofFloatPixels.
    dlib::gaussian_blur(floatPix, floatPixBlurred, 3);

    // The key point: when reading or writing ofFloatPixels directly, dlib will
    // assume that ofFloatPixels _always_ have the OF_PIXELS_GRAY format.
    floatPixTexture.loadData(floatPix);
    floatPixBlurredTexture.loadData(floatPixBlurred);

}


void ofApp::draw()
{
    float w = pixTexture.getWidth();
    float h = pixTexture.getHeight();

    ofColor fg(255);
    ofColor bg(0, 200);
    float x = 10;
    float y = 20;

    ofPushMatrix();
    ofTranslate(0, 0);
    pixTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Original Image", x, y, bg);

    ofTranslate(w, 0);
    pixBlurredTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Blurred Image", x, y, bg);
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(0, h);
    shortPixTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Short Pix. Image", x, y, bg);

    ofTranslate(w, 0);
    shortPixBlurredTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Short Pix. Blurred", x, y, bg);
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(0, h + h);
    floatPixTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Float Pix. Image", x, y, bg);

    ofTranslate(w, 0);
    floatPixBlurredTexture.draw(0, 0);
    ofDrawBitmapStringHighlight("Float Pix. Blurred", x, y, bg);
    ofPopMatrix();

}

