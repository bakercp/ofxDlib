//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Example from NIST
    // https://www.itl.nist.gov/div898/handbook/pmc/section5/pmc541.htm

    dlib::matrix<double> X(5, 3);
    X = 4.0, 2.0, 0.60,
        4.2, 2.1, 0.59,
        3.9, 2.0, 0.58,
        4.3, 2.1, 0.62,
        4.1, 2.2, 0.63;

    std::cout << "X = \n" << X << std::endl;

    // Calculate the row mean.
    dlib::matrix<double> row_mean = dlib::sum_rows(X) / X.nr();;

    std::cout << "row_mean = \n" << row_mean << std::endl;

    // Remove mean from each row.
    for (std::size_t row = 0; row < X.nr(); ++row)
        dlib::set_rowm(X, row) -= row_mean;

    // Caculate the sample covariance of X.
    dlib::matrix<double> cov = dlib::trans(X) * X / (X.nr() - 1);

    std::cout << "cov = \n" << cov << std::endl;

    // Alternatively, use ofxDlib::cov(X) to get the same results as above.
    //
    // dlib::matrix<double> cov = ofxDlib::cov(X);

    ofExit();
}
