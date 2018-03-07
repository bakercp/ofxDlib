//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <vector>
#include "dlib/geometry.h"
#include "dlib/image_processing/full_object_detection.h"
#include "dlib/dnn.h"
#include "ofImage.h"
#include "ofPixels.h"
#include "ofRectangle.h"
#include "ofTypes.h"
#include "dlib/of_image.h"
#include "dlib/to_of.h"


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


/// \brief Scale the given object detection by the given amount.
/// \param v The object to be scaled.
/// \param scale The amount the scale the object by.
inline void scale(dlib::rectangle& in, double scaler)
{
    in.left() *= scaler;
    in.top() *= scaler;
    in.right() *= scaler;
    in.bottom() *= scaler;
}

/// \brief Scale the given object detection by the given amount.
/// \param v The object to be scaled.
/// \param scale The amount the scale the object by.
inline void scale(dlib::full_object_detection& in, double scaler)
{
    for (unsigned int i = 0; i < in.num_parts(); ++i)
    {
        // Must be done individually.
        in.part(i).x() *= scaler;
        in.part(i).y() *= scaler;
    }

    scale(in.get_rect(), scaler);

}

/// \brief Scale the given object detection by the given amount.
/// \param v The object to be scaled.
/// \param scale The amount the scale the object by.
inline void scale(dlib::mmod_rect& in, double scaler)
{
    scale(in.rect, scaler);
}

/// \brief A helper function to convert an ofPixels object to grayscale.
///
/// This function is not particularly optimized.
///
/// \param pixels The pixels to convert.
/// \returns A grayscale version of the image.
template <typename PixelType>
inline ofPixels_<PixelType> toGrayscale(const ofPixels_<PixelType>& pixels)
{
    if (pixels.getPixelFormat() == OF_PIXELS_GRAY) return pixels;

    ofPixels_<PixelType> out;
    out.allocate(pixels.getWidth(), pixels.getHeight(), OF_PIXELS_GRAY);

    for (std::size_t x = 0; x < pixels.getWidth(); ++x)
    {
        for (std::size_t y = 0; y < pixels.getHeight(); ++y)
        {
            out.setColor(x, y, pixels.getColor(x, y).getBrightness());
        }
    }

    return out;
}


/// \brief A helper function to convert an ofPixels object to grayscale.
///
/// When loading images into dlib array2d, matrix, etc with non 8-bit values,
/// dlib does not rescale values. Thus to get ofFloatPixels and ofShortPixels
/// to work as expected in openFrameworks, we need to rescale the values loaded
/// by dlib into the range expected by openFrameworks.
///
/// This function is not optimized and certainly could be with simd.
///
/// \param pixels The pixels to map.
/// \param inMin The input minimum. Usually 0.
/// \param inMax The input maximum. Often 255 for 8-bit images.
/// \param outMin The output minimum. Usually 0.
/// \param outMax The output maximum. Usually equal to ofColor_<PixelType>::limit().
/// \returns A value remapped version of the input image.
/// \tparam PixelType The openFrameworks ofPixels internal pixel type.
template <typename PixelType>
inline ofPixels_<PixelType> map(const ofPixels_<PixelType>& pixels,
                                PixelType inMin, PixelType inMax,
                                PixelType outMin = 0, PixelType outMax = ofColor_<PixelType>::limit())
{
    ofPixels_<PixelType> _pixels = pixels;
    map(_pixels, inMin, inMax, outMin, outMax);
    return _pixels;
}


/// \brief A helper function to convert an ofPixels object to grayscale.
///
/// When loading images into dlib array2d, matrix, etc with non 8-bit values,
/// dlib does not rescale values. Thus to get ofFloatPixels and ofShortPixels
/// to work as expected in openFrameworks, we need to rescale the values loaded
/// by dlib into the range expected by openFrameworks.
///
/// This function is not optimized and certainly could be with simd.
///
/// \param pixels The pixels to map.
/// \param inMin The input minimum. Usually 0.
/// \param inMax The input maximum. Often 255 for 8-bit images.
/// \param outMin The output minimum. Usually 0.
/// \param outMax The output maximum. Usually equal to ofColor_<PixelType>::limit().
/// \returns A value remapped version of the input image.
/// \tparam PixelType The openFrameworks ofPixels internal pixel type.
template <typename PixelType>
inline void map(ofPixels_<PixelType>& pixels,
                PixelType inMin, PixelType inMax,
                PixelType outMin = 0, PixelType outMax = ofColor_<PixelType>::limit())
{
    for (std::size_t i = 0; i < pixels.size(); ++i)
    {
        pixels[i] = static_cast<PixelType>(ofMap(double(pixels[i]), inMin, inMax, outMin, outMax, true));
    }
}


