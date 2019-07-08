//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "ofPixels.h"
#include "dlib/pixel.h"
#include "dlib/of_image.h"


/// \sa http://dlib.net/dlib/opencv/to_open_cv.h.html
namespace dlib
{


template <typename image_type>
ofPixels_<typename pixel_traits<typename image_traits<image_type>::pixel_type>::basic_pixel_type> to_of_pixels(image_type& img)
{
    typedef typename image_traits<image_type>::pixel_type type;
    typedef typename pixel_traits<type>::basic_pixel_type basic_pixel_type;

    ofPixels_<basic_pixel_type> pixels;

    pixels.setFromExternalPixels(reinterpret_cast<basic_pixel_type*>(image_data(img)),
                                 std::size_t(num_columns(img)),
                                 std::size_t(num_rows(img)),
                                 get_of_pixel_format<type>());
    return pixels;
}


template <typename image_type>
ofPixels_<typename pixel_traits<typename image_traits<image_type>::pixel_type>::basic_pixel_type> to_of_pixels(const image_type& img)
{
    typedef typename image_traits<image_type>::pixel_type type;
    typedef typename pixel_traits<type>::basic_pixel_type basic_pixel_type;

    ofPixels_<basic_pixel_type> pixels;

    pixels.setFromPixels(reinterpret_cast<const basic_pixel_type*>(image_data(img)),
                         std::size_t(num_columns(img)),
                         std::size_t(num_rows(img)),
                         get_of_pixel_format<type>());
    return pixels;
}


} // namespace dlib
