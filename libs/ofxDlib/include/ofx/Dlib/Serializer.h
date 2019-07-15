//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#if defined(OF_SERIALIZER_H)


#include "ofSerializer.h"
#include "json.hpp"
#include "ofx/Dlib/FaceDetector.h"
#include "ofx/Dlib/FaceTracker.h"


namespace ofx {
namespace Dlib {


NLOHMANN_JSON_SERIALIZE_ENUM( FaceDetector::Type, {
    { FaceDetector::Type::FACE_DETECTOR_HOG, "FACE_DETECTOR_HOG" },
    { FaceDetector::Type::FACE_DETECTOR_MMOD, "FACE_DETECTOR_MMOD"}
})


NLOHMANN_JSON_SERIALIZE_ENUM( FaceShape::Type, {
    { FaceShape::Type::FACE_SHAPE_5_LANDMARKS, "FACE_SHAPE_5_LANDMARKS" },
    { FaceShape::Type::FACE_SHAPE_68_LANDMARKS, "FACE_SHAPE_68_LANDMARKS"}
})


inline void to_json(nlohmann::json& j, const FaceDetector::Settings& v)
{
    j["detector_type"] = v.detectorType;
    j["input_scale"] = v.inputScale;
    j["input_scale_interpolation"] = v.inputScaleInterpolation;
    j["input_roi"] = v.inputROI;
    j["minimum_detection_confidence"] = v.minimumDetectionConfidence;
    j["minimum_detection_size"] = v.minimumDetectionSize;
    j["models_path"] = v.modelsPath;
    j["gpu_device"] = v.gpuDevice;
}


inline void from_json(const nlohmann::json& j, FaceDetector::Settings& v)
{
    v.detectorType = j.value("detector_type", v.detectorType);
    v.inputScale = j.value("input_scale", v.inputScale);
    v.inputScaleInterpolation = j.value("input_scale_interpolation", v.inputScaleInterpolation);
    v.inputROI = j.value("input_roi", v.inputROI);
    v.minimumDetectionConfidence = j.value("minimum_detection_confidence", v.minimumDetectionConfidence);
    v.minimumDetectionSize = j.value("minimum_detection_size", v.minimumDetectionSize);
    v.modelsPath = j.value("models_path", v.modelsPath);
    v.gpuDevice = j.value("gpu_device", v.gpuDevice);

//    j.at("name").get_to(p.name);
//    j.at("address").get_to(p.address);
//    j.at("age").get_to(p.age);

}


template <class T>
inline void to_json(nlohmann::json& j, const typename Tracker_<T>::Settings& v)
{
    j["persistence"] = v.persistence;
    j["maximum_distance"] = v.maximumDistance;
}



template <class T>
inline void from_json(const nlohmann::json& j, typename Tracker_<T>::Settings& v)
{
    v.persistence = j.value("persistence", v.persistence);
    v.maximumDistance = j.value("maximum_distance", v.maximumDistance);
}



inline void to_json(nlohmann::json& j, const FaceTracker::Settings& v)
{
//    j["async"] = v.async;

//    to_json<ObjectDetection>(j["tracker_settings"], v.trackerSettings);

////    j["tracker_settings"] = v.trackerSettings;
//    j["face_detector_settings"] = v.faceDetectorSettings;
//    //j["face_shape_predictor_settings"] = v.faceShapePredictorSettings;
//    j["face_detector_filter_smoothness"] = v.faceDetectorFilterSmoothness;
//    j["face_shape_filter_smoothness"] = v.faceShapeFilterSmoothness;
}


inline void from_json(const nlohmann::json& j, FaceTracker::Settings& v)
{
//    v.async = j.value("async", v.async);
//    //v.trackerSettings = j.value("tracker_settings", v.trackerSettings);
//    v.faceDetectorSettings = j.value("face_detector_settings", v.faceDetectorSettings);
//    //v.faceShapePredictorSettings = j.value("face_shape_predictor_settings", v.faceShapePredictorSettings);
//    v.faceDetectorFilterSmoothness = j.value("face_detector_filter_smoothness", v.faceDetectorFilterSmoothness);
//    v.faceShapeFilterSmoothness = j.value("face_shape_filter_smoothness", v.faceShapeFilterSmoothness);
}



} } // namespace ofx::Dlib


#endif
