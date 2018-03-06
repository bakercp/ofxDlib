//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"



void ofApp::setup()
{
    {
        dlib::array2d<uint8_t> img;
        test_display_dlib_image(img);
    }
    {
        dlib::array2d<uint16_t> img;
        // We normalize non-8-bit types when loading w/ dlib::load_image.
        test_display_dlib_image(img, true);
    }
    {
        dlib::array2d<float> img;
        // We normalize non-8-bit types when loading w/ dlib::load_image.
        test_display_dlib_image(img, true);
    }
    {
        dlib::array2d<dlib::rgb_pixel> img;
        test_display_dlib_image(img);
    }
    {
        dlib::array2d<dlib::rgb_alpha_pixel> img;
        test_display_dlib_image(img);
    }
    // --
    {
        dlib::matrix<uint8_t> img;
        test_display_dlib_image(img);
    }
    {
        dlib::matrix<uint16_t> img;
        // We normalize non-8-bit types when loading w/ dlib::load_image.
        test_display_dlib_image(img, true);
    }
    {
        dlib::matrix<float> img;
        // We normalize non-8-bit types when loading w/ dlib::load_image.
        test_display_dlib_image(img, true);
    }
    {
        dlib::matrix<dlib::rgb_pixel> img;
        test_display_dlib_image(img);
    }
    {
        dlib::matrix<dlib::rgb_alpha_pixel> img;
        test_display_dlib_image(img);
    }

    // --
    {
        ofPixels img;
        test_load_of_image_auto(img);
    }
    {
        ofShortPixels img;
        // We normalize non-8-bit types when loading w/ dlib::load_image.
        test_load_of_image_auto(img, true);
    }
    {
        ofFloatPixels img;
        // We normalize non-8-bit types when loading w/ dlib::load_image.
        test_load_of_image_auto(img, true);
    }
    {
        ofPixels img;
        test_load_of_image<dlib::rgb_pixel>(img);
    }
    {
        ofPixels img;
        test_load_of_image<dlib::bgr_pixel>(img);
    }
    {
        ofPixels img;
        test_load_of_image<dlib::rgb_alpha_pixel>(img);
    }
    {
        ofPixels img;
        test_load_of_image<dlib::hsi_pixel>(img);
        // Could convert the results with ofColor::fromHSB.
    }
    {
        ofPixels img;
        test_load_of_image<dlib::lab_pixel>(img);
        // Could convert the results with dlib::Lab2RGB.
    }
    {
        ofPixels img;
        test_load_of_image<uint8_t>(img);
    }
    {
        ofShortPixels img;
        // We normalize non-8-bit types when loading w/ dlib::load_image.
        test_load_of_image<uint16_t>(img, true);
    }
    {
        ofFloatPixels img;
        // We normalize non-8-bit types when loading w/ dlib::load_image.
        test_load_of_image<float>(img, true);
    }
}


void ofApp::draw()
{
    ofColor fg(255);
    ofColor bg(0, 200);
    glm::vec2 size(128, 164);
    glm::vec2 textOffset(10, 20);
    glm::vec2 pos(0, 0);
    float margin = 20;
    
    for (const auto& texture: textures)
    {
        ofSetColor(255);
        texture.second.draw(pos, size.x, size.y);
        ofDrawBitmapStringHighlight(texture.first, pos + textOffset, bg);

        if (pos.x + (2 * (size.x + margin)) > ofGetWidth())
            pos = { 0, pos.y + size.y + margin };
        else pos.x += (size.x + margin);
    }
}

