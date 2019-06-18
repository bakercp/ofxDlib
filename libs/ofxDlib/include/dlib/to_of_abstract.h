#undef DLIB_TO_OF_AbSTRACT_H_
#ifdef DLIB_TO_OF_AbSTRACT_H_


#include "ofPixels.h"
#include <dlib/pixel.h>


/// \sa http://dlib.net/dlib/opencv/to_open_cv_abstract.h.html
namespace dlib
{


template <typename image_type>
ofPixels_<typename pixel_traits<typename image_traits<image_type>::pixel_type>::basic_pixel_type> to_of_pixels(image_type& img);
template <typename image_type>
ofPixels_<typename pixel_traits<typename image_traits<image_type>::pixel_type>::basic_pixel_type> to_of_pixels(const image_type& img);


} // namespace dlib


#endif // DLIB_TO_OF_AbSTRACT_H_

