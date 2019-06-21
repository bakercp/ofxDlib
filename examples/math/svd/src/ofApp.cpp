//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Example from https://www.mathworks.com/help/matlab/ref/svd.html
    dlib::matrix<double> A(4, 2);
    A = 1, 2,
        3, 4,
        5, 6,
        7, 8;

    std::cout << "Matrix A = \n" << A << std::endl;

    // SVD usually takes the form of M = U * Σ * trans(V)
    dlib::matrix<double> U,
                         Σ, // aka S or W
                         V;

    // Calculate the singular value decomposion.
    dlib::svd(A, // m x n, where m >= n).
              U, // m x n, orthogonal columns (i.e. U*trans(U) == I)
              Σ, // n x n, diagonal matrix, with singular values on diagonal (returned as n-col matrix).
              V  // n x n, orthonormal matrix, (i.e. V*trans(V) == trans(V)*V == I)
              );

    std::cout << "U = \n" << U << std::endl;
    std::cout << "Σ = \n" << Σ << std::endl;
    std::cout << "V = \n" << V << std::endl;;

    dlib::matrix<double> A_reconstructed = U * Σ * dlib::trans(V);
    std::cout << "A_reconstructed = \n" << A_reconstructed << std::endl;;

    // The reconstructed A matrix may not be exact due to rounding and numerical
    // errors, especially if using dlib::svd_fast(...).

    ofExit();
}