/// \brief Wrap ofPixels_<> in an of_image to use with dlib functions.
///
/// This function makes a shallow wrapper for pixels. The returned value will
/// contain a pointer to the \p pixels. The passed pixels must remain valid
/// for the life the the returned value.
///
/// \tparam dlib_pixel_type The destination dlib pixel type.
/// \tparam of_pixel_type The source ofPixels pixel type (typically inferred).
/// \param pixels The pixels to wrap.
/// \returns an of_image object that wraps the passed pixels.
template <typename dlib_pixel_type, typename of_pixel_type>
inline dlib::of_image<dlib_pixel_type, of_pixel_type> toDlib(ofPixels_<of_pixel_type>& pixels)
{
    return dlib::of_image<dlib_pixel_type, of_pixel_type>(pixels);
}


/// \brief Wrap a dlib image_type as ofPixels_<>
///
/// This function makes a shallow wrapper for dlib pixels. The returned ofPixels
/// will point to the data in the passed dlib pixels. Thus, the passed dlib
/// pixels must remain valid for the life the the returned ofPixels.
///
/// \tparam image_type Any dlib compatible generic image_type.
/// \param pixels The pixels to wrap.
/// \returns an ofPixels_ object pointing to the passed dlib image.
template <typename image_type>
ofPixels_<typename dlib::pixel_traits<typename dlib::image_traits<image_type>::pixel_type>::basic_pixel_type> toOf(image_type& img)
{
    return dlib::to_of_pixels<image_type>(img);
}


/// \brief Wrap a dlib image_type as ofPixels_<>
///
/// This function makes a deep copy of dlib pixels.
///
/// \tparam image_type Any dlib compatible generic image_type.
/// \param pixels The pixels to wrap.
/// \returns an ofPixels_ object representing the passed dlib image.
template <typename image_type>
ofPixels_<typename dlib::pixel_traits<typename dlib::image_traits<image_type>::pixel_type>::basic_pixel_type> toOf(const image_type& img)
{
    return dlib::to_of_pixels<image_type>(img);
}
    
    
    
    
/// DNN

enum ImageMapType
{
    IMAGE_MAP_NONE,
    IMAGE_MAP_LAYER,
    IMAGE_MAP_SAMPLE
};
    
template <template<typename> class TAG_TYPE, std::size_t NR, std::size_t NC, typename NET>
inline std::vector<dlib::matrix<float, NR, NC>> layerOutputsToMatrices(const NET& net)
{
    std::vector<dlib::matrix<float, NR, NC>> matrices;

    // Get the layer tag + 1.
    auto& layer = dlib::layer<TAG_TYPE, 1>(net);
    auto& lo = layer.get_output();
    
    auto output_nr = lo.nr();
    auto output_nc = lo.nc();
    auto output_k = lo.k();
    auto output_ns = lo.num_samples();
    
    for (std::size_t k = 0; k < output_k; ++k)
    {
        std::size_t offset = output_ns * k * output_nr * output_nc;
        dlib::alias_tensor a(1, 1, output_nr, output_nc);
        dlib::matrix<float, NR, NC> m = dlib::mat(a(lo, offset));
        matrices.push_back(m);
    }
}

    
    
template <template<typename> class TAG_TYPE, std::size_t NR, std::size_t NC, typename NET>
inline std::vector<ofTexture> layerOutputsToTextures(const NET& net)
{
    //auto matrices = layerOutputsToMatrices<TAG_TYPE, NR, NC, NET>(net);

    std::vector<ofTexture> textures;
    
    // Get the layer tag + 1.
    auto& layer = dlib::layer<TAG_TYPE, 1>(net);
    auto& layer_details = layer.layer_details();
    auto& layer_output = layer.get_output();
    auto& layer_parameters = layer_details.get_layer_params();
    
    auto output_nr = layer_output.nr();
    auto output_nc = layer_output.nr();
    auto output_k = layer_output.k();
    auto output_ns = layer_output.num_samples();
    
    for (std::size_t k = 0; k < output_k; ++k)
    {
        std::size_t offset = 1 * k * output_nr * output_nc;
        dlib::alias_tensor a(1, 1, output_nr, output_nc);
        dlib::matrix<float, NR, NC> m = dlib::mat(a(layer_output, offset));
        
        auto mm = std::minmax_element(m.begin(), m.end());
        std::cout << "---- " << *mm.first << "," << *mm.second << std::endl;
        std::cout << std::endl;
        ofFloatPixels p = toOf(m);
        
        map(p, *mm.first, *mm.second);

        textures.push_back(ofTexture());
        textures.back().loadData(p);
        textures.back().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    }
    
    return textures;
}


    
    
    
    
    
    
    
    
    
    

} } // namespace ofx::Dlib
