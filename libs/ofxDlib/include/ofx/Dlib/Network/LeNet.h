//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "dlib/dnn.h"


namespace ofx {
namespace Dlib {
namespace Network {


using LeNet = dlib::loss_multiclass_log<
                                        dlib::fc<10,
                                        dlib::relu<dlib::fc<84,
                                        dlib::relu<dlib::fc<120,
                                        dlib::max_pool<2,2,2,2,dlib::relu<dlib::con<16,5,5,1,1,
                                        dlib::max_pool<2,2,2,2,dlib::relu<dlib::con<6,5,5,1,1,
                                        dlib::input<dlib::matrix<unsigned char>>
                                        >>>>>>>>>>>>;


} } } // namespace ofx::Dlib::Network
