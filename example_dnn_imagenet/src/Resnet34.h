//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


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

// This `anet_type` is an alias for the big network.
using anet_type = dlib::loss_multiclass_log<dlib::fc<1000,dlib::avg_pool_everything<
level1<
level2<
level3<
level4<
dlib::max_pool<3,3,2,2,dlib::relu<dlib::affine<dlib::con<64,7,7,2,2,
dlib::input_rgb_image_sized<227>
>>>>>>>>>>>;


inline dlib::rectangle make_random_cropping_rect_resnet(const dlib::matrix<dlib::rgb_pixel>& img,
                                                        dlib::rand& rnd)
{
    // figure out what rectangle we want to crop from the image
    double mins = 0.466666666, maxs = 0.875;
    auto scale = mins + rnd.get_random_double()*(maxs-mins);
    auto size = scale*std::min(img.nr(), img.nc());
    dlib::rectangle rect(size, size);
    // randomly shift the box around
    dlib::point offset(rnd.get_random_32bit_number()%(img.nc()-rect.width()),
                       rnd.get_random_32bit_number()%(img.nr()-rect.height()));
    return move_rect(rect, offset);
}


inline void randomly_crop_images(const dlib::matrix<dlib::rgb_pixel>& img,
                                 dlib::array<dlib::matrix<dlib::rgb_pixel>>& crops,
                                 dlib::rand& rnd,
                                 long num_crops)
{
    std::vector<dlib::chip_details> dets;

    for (long i = 0; i < num_crops; ++i)
    {
        auto rect = make_random_cropping_rect_resnet(img, rnd);
        dets.push_back(dlib::chip_details(rect, dlib::chip_dims(227,227)));
    }

    dlib::extract_image_chips(img, dets, crops);

    for (auto&& img : crops)
    {
        // Also randomly flip the image
        if (rnd.get_random_double() > 0.5)
            img = dlib::fliplr(img);

        // And then randomly adjust the colors.
        dlib::apply_random_color_offset(img, rnd);
    }
}
