//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


int main()
{
    ofSetupOpenGL(160 * 2, 193 * 3, OF_WINDOW);
    return ofRunApp(std::make_shared<ofApp>());
}
