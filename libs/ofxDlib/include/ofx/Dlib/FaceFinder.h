//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <vector>
#include "dlib/of_default_adapter.h"
#include "dlib/of_image.h"
#include "dlib/image_processing/frontal_face_detector.h"
#include "ofx/Dlib/Face.h"
#include "ofx/Dlib/Utils.h"
#include "ofx/Dlib/Network/MMODFaceDetector.h"
#include "ofx/Dlib/Network/FaceRecognitionResnetModelV1.h"
#include "ofThreadChannel.h"
#include "ofWindowSettings.h"


namespace ofx {
namespace Dlib {


/// \brief The FaceFinder class wraps several dlib face detection types.
class FaceFinder
{
public:
    struct Settings;

    /// \brief An enum for the FaceDetector algorithm.
    enum class FaceDetector
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

    enum class FaceShapePredictor
    {
        /// \brief The fast 5 landmark face shape predictor.
        FACE_SHAPE_PREDICTOR_5_LANDMARKS,

        /// \brief The precise 68 landmark face shape predictor.
        FACE_SHAPE_PREDICTOR_68_LANDMARKS
    };


    /// \brief Create a default FaceFinder.
    FaceFinder();

    /// \brief Create a FaceFinder with the given settings.
    /// \param settings The Settings to use.
    FaceFinder(const Settings& settings);

    /// \brief Set up the FaceFinder.
    ///
    /// Calling setup will reset the FaceFinder completely.
    ///
    /// \param settings The Settings to use.
    bool setup(const Settings& settings);

