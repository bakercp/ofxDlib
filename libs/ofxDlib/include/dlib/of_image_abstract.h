//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#undef DLIB_OF_IMAGE_AbSTRACT_H_
#ifdef DLIB_OF_IMAGE_AbSTRACT_H_


#include "ofPixels.h"
#include <dlib/algs.h>
#include <dlib/pixel.h>
#include <dlib/matrix.h>


/// \sa http://dlib.net/dlib/opencv/cv_image_abstract.h.html
namespace dlib
{

template <
    typename pixel_type,
    typename of_pixel_type
    >
class of_image
{
public:
    typedef pixel_type type;
    typedef default_memory_manager mem_manager_type;

    of_image (const ofPixels_<of_pixel_type>* img);

    of_image (const ofPixels_<of_pixel_type> img);
    of_image();
    ~of_image ();
    long nr() const;
    long nc() const;
    unsigned long size () const;
    inline pixel_type* operator[] (const long row);
    inline const pixel_type* operator[] (const long row) const;
    inline const pixel_type& operator()(const long row, const long column) const;
    inline pixel_type& operator()(const long row, const long column);
    of_image& operator= (const of_image& item);
    of_image& operator=(const ofPixels_<of_pixel_type>* img);
    of_image& operator=(const ofPixels_<of_pixel_type> img);
    long width_step () const;
};

// ----------------------------------------------------------------------------------------

template <typename pixel_type, typename of_pixel_type>
const matrix_exp<pixel_type> mat (const of_image<pixel_type, of_pixel_type>& img);


template <
    typename pixel_type,
    typename of_pixel_type
    >
class of_image
{
public:
    typedef pixel_type type;
    typedef default_memory_manager mem_manager_type;

    of_image (const ofPixels_<of_pixel_type>* img);

    of_image (const ofPixels_<of_pixel_type> img);
    of_image();
    ~of_image ();
    long nr() const;
    long nc() const;
    unsigned long size () const;
    inline pixel_type* operator[] (const long row);
    inline const pixel_type* operator[] (const long row) const;
    inline const pixel_type& operator()(const long row, const long column) const;
    inline pixel_type& operator()(const long row, const long column);
    of_image& operator= (const of_image& item);
    of_image& operator=(const ofPixels_<of_pixel_type>* img);
    of_image& operator=(const ofPixels_<of_pixel_type> img);
    long width_step () const;
};

// ----------------------------------------------------------------------------------------

template <typename pixel_type, typename of_pixel_type>
const matrix_exp<pixel_type> mat (const of_image<pixel_type, of_pixel_type>& img);


} // namespace dlib


#endif // DLIB_OF_IMAGE_AbSTRACT_H_
