//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//

//
// From http://dlib.net/matrix_expressions_ex.cpp.html
//
// This example contains a detailed discussion of the template expression
// technique used to implement the matrix tools in the dlib C++ library.
//
// It also gives examples showing how a user can create their own custom
// matrix expressions.
//
// Note that you should be familiar with the dlib::matrix before reading
// this example.  So if you haven't done so already you should read the
// matrix_ex.cpp example program.
//


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"
#include "Example.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;

    void custom_matrix_expressions_example();
};
