//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once

#include "ofConstants.h"
#include "ofPixels.h"
#include <dlib/matrix.h>

/// \sa http://dlib.net/dlib/image_processing/generic_image.h.html
namespace dlib
{

template <typename T>
struct image_traits;

template <>
struct image_traits<ofPixels_<unsigned char>>
{
    typedef rgb_pixel pixel_type;
};

template <>
struct image_traits<const ofPixels_<unsigned char>>
{
    typedef rgb_pixel pixel_type;
};

template <typename T>
struct image_traits<ofPixels_<T>>
{
    typedef T pixel_type;
};

template <typename T>
struct image_traits<const ofPixels_<T>>
{
    typedef rgb_pixel pixel_type;
};


template <>
inline void set_image_size(ofPixels_<unsigned char>& img, long rows, long cols)
{
    // We force rgb_pixel for generic ofPixels_<unsigned char> aka ofPixels.
    img.allocate(std::size_t(cols), std::size_t(rows), OF_PIXELS_RGB);
}

template <typename T>
inline long num_rows(const ofPixels_<T>& img)
{
    return img.getHeight();
}

template <typename T>
inline long num_columns(const ofPixels_<T>& img)
{
    return img.getWidth();
}

template <typename T>
inline void* image_data(ofPixels_<T>& img)
{
    if (img.isAllocated())
        return img.getData();

    return nullptr;
}

template <typename T>
inline const void* image_data(const ofPixels_<T>& img)
{
    if (img.isAllocated())
        return img.getData();

    return nullptr;
}


template <typename T>
inline long width_step(const ofPixels_<T>& img)
{
    return img.getBytesStride();
}


/*
// defined in ofPixels.h
void swap(ofPixels_<PixelType> & src, ofPixels_<PixelType> & dst){
    src.swap(dst);
}
*/



 }


namespace ofx {
namespace Dlib {


} } // namespace ofx::Dlib
