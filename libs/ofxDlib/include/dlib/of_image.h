//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofPixels.h"
#include <dlib/algs.h>
#include <dlib/pixel.h>
#include <dlib/matrix.h>
#include <dlib/image_processing.h>


namespace dlib
{

/// \brief Determine the best openFrameworks ofPixelFormat given a dlib::pixel_type.
/// \returns the best pixel format given the templated parameter.
/// \tparam The pixel_type called like getPixelFormat<pixel_type>();
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
template <typename dlib_pixel_type,
          template <class> class HasPixelsClass_,
          typename PixelType>
class of_pixels_
{
public:
    typedef dlib_pixel_type type;
    typedef default_memory_manager mem_manager_type;

    of_pixels_(): of_pixels_(nullptr)
    {
        _type_check();
    }

    of_pixels_(HasPixelsClass_<PixelType>& pixels): of_pixels_(&pixels)
    {
        _type_check();
    }

    of_pixels_(HasPixelsClass_<PixelType>* pPixels): _pPixels(pPixels)
    {
        _type_check();
    }

    unsigned long size () const
    {
        if (_pPixels)
            return _pPixels->getWidth() * _pPixels->getHeight();
        return 0;
    }

    inline dlib_pixel_type* operator[](const long row )
    {
        if (_pPixels)
        {
            // make sure requires clause is not broken
            DLIB_ASSERT(0 <= row && row < nr(),
                "\tpixel_type* of_image::operator[](row)"
                << "\n\t you have asked for an out of bounds row "
                << "\n\t row:  " << row
                << "\n\t nr(): " << nr()
                << "\n\t this:  " << this
                );

            return reinterpret_cast<dlib_pixel_type*>(_pPixels->getData() + _pPixels->getBytesStride() * row);
        }

        return nullptr;
    }

