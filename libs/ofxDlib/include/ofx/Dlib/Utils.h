//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
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
#include "ofVectorMath.h"
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


/// \brief Copy a ofRectangle to a dlib::rectangle.
/// \param r the ofRectangle.
/// \returns a dlib::rectangle.
inline dlib::rectangle toDlib(const ofRectangle& r)
{
    return dlib::rectangle(r.getLeft(),
                           r.getTop(),
                           r.getRight(),
                           r.getBottom());
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
    if (pixels.getPixelFormat() == OF_PIXELS_GRAY)
        return pixels;

    ofPixels_<PixelType> out;
    out.allocate(pixels.getWidth(), pixels.getHeight(), OF_PIXELS_GRAY);

    for (std::size_t x = 0; x < pixels.getWidth(); ++x)
        for (std::size_t y = 0; y < pixels.getHeight(); ++y)
            out.setColor(x, y, pixels.getColor(x, y).getBrightness());

    return out;
}


/// \brief Rotate images by 90 degree increments.
///
/// This function is similar to ofPixels_::rotate90To(...).
///
/// In tests this function runs faster by as much as 3x when nClockwiseRotations
/// is equal to 2 or 3.
///
/// The ouput image will be resized / reallocated as needed. Thus, to avoid
/// reallocations, provide an image that is already correctly sized for the
/// number of rotations desired.
///
/// \tparam input_image_type Any dlib compatible generic image_type.
/// \tparam output_image_type Any dlib compatible generic image_type.
/// \param input The input image.
/// \param output The output image. The output image will be resized / reallocated as needed.
template <typename input_image_type, typename output_image_type>
void rotate_image_90(const input_image_type& input,
                     output_image_type& output,
                     std::size_t nClockwiseRotations)
{
    switch (nClockwiseRotations % 4)
    {
        // Rotate 90deg CW
        case 1:
            dlib::assign_image(output, dlib::trans(dlib::flipud(dlib::mat(input))));
            return;
        // Rotate 180deg CW
        case 2:
            dlib::assign_image(output, dlib::flip(dlib::mat(input)));
            return;
        // Rotate 270deg CW
        case 3:
            dlib::assign_image(output, dlib::flipud(dlib::trans(dlib::mat(input))));
            return;
    }

    // Just copy.
    output = input;
    return;
}

/// \brief Resize an image using dlib.
///
/// This function is similar to ofPixels_::resizeTo(...).
///
/// In tests this function runs faster than ofPixels_::resizeTo(...).
///
/// OF_INTERPOLATE_BICUBIC is not supported by dlib. dlib also offers
/// dlib::interpolate_quadratic. To use this method, simply call,
///
///     dlib::resize_image(input, output, dlib::interpolate_quadratic());
///
/// The ouput image will be resized / reallocated as needed. Thus, to avoid
/// reallocations, provide an image that is already correctly sized for the
/// number of rotations desired.
///
/// \tparam input_image_type Any dlib compatible generic image_type.
/// \tparam output_image_type Any dlib compatible generic image_type.
/// \param input The input image.
/// \param output The output image. The size of the output image size is the target size.
template <typename input_image_type, typename output_image_type>
void resize_image(const input_image_type& input,
                  output_image_type& output,
                  ofInterpolationMethod interp = OF_INTERPOLATE_NEAREST_NEIGHBOR)
{
    switch (interp)
    {
        case OF_INTERPOLATE_NEAREST_NEIGHBOR:
            dlib::resize_image(input,
                               output,
                               dlib::interpolate_nearest_neighbor());
            return;
        case OF_INTERPOLATE_BILINEAR:
            dlib::resize_image(input,
                               output,
                               dlib::interpolate_bilinear());
            return;
        case OF_INTERPOLATE_BICUBIC:
            ofLogWarning("resize_image") << "OF_INTERPOLATE_BICUBIC not implemented. For bicubic interpolation, use ofPixels_::resizeTo(..., OF_INTERPOLATE_BICUBIC).";
            return;
    }
}


/// \brief A helper function to convert an ofPixels object to grayscale.
///
/// When loading images into dlib array2d, matrix, etc with non 8-bit values,
/// dlib does not rescale values. Thus to get ofFloatPixels and ofShortPixels
/// to work as expected in openFrameworks, we need to rescale the values loaded
/// by dlib into the range expected by openFrameworks.
///
/// This function is not optimized and certainly could be with SIMD.
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



