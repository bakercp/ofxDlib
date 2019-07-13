//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "dlib/image_processing/frontal_face_detector.h"
#include "ofx/Dlib/Tracker.h"
#include "ofRectangle.h"


namespace ofx {
namespace Dlib {


/// \brief An object detection result with confidence in pixel cooridinates.
class ObjectDetection
{
public:
    ObjectDetection();
    ObjectDetection(const dlib::mmod_rect& detection);
    ObjectDetection(const std::pair<double, dlib::rectangle>& detection);
    ObjectDetection(double confidence, const dlib::rectangle& rectangle);
    ObjectDetection(double confidence, const ofRectangle& rectangle);

    /// \brief The detection confidence.
    ///
    ///       0.0 => no confidence,
    ///     > 1.0 => very confident
    ///
    /// If confidence is unsupported, a confidence of 1 will returned.
    double confidence = 0.0;

    /// \brief The detection rectangle in pixel coordinates.
    ofRectangle rectangle;

};


/// \brief A distance function used by trackers, etc.
inline float distance(const ObjectDetection& _a, const ObjectDetection& _b)
{
    return distance(_a.rectangle, _b.rectangle);
}


} } // namespace ofx::Dlib
