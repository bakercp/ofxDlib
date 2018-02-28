//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Use points.
    mesh.setMode(OF_PRIMITIVE_POINTS);

    // Let's make a point cloud that looks like a 3D spiral.
    dlib::rand rnd;

    for (float i = -10; i < 10; i += 0.001)
    {
        // Get a point on a spiral.
        dlib::vector<float> val(sin(i), cos(i), i / 4);

        // Now add some random noise to it.
        dlib::vector<float> temp(rnd.get_random_gaussian(),
                                 rnd.get_random_gaussian(),
                                 rnd.get_random_gaussian());
        val += temp / 20;

        // Scale it up.
        val *= 200;

        // Pick a color based on how far we are along the spiral.
        dlib::rgb_pixel color = dlib::colormap_jet(i, 0, 20);

        mesh.addVertex(ofxDlib::toOf(val));
        mesh.addColor(ofxDlib::toOf(color));
    }

    ofEnableDepthTest();
}


void ofApp::draw()
{
    ofBackground(0);
    camera.begin();
    mesh.draw();
    camera.end();
}