/// \brief Copy ofPixels to a row matrix.
template <typename of_pixel_type>
inline dlib::matrix<of_pixel_type> to_rowm(const ofPixels_<of_pixel_type>& pixels)
{
    return dlib::mat(pixels.getData(), 1, pixels.size());
}


/// \brief Copy ofPixels to a column matrix.
template <typename of_pixel_type>
inline dlib::matrix<of_pixel_type> to_colm(const ofPixels_<of_pixel_type>& pixels)
{
    return dlib::mat(pixels.getData(), pixels.size(), 1);
}


/// \brief Copy a std::vector of ofPixels to the rows in a dlib::matrix.
template <typename of_pixel_type>
inline dlib::matrix<of_pixel_type> to_rows(const std::vector<ofPixels_<of_pixel_type>>& pixels)
{
    dlib::matrix<of_pixel_type> results;

    if (pixels.size() > 0)
    {
        results.set_size(pixels.size(), pixels[0].size());
        for (std::size_t row = 0; row < pixels.size(); ++row)
            dlib::set_rowm(results, row) = to_rowm(pixels[row]);
    }
    else ofLogWarning("to_rows") << "Empty vector, skipping.";

    return results;
}

/// \brief Copy a std::vector of ofPixels to the columns in a dlib::matrix.
template <typename of_pixel_type>
inline dlib::matrix<of_pixel_type> to_cols(const std::vector<ofPixels_<of_pixel_type>>& pixels)
{
    dlib::matrix<of_pixel_type> results;

    if (pixels.size() > 0)
    {
        results.set_size(pixels[0].size(), pixels.size());
        for (std::size_t col = 0; col < pixels.size(); ++col)
            dlib::set_colm(results, col) = to_colm(pixels[col]);
    }
    else ofLogWarning("to_cols") << "Empty vector, skipping.";

    return results;
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

//enum ImageMapType
//{
//    IMAGE_MAP_NONE,
//    IMAGE_MAP_LAYER,
//    IMAGE_MAP_SAMPLE
//};

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
//        std::cout << "---- " << *mm.first << "," << *mm.second << std::endl;
//        std::cout << std::endl;
        ofFloatPixels p = toOf(m);

        map(p, *mm.first, *mm.second);

        textures.push_back(ofTexture());
        textures.back().loadData(p);
        textures.back().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    }

    return textures;
}



template <typename PixelType>
std::vector<ofPixels_<PixelType>> loadImages(const std::string& path,
                                             const std::vector<std::string>& extensions = { },
                                             bool requireSameDimensions = true)

{
    std::vector<ofPixels_<PixelType>> images;

    std::size_t imageWidth = 0;
    std::size_t imageHeight = 0;
    std::size_t imageNumChannels = 0;

    ofDirectory dir(path);
    for (auto&& ext: extensions) dir.allowExt(ext);

    for (auto&& file: dir)
    {
        ofPixels_<PixelType> pix;

        if (ofLoadImage(pix, file))
        {
            if (requireSameDimensions)
            {
                if (imageWidth == 0 && imageHeight == 0 && imageNumChannels == 0)
                {
                    imageWidth = pix.getWidth();
                    imageHeight = pix.getHeight();
                    imageNumChannels = pix.getNumChannels();
                }
                else if (imageWidth != pix.getWidth()
                     || imageHeight != pix.getHeight()
                     || imageNumChannels != pix.getNumChannels())
                {
                    ofLogError("loadImages") << "The first image was: " << imageWidth << " x " << imageHeight << " x " << imageNumChannels << " but this image was " << pix.getWidth() << " x " << pix.getHeight() << " x " << pix.getNumChannels() << ": " << file.getAbsolutePath();
                    return images;
                }
            }

            images.emplace_back(std::move(pix));
        }
        else
        {
            ofLogError("loadImages") << "Unable to load image: " << file.getAbsolutePath() << ", skipping.";
        }
    }

    return images;

}


