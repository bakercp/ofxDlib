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


// TODO enable this class if PixelType ==

//std::is_same<PixelType, pixel_traits<dlib_pixel_type>::basic_pixel_type>

//template<class T>
//class A<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
//    template<class T>
//    class A<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {


//template <template <class> class HasPixelsClass_,
//          typename PixelType,
//          typename dlib_pixel_type,
//          typename Enable = void>
//class of_pixels_
//{};


//typename std::enable_if<std::is_integral<T>::value>::type

// we have to have a pointer because this must be default constructable.
template <template <class> class HasPixelsClass_,
          typename PixelType,
          typename dlib_pixel_type>
class of_pixels_
{
public:
    typedef dlib_pixel_type type;
    typedef default_memory_manager mem_manager_type;

    of_pixels_(): of_pixels_(nullptr) {}
    of_pixels_(HasPixelsClass_<PixelType>& pixels): of_pixels_(&pixels) {}
    of_pixels_(HasPixelsClass_<PixelType>* pPixels): _pPixels(pPixels) {}

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

        ofPixelFormat format;

        if (std::is_same<dlib_pixel_type, dlib::rgb_pixel>::value)
        {
            format = OF_PIXELS_RGB;
        }
        else if (std::is_same<dlib_pixel_type, dlib::rgb_alpha_pixel>::value)
        {
            format = OF_PIXELS_RGBA;
        }
        else if (std::is_same<dlib_pixel_type, dlib::bgr_pixel>::value)
        {
            format = OF_PIXELS_BGR;
        }
        else if (std::is_same<dlib_pixel_type, dlib::hsi_pixel>::value)
        {
            ofLogWarning("getPixelFormat") << "HSI pixel format not supported, using RGB.";
            format = OF_PIXELS_RGB;
        }
        else if (std::is_same<dlib_pixel_type, dlib::lab_pixel>::value)
        {
            ofLogWarning("getPixelFormat") << "LAB pixel format not supported, using RGB.";
            format = OF_PIXELS_BGR;
        }
        else if (dlib::pixel_traits<dlib_pixel_type>::num == 1)
        {
            // All other values are considered single-plane grayscale values.
            format = OF_PIXELS_GRAY;
        }
        else
        {
            ofLogWarning("getPixelFormat") << "Unsupported option.";
            format = OF_PIXELS_UNKNOWN;
        }

        _pPixels->allocate(std::size_t(cols), std::size_t(rows), format);
    }


private:
    HasPixelsClass_<PixelType>* _pPixels = nullptr;

};

// ----------------------------------------------------------------------------------------

template <template <class> class HasPixelsClass_, typename PixelType, typename dlib_pixel_type>
const matrix_op<op_array2d_to_mat<of_pixels_<HasPixelsClass_, PixelType, dlib_pixel_type>>> mat (const of_pixels_<HasPixelsClass_, PixelType, dlib_pixel_type>& m)
{
    typedef op_array2d_to_mat<of_pixels_<HasPixelsClass_, PixelType, dlib_pixel_type>> op;
    return matrix_op<op>(op(m));
}

// ----------------------------------------------------------------------------------------

// Define the global functions that make of_pixels_ a proper "generic image" according to
// ../image_processing/generic_image.h
template <template <class> class HasPixelsClass_, typename PixelType, typename dlib_pixel_type>
struct image_traits<of_pixels_<HasPixelsClass_, PixelType, dlib_pixel_type> >
{
    typedef dlib_pixel_type pixel_type;
};

template <template <class> class HasPixelsClass_, typename PixelType, typename dlib_pixel_type>
inline long num_rows( const of_pixels_<HasPixelsClass_, PixelType, dlib_pixel_type>& img) { return img.nr(); }
template <template <class> class HasPixelsClass_, typename PixelType, typename dlib_pixel_type>
inline long num_columns( const of_pixels_<HasPixelsClass_, PixelType, dlib_pixel_type>& img) { return img.nc(); }

template <template <class> class HasPixelsClass_, typename PixelType, typename dlib_pixel_type>
inline void* image_data(of_pixels_<HasPixelsClass_, PixelType, dlib_pixel_type>& img)
{
    if (img.size() != 0)
        return &img[0][0];
    else
        return 0;
}

template <template <class> class HasPixelsClass_, typename PixelType, typename dlib_pixel_type>
inline const void* image_data(const of_pixels_<HasPixelsClass_, PixelType, dlib_pixel_type>& img)
{
    if (img.size() != 0)
        return &img[0][0];
    else
        return 0;
}

template <template <class> class HasPixelsClass_, typename PixelType, typename dlib_pixel_type>
inline long width_step(const of_pixels_<HasPixelsClass_, PixelType, dlib_pixel_type>& img)
{
    return img.width_step();
}

template <template <class> class HasPixelsClass_, typename PixelType, typename dlib_pixel_type>
inline void set_image_size(of_pixels_<HasPixelsClass_, PixelType, dlib_pixel_type>& img, long rows, long cols)
{
    img.set_image_size(rows, cols);
}




// ----------------------------------------------------------------------------------------

}