    /// \returns true if the FaceFinder has been set up successfully.
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
    std::vector<Face> detect(const image_type& image)
    {
        //dlib::cuda::raii_set_device gpuDevice(_settings.gpuDevice);

        std::vector<Face> faces;
        std::vector<std::pair<double, dlib::rectangle>> detections;

        if (!_isLoaded)
        {
            ofLogWarning("FaceFinder::detect") << "The face detector is not set up correctly. Please check your error log and confirm your settings.";
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

        if (_settings.detectorType == FaceDetector::FACE_DETECTOR_HOG)
        {
            (*_faceDetectorHOG)(_pixels, detections);
        }
        else if (_settings.detectorType == FaceDetector::FACE_DETECTOR_MMOD)
        {
            dlib::matrix<dlib::rgb_pixel> img = dlib::mat(dlib::of_image<dlib::rgb_pixel, unsigned char>(_pixels));
            std::vector<dlib::mmod_rect> _detections = (*_faceDetectorMMOD)(img);
            detections.reserve(_detections.size());
            for (auto& detection: _detections)
                detections.push_back(std::make_pair(detection.detection_confidence, detection.rect));
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
                auto faceShape = _faceShapePredictor[i](image, detections[i].second);
                auto faceDetails = dlib::get_face_chip_details(faceShape,
                                                               _settings.faceChipSize,
                                                               _settings.faceChipPadding);
                ofPixels faceChip;
                dlib::extract_image_chip(image, faceDetails, faceChip);
                dlib::matrix<dlib::rgb_pixel> fc = dlib::mat(faceChip);

                std::size_t nJitters = std::max(std::size_t(0), _settings.jitterIterations);
                dlib::matrix<float, 0, 1> faceCodes;

                if (_settings.jitterIterations > 0)
                    faceCodes = dlib::mean(dlib::mat(_faceCoder[i](jitter_image(fc, nJitters))));
                else
                    faceCodes = _faceCoder[i](fc);

                std::vector<float> faceCode(faceCodes.begin(), faceCodes.end());

                faces.push_back(Face(detections[i], faceShape, faceChip, faceCode));
            }
        }

        return faces;
    }



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
    std::vector<Face> find(const image_type& image)
    {
        //dlib::cuda::raii_set_device gpuDevice(_settings.gpuDevice);

        std::vector<Face> faces;
        std::vector<std::pair<double, dlib::rectangle>> detections;

        if (!_isLoaded)
        {
            ofLogWarning("FaceFinder::detect") << "The face detector is not set up correctly. Please check your error log and confirm your settings.";
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

        if (_settings.detectorType == FaceDetector::FACE_DETECTOR_HOG)
        {
            (*_faceDetectorHOG)(_pixels, detections);
        }
        else if (_settings.detectorType == FaceDetector::FACE_DETECTOR_MMOD)
        {
            dlib::matrix<dlib::rgb_pixel> img = dlib::mat(dlib::of_image<dlib::rgb_pixel, unsigned char>(_pixels));
            std::vector<dlib::mmod_rect> _detections = (*_faceDetectorMMOD)(img);
            detections.reserve(_detections.size());
            for (auto& detection: _detections)
                detections.push_back(std::make_pair(detection.detection_confidence, detection.rect));
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
                auto faceShape = _faceShapePredictor[i](image, detections[i].second);
                auto faceDetails = dlib::get_face_chip_details(faceShape,
                                                               _settings.faceChipSize,
                                                               _settings.faceChipPadding);
                ofPixels faceChip;
                dlib::extract_image_chip(image, faceDetails, faceChip);
                dlib::matrix<dlib::rgb_pixel> fc = dlib::mat(faceChip);

                std::size_t nJitters = std::max(std::size_t(0), _settings.jitterIterations);
                dlib::matrix<float, 0, 1> faceCodes;

                if (_settings.jitterIterations > 0)
                    faceCodes = dlib::mean(dlib::mat(_faceCoder[i](jitter_image(fc, nJitters))));
                else
                    faceCodes = _faceCoder[i](fc);

                std::vector<float> faceCode(faceCodes.begin(), faceCodes.end());

                faces.push_back(Face(detections[i], faceShape, faceChip, faceCode));
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
        /// \brief The number of threads to use.
        ///
        /// A value of 0 queries the hardware for maximum concurrent threads.
        std::size_t numThreads = 0;

        /// \brief Set the back-end detection algorithm.
        FaceDetector detectorType = FaceDetector::FACE_DETECTOR_HOG;

        /// \brief Set the shape predictor algorithm.
        FaceShapePredictor shapePredictorType = FaceShapePredictor::FACE_SHAPE_PREDICTOR_68_LANDMARKS;

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

        /// \brief The size of the extracted face chips.
        std::size_t faceChipSize = 150;

        /// \brief The padding of the extracted face chips.
        ///
        /// Adds padding around the face chip to capture additional context.
        double faceChipPadding = 0.25;

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
        // int gpuDevice = 0;

    };

private:
    /// \brief Create a collection of slight image variations.
    ///
    /// This calls `dlib::jitter_image` a total of `n` times and returns the
    /// collection of jittered imaged. Jittered images are slightly rotated,
    /// zoomed, translated and mirrored randomly.
    ///
    /// \param img The image to jitter.
    /// \param n The number of jittered image variations to return.
    /// \returns a collection of `n` jittered images.
    /// \tparam image_type The image type to itter. This can be any compatible
    /// openFrameworks or dlib image type.
    template<typename image_type>
    std::vector<image_type> jitter_image(const image_type& img, std::size_t n)
    {
        // All this function does is make `n` copies of img, all slightly jittered by being
        // zoomed, rotated, and translated a little bit differently. They are also randomly
        // mirrored left to right.
        thread_local dlib::rand rnd;

        std::vector<image_type> crops;

        for (std::size_t i = 0; i < n; ++i)
            crops.push_back(dlib::jitter_image(img, rnd));

        return crops;
    }

    /// \brief The current Settings.
    Settings _settings;

    /// \brief The number of threads to use.
    std::size_t _effectiveNumThreads = 0;

    /// \brief True if the settings were loaded correctly.
    bool _isLoaded = false;

    /// \brief A pointer to the HOG face detector.
    std::unique_ptr<dlib::frontal_face_detector> _faceDetectorHOG = nullptr;

    /// \brief A pointer to the MMOD face detector.
    std::unique_ptr<MMODFaceDetector::Net> _faceDetectorMMOD = nullptr;

    /// \brief Our shape predictor.
    std::vector<dlib::shape_predictor> _faceShapePredictor;

    /// \brief The neural network used to calculate a 128 digit face code.
    std::vector<FaceRecognitionResnetModelV1::Net> _faceCoder;

    /// \brief The scaled / ROI copy of the pixels passed to the detector.
    ofPixels _pixels;

};


} } // namespace ofx::Dlib