    inline const dlib_pixel_type* operator[](const long row ) const
    {
        if (_pPixels)
        {
            // make sure requires clause is not broken
            DLIB_ASSERT(0 <= row && row < nr(),
                "\tpixel_type* of_image::operator[](row)"
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
          "\tcont pixel_type& of_image::operator()(const long rown const long column)"
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
          "\tcont pixel_type& of_image::operator()(const long rown const long column)"
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

    of_pixels_& operator=(of_pixels_& img)
    {
        _pPixels = img._pPixels;
        return *this;
    }

    of_pixels_& operator=(HasPixelsClass_<PixelType>* pPixels)
    {
        _pPixels = pPixels;
        return *this;
    }

    void set_image_size(long rows, long cols)
    {
        if (!_pPixels)
        {
            ofLogError("of_pixels_::set_image_size") << "No ofPixels are wrapped.";
            return;
        }

        _pPixels->allocate(std::size_t(cols),
                           std::size_t(rows),
                           get_of_pixel_format<dlib_pixel_type>());
    }


private:
    void _type_check()
    {
        static_assert(std::is_same<typename pixel_traits<dlib_pixel_type>::basic_pixel_type, PixelType>::value,
                      "The dlib::pixel_traits<dlib_pixel_type>::basic_pixel_type must match the PixelType. "
                      "For example, ofFloatPixels (aka ofPixels_<float>) cannot be used with dlib::rgb_pixel because "
                      "the dlib::pixel_traits<dlib_pixel_type>::basic_pixel_type is `unsigned char`. "
                      "ofFloatPixels would only be compatible with dlib_pixel_type is `float`.");
    }

    HasPixelsClass_<PixelType>* _pPixels = nullptr;

};


// ----------------------------------------------------------------------------------------

template <typename dlib_pixel_type, template <class> class HasPixelsClass_, typename PixelType>
const matrix_op<op_array2d_to_mat<of_pixels_<dlib_pixel_type, HasPixelsClass_, PixelType>>> mat (const of_pixels_<dlib_pixel_type, HasPixelsClass_, PixelType>& m)
{
    typedef op_array2d_to_mat<of_pixels_<dlib_pixel_type, HasPixelsClass_, PixelType>> op;
    return matrix_op<op>(op(m));
}

// ----------------------------------------------------------------------------------------

// Define the global functions that make of_pixels_ a proper "generic image" according to
// ../image_processing/generic_image.h
template <typename dlib_pixel_type, template <class> class HasPixelsClass_, typename PixelType>
struct image_traits<of_pixels_<dlib_pixel_type, HasPixelsClass_, PixelType> >
{
    typedef dlib_pixel_type pixel_type;
};

template <typename dlib_pixel_type, template <class> class HasPixelsClass_, typename PixelType>
inline long num_rows( const of_pixels_<dlib_pixel_type, HasPixelsClass_, PixelType>& img) { return img.nr(); }

template <typename dlib_pixel_type, template <class> class HasPixelsClass_, typename PixelType>
inline long num_columns( const of_pixels_<dlib_pixel_type, HasPixelsClass_, PixelType>& img) { return img.nc(); }

template <typename dlib_pixel_type, template <class> class HasPixelsClass_, typename PixelType>
inline void* image_data(of_pixels_<dlib_pixel_type, HasPixelsClass_, PixelType>& img)
{
    if (img.size() != 0)
        return &img[0][0];
    else
        return 0;
}

template <typename dlib_pixel_type, template <class> class HasPixelsClass_, typename PixelType>
inline const void* image_data(const of_pixels_<dlib_pixel_type, HasPixelsClass_, PixelType>& img)
{
    if (img.size() != 0)
        return &img[0][0];
    else
        return 0;
}

template <typename dlib_pixel_type, template <class> class HasPixelsClass_, typename PixelType>
inline long width_step(const of_pixels_<dlib_pixel_type, HasPixelsClass_, PixelType>& img)
{
    return img.width_step();
}

template <typename dlib_pixel_type, template <class> class HasPixelsClass_, typename PixelType>
inline void set_image_size(of_pixels_<dlib_pixel_type, HasPixelsClass_, PixelType>& img, long rows, long cols)
{
    img.set_image_size(rows, cols);
}



//// we have to have a pointer because this must be default constructable.
//template <typename dlib_pixel_type,
//          template <class> class HasPixelsClass_,
//          typename PixelType>
//class of_pixels_const_
//{
//public:
//    typedef dlib_pixel_type type;
//    typedef default_memory_manager mem_manager_type;

//    of_pixels_const_(): of_pixels_const_(nullptr)
//    {
//        _type_check();
//    }

//    of_pixels_const_(const HasPixelsClass_<PixelType>& pixels): of_pixels_const_(&pixels)
//    {
//        _type_check();
//    }

//    of_pixels_const_(const HasPixelsClass_<PixelType>* pPixels): _pPixels(pPixels)
//    {
//        _type_check();
//    }

//    unsigned long size () const
//    {
//        if (_pPixels)
//            return _pPixels->getWidth() * _pPixels->getHeight();
//        return 0;
//    }

//    inline const dlib_pixel_type* operator[](const long row ) const
//    {
//        if (_pPixels)
//        {
//            // make sure requires clause is not broken
//            DLIB_ASSERT(0 <= row && row < nr(),
//                "\tpixel_type* of_image::operator[](row)"
//                << "\n\t you have asked for an out of bounds row "
//                << "\n\t row:  " << row
//                << "\n\t nr(): " << nr()
//                << "\n\t this:  " << this
//                );

//            return reinterpret_cast<const dlib_pixel_type*>(_pPixels->getData() + _pPixels->getBytesStride() * row);
//        }

//        return nullptr;
//    }

//    inline const dlib_pixel_type& operator()(const long row, const long column) const
//    {
//      DLIB_ASSERT(0<= column && column < nc(),
//          "\tcont pixel_type& of_image::operator()(const long rown const long column)"
//          << "\n\t you have asked for an out of bounds column "
//          << "\n\t column: " << column
//          << "\n\t nc(): " << nc()
//          << "\n\t this:  " << this
//          );

//      return (*this)[row][column];
//    }

//    inline dlib_pixel_type& operator()(const long row, const long column)
//    {
//      DLIB_ASSERT(0<= column && column < nc(),
//          "\tcont pixel_type& of_image::operator()(const long rown const long column)"
//          << "\n\t you have asked for an out of bounds column "
//          << "\n\t column: " << column
//          << "\n\t nc(): " << nc()
//          << "\n\t this:  " << this
//          );

//      return (*this)[row][column];
//    }

//    long nr() const
//    {
//        if (_pPixels) return _pPixels->getHeight();
//        return 0;
//    }

//    long nc() const
//    {
//        if (_pPixels) return _pPixels->getWidth();
//        return 0;
//    }

//    long width_step() const
//    {
//        if (_pPixels) return _pPixels->getBytesStride();
//        return 0;
//    }

//    const of_pixels_const_& operator=(const of_pixels_const_& img)
//    {
//        _pPixels = img._pPixels;
//        return *this;
//    }

//    const of_pixels_const_& operator=(const HasPixelsClass_<PixelType>* pPixels)
//    {
//        _pPixels = pPixels;
//        return *this;
//    }

//private:
//    void _type_check()
//    {
//        static_assert(std::is_same<typename pixel_traits<dlib_pixel_type>::basic_pixel_type, PixelType>::value,
//                      "The dlib::pixel_traits<dlib_pixel_type>::basic_pixel_type must match the PixelType. "
//                      "For example, ofFloatPixels (aka ofPixels_<float>) cannot be used with dlib::rgb_pixel because "
//                      "the dlib::pixel_traits<dlib_pixel_type>::basic_pixel_type is `unsigned char`. "
//                      "ofFloatPixels would only be compatible with dlib_pixel_type is `float`.");
//    }

//    const HasPixelsClass_<PixelType>* _pPixels = nullptr;

//};


}
