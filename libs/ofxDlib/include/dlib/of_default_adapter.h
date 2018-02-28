//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofPixels.h"
#include <dlib/pixel.h>
//#include <dlib/matrix.h>


/// \sa http://dlib.net/dlib/image_processing/generic_image.h.html
namespace dlib
{


template <typename PixelType>
struct image_traits;


template <typename PixelType>
struct image_traits<ofPixels_<PixelType>>
{
    typedef PixelType pixel_type;
};


template <typename PixelType>
struct image_traits<const ofPixels_<PixelType>>
{
    typedef PixelType pixel_type;
};


template <typename PixelType>
inline void set_image_size(ofPixels_<PixelType>& img, long rows, long cols)
{
    if (img.getPixelFormat() != OF_PIXELS_GRAY)
    {
        ofLogVerbose("set_image_size") << "Reallocating ofPixels_<PixelType> with pixelFormat = OF_PIXELS_GRAY.";
    }

    img.allocate(std::size_t(cols), std::size_t(rows), OF_PIXELS_GRAY);
}


template <typename PixelType>
inline long num_rows(const ofPixels_<PixelType>& img)
{
    return img.getHeight();
}


template <typename PixelType>
inline long num_columns(const ofPixels_<PixelType>& img)
{
    return img.getWidth();
}


template <typename PixelType>
inline void* image_data(ofPixels_<PixelType>& img)
{
    if (img.isAllocated())
        return img.getData();

    return nullptr;
}


template <typename PixelType>
inline const void* image_data(const ofPixels_<PixelType>& img)
{
    if (img.isAllocated())
        return img.getData();

    return nullptr;
}


template <typename PixelType>
inline long width_step(const ofPixels_<PixelType>& img)
{
    return img.getBytesStride();
}


// defined in ofPixels.h
//void swap(ofPixels_<PixelType> & src, ofPixels_<PixelType> & dst){
//    src.swap(dst);
//}



/// \brief Specialization to automatically use RGB pixels by default with ofPixels.
template <>
struct image_traits<const ofPixels_<unsigned char>>
{
    typedef rgb_pixel pixel_type;
};


/// \brief Specialization to automatically use RGB pixels by default with ofPixels.
template <>
struct image_traits<ofPixels_<unsigned char>>
{
    typedef rgb_pixel pixel_type;
};


/// \brief Specialization to automatically use RGB pixels by default with ofPixels.
template <>
inline void set_image_size(ofPixels_<unsigned char>& img, long rows, long cols)
{
    if (img.getPixelFormat() != OF_PIXELS_RGB)
    {
        ofLogVerbose("set_image_size") << "Reallocating ofPixels_<unsigned char> with pixelFormat = OF_PIXELS_RGB.";
    }

    // We force rgb_pixel for generic ofPixels_<unsigned char> aka ofPixels.
    img.allocate(std::size_t(cols), std::size_t(rows), OF_PIXELS_RGB);
}


} // namespace dlib
