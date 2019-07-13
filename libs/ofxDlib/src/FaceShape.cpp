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
                     const std::vector<glm::vec2>& faceLandmarks):
    FaceShape(shape.type(), faceLandmarks, shape.alignedFace())
{
}


FaceShape::FaceShape(Type type,
                     const std::vector<glm::vec2>& faceLandmarks,
                     const ofPixels& alignedFace):
    _type(type),
    _faceLandmarks(faceLandmarks),
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

const std::vector<glm::vec2>& FaceShape::faceLandmarks() const
{
    return _faceLandmarks;
}


const ofPixels& FaceShape::alignedFace() const
{
    return _alignedFace;
}


std::vector<std::size_t> FaceShape::getFeatureIndices(Feature feature) const
{
    switch (_type)
    {
        case Type::FACE_SHAPE_5_LANDMARKS:
        {

        }
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

                case ALL_FEATURES: return consecutive(0, 68);
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
        auto point = _faceLandmarks[featureIndices[i]];
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


std::vector<glm::vec2> FaceShape::getFeature(Feature feature) const
{
    std::vector<glm::vec2> polyline;

    auto featureIndices = getFeatureIndices(feature);

    for (std::size_t i = 0; i < featureIndices.size(); ++i)
    {
        polyline.push_back(_faceLandmarks[featureIndices[i]]);
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
