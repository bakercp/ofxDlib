//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "ofPixels.h"
#include "of_image_abstract.h"
#include <dlib/algs.h>
#include <dlib/pixel.h>
#include <dlib/matrix.h>
#include <dlib/image_processing.h>


namespace dlib
{


/// \brief Determine the best openFrameworks ofPixelFormat given a dlib::pixel_type.
/// \returns the best pixel format given the templated parameter.
/// \tparam dlib_pixel_type The pixel_type called like getPixelFormat<dlib_pixel_type>();
template<typename dlib_pixel_type>
inline ofPixelFormat get_of_pixel_format()
{
    if (std::is_same<dlib_pixel_type, dlib::rgb_pixel>::value)
    {
        return OF_PIXELS_RGB;
    }
    else if (std::is_same<dlib_pixel_type, dlib::rgb_alpha_pixel>::value)
    {
        return OF_PIXELS_RGBA;
    }
    else if (std::is_same<dlib_pixel_type, dlib::bgr_pixel>::value)
    {
        return OF_PIXELS_BGR;
    }
    else if (std::is_same<dlib_pixel_type, dlib::hsi_pixel>::value)
    {
        ofLogWarning("get_of_pixel_format") << "HSI pixel format not supported, using RGB.";
        return OF_PIXELS_RGB;
    }
    else if (std::is_same<dlib_pixel_type, dlib::lab_pixel>::value)
    {
        ofLogWarning("get_of_pixel_format") << "LAB pixel format not supported, using RGB.";
        return OF_PIXELS_BGR;
    }
    else if (dlib::pixel_traits<dlib_pixel_type>::num == 1)
    {
        // All other values are considered single-plane grayscale values.
        return OF_PIXELS_GRAY;
    }

    ofLogError("get_of_pixel_format") << "Unsure how we got here. Please file a bug report.";
    return OF_PIXELS_UNKNOWN;
}


// we have to have a pointer because this must be default constructable.
template <typename dlib_pixel_type, typename of_pixel_type>
class of_image
{
public:
    typedef dlib_pixel_type type;
    typedef default_memory_manager mem_manager_type;

    of_image(): of_image(nullptr)
    {
        _type_check();
    }

    of_image(ofPixels_<of_pixel_type>& pixels): of_image(&pixels)
    {
        _type_check();
    }

    of_image(ofPixels_<of_pixel_type>* pPixels): _pPixels(pPixels)
    {
        _type_check();
    }

    unsigned long size() const
    {
        if (_pPixels)
            return _pPixels->getWidth() * _pPixels->getHeight();
        return 0;
    }

    inline dlib_pixel_type* operator[](const long row)
    {
        if (_pPixels)
        {
            // make sure requires clause is not broken
            DLIB_ASSERT(0 <= row && row < nr(),
                "\tdlib_pixel_type* of_image::operator[](row)"
                << "\n\t you have asked for an out of bounds row "
                << "\n\t row:  " << row
                << "\n\t nr(): " << nr()
                << "\n\t this:  " << this
                );

            return reinterpret_cast<dlib_pixel_type*>(_pPixels->getData() + _pPixels->getBytesStride() * row);
        }

        return nullptr;
    }

    inline const dlib_pixel_type* operator[](const long row) const
    {
        if (_pPixels)
        {
            // make sure requires clause is not broken
            DLIB_ASSERT(0 <= row && row < nr(),
                "\tdlib_pixel_type* of_image::operator[](row)"
                << "\n\t you have asked for an out of bounds row "
                << "\n\t row:  " << row
                << "\n\t nr(): " << nr()
                << "\n\t this:  " << this
                );

            return reinterpret_cast<const dlib_pixel_type*>(_pPixels->getData() + _pPixels->getBytesStride() * row);
        }

        return nullptr;
    }

    inline const dlib_pixel_type& operator()(const long row, const long column) const
    {
      DLIB_ASSERT(0<= column && column < nc(),
          "\tconst dlib_pixel_type& of_image::operator()(const long rown const long column)"
          << "\n\t you have asked for an out of bounds column "
          << "\n\t column: " << column
          << "\n\t nc(): " << nc()
          << "\n\t this:  " << this
          );

      return (*this)[row][column];
    }

    inline dlib_pixel_type& operator()(const long row, const long column)
    {
        DLIB_ASSERT(0<= column && column < nc(),
          "\tdlib_pixel_type& of_image::operator()(const long rown const long column)"
          << "\n\t you have asked for an out of bounds column "
          << "\n\t column: " << column
          << "\n\t nc(): " << nc()
          << "\n\t this:  " << this
          );

        return (*this)[row][column];
    }

