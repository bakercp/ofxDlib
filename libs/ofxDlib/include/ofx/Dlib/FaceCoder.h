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
//#include "ofx/Dlib/FaceShape.h"
#include "ofx/Dlib/Utils.h"
//#include "ofx/Dlib/Network/MMODFaceDetector.h"
#include "ofx/Dlib/Network/FaceRecognitionResnetModelV1.h"
//#include "ofThreadChannel.h"
//#include "ofWindowSettings.h"


namespace ofx {
namespace Dlib {


/// \brief The FaceCoder wraps common face coding strategies.
class FaceCoder
{
public:
    struct Settings;

    /// \brief Create a default FaceCoder.
    FaceCoder();

    /// \brief Create a FaceCoder with the given settings.
    /// \param settings The Settings to use.
    FaceCoder(const Settings& settings);

    /// \brief Set up the FaceCoder.
    ///
    /// Calling setup will reset the FaceCoder completely.
    ///
    /// \param settings The Settings to use.
    bool setup(const Settings& settings);

    /// \returns true if the FaceCoder has been set up successfully.
    bool isLoaded() const;

    /// \brief Predict the face shape from the given image and face detection.
    /// \param image The image to search.
    /// \param detection The location of the detected face.
    /// \returns a FaceShape with the predictions.
    /// \tparam An image implementing the dlib generic image template interface.
    template<typename image_type>
    std::vector<float> code(const image_type& image)
    {
        // Set the gpu device during use.
        dlib::cuda::raii_set_device gpuDevice(_settings.gpuDevice);

        dlib::matrix<dlib::rgb_pixel> fc = dlib::mat(image);

        std::size_t nJitters = std::max(std::size_t(0), _settings.jitterIterations);
        dlib::matrix<float, 0, 1> faceCodes;

        if (_settings.jitterIterations > 0)
            faceCodes = dlib::mean(dlib::mat((*_faceCoder)(jitterImage(fc, nJitters))));
        else
            faceCodes = (*_faceCoder)(fc);

        return std::vector<float>(faceCodes.begin(), faceCodes.end());
    }

    /// \returns the current settings.
    Settings settings() const;

    struct Settings
    {
        /// \brief The number image jitters used to calculate a face code.
        std::size_t jitterIterations = 0;

        /// \brief The path where any models will be found.
        ///
        /// This addon can use the mmod_human_face_detector.dat
        /// shape_predictor_5_face_landmarks.dat and
        /// shape_predictor_68_face_landmarks.dat. By default it will search
        /// for these files in the bin/data/. The user can specify an alternate
        /// location by modifying this variable.
        std::string modelsPath;

        /// \brief Choose the GPU device to use if available.
        int gpuDevice = 0;

    };

private:
    /// \brief The current Settings.
    Settings _settings;

    /// \brief True if the settings were loaded correctly.
    bool _isLoaded = false;

    /// \brief Our face coder.
    std::shared_ptr<FaceRecognitionResnetModelV1::Net> _faceCoder;

};


} } // namespace ofx::Dlib
