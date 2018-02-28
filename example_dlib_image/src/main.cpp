//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


int main()
{
    ofSetupOpenGL(256 * 3, 328 * 3, OF_WINDOW);
    return ofRunApp(std::make_shared<ofApp>());
}
