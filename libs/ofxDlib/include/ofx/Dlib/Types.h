//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once

#include "ofConstants.h"
#include "ofPixels.h"


/// \sa http://dlib.net/dlib/image_processing/generic_image.h.html
namespace dlib
{
    template <typename T>
    struct image_traits;

    template <typename T>
    struct image_traits<ofPixels_<T>>
    {
        typedef T pixel_type;
    };

    template <typename T>
    struct image_traits<const ofPixels_<T>>
    {
        typedef T pixel_type;
    };

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
    inline void set_image_size(ofPixels_<T>& img, long rows, long cols)
    {
        ofPixelFormat pixelFormat = img.getPixelFormat();

        if (pixelFormat == OF_PIXELS_UNKNOWN)
        {
            pixelFormat = OF_PIXELS_GRAY;
        }

        img.allocate(std::size_t(cols),
                     std::size_t(rows),
                     pixelFormat);
    }

    template <typename T>
    inline void* image_data(ofPixels_<T>& img)
    {
        if (img.isAllocated())
            return img.getData();
        else
            return nullptr;
    }

    template <typename T>
    inline const void* image_data(const ofPixels_<T>& img)
    {
        if (img.isAllocated())
            return img.getData();
        else
            return nullptr;
    }


    template <typename T>
    inline long width_step(const ofPixels_<T>& img)
    {
        return img.getWidth() * img.getBytesPerPixel();
    }
}


namespace ofx {
namespace Dlib {


} } // namespace ofx::Dlib
