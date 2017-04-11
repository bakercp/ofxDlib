//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


int main()
{
    ofSetupOpenGL(1280, 720, OF_WINDOW);
    return ofRunApp(std::make_shared<ofApp>());
}