//  http://dlib.net/dlib/matrix/matrix.h.html
template<typename T>
std::string prettyPrint(const dlib::matrix_exp<T>& exp, const std::string& name = "")
{
    std::string result;

    std::size_t len = ofUTF8Length(name);
    static const std::string _eq = " = ";
    static const std::string left_bracket = "⎡⎢⎣";
    static const std::string right_bracket = "⎤⎥⎦";

    std::size_t xOffset = len > 0 ? (len + _eq.length()) : 0;

    for (std::size_t r = 0; r < exp.nr(); ++r)
    {
//        if (r == 0) {   }
//        else if (r == mat.nr() - 1) {   }
//        else {   }
//
//        for (std::size_t c = 0; c < mat.nc(); ++c)
//        {
////            result += ofToString(
//        }
//
//        if (r == 0) {   }
//        else if (r == mat.nr() - 1) {   }
//        else {   }
    }

    return result;
}



template<typename PixelType>
ofTexture toTexture(const dlib::matrix<PixelType>& _mat,
                    std::size_t width,
                    std::size_t height,
                    std::size_t numChannels,
                    bool normalize = false)
{
    dlib::matrix<PixelType> mat;

    if (normalize)
    {
        double min = dlib::min(_mat);
        double max = dlib::max(_mat);

        mat = dlib::clamp((_mat - min) / (max - min), 0, 1);
    }
    else
    {
        mat = _mat;
    }

    ofPixels_<PixelType> pix;
    pix.setFromExternalPixels(mat.begin(), width, height, numChannels);

    ofTexture tex;
    tex.loadData(pix);
    return tex;
}



template<typename PixelType>
ofTexture toTexture(const dlib::matrix<PixelType>& _mat,
                    std::size_t width,
                    std::size_t height,
                    ofPixelFormat pixelFormat,
                    bool normalize = false)
{
    dlib::matrix<PixelType> mat;

    if (normalize)
    {
        double min = dlib::min(_mat);
        double max = dlib::max(_mat);

        mat = dlib::clamp((_mat - min) / (max - min), 0, 1);
    }
    else
    {
        mat = _mat;
    }

    ofPixels_<PixelType> pix;
    pix.setFromExternalPixels(mat.begin(), width, height, pixelFormat);

    ofTexture tex;
    tex.loadData(pix);
    return tex;
}



//inline glm::vec2 position(const ofRectangle& item)
//{
//    return item.getCenter();
//}
//
//inline ofRectangle lerp(const ofRectangle& a,
//                 const ofRectangle& b,
//                 float smoothingRate)
//{
//    ofRectangle result;
//    result.x = ofLerp(a.x, b.x, smoothingRate);
//    result.y = ofLerp(a.y, b.y, smoothingRate);
//    result.width = ofLerp(a.width, b.width, smoothingRate);
//    result.height = ofLerp(a.height, b.height, smoothingRate);
//    return result;
//}

inline float distance(const ofRectangle& a, const ofRectangle& b)
{
    // Calculate the spatial "distance" between the objects.
    glm::vec2 dp = glm::vec2(a.getCenter()) - glm::vec2(b.getCenter());
    glm::vec2 ds = { a.width - b.width, a.height - b.height };

    // Distance between the centers.
    float centerDistance = glm::length(dp);

    // Difference between the sizes.
    float sizeDistance = glm::length(ds);

    // The combined distance.
    return centerDistance + sizeDistance;
}



/// \brief Create a collection of slight image variations.
///
/// This calls `dlib::jitter_image` a total of `n` times and returns the
/// collection of jittered imaged. Jittered images are slightly rotated,
/// zoomed, translated and mirrored randomly.
///
/// \param img The image to jitter.
/// \param n The number of jittered image variations to return.
/// \returns a collection of `n` jittered images.
/// \tparam image_type The image type to itter. This can be any compatible
/// openFrameworks or dlib image type.
template<typename image_type>
std::vector<image_type> jitterImage(const image_type& img, std::size_t n)
{
    // All this function does is make `n` copies of img, all slightly
    // jittered by being zoomed, rotated, and translated a little bit
    // differently. They are also randomly mirrored left to right.
    thread_local dlib::rand rnd;

    std::vector<image_type> crops;

    for (std::size_t i = 0; i < n; ++i)
        crops.push_back(dlib::jitter_image(img, rnd));

    return crops;
}

//
inline std::vector<glm::vec2> createGuassianCluster(const glm::vec2& mean,
                                                    float stdDev,
                                                    std::size_t n)
{
    std::vector<glm::vec2> results(n);
    for (std::size_t i = 0; i < n; ++i)
        results[i] = glm::gaussRand(mean, { stdDev, stdDev });
    return results;
}






} } // namespace ofx::Dlib
