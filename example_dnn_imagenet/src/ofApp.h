//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once

//
// From http://dlib.net/dnn_imagenet_ex.cpp.html
//
// This example shows how to classify an image into one of the 1000 imagenet
// categories using the deep learning tools from the dlib C++ Library.  We will
// use the pretrained ResNet34 model available on the dlib website.
//
// The ResNet34 architecture is from the paper Deep Residual Learning for Image
// Recognition by He, Zhang, Ren, and Sun.  The model file that comes with dlib
// was trained using the dnn_imagenet_train_ex.cpp program on a Titan X for
// about 2 weeks.  This pretrained model has a top5 error of 7.572% on the 2012
// imagenet validation dataset.
//

#include "ofMain.h"
#include "ofxDlib.h"


// This block of statements defines the resnet-34 network

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = dlib::add_prev1<block<N,BN,1,dlib::tag1<SUBNET>>>;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = dlib::add_prev2<dlib::avg_pool<2,2,2,2,dlib::skip1<dlib::tag2<block<N,BN,2,dlib::tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block  = BN<dlib::con<N,3,3,1,1,dlib::relu<BN<dlib::con<N,3,3,stride,stride,SUBNET>>>>>;

template <int N, typename SUBNET> using ares      = dlib::relu<residual<block,N,dlib::affine,SUBNET>>;
template <int N, typename SUBNET> using ares_down = dlib::relu<residual_down<block,N,dlib::affine,SUBNET>>;

template <typename SUBNET> using level1 = ares<512,ares<512,ares_down<512,SUBNET>>>;
template <typename SUBNET> using level2 = ares<256,ares<256,ares<256,ares<256,ares<256,ares_down<256,SUBNET>>>>>>;
template <typename SUBNET> using level3 = ares<128,ares<128,ares<128,ares_down<128,SUBNET>>>>;
template <typename SUBNET> using level4 = ares<64,ares<64,ares<64,SUBNET>>>;

using anet_type = dlib::loss_multiclass_log<dlib::fc<1000,dlib::avg_pool_everything<
level1<
level2<
level3<
level4<
dlib::max_pool<3,3,2,2,dlib::relu<dlib::affine<dlib::con<64,7,7,2,2,
dlib::input_rgb_image_sized<227>
>>>>>>>>>>>;


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;
    void keyPressed(int key) override;

    anet_type net;
    dlib::softmax<anet_type::subnet_type> snet;
    std::vector<string> labels;
    dlib::rand rnd;


    bool loadNext = true;
};
