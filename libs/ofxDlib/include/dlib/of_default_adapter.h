//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "ofGraphicsBaseTypes.h"
#include "ofImage.h"
#include "ofPixels.h"
#include "ofVideoGrabber.h"
#include "ofVideoPlayer.h"
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
    return static_cast<long>(img.getHeight());
}


template <typename PixelType>
inline long num_columns(const ofPixels_<PixelType>& img)
{
    return static_cast<long>(img.getWidth());
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
    return static_cast<long>(img.getBytesStride());
}


/// \note This is also defined in ofPixels.h, but not in the dlib namespace.
//template <typename PixelType>
//void swap(ofPixels_<PixelType>& src, ofPixels_<PixelType>& dst){
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


// =============================================================================
// TODO: Currently unable to get a template to deduce a base class.


//template <typename PixelType>
//struct image_traits<ofBaseHasPixels_<PixelType>>
//{
//    typedef PixelType pixel_type;
//};


//template <typename PixelType>
//struct image_traits<const ofBaseHasPixels_<PixelType>>
//{
//    typedef PixelType pixel_type;
//};


//template <typename PixelType>
//inline void set_image_size(ofBaseHasPixels_<PixelType>& img, long rows, long cols)
//{
//    set_image_size(img.getPixels(), rows, cols);
//}


//template <typename PixelType>
//inline long num_rows(const ofBaseHasPixels_<PixelType>& img)
//{
//    return num_rows(img.getPixels());
//}


//template <typename PixelType>
//inline long num_columns(const ofBaseHasPixels_<PixelType>& img)
//{
//    return num_columns(img.getPixels());
//}


//template <typename PixelType>
//inline void* image_data(ofBaseHasPixels_<PixelType>& img)
//{
//    return image_data(img.getPixels());
//}


//template <typename PixelType>
//inline const void* image_data(const ofBaseHasPixels_<PixelType>& img)
//{
//    return image_data(img.getPixels());
//}


//template <typename PixelType>
//inline long width_step(const ofBaseHasPixels_<PixelType>& img)
//{
//    return width_step(img.getPixels());
//}


///// \note This is also defined in ofPixels.h, but not in the dlib namespace.
////template <typename PixelType>
////void swap(ofPixels_<PixelType>& src, ofPixels_<PixelType>& dst){
////    src.swap(dst);
////}



///// \brief Specialization to automatically use RGB pixels by default with ofPixels.
//template <>
//struct image_traits<const ofBaseHasPixels_<unsigned char>>
//{
//    typedef rgb_pixel pixel_type;
//};


///// \brief Specialization to automatically use RGB pixels by default with ofPixels.
//template <>
//struct image_traits<ofBaseHasPixels_<unsigned char>>
//{
//    typedef rgb_pixel pixel_type;
//};


///// \brief Specialization to automatically use RGB pixels by default with ofPixels.
//template <>
//inline void set_image_size(ofBaseHasPixels_<unsigned char>& img, long rows, long cols)
//{
//    set_image_size(img.getPixels(), rows, cols);
//}

// =============================================================================


template <typename PixelType>
struct image_traits<ofImage_<PixelType>>
{
    typedef PixelType pixel_type;
};


template <typename PixelType>
struct image_traits<const ofImage_<PixelType>>
{
    typedef PixelType pixel_type;
};


template <typename PixelType>
inline void set_image_size(ofImage_<PixelType>& img, long rows, long cols)
{
    set_image_size(img.getPixels(), rows, cols);
}


template <typename PixelType>
inline long num_rows(const ofImage_<PixelType>& img)
{
    return num_rows(img.getPixels());
}


template <typename PixelType>
inline long num_columns(const ofImage_<PixelType>& img)
{
    return num_columns(img.getPixels());
}


template <typename PixelType>
inline void* image_data(ofImage_<PixelType>& img)
{
    return image_data(img.getPixels());
}


template <typename PixelType>
inline const void* image_data(const ofImage_<PixelType>& img)
{
    return image_data(img.getPixels());
}


template <typename PixelType>
inline long width_step(const ofImage_<PixelType>& img)
{
    return width_step(img.getPixels());
}


/// \note This is also defined in ofPixels.h, but not in the dlib namespace.
template <typename PixelType>
void swap(ofImage_<PixelType>& src, ofImage_<PixelType>& dst){
    src.swap(dst);
}



/// \brief Specialization to automatically use RGB pixels by default with ofPixels.
template <>
struct image_traits<const ofImage_<unsigned char>>
{
    typedef rgb_pixel pixel_type;
};


/// \brief Specialization to automatically use RGB pixels by default with ofPixels.
template <>
struct image_traits<ofImage_<unsigned char>>
{
    typedef rgb_pixel pixel_type;
};


/// \brief Specialization to automatically use RGB pixels by default with ofPixels.
template <>
inline void set_image_size(ofImage_<unsigned char>& img, long rows, long cols)
{
    set_image_size(img.getPixels(), rows, cols);
}


// =============================================================================
// Currently unable to get a template to deduce a base class.


template<>
struct image_traits<ofVideoGrabber>
{
    typedef rgb_pixel pixel_type;
};


template <>
struct image_traits<const ofVideoGrabber>
{
    typedef rgb_pixel pixel_type;
};


inline void set_image_size(ofVideoGrabber& img, long rows, long cols)
{
    set_image_size(img.getPixels(), rows, cols);
}


inline long num_rows(const ofVideoGrabber& img)
{
    return num_rows(img.getPixels());
}


inline long num_columns(const ofVideoGrabber& img)
{
    return num_columns(img.getPixels());
}


inline void* image_data(ofVideoGrabber& img)
{
    return image_data(img.getPixels());
}


inline const void* image_data(const ofVideoGrabber& img)
{
    return image_data(img.getPixels());
}


inline long width_step(const ofVideoGrabber& img)
{
    return width_step(img.getPixels());
}


// =============================================================================




// =============================================================================
// Currently unable to get a template to deduce a base class.


template<>
struct image_traits<ofVideoPlayer>
{
    typedef rgb_pixel pixel_type;
};


template <>
struct image_traits<const ofVideoPlayer>
{
    typedef rgb_pixel pixel_type;
};


inline void set_image_size(ofVideoPlayer& img, long rows, long cols)
{
    set_image_size(img.getPixels(), rows, cols);
}


inline long num_rows(const ofVideoPlayer& img)
{
    return num_rows(img.getPixels());
}


inline long num_columns(const ofVideoPlayer& img)
{
    return num_columns(img.getPixels());
}


inline void* image_data(ofVideoPlayer& img)
{
    return image_data(img.getPixels());
}


inline const void* image_data(const ofVideoPlayer& img)
{
    return image_data(img.getPixels());
}


inline long width_step(const ofVideoPlayer& img)
{
    return width_step(img.getPixels());
}

} // namespace dlib
