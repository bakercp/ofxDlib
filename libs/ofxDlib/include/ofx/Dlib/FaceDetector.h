//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <vector>
#include "dlib/of_default_adapter.h"
#include "dlib/of_image.h"
#include "dlib/cuda/cuda_dlib.h"
#include "dlib/image_processing/frontal_face_detector.h"
#include "ofLog.h"
#include "ofx/Dlib/ObjectDetection.h"
#include "ofx/Dlib/Network/MMODFaceDetector.h"


namespace ofx {
namespace Dlib {


/// \brief The FaceDetector class wraps several dlib face detection types.
class FaceDetector
{
public:
    struct Settings;

    /// \brief An enum for the FaceDetector algorithm.
    /// \sa https://www.learnopencv.com/face-detection-opencv-dlib-and-deep-learning-c-python/
    enum class Type
    {
        /// \brief The standard HOG based face detector.
        ///
        /// Appropriate for CPU-based applications.
        FACE_DETECTOR_HOG,

        /// \brief The MMOD Deep Neural-Network face detector.
        ///
        /// More accurate than the FACE_DETECTOR_HOG, but slower on CPU.
        /// Runs very fast on GPUs.
        FACE_DETECTOR_MMOD

    };

    /// \brief Create a default FaceDetector.
    FaceDetector();

    /// \brief Create a FaceDetector with the given settings.
    /// \param settings The Settings to use.
    FaceDetector(const Settings& settings);

    /// \brief Set up the FaceDetector.
    ///
    /// Calling setup will reset the FaceDetector completely.
    ///
    /// \param settings The Settings to use.
    bool setup(const Settings& settings);

    /// \returns true if the FaceDetector has been set up successfully.
    bool isLoaded() const;

    /// \brief Detect all faces.
    ///
    /// Detect all faces and return their location in pixel coordinates and
    /// the normalized detection confidence.
    ///
    ///       0.0 => no confidence,
    ///     > 1.0 => very confident
    ///
    /// \param image The pixels to search.
    /// \returns a vector of rectangles in pixel coordinates and confidences.
    /// \tparam An image implementing the dlib generic image template interface.
    template<typename image_type>
    std::vector<ObjectDetection> detect(const image_type& image)
    {
        std::vector<ObjectDetection> faces;
        std::vector<std::pair<double, dlib::rectangle>> detections;

        if (!_isLoaded)
        {
            ofLogWarning("FaceDetector::detect") << "The face detector is not set up correctly. Please check your error log and confirm your settings.";
            return faces;
        }

        // Establish ROI.
        dlib::rectangle roi(0,
                            0,
                            dlib::num_columns(image),
                            dlib::num_rows(image));

        if (!_settings.inputROI.isEmpty())
        {
            roi.left() = static_cast<long>(_settings.inputROI.getLeft());
            roi.top() = static_cast<long>(_settings.inputROI.getTop());
            roi.right() = static_cast<long>(_settings.inputROI.getRight());
            roi.bottom() = static_cast<long>(_settings.inputROI.getBottom());
        }

        // Toll-free roi image. Using `auto` in these places seems to cause problems.
        dlib::const_sub_image_proxy<image_type> roiImage = dlib::sub_image(image, roi);

        dlib::set_image_size(_pixels,
                             std::round(_settings.inputScale * dlib::num_rows(roiImage)),
                             std::round(_settings.inputScale * dlib::num_columns(roiImage)));

        dlib::resize_image(roiImage, _pixels);

        if (_settings.detectorType == Type::FACE_DETECTOR_HOG)
        {
            (*_faceDetectorHOG)(_pixels, detections, _settings.minimumDetectionConfidence);
        }
        else if (_settings.detectorType == Type::FACE_DETECTOR_MMOD)
        {
            // Set the gpu device during use.
            dlib::cuda::raii_set_device gpuDevice(_settings.gpuDevice);
            dlib::matrix<dlib::rgb_pixel> img = dlib::mat(dlib::of_image<dlib::rgb_pixel, unsigned char>(_pixels));
            std::vector<dlib::mmod_rect> _detections = (*_faceDetectorMMOD)(img);

            detections.reserve(_detections.size());

            for (auto& detection: _detections)
            {
                detections.push_back(std::make_pair(detection.detection_confidence, detection.rect));
            }
        }

        faces.reserve(detections.size());

        for (std::size_t i = 0; i < detections.size(); ++i)
        {
            // Scale and translate detections back to input pixel coordinates.
            detections[i].second = dlib::scale_rect(detections[i].second, 1.0 / double(_settings.inputScale));
            detections[i].second = dlib::translate_rect(detections[i].second, roi.left(), roi.top());

            // Filter out any detections that don't meet requirements.
            if (detections[i].first >= _settings.minimumDetectionConfidence
            &&  detections[i].second.width()  >= _settings.minimumDetectionSize
            &&  detections[i].second.height() >= _settings.minimumDetectionSize)
            {
                faces.push_back(ObjectDetection(detections[i]));
            }
        }

        return faces;
    }

    /// \returns the current settings.
    Settings settings() const;

    /// \returns the scaled, cropped input pixels used for detections.
    const ofPixels& pixels() const;

    struct Settings
    {
        /// \brief Set the back-end detection algorithm.
        Type detectorType = Type::FACE_DETECTOR_HOG;

        /// \brief The video scale used for detection.
        ///
        /// Typically this is set smaller to speed detection. In some cases it
        /// might be set larger and used with an ROI if the faces are too small.
        double inputScale = 1.0;

        /// \brief The interpolation method used when scaling.
        ofInterpolationMethod inputScaleInterpolation = OF_INTERPOLATE_NEAREST_NEIGHBOR;

        /// \brief Detection ROI.
        ///
        /// The coordinates are in input image pixels.
        ///
        /// Detection searches will be limited to this region.
        ofRectangle inputROI;

        /// \brief Ignore detections less than the minimum confidence.
        double minimumDetectionConfidence = 0.0;

        /// \brief Ignore detections with a size (width or height) less than the minimum size.
        double minimumDetectionSize = 0.0;

        /// \brief The path where any models will be found.
        ///
        /// This addon can use the mmod_human_face_detector.dat
        /// shape_predictor_5_face_landmarks.dat and
        /// shape_predictor_68_face_landmarks.dat. By default it will search
        /// for these files in the bin/data/. The user can specify an alternate
        /// location by modifying this variable.
        std::string modelsPath;

        /// \brief Choose the GPU device to use if applicable.
        /// \sa http://dlib.net/dlib/dnn/cuda_dlib.h.html
        int gpuDevice = 0;

    };

private:
    /// \brief The current Settings.
    Settings _settings;

    /// \brief True if the settings were loaded correctly.
    bool _isLoaded = false;

    /// \brief A pointer to the HOG face detector.
    std::unique_ptr<dlib::frontal_face_detector> _faceDetectorHOG = nullptr;

    /// \brief A pointer to the MMOD face detector.
    std::unique_ptr<MMODFaceDetector::Net> _faceDetectorMMOD = nullptr;

    /// \brief The scaled / ROI copy of the pixels passed to the detector.
    ofPixels _pixels;

};


} } // namespace ofx::Dlib
