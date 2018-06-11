//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"



void ofApp::setup()
{
    // Define a resizable matrix with 2 rows and 3 columns with
    // [row-major](https://en.wikipedia.org/wiki/Row-_and_column-major_order)
    // layout.
    dlib::matrix<float, 2, 3> A;

    // We can use an initializer list and it will be interpreted as a 2 x 3
    // row-major matrix.
    A =  1, 2, 3,
         4, 5, 6;
    
    ofxDlib::prettyPrint(A, "A");
    
    for (auto i = 0; i < A.nr(); i++)
        for (auto j = 0; j < A.nc(); j++)
            std::cout << A(i,j) << std::endl;

    std::cout << A << std::endl;
    
    for (auto i = 0; i < A.size(); ++i)
        std::cout << *(A.begin() + i) << (i < (A.size() - 1) ? "," : "\n");
    
    std::cout << dlib::sum_rows(A) << std::endl;
    std::cout << dlib::sum_cols(A) << std::endl;

    dlib::matrix<float, 4, 1> newRow; // STRANGE
    newRow = 99., 88., 77., 66;
    dlib::set_colm(A, 0) = newRow;

    std::cout << A << std::endl;

    dlib::matrix<float, 4, 1> newRow2;
    
    
    dlib::set_rowm(A, 1) = newRow;
    std::cout << A << std::endl;

    
    // There are other ways we could access the matrix.
//
//    A(0, 0) = 4;
//    A(0, 1) = -5;
//    A(1, 0) = -2;
//    A(1, 1) = 3;
//
//    dlib::rowm(A, 0)
//
////    dlib::set_rowm(A, 0) = { 4, -5 };
////    dlib::set_rowm(A, 1) = { -2, -3 };
//
//    // Define a fixed matrix with 2 rows and 1 column.
//    dlib::matrix<float, 2, 1> b;
//
//    b = -13,
//          9;
    
    
//    x⁰    xⁱ            x⁴    x⁵    x⁶    x⁷    x⁸    x⁹    x⁺    x⁻    x⁼    x⁽    x⁾    xⁿ
//    x₀    x₁    x₂    x₃    x₄    x₅    x₆    x₇    x₈    x₉    x₊    x₋    x₌    x₍    x₎
    
    
}


void ofApp::draw()
{

}

