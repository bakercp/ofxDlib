//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/ObjectDetection.h"
#include "ofx/Dlib/Utils.h"


namespace ofx {
namespace Dlib {


ObjectDetection::ObjectDetection()
{
}


ObjectDetection::ObjectDetection(const dlib::mmod_rect& detection):
    ObjectDetection(detection.detection_confidence, detection.rect)
{
}


ObjectDetection::ObjectDetection(const std::pair<double,
                                 dlib::rectangle>& det):
    ObjectDetection(det.first, det.second)
{
}


ObjectDetection::ObjectDetection(double _confidence,
                                 const dlib::rectangle& _rectangle):
    ObjectDetection(_confidence, toOf(_rectangle))
{
}

ObjectDetection::ObjectDetection(double _confidence,
                                 const ofRectangle& _rectangle):
    confidence(_confidence),
    rectangle(_rectangle)
{
}


} } // namespace ofx::Dlib