    long nr() const
    {
        if (_pPixels) return _pPixels->getHeight();
        return 0;
    }

    long nc() const
    {
        if (_pPixels) return _pPixels->getWidth();
        return 0;
    }

    long width_step() const
    {
        if (_pPixels) return _pPixels->getBytesStride();
        return 0;
    }

    of_image& operator=(of_image& img)
    {
        _pPixels = img._pPixels;
        return *this;
    }

    of_image& operator=(ofPixels_<of_pixel_type>* pPixels)
    {
        _pPixels = pPixels;
        return *this;
    }

    void set_image_size(long rows, long cols)
    {
        if (!_pPixels)
        {
            ofLogError("of_image::set_image_size") << "No ofPixels are wrapped.";
            return;
        }

        _pPixels->allocate(std::size_t(cols),
                           std::size_t(rows),
                           get_of_pixel_format<dlib_pixel_type>());
    }


private:
    void _type_check()
    {
        static_assert(std::is_same<typename pixel_traits<dlib_pixel_type>::basic_pixel_type, of_pixel_type>::value,
                      "The dlib::pixel_traits<dlib_pixel_type>::basic_pixel_type must match the of_pixel_type. "
                      "For example, ofFloatPixels (aka ofPixels_<float>) cannot be used with dlib::rgb_pixel because "
                      "the dlib::pixel_traits<dlib_pixel_type>::basic_pixel_type is `unsigned char`. "
                      "ofFloatPixels would only be compatible with dlib_pixel_type is `float`.");
    }

    ofPixels_<of_pixel_type>* _pPixels = nullptr;

};


// ----------------------------------------------------------------------------------------

// Define the global functions that make of_pixels_ a proper "generic image" according to
// ../image_processing/generic_image.h

template <typename dlib_pixel_type, typename of_pixel_type>
const matrix_op<op_array2d_to_mat<of_image<dlib_pixel_type, of_pixel_type>>> mat(const of_image<dlib_pixel_type, of_pixel_type>& m)
{
    typedef op_array2d_to_mat<of_image<dlib_pixel_type, of_pixel_type>> op;
    return matrix_op<op>(op(m));
}



template <typename dlib_pixel_type, typename of_pixel_type>
const matrix_op<op_array2d_to_mat<of_image<dlib_pixel_type, of_pixel_type>>> mat(const ofPixels_<of_pixel_type>& m)
{
    typedef op_array2d_to_mat<of_image<dlib_pixel_type, of_pixel_type>> op;
    auto dlib_img = of_image<dlib_pixel_type, of_pixel_type>(m);
    return matrix_op<op>(op(dlib_img));
}

// ----------------------------------------------------------------------------------------

// Define the global functions that make cv_image a proper "generic image" according to
// ../image_processing/generic_image.h
template <typename dlib_pixel_type, typename of_pixel_type>
struct image_traits<of_image<dlib_pixel_type, of_pixel_type>>
{
    typedef dlib_pixel_type pixel_type;
};

template <typename dlib_pixel_type, typename of_pixel_type>
inline long num_rows( const of_image<dlib_pixel_type, of_pixel_type>& img) { return img.nr(); }
template <typename dlib_pixel_type, typename of_pixel_type>
inline long num_columns( const of_image<dlib_pixel_type, of_pixel_type>& img) { return img.nc(); }

template <typename dlib_pixel_type, typename of_pixel_type>
inline void set_image_size(of_image<dlib_pixel_type, of_pixel_type>& img, long rows, long cols)
{
    img.set_image_size(rows, cols);
}


template <typename dlib_pixel_type, typename of_pixel_type>
inline void* image_data(of_image<dlib_pixel_type, of_pixel_type>& img)
{
    if (img.size() != 0) return &img[0][0];
    return 0;
}

template <typename dlib_pixel_type, typename of_pixel_type>
inline const void* image_data(const of_image<dlib_pixel_type, of_pixel_type>& img)
{
    if (img.size() != 0) return &img[0][0];
    return 0;
}

template <typename dlib_pixel_type, typename of_pixel_type>
inline long width_step(const of_image<dlib_pixel_type, of_pixel_type>& img)
{
    return img.width_step();
}


template <typename dlib_pixel_type, typename of_pixel_type>
inline void swap(of_image<dlib_pixel_type, of_pixel_type>& a,
                 of_image<dlib_pixel_type, of_pixel_type>& b)
{
    std::swap(a, b);
}


} // namespace dlib
