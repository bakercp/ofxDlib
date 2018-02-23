//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"
#include "ofLog.h"


void ofApp::setup()
{
    // load the models as well as glasses and mustache.
    net_type net;
    shape_predictor sp;
    matrix<rgb_alpha_pixel> glasses, mustache;

    deserialize(ofToDataPath("mmod_dog_hipsterizer.dat", true)) >> net >> sp >> glasses >> mustache;

    pyramid_up(glasses);
    pyramid_up(mustache);

//    image_window win1(glasses);
//    image_window win2(mustache);
//    image_window win_wireframe, win_hipster;

    // Now process each image, find dogs, and hipsterize them by drawing glasses and a
    // mustache on each dog :)
    matrix<rgb_pixel> img;

    load_image(img, ofToDataPath("dogs.jpg", true));

    // Upsampling the image will allow us to find smaller dog faces but will use more
    // computational resources.
    //pyramid_up(img);

    auto dets = net(img);
//    win_wireframe.clear_overlay();
//    win_wireframe.set_image(img);
    // We will also draw a wireframe on each dog's face so you can see where the
    // shape_predictor is identifying face landmarks.
//    std::vector<image_window::overlay_line> lines;

    for (auto&& d : dets)
    {
        // get the landmarks for this dog's face
        auto shape = sp(img, d.rect);

        const rgb_pixel color(0,255,0);
        auto top  = shape.part(0);
        auto lear = shape.part(1);
        auto leye = shape.part(2);
        auto nose = shape.part(3);
        auto rear = shape.part(4);
        auto reye = shape.part(5);

        // The locations of the left and right ends of the mustache.
        auto lmustache = 1.3*(leye-reye)/2 + nose;
        auto rmustache = 1.3*(reye-leye)/2 + nose;

        // Draw the glasses onto the image.
        std::vector<point> from = {2*point(176,36), 2*point(59,35)}, to = {leye, reye};
        auto tform = find_similarity_transform(from, to);
        for (long r = 0; r < glasses.nr(); ++r)
        {
            for (long c = 0; c < glasses.nc(); ++c)
            {
                point p = tform(point(c,r));
                if (get_rect(img).contains(p))
                    assign_pixel(img(p.y(),p.x()), glasses(r,c));
            }
        }

        // Draw the mustache onto the image right under the dog's nose.
        auto mrect = get_rect(mustache);
        from = {mrect.tl_corner(), mrect.tr_corner()};
        to = {rmustache, lmustache};
        tform = find_similarity_transform(from, to);
        for (long r = 0; r < mustache.nr(); ++r)
        {
            for (long c = 0; c < mustache.nc(); ++c)
            {
                point p = tform(point(c,r));
                if (get_rect(img).contains(p))
                    assign_pixel(img(p.y(),p.x()), mustache(r,c));
            }
        }


        // Record the lines needed for the face wire frame.
//        lines.push_back(image_window::overlay_line(leye, nose, color));
//        lines.push_back(image_window::overlay_line(nose, reye, color));
//        lines.push_back(image_window::overlay_line(reye, leye, color));
//        lines.push_back(image_window::overlay_line(reye, rear, color));
//        lines.push_back(image_window::overlay_line(rear, top, color));
//        lines.push_back(image_window::overlay_line(top, lear,  color));
//        lines.push_back(image_window::overlay_line(lear, leye,  color));
    }

//    win_wireframe.add_overlay(lines);
//    win_hipster.set_image(img);
    

    ofLogNotice(__FUNCTION__) << "Hello";   
    std::cout << "DONE" << std::endl;
}


void ofApp::draw()
{
    ofBackground(0);
    ofNoFill();
    ofSetColor(ofColor::white);

    image.draw(0, 0);

    for (auto& faceRect: faceRects)
    {
        ofSetColor(ofColor::yellow);
        ofDrawRectangle(ofxDlib::toOf(faceRect));
    }

    ofDrawBitmapStringHighlight("Num. faces detected: " + ofToString(faceRects.size()), 14, 20);
}

