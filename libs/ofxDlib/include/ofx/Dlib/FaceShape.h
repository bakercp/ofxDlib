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
        // FACE_SHAPE_5_LANDMARKS,

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

        FACE_OUTLINE
    };

    enum FeatureSet
    {
        EYES,
        EYEBROWS,
        MOUTH,
        NOSE
    };

    enum Measurement
    {
        /// \brief The distance between points 48 and 54.
        OUTER_MOUTH_WIDTH,

        MOUTH_WIDTH = OUTER_MOUTH_WIDTH,

        /// \brief The distance between points 60 and 64.
        INNER_MOUTH_WIDTH,

        /// \brief The distance between points 51 and 57.
        OUTER_MOUTH_HEIGHT,

        /// \brief The distance between points 62 and 66.
        INNER_MOUTH_HEIGHT,

        MOUTH_HEIGHT = INNER_MOUTH_HEIGHT,

        /// \brief The distance between points 38 and 20.
        LEFT_EYEBROW_HEIGHT,

        /// \brief The distance between points 43 and 24.
        RIGHT_EYEBROW_HEIGHT,

        /// \brief The distance between points 8 and 33.
        JAW_OPENNESS,

        /// \brief MOUTH_WIDTH / MOUTH_HEIGHT
        MOUTH_ASPECT,

        /// \brief JAW_OPENNESS / MOUTH_ASPECT.
        YAWN_FACTOR
    };


    FaceShape();

    FaceShape(Type type,
              const std::vector<glm::vec2>& landmarks,
              const ofPixels& alignedFace);

    FaceShape(const FaceShape& shape,
              const std::vector<glm::vec2>& landmarks);

    ~FaceShape();

    /// \returns the face shape type.
    FaceShape::Type type() const;

    /// \returns all landmarks.
    const std::vector<glm::vec2>& landmarks() const;

    /// \returns the aligned face image if available, otherwise empty pixels.
    const ofPixels& alignedFace() const;

    /// \brief Take a measurement.
    /// \param measurement The measurement to take.
    /// \returns the measured value or -1 if not available.
    float getMeasurement(Measurement measurement) const;

    /// \brief Get the landmarks for the given feature as a polyline.
    /// \param feature The feature to query.
    /// \returns the feature polyline or an empty polyline if not available.
    ofPolyline getFeatureAsPolyline(Feature feature) const;

    /// \brief Get the landmarks for the given set of features as polylines.
    /// \param featureSet The feature set to query.
    /// \returns the mapped feature set or an empty map if none available.
    std::map<Feature, ofPolyline> getFeatureSetAsPolylines(FeatureSet featureSet) const;

    /// \brief Get the landmarks for the given feature.
    /// \param feature The feature to query.
    /// \returns the feature indicies or an empty vector if not available.
    std::vector<glm::vec2> getFeature(Feature feature) const;

    /// \brief Get the landmark indices for the given feature.
    /// \param feature The feature to query.
    /// \returns the feature indicies or an empty vector if not available.
    std::vector<std::size_t> getFeatureIndices(Feature feature) const;

    /// \brief Get consecutive indicies.
    /// \param startIndex The starting index.
    /// \param endIndex The ending index.
    /// \returns a vector of consecutive indicies.
    static std::vector<std::size_t> consecutive(std::size_t startIndex,
                                                std::size_t endIndex);

private:
    /// \brief The type of landmarks.
    FaceShape::Type _type = Type::FACE_SHAPE_68_LANDMARKS;

    /// \brief The landmarks.
    std::vector<glm::vec2> _landmarks;

    /// \brief The aligned face pixels.
    ofPixels _alignedFace;

};



} } // namespace ofx::Dlib
