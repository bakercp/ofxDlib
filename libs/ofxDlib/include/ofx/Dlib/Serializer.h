//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#if defined(OF_SERIALIZER_H)


#include "json.hpp"
#include "ofx/Dlib/FaceDetector.h"


namespace ofx {
namespace Dlib {



inline std::string to_string(const FaceDetector::Type& v)
{
    switch (v)
    {
        case FaceDetector::Type::FACE_DETECTOR_HOG: return "FACE_DETECTOR_HOG";
        case FaceDetector::Type::FACE_DETECTOR_MMOD: return "FACE_DETECTOR_MMOD";
    }

    return "FACE_DETECTOR_HOG";
}

inline void to_json(nlohmann::json& j, const FaceDetector::Type& v)
{
    j = to_string(v);
}


inline void from_json(const nlohmann::json& j, FaceDetector::Type& v)
{
    std::string s = j.get<std::string>();
    if (!s.empty())
    {
        if (s == to_string(FaceDetector::Type::FACE_DETECTOR_HOG))
        {
            v = FACE_DETECTOR_HOG;
            return;
        }
        else if (s == to_string(FaceDetector::Type::FACE_DETECTOR_MMOD))
        {
            v = FaceDetector::Type::FACE_DETECTOR_MMOD;
            return;
        }
    }

    ofLogWarning("from_json") << "Unknown value: " << s;
    v = FaceDetector::Type::FACE_DETECTOR_HOG;
    return;
}



inline void to_json(nlohmann::json& j, const FaceDetector::Settings& v)
{
    j["detector_type"] = v.detectorType;
    j["input_scale"] = v.inputScale;
    j["input_scale_interpolation"] = v.inputScaleInterpolation;
    j["input_roi"] = v.inputROI;
    j["minimum_detection_confidence"] = v.minimumDetectionConfidence;
    j["minimum_detection_size"] = v.minimumDetectionSize;
    j["models_path"] = v.modelsPath;
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
}


} } // namespace ofx::Dlib


#endif
