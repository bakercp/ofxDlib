//
// Copyright (c) 2018 Dario Longobardi 
//
// License: MIT
//

#include "ofApp.h"


int main()
{
    ofSetupOpenGL(800,600, OF_WINDOW);
    return ofRunApp(std::make_shared<ofApp>());
}
