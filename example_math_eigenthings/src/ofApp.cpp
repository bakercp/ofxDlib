//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Example from Mathworks Reference 10.2
    // https://www.mathworks.com/content/dam/mathworks/mathworks-dot-com/moler/eigs.pdf
    
    // This is a matrix that has integer eigenvectors and eigenvalues.
    dlib::matrix<double> A(3, 3);
    
    A = -149, -50, -154,
         537, 180,  546,
        -27,  -9,  -25;
    
    std::cout << "Matrix A = \n" << A << std::endl;

    // Create the decomposition object.
    dlib::eigenvalue_decomposition<dlib::matrix<double>> E(A);
    
    // Get the real eigen vectors.
    dlib::matrix<double> X = E.get_pseudo_v();
    
    // Get the real eigenvalues * an identity matrix.
    dlib::matrix<double> Λ = E.get_pseudo_d();

    std::cout << "Eigen Values Λ = \n" << Λ << std::endl;
    
    // The given eigen vectors can be normalized to produce integer elements.
    // This performs that column normalization based on the example.
    // The reconstruction will be the same if this is left out.
    {
        std::vector<double> n = { X(0, 0) / 1.0, X(0, 1) / -4.0, X(0, 2) / 7.0 };
        
        for (std::size_t col = 0; col < X.nc(); ++col)
            for (std::size_t row = 0; row < X.nr(); ++row)
                X(row, col) = std::round(X(row, col) / n[col]);
    }
    
    
    std::cout << "Eigen Vectors X = \n" << X << std::endl;

    // Calculate the inverse of X.
    dlib::matrix<double> Xinv = dlib::inv(X);
    
    std::cout << "Xinv = \n" << Xinv << std::endl;

    // Reconstruct the A matrix from the eigen values / vectors.
    dlib::matrix<double> Acomputed = X * Λ * Xinv;

    std::cout << "Acomputed = \n" << Acomputed << std::endl;

    // Note: Using dlib::eigenvalue_decomposition directly for large matrices
    // is not efficient. It is often faster to work with a faster aglorithm like
    // dlib::svd, dlib::svd_fast, etc. This may require reformatting or
    // otherwise preparing your data.
    
    ofExit();
}
