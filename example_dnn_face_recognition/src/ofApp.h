//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


//
// From http://dlib.net/dnn_face_recognition_ex.cpp.html
//
// This is an example illustrating the use of the deep learning tools from the dlib C++
// Library.  In it, we will show how to do face recognition.  This example uses the
// pretrained dlib_face_recognition_resnet_model_v1 model which is freely available from
// the dlib web site.  This model has a 99.38% accuracy on the standard LFW face
// recognition benchmark, which is comparable to other state-of-the-art methods for face
// recognition as of February 2017.
//
// In this example, we will use dlib to do face clustering.  Included in the examples
// folder is an image, bald_guys.jpg, which contains a bunch of photos of action movie
// stars Vin Diesel, The Rock, Jason Statham, and Bruce Willis.   We will use dlib to
// automatically find their faces in the image and then to automatically determine how
// many people there are (4 in this case) as well as which faces belong to each person.
//
// Finally, this example uses a network with the loss_metric loss.  Therefore, if you want
// to learn how to train your own models, or to get a general introduction to this loss
// layer, you should read the dnn_metric_learning_ex.cpp and
// dnn_metric_learning_on_images_ex.cpp examples.
//


#include "ofMain.h"
#include "ofxDlib.h"


using namespace dlib;
using namespace std;

// ----------------------------------------------------------------------------------------

// The next bit of code defines a ResNet network.  It's basically copied
// and pasted from the dnn_imagenet_ex.cpp example, except we replaced the loss
// layer with loss_metric and made the network somewhat smaller.  Go read the introductory
// dlib DNN examples to learn what all this stuff means.
//
// Also, the dnn_metric_learning_on_images_ex.cpp example shows how to train this network.
// The dlib_face_recognition_resnet_model_v1 model used by this example was trained using
// essentially the code shown in dnn_metric_learning_on_images_ex.cpp except the
// mini-batches were made larger (35x15 instead of 5x5), the iterations without progress
// was set to 10000, and the training dataset consisted of about 3 million images instead of
// 55.  Also, the input layer was locked to images of size 150.
template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N,BN,1,tag1<SUBNET>>>;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2,2,2,2,skip1<tag2<block<N,BN,2,tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block  = BN<con<N,3,3,1,1,relu<BN<con<N,3,3,stride,stride,SUBNET>>>>>;

template <int N, typename SUBNET> using ares      = relu<residual<block,N,affine,SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block,N,affine,SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256,SUBNET>;
template <typename SUBNET> using alevel1 = ares<256,ares<256,ares_down<256,SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128,ares<128,ares_down<128,SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64,ares<64,ares<64,ares_down<64,SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32,ares<32,ares<32,SUBNET>>>;

using anet_type = loss_metric<fc_no_bias<128,avg_pool_everything<
                            alevel0<
                            alevel1<
                            alevel2<
                            alevel3<
                            alevel4<
                            max_pool<3,3,2,2,relu<affine<con<32,7,7,2,2,
                            input_rgb_image_sized<150>
                            >>>>>>>>>>>>;


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;


    std::map<std::size_t, std::vector<ofRectangle>> results;

    ofImage image;
};
