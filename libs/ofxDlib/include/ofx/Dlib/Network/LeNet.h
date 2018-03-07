//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "dlib/dnn.h"


namespace ofx {
namespace Dlib {
namespace LeNet5 {

// From dlib:
// Now let's define the LeNet.  Broadly speaking, there are 3 parts to a network
// definition.  The loss layer, a bunch of computational layers, and then an input
// layer.  You can see these components in the network definition below.
//
// The input layer here says the network expects to be given matrix<unsigned char>
// objects as input.  In general, you can use any dlib image or matrix type here, or
// even define your own types by creating custom input layers.
//
// Then the middle layers define the computation the network will do to transform the
// input into whatever we want.  Here we run the image through multiple convolutions,
// ReLU units, max pooling operations, and then finally a fully connected layer that
// converts the whole thing into just 10 numbers.
//
// Finally, the loss layer defines the relationship between the network outputs, our 10
// numbers, and the labels in our dataset.  Since we selected loss_multiclass_log it
// means we want to do multiclass classification with our network.   Moreover, the
// number of network outputs (i.e. 10) is the number of possible labels.  Whichever
// network output is largest is the predicted label.  So for example, if the first
// network output is largest then the predicted digit is 0, if the last network output
// is largest then the predicted digit is 9.

using Net = dlib::loss_multiclass_log<dlib::fc<10,
                                      dlib::relu<dlib::fc<84,
                                      dlib::relu<dlib::fc<120,
                                      dlib::max_pool<2,2,2,2,dlib::relu<dlib::con<16,5,5,1,1,
                                      dlib::max_pool<2,2,2,2,dlib::relu<dlib::con<6,5,5,1,1,
                                      dlib::input<dlib::matrix<unsigned char>>
                                      >>>>>>>>>>>>;

static const std::size_t ln_base    = 1100;
static const std::size_t fc_0       = ln_base + 0;
static const std::size_t relu_0     = ln_base + 1;
static const std::size_t fc_1       = ln_base + 2;
static const std::size_t relu_1     = ln_base + 3;
static const std::size_t fc_2       = ln_base + 4;
static const std::size_t max_pool_0 = ln_base + 5;
static const std::size_t relu_2     = ln_base + 6;
static const std::size_t con_0      = ln_base + 7;
static const std::size_t max_pool_1 = ln_base + 8;
static const std::size_t relu_3     = ln_base + 9;
static const std::size_t con_1      = ln_base + 10;

template <typename SUBNET> using tag_0_fc_0       = dlib::add_tag_layer<fc_0,       SUBNET>;
template <typename SUBNET> using tag_1_relu_0     = dlib::add_tag_layer<relu_0,     SUBNET>;
template <typename SUBNET> using tag_2_fc_1       = dlib::add_tag_layer<fc_1,       SUBNET>;
template <typename SUBNET> using tag_3_relu_1     = dlib::add_tag_layer<relu_2,     SUBNET>;
template <typename SUBNET> using tag_4_fc_2       = dlib::add_tag_layer<fc_2,       SUBNET>;
template <typename SUBNET> using tag_5_max_pool_0 = dlib::add_tag_layer<max_pool_0, SUBNET>;
template <typename SUBNET> using tag_6_relu_2     = dlib::add_tag_layer<relu_2,     SUBNET>;
template <typename SUBNET> using tag_7_con_0      = dlib::add_tag_layer<con_0,      SUBNET>;
template <typename SUBNET> using tag_8_max_pool_1 = dlib::add_tag_layer<max_pool_1, SUBNET>;
template <typename SUBNET> using tag_9_relu_3     = dlib::add_tag_layer<relu_3,     SUBNET>;
template <typename SUBNET> using tag_10_con_1     = dlib::add_tag_layer<con_1,      SUBNET>;

using TaggedNet = dlib::loss_multiclass_log<
                    tag_0_fc_0<dlib::fc<10,
                    tag_1_relu_0<dlib::relu<
                    tag_2_fc_1<dlib::fc<84,
                    tag_3_relu_1<dlib::relu<
                    tag_4_fc_2<dlib::fc<120,
                    tag_5_max_pool_0<dlib::max_pool<2,2,2,2,
                    tag_6_relu_2<dlib::relu<
                    tag_7_con_0<dlib::con<16,5,5,1,1,
                    tag_8_max_pool_1<dlib::max_pool<2,2,2,2,
                    tag_9_relu_3<dlib::relu<
                    tag_10_con_1<dlib::con<6,5,5,1,1,
                    dlib::input<dlib::matrix<unsigned char>
                    >>>>>>>>>>>>>>>>>>>>>>>>;
    
    
    
} } } // namespace ofx::Dlib::LeNet5
