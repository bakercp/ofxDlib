//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofTypes.h"
#include <iostream>
#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include <dlib/image_processing.h>

#if !defined(DLIB_NO_GUI_SUPPORT)
#include <dlib/gui_widgets.h>
#endif

#include "ofx/dlib/Utils.h"


template<typename T>
inline const glm::tvec4<T>& toOf(const dlib::vector<T, 4>& v)
{
    return *reinterpret_cast<const glm::tvec4<T>*>(&v);
}

template<typename T>
inline const glm::tvec3<T>& toOf(const dlib::vector<T, 3>& v)
{
    return *reinterpret_cast<const glm::tvec3<T>*>(&v);
}

template<typename T>
inline const glm::tvec2<T>& toOf(const dlib::vector<T, 2>& v)
{
    return *reinterpret_cast<const glm::tvec2<T>*>(&v);
}

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


inline ofColor toOf(const dlib::rgb_pixel& v)
{
    return ofColor(v.red, v.green, v.blue);
}


inline ofColor toOf(const dlib::rgb_alpha_pixel& v)
{
    return ofColor(v.red, v.green, v.blue, v.alpha);
}
