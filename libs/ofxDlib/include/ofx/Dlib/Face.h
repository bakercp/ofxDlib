//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <vector>
#include "dlib/image_processing/frontal_face_detector.h"
//#include "ofx/Dlib/ObjectDetection.h"
//#include "ofx/Dlib/Network/MMODFaceDetector.h"
//#include "ofx/Dlib/Network/FaceRecognitionResnetModelV1.h"
#include "ofPath.h"
#include "ofPixels.h"
#include "ofRectangle.h"
#include "ofTexture.h"


namespace ofx {
namespace Dlib {


/// \brief A structure representing a Face.
class Face
{
public:
    Face();

    Face(const std::pair<double, dlib::rectangle>& face,
         const dlib::full_object_detection& shape,
         const ofPixels& faceChip,
         const std::vector<float>& faceCode);

    /// \returns true if a Face has been loaded.
    bool isLoaded() const;

    /// \returns The confidence of the face detection.
    double confidence() const;

    /// \returns The location of the face detection.
    ofRectangle rectangle() const;

    /// \returns the location of the face shape landmarks in image space.
    std::vector<glm::vec2> landmarks() const;

    /// \returns the face shape.
    dlib::full_object_detection shape() const;

    /// \returns the face chip.
    const ofPixels& faceChip() const;

    /// \returns the face code.
    const std::vector<float>& faceCode() const;

    /// \brief Draw the detection.
    //void draw() const;

private:
    std::string _key;

    /// \brief True if the settings were loaded correctly.
    bool _isLoaded = false;

    /// \brief The confidence of the detection.
    double _confidence = 0;

    /// \brief The location of the face.
    ofRectangle _rectangle;

    /// \brief The face shape.
    dlib::full_object_detection _shape;

    /// \brief The face chip.
    ofPixels _faceChip;

    /// \brief The face code.
    std::vector<float> _faceCode;

    /// \brief The shape mesh for debug drawing.
    //mutable ofPath _shapePath;
    //mutable bool _hasShapePath = false;
    //mutable ofTexture _faceChipTexture;
};


} } // namespace ofx::Dlib