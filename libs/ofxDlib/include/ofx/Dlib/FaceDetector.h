//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <vector>
#include <dlib/image_processing/frontal_face_detector.h>
#include "ofx/Dlib/Utils.h"
#include "ofx/Dlib/Network/MMODFaceDetector.h"
#include "ofWindowSettings.h"


namespace ofx {
namespace Dlib {


/// \brief An object detection result with confidence in pixel cooridinates.
class ObjectDetection
{
public:
    ObjectDetection()
    {
    }

    ObjectDetection(const dlib::mmod_rect& rect):
        confidence(rect.detection_confidence),
        rectangle(toOf(rect.rect))
    {
    }

    ObjectDetection(const std::pair<double, dlib::rectangle>& det):
        ObjectDetection(det.second, det.first)
    {
    }

    ObjectDetection(const dlib::rectangle& _rectangle, float _confidence):
        rectangle(toOf(_rectangle)),
        confidence(_confidence)
    {
    }

    ObjectDetection(const ofRectangle& _rectangle, float _confidence):
        rectangle(_rectangle),
        confidence(_confidence)
    {
    }

    /// \brief The detection rectangle in pixel coordinates.
    ofRectangle rectangle;

    /// \brief The detection confidence.
    ///
    ///       0.0 => no confidence,
    ///     > 1.0 => very confident
    ///
    /// If confidence is unsupported, a confidence of 1 will returned.
    float confidence = 0.0;

};



/// \brief The FaceDetector class wraps several dlib face detection types.
class FaceDetector
{
public:
    struct Settings;

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
    /// \tparam The image type.
    template<typename image_type>
    std::vector<ObjectDetection> detect(const image_type& image)
    {
        std::vector<ObjectDetection> results;
        std::vector<std::pair<double, dlib::rectangle>> detections;

        if (!_isLoaded)
        {
            ofLogWarning("FaceDetector::detect") << "The face detector is not set up correctly. Please check your error log and confirm your settings.";
            return results;
        }

        // Establish ROI.
        dlib::rectangle roi(0,
                            0,
                            dlib::num_columns(image),
                            dlib::num_rows(image));

        if (!_settings.inputROI.isEmpty())
        {
            roi.left() = _settings.inputROI.getLeft();
            roi.top() = _settings.inputROI.getTop();
            roi.right() = _settings.inputROI.getRight();
            roi.bottom() = _settings.inputROI.getBottom();
        }

        // Toll-free roi image.
        auto roiImage = dlib::sub_image(image, roi);

        dlib::set_image_size(_pixels,
                             std::round(_settings.inputScale * dlib::num_rows(roiImage)),
                             std::round(_settings.inputScale * dlib::num_columns(roiImage)));

        dlib::resize_image(roiImage, _pixels);

        if (_settings.detectorType == Type::FACE_DETECTOR_HOG)
        {
            (*_faceDetectorHOG)(_pixels, detections);
        }
        else if (_settings.detectorType == Type::FACE_DETECTOR_MMOD)
        {
            dlib::matrix<dlib::rgb_pixel> img = dlib::mat(dlib::of_image<dlib::rgb_pixel, unsigned char>(_pixels));
            std::vector<dlib::mmod_rect> _detections = (*_faceDetectorMMOD)(img);
            detections.reserve(_detections.size());
            for (auto& detection: _detections)
                detections.push_back(std::make_pair(detection.detection_confidence, detection.rect));
        }

        for (auto& detection: detections)
        {
            // Scale and translate detections back to input pixel coordinates.
            detection.second = dlib::scale_rect(detection.second, 1.0 / _settings.inputScale);
            detection.second = dlib::translate_rect(detection.second, roi.left(), roi.top());

            // Filter out any detections that don't meet requirements.
            if (detection.first >= _settings.minimumDetectionConfidence
            &&  detection.second.width()  >= _settings.minimumDetectionSize
            &&  detection.second.height() >= _settings.minimumDetectionSize)
            {
                results.push_back(ObjectDetection(detection));
            }
        }

        return results;
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
        float inputScale = 1.0;

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

    };

private:
    /// \brief The current Settings.
    Settings _settings;

    /// \brief True if the FaceFinder is loaded.
    bool _isLoaded = false;

    /// \brief A pointer to the HOG face detector.
    std::unique_ptr<dlib::frontal_face_detector> _faceDetectorHOG = nullptr;

    /// \brief A pointer to the MMOD face detector.
    std::unique_ptr<MMODFaceDetector::Net> _faceDetectorMMOD = nullptr;

    /// \brief The scaled / ROI copy of the pixels passed to the detector.
    ofPixels _pixels;

};


} } // namespace ofx::Dlib
