//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/FaceShape.h"
#include <numeric>


namespace ofx {
namespace Dlib {


FaceShape::FaceShape()
{
}


FaceShape::FaceShape(const FaceShape& shape,
                     const std::vector<glm::vec2>& landmarks):
    FaceShape(shape.type(), landmarks, shape.alignedFace())
{
}


FaceShape::FaceShape(Type type,
                     const std::vector<glm::vec2>& landmarks,
                     const ofPixels& alignedFace):
    _type(type),
    _landmarks(landmarks),
    _alignedFace(alignedFace)
{
}


FaceShape::~FaceShape()
{
}


FaceShape::Type FaceShape::type() const
{
    return _type;
}

const std::vector<glm::vec2>& FaceShape::landmarks() const
{
    return _landmarks;
}


const ofPixels& FaceShape::alignedFace() const
{
    return _alignedFace;
}


float FaceShape::getMeasurement(Measurement measurement) const
{
    switch (_type)
    {
//        case Type::FACE_SHAPE_5_LANDMARKS:
//        {
//            // TODO
//            break;
//        }
        case Type::FACE_SHAPE_68_LANDMARKS:
        {
            switch (measurement)
            {
                case OUTER_MOUTH_WIDTH:
                    return glm::length(_landmarks[48] - _landmarks[54]);
                case INNER_MOUTH_WIDTH:
                    return glm::length(_landmarks[60] - _landmarks[64]);
                case OUTER_MOUTH_HEIGHT:
                    return glm::length(_landmarks[51] - _landmarks[57]);
                case INNER_MOUTH_HEIGHT:
                    return glm::length(_landmarks[62] - _landmarks[66]);
                case LEFT_EYEBROW_HEIGHT:
                    return glm::length(_landmarks[38] - _landmarks[20]);
                case RIGHT_EYEBROW_HEIGHT:
                    return glm::length(_landmarks[43] - _landmarks[24]);
                case JAW_OPENNESS:
                    return glm::length(_landmarks[8] - _landmarks[33]);
                case MOUTH_ASPECT:
                    return getMeasurement(MOUTH_WIDTH) / getMeasurement(MOUTH_HEIGHT);
                case YAWN_FACTOR:
                    return getMeasurement(JAW_OPENNESS) / getMeasurement(MOUTH_ASPECT);
            }
        }
    }

    return -1.0f;
}


std::vector<std::size_t> FaceShape::getFeatureIndices(Feature feature) const
{
    switch (_type)
    {
//        case Type::FACE_SHAPE_5_LANDMARKS:
//        {
//            // TODO
//            break;
//        }
        case Type::FACE_SHAPE_68_LANDMARKS:
        {
            switch (feature)
            {
                case LEFT_EYE_TOP:
                    return consecutive(36, 40);
                case RIGHT_EYE_TOP:
                    return consecutive(42, 46);
                case LEFT_JAW:
                    return consecutive(0, 9);
                case RIGHT_JAW:
                    return consecutive(8, 17);
                case JAW:
                    return consecutive(0, 17);
                case LEFT_EYEBROW:
                    return consecutive(17, 22);
                case RIGHT_EYEBROW:
                    return consecutive(22, 27);
                case LEFT_EYE :
                    return consecutive(36, 42);
                case RIGHT_EYE :
                    return consecutive(42, 48);
                case OUTER_MOUTH:
                    return consecutive(48, 60);
                case INNER_MOUTH:
                    return consecutive(60, 68);
                case NOSE_BRIDGE:
                    return consecutive(27, 31);
                case NOSE_BASE:
                    return consecutive(31, 36);
                case FACE_OUTLINE:
                {
                    static std::size_t faceOutline[] = {17,18,19,20,21,22,23,24,25,26, 16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
                    return std::vector<std::size_t>(faceOutline, faceOutline + 27);
                }

                //case ALL_FEATURES: return consecutive(0, 68);
            }
        }
    }

    ofLogWarning("FaceShape::getFeatureIndices") << "Unknown face shape for face type.";
    return std::vector<std::size_t>();
}


ofPolyline FaceShape::getFeatureAsPolyline(Feature feature) const
{
    ofPolyline polyline;

    auto featureIndices = getFeatureIndices(feature);

    for (std::size_t i = 0; i < featureIndices.size(); ++i)
    {
        auto point = _landmarks[featureIndices[i]];
        polyline.addVertex(point.x, point.y, 0);
    }

    switch(feature)
    {
        case LEFT_EYE:
        case RIGHT_EYE:
        case OUTER_MOUTH:
        case INNER_MOUTH:
        case FACE_OUTLINE:
            polyline.close();
            break;
        default:
            break;
    }

    return polyline;
}


std::map<FaceShape::Feature, ofPolyline> FaceShape::getFeatureSetAsPolylines(FeatureSet featureSet) const
{
    std::map<Feature, ofPolyline> results;

    switch (featureSet)
    {
        case EYES:
        {
            results[Feature::LEFT_EYEBROW] = getFeatureAsPolyline(Feature::LEFT_EYEBROW);
            results[Feature::RIGHT_EYEBROW] = getFeatureAsPolyline(Feature::RIGHT_EYEBROW);
            results[Feature::LEFT_EYE] = getFeatureAsPolyline(Feature::LEFT_EYE);
            results[Feature::RIGHT_EYE] = getFeatureAsPolyline(Feature::RIGHT_EYE);
            break;
        }
        case MOUTH:
        {
            results[Feature::OUTER_MOUTH] = getFeatureAsPolyline(Feature::OUTER_MOUTH);
            results[Feature::INNER_MOUTH] = getFeatureAsPolyline(Feature::INNER_MOUTH);
            break;
        }
        case NOSE:
        {
            results[Feature::NOSE_BRIDGE] = getFeatureAsPolyline(Feature::NOSE_BRIDGE);
            results[Feature::NOSE_BASE] = getFeatureAsPolyline(Feature::NOSE_BASE);
        }
    }

    return results;
}


std::vector<glm::vec2> FaceShape::getFeature(Feature feature) const
{
    std::vector<glm::vec2> polyline;

    auto featureIndices = getFeatureIndices(feature);

    for (std::size_t i = 0; i < featureIndices.size(); ++i)
    {
        polyline.push_back(_landmarks[featureIndices[i]]);
    }

    return polyline;
}


std::vector<std::size_t> FaceShape::consecutive(std::size_t startIndex,
                                                std::size_t endIndex)
{
    if (endIndex < startIndex)
        std::swap(startIndex, endIndex);

    std::vector<std::size_t> result(endIndex - startIndex, 0);
    std::iota(result.begin(), result.end(), startIndex);
    return result;
}


} } // namespace ofx::Dlib
