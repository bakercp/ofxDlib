//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <vector>
#include "dlib/of_default_adapter.h"
#include "dlib/of_image.h"
//#include "dlib/image_processing/frontal_face_detector.h"
#include "ofx/Dlib/FaceShape.h"
#include "ofx/Dlib/Utils.h"
//#include "ofx/Dlib/Network/MMODFaceDetector.h"
//#include "ofx/Dlib/Network/FaceRecognitionResnetModelV1.h"
//#include "ofThreadChannel.h"
//#include "ofWindowSettings.h"


namespace ofx {
namespace Dlib {


/// \brief The FaceShapePredictor class wraps several dlib face detection types.
class FaceShapePredictor
{
public:
    struct Settings;

    /// \brief Create a default FaceShapePredictor.
    FaceShapePredictor();

    /// \brief Create a FaceShapePredictor with the given settings.
    /// \param settings The Settings to use.
    FaceShapePredictor(const Settings& settings);

    /// \brief Set up the FaceShapePredictor.
    ///
    /// Calling setup will reset the FaceShapePredictor completely.
    ///
    /// \param settings The Settings to use.
    bool setup(const Settings& settings);

    /// \returns true if the FaceShapePredictor has been set up successfully.
    bool isLoaded() const;

    /// \brief Predict the face shape from the given image and face detection.
    /// \param image The image to search.
    /// \param detection The location of the detected face.
    /// \returns a FaceShape with the predictions.
    /// \tparam An image implementing the dlib generic image template interface.
    template<typename image_type>
    FaceShape predict(const image_type& image,
                      const ofRectangle& detection)
    {
        ofPixels alignedImage;
        dlib::full_object_detection fullObjectDetection = _faceShapePredictor(image, toDlib(detection));

        if (_settings.createAlignedImage)
        {
            dlib::chip_details faceDetails = dlib::get_face_chip_details(fullObjectDetection,
                                                                         _settings.alignedImageSize,
                                                                         _settings.alignedImagePadding);

            dlib::extract_image_chip(image, faceDetails, alignedImage);
        }

        std::vector<glm::vec2> faceLandmarks(fullObjectDetection.num_parts());

        for (std::size_t i = 0; i < fullObjectDetection.num_parts(); ++i)
        {
            faceLandmarks[i] = toOf(fullObjectDetection.part(i));
        }

        return FaceShape(_settings.faceShapeType, faceLandmarks, alignedImage);
    }

    /// \returns the current settings.
    Settings settings() const;

    /// \returns the scaled, cropped input pixels used for detections.
    const ofPixels& pixels() const;

    struct Settings
    {
        /// \brief Set the shape predictor algorithm.
        FaceShape::Type faceShapeType = FaceShape::Type::FACE_SHAPE_68_LANDMARKS;

        /// \brief Generate a feature-aligned face image.
        ///
        /// If this is not required it can be set to false for a minor
        /// performance improvement.
        bool createAlignedImage = true;

        /// \brief The size of the extracted aligned image.
        std::size_t alignedImageSize = 150;

        /// \brief The padding of the extracted aligned image.
        ///
        /// Adds padding around the face chip to capture additional context.
        double alignedImagePadding = 0.25;

        /// \brief The path where any models will be found.
        ///
        /// This addon can use shape_predictor_5_face_landmarks.dat and
        /// shape_predictor_68_face_landmarks.dat. By default it will search
        /// for these files in the bin/data/. The user can specify an alternate
        /// location by modifying this variable.
        std::string modelsPath;

    };

private:
    /// \brief The current Settings.
    Settings _settings;

    /// \brief True if the settings were loaded correctly.
    bool _isLoaded = false;

    /// \brief Our shape predictor.
    dlib::shape_predictor _faceShapePredictor;

};


} } // namespace ofx::Dlib
