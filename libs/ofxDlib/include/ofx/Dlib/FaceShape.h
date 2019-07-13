//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "dlib/image_transforms/interpolation.h"
#include "ofPixels.h"
#include "ofPolyline.h"


namespace ofx {
namespace Dlib {


class FaceShape
{
public:
    enum class Type
    {
        /// \brief The fast 5 landmark face shape predictor.
        FACE_SHAPE_5_LANDMARKS,

        /// \brief The precise 68 landmark face shape predictor.
        FACE_SHAPE_68_LANDMARKS
    };

    enum Feature
    {
        LEFT_EYE_TOP = 0,
        RIGHT_EYE_TOP,

        LEFT_EYEBROW,
        RIGHT_EYEBROW,

        LEFT_EYE,
        RIGHT_EYE,

        LEFT_JAW,
        RIGHT_JAW,

        JAW,

        OUTER_MOUTH,
        INNER_MOUTH,

        NOSE_BRIDGE,
        NOSE_BASE,

        FACE_OUTLINE,
        
        ALL_FEATURES,
    };

    FaceShape();

    FaceShape(Type type,
              const std::vector<glm::vec2>& faceLandmarks,
              const ofPixels& alignedFace);

    FaceShape(const FaceShape& shape,
              const std::vector<glm::vec2>& faceLandmarks);

    ~FaceShape();

    /// \returns the face shape type.
    FaceShape::Type type() const;

    const std::vector<glm::vec2>& faceLandmarks() const;

    /// \returns the aligned face image if available, otherwise empty pixels.
    const ofPixels& alignedFace() const;

    ofPolyline getFeatureAsPolyline(Feature feature) const;

    std::vector<glm::vec2> getFeature(Feature feature) const;

    std::vector<std::size_t> getFeatureIndices(Feature feature) const;

    static std::vector<std::size_t> consecutive(std::size_t startIndex,
                                                std::size_t endIndex);

private:
    FaceShape::Type _type = Type::FACE_SHAPE_68_LANDMARKS;

    std::vector<glm::vec2> _faceLandmarks;

    ofPixels _alignedFace;
};



} } // namespace ofx::Dlib
