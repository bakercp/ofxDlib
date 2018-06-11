//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"
#include "Encoder.h"


// Auto encoders need:
//
// 1. Encoder
// 2. Decoder
// 3. Loss function.

// 1. Data de-noising.
// 2. Dimensionality reduction.





const std::size_t batch_size = 100;
const std::size_t input_dim = 784;
const std::size_t latent_dim = 2;
const std::size_t epochs = 50;
const std::size_t intermediate_dim = 256;
const float epsilon_std = 1.0;


// Input samples.
using x = dlib::input<dlib::matrix<float>>;

// The simple fully connected layer.
template<std::size_t INTERMEDIATE_DIM, typename SUBNET>
using h = dlib::fc<INTERMEDIATE_DIM, dlib::relu<SUBNET>>;

// The following two terms describe the mean and the gausian.

// The mean of the latent Gaussian distribution.
template<std::size_t LATENT_DIM, typename SUBNET>
using z_mean = dlib::fc<LATENT_DIM, dlib::relu<SUBNET>>;

// Log of the variance of the latent Gaussian distribution.
template<std::size_t LATENT_DIM, typename SUBNET>
using z_log_sigma = dlib::fc<LATENT_DIM, dlib::relu<SUBNET>>;


// Log of the variance of the latent Gaussian distribution.
template<std::size_t LATENT_DIM, typename SUBNET>



//add_prev1<tag1<z_log_sigma<tag1<SUBNET>>


//using z_log_sigma = dlib::fc<LATENT_DIM, >;




//using encoder_t = dlib::add_prev1<
//
//
//                                    z_log_sigma<latent_dim, dlib::tag1<h<intermediate_dim, x>>
//





//using encoder_t = dlib::vae_sampler<dlib::concat<z_mean, z_log_var



template<std::size_t INTERMEDIATE_DIM, typename SUBNET>
using decoder_t = dlib::fc<INTERMEDIATE_DIM, dlib::relu<SUBNET>>;

template<std::size_t INPUT_DIM, typename SUBNET>
using decoder_mean_t = dlib::fc<INPUT_DIM, dlib::sig<SUBNET>>;



//# we instantiate these layers separately so as to reuse them later
//h_decoded = decoder_h(z)
//x_decoded_mean = decoder_mean(h_decoded)




//using h_decoded_t = decoder_t<z>;






//template<typename SUBNET>
//using decoded_t = dlib::fc<784, dlib::sig<
//                  dlib::fc<128, dlib::relu<
//                  dlib::fc<64, dlib::relu<SUBNET>>>>>>;
//
//template<typename SUBNET>
//using encoded_t = dlib::fc<32, dlib::relu<
//                  dlib::fc<128, dlib::relu<
//                  dlib::fc<784, dlib::relu<SUBNET>>>>>>;
//
//using input_t =
//
//using autoencoder = dlib::loss_mean_squared_multioutput<decoded_t<encoded_t<input_t>>>;
//
//using encoder = encoded_t<input_t>;
//using decoder = decoded_t<input_t>;

class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;
    
//    autoencoder net;

    uint64_t delay = 1000;
    uint64_t lastUpdate = 0;
    
    std::vector<dlib::matrix<float>> trainingImages;
    std::vector<dlib::matrix<float>> testingImages;

    std::vector<ofTexture> testingTex;
    std::vector<ofTexture> encodedTex;
    
    std::size_t testingImageOffset = 0;
};
