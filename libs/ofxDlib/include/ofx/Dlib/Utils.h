//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <vector>
#include <dlib/geometry.h>
#include <dlib/image_processing/full_object_detection.h>
#include "ofPixels.h"
#include "ofRectangle.h"
#include "ofTypes.h"


namespace ofx {
namespace Dlib {


/// \brief Wrap a dlib::vector to a glm vector with no copies.
/// \param v The input dlib vector.
/// \tparam T vector data type.
/// \returns a const reference to the glm vector.
template<typename T>
inline const glm::tvec4<T>& toOf(const dlib::vector<T, 4>& v)
{
    return *reinterpret_cast<const glm::tvec4<T>*>(&v);
}

/// \brief Wrap a dlib::vector to a glm vector with no copies.
/// \param v The input dlib vector.
/// \tparam T vector data type.
/// \returns a const reference to the glm vector.
template<typename T>
inline const glm::tvec3<T>& toOf(const dlib::vector<T, 3>& v)
{
    return *reinterpret_cast<const glm::tvec3<T>*>(&v);
}

/// \brief Wrap a dlib::vector to a glm vector with no copies.
/// \param v The input dlib vector.
/// \tparam T vector data type.
/// \returns a const reference to the glm vector.
template<typename T>
inline const glm::tvec2<T>& toOf(const dlib::vector<T, 2>& v)
{
    return *reinterpret_cast<const glm::tvec2<T>*>(&v);
}

/// \brief Wrap a dlib::vector to a glm vector with no copies.
/// \param v The input dlib vector.
/// \tparam T vector data type.
/// \returns a const reference to the glm vector.
template<typename T>
inline const glm::tvec1<T>& toOf(const dlib::vector<T, 1>& v)
{
    return *reinterpret_cast<const glm::tvec1<T>*>(&v);
}

/// \brief Copy a dlib::rectangle to ofRectangle.
/// \param r the dlib::rectangle.
/// \returns an ofRectangle.
inline ofRectangle toOf(const dlib::rectangle& r)
{
    return ofRectangle(r.left(), r.top(), r.width(), r.height());
}

/// \brief Copy a dlib::rgb_pixel to an ofColor.
/// \param v The dlib::rgb_pixel to convert.
/// \returns an ofColor.
inline ofColor toOf(const dlib::rgb_pixel& v)
{
    return ofColor(v.red, v.green, v.blue);
}

/// \brief Convert a dlib::rgb_alpha_pixel to an ofColor.
/// \param v The dlib::rgb_alpha_pixel to convert.
/// \returns an ofColor.
inline ofColor toOf(const dlib::rgb_alpha_pixel& v)
{
    return ofColor(v.red, v.green, v.blue, v.alpha);
}


/// \todo Re-work.
inline void toDlib(const ofPixels& input,
                   dlib::array2d<dlib::rgb_pixel>& output)
{
    auto width = input.getWidth();
    auto height = input.getHeight();
    auto chans = input.getNumChannels();

    const unsigned char* data = input.getData();

    output.set_size(static_cast<long>(height), static_cast<long>(width));

    for (std::size_t n = 0; n < height; ++n)
    {
        const unsigned char* v =  &data[n * width *  chans];

        for (std::size_t m = 0; m < width; ++m)
        {
            if (chans == 1)
            {
                auto p = v[m];
                dlib::assign_pixel(output[n][m], p);
            }
            else
            {
                dlib::rgb_pixel p;
                p.red = v[m * 3];
                p.green = v[m * 3 + 1];
                p.blue = v[m * 3 + 2];
                dlib::assign_pixel(output[n][m], p);
            }
        }
    }
}


/// \todo Re-work.
inline ofPixels toOf(const dlib::matrix<unsigned char>& input)
{
    auto w = std::size_t(input.nc());
    auto h = std::size_t(input.nr());

    ofPixels output;

    output.allocate(w, h, 1);

    for (std::size_t y = 0; y < h; ++y)
    {
        for (std::size_t x = 0; x < w; ++x)
        {
            output.setColor(x, y, ofColor(input(y, x)));
        }
    }

    return output;
}


/// \todo Re-work.
inline bool toOf(const dlib::array2d<dlib::rgb_pixel>& inPix,ofPixels& outPix ){

    int h = inPix.nr(); //number of rows
    int w = inPix.nc(); //nuber of cols

//    auto w = std::size_t(input.nc());
//    auto h = std::size_t(input.nr());
//
//
    ofLog()<<"inPix.nr() "<<h<<",  inPix.nc() "<<w;
    outPix.allocate(w, h, 3);

    for(int y = 0; y<h; y++){
        for(int x=0; x<w;x++){
            outPix.setColor(x, y, toOf(inPix[y][x]));
        }
    }
    return true;
}

/// \todo Re-work.
inline bool toOf(const dlib::array2d<unsigned char>& inPix,ofPixels& outPix ){

    int h = inPix.nr(); //number of rows
    int w = inPix.nc(); //nuber of cols
    ofLog()<<"inPix.nr() "<<h<<",  inPix.nc() "<<w;
    outPix.allocate(w, h, 1);

    for(int y = 0; y<h; y++){
        for(int x=0; x<w;x++){
            outPix.setColor(x, y, ofColor(inPix[y][x]));
        }
    }
    return true;
}


//template<typename image_type>
//ofPixels toOf(const image_type& img)
//{
//    if (dlib::image_size(img) == 0)
//        return ofPixels();
//
//    typedef typename dlib::image_traits<image_type>::pixel_type type;
//
////    if (dlib::pixel_traits<type>::num == 1)
////    {
////        return cv::Mat(num_rows(img),
////                       num_columns(img),
////                       cv::DataType<type>::type,
////                       image_data(img),
////                       width_step(img));
////    }
////    else
////    {
////        int depth = sizeof(typename pixel_traits<type>::basic_pixel_type) * 8;
////        int channels = dlib::pixel_traits<type>::num;
////        int thetype = CV_MAKETYPE(depth, channels);
////        return cv::Mat(num_rows(img), num_columns(img), thetype, image_data(img), width_step(img));
////    }
//
//}


//template <
//typename image_type
//>
//cv::Mat toMat (
//               image_type& img
//               )
//{
//    if (image_size(img) == 0)
//        return cv::Mat();
//
//    typedef typename image_traits<image_type>::pixel_type type;
//    if (pixel_traits<type>::num == 1)
//    {
//        return cv::Mat(num_rows(img), num_columns(img), cv::DataType<type>::type, image_data(img), width_step(img));
//    }
//    else
//    {
//        int depth = sizeof(typename pixel_traits<type>::basic_pixel_type)*8;
//        int channels = pixel_traits<type>::num;
//        int thetype = CV_MAKETYPE(depth, channels);
//        return cv::Mat(num_rows(img), num_columns(img), thetype, image_data(img), width_step(img));
//    }
//}


/// \brief scale the given object detection by the given amount.
/// \param v The object to be scaled.
/// \param scale The amount the scale the object by.
inline void scale(dlib::full_object_detection& v, double scale)
{
    for (unsigned int i = 0; i < v.num_parts(); ++i)
    {
        v.part(i) *= scale;
    }

    auto oldRect = v.get_rect();

    v.get_rect() = dlib::rectangle(oldRect.left() * scale,
                                   oldRect.top() * scale,
                                   oldRect.right() * scale,
                                   oldRect.bottom() * scale);


}


} } // namespace ofx::Dlib

