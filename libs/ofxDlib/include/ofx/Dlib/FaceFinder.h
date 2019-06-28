//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <vector>
#include <dlib/image_processing/frontal_face_detector.h>
#include "ofx/Dlib/Face.h"
#include "ofx/Dlib/Utils.h"
#include "ofx/Dlib/Network/MMODFaceDetector.h"
#include "ofx/Dlib/Network/FaceRecognitionResnetModelV1.h"
#include "ofPath.h"
#include "ofPixels.h"
#include "ofRectangle.h"


namespace ofx {
namespace Dlib {


/// \brief A class that finds faces.
///
/// There are lots of ways to find faces in an image.  This class provides a
/// simple interface for finding faces.
///
/// All coordinates of rectangles and shapes are in the original image
/// coordiantes.
class FaceFinder
{
public:
    struct Settings;

    typedef std::vector<Face> Faces;

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

    /// \brief Set up the FaceDetector.
    ///
    /// Calling setup will reset the FaceFinder completely.
    ///
    /// \param settings The Settings to use.
    bool setup(const Settings& settings);

    /// \returns true if the FaceFinder has been set up successfully.
    bool isLoaded() const;

    /// \brief Find all faces.
    ///
    /// Find all faces and return their location in pixel coordinates and
    /// the normalized detection confidence on (0 = no confidence, 1 = total
    /// confidence). If confidence is unsupported, a confidence of 1 will be
    /// returned.
    ///
    /// \param pixels The pixels to search.
    /// \returns a vector of rectangles in pixel coordinates and confidences.
    /// \tparam The image type.
    template<typename image_type>
    Faces findAll(const image_type& image)
    {
        Faces faces;

        if (!_isLoaded)
        {
            ofLogWarning("FaceFinder::findAll") << "The face finder is not set up correctly. Please check your error log and confirm your settings.";
            return faces;
        }

        std::vector<std::pair<double, dlib::rectangle>> detections;
        dlib::matrix<dlib::rgb_pixel> img = dlib::mat(image);

        if (_settings.detectorType == FaceDetector::FACE_DETECTOR_HOG)
        {
            _faceDetectorHOG[0](img, detections);
        }
        else if (_settings.detectorType == FaceDetector::FACE_DETECTOR_MMOD)
        {
            std::vector<dlib::mmod_rect> _detections = (*_faceDetectorMMOD)(img);
            detections.reserve(_detections.size());
            for (auto& detection: _detections)
                detections.push_back(std::make_pair(detection.detection_confidence, detection.rect));
        }

        faces.reserve(detections.size());
        for (auto& detection: detections)
        {
            auto faceShape = _faceShapePredictor[0](img, detection.second);
            auto faceDetails = dlib::get_face_chip_details(faceShape,
                                                           _settings.faceChipSize,
                                                           _settings.faceChipPadding);
            image_type faceChip;
            dlib::extract_image_chip(image, faceDetails, faceChip);
            dlib::matrix<dlib::rgb_pixel> fc = dlib::mat(faceChip);
            std::size_t nJitters = std::max(std::size_t(0), _settings.jitterIterations);
            dlib::matrix<float, 0, 1> faceCodes;

            if (nJitters > 0)
                faceCodes = dlib::mean(dlib::mat((*_faceCoder)(jitter_image(fc, nJitters))));
            else
                faceCodes = (*_faceCoder)(fc);

            std::vector<float> faceCode(faceCodes.begin(), faceCodes.end());
            faces.push_back(Face(detection, faceShape, faceChip, faceCode));
        }

        return faces;
    }


//    dlib::matrix<float, 0, 1> getEmbeddings(

//
//    /// \brief Find all faces in batch mode.
//    ///
//    /// Finding faces in batch mode can speed execution in some cases.
//    ///
//    /// Find all faces and return their location in pixel coordinates and
//    /// the normalized detection confidence on (0 = no confidence, 1 = total
//    /// confidence). If confidence is unsupported, a confidence of 1 will be
//    /// returned.
//    ///
//    /// \param pixels The pixels to search.
//    /// \returns a vector of vectors of rectangles in pixel coordinates and confidences.
//    /// \tparam The image type.
//    template<typename image_type>
//    std::vector<Faces> findAll(const std::vector<image_type>& images)
//    {
//        std::vector<Faces> results;
//
//        if (!_isLoaded)
//        {
//            ofLogWarning("FaceFinder::findAll") << "The face finder is not set up correctly. Please check your error log and confirm your settings.";
//            return results;
//        }
//
//        results.resize(images.size());
//
//        if (_settings.detectorType == FaceDetector::FACE_DETECTOR_HOG)
//        {
//            dlib::parallel_for(0, images.size(), [&](long i)
//            {
//                std::vector<std::pair<double, dlib::rectangle>> detections;
//                _faceDetectorHOG[i](images[i], detections);
//                for (auto& detection: detections)
//                {
//                    auto faceShape = _faceShapePredictor[i](images[i], detection);
//                    results[i].push_back(Face(detection, faceShape));
//                }
//            });
//        }
//        else if (_settings.detectorType == FaceDetector::FACE_DETECTOR_MMOD)
//        {
//
//        }
//
//        return results;
//    }
//
//
    /// \returns the current settings.
    Settings settings() const;

    struct Settings
    {
        /// \brief Set the back-end detection algorithm.
        FaceDetector detectorType = FaceDetector::FACE_DETECTOR_HOG;

        /// \brief Set the shape predictor algorithm.
        FaceShapePredictor shapePredictorType = FaceShapePredictor::FACE_SHAPE_PREDICTOR_5_LANDMARKS;

        /// \brief Set the number of threads for parallel execution.
        ///
        /// A value of 0 will use the maximum threads available to the system.
        std::size_t numThreads = 1;

        /// \brief The path where any models will be found.
        ///
        /// This addon can use the mmod_human_face_detector.dat
        /// shape_predictor_5_face_landmarks.dat and
        /// shape_predictor_68_face_landmarks.dat. By default it will search
        /// for these files in the bin/data/. The user can specify an alternate
        /// location by modifying this variable.
        std::string modelsPath;

        /// \brief The size of the extracted face chips.
        ///
        /// The default used by
        std::size_t faceChipSize = 150;

        /// \brief The padding of the extracted face chips.
        float faceChipPadding = 0.25;

        std::size_t jitterIterations = 1;

        enum
        {
            MAX_THREADS = 0
        };
    };

private:
    /// \brief The current Settings.
    Settings _settings;

    /// \brief True if the FaceFinder is loaded.
    bool _isLoaded = false;

    /// \brief The HOG face detector.
    ///
    /// The number of instances will match the number of threads specified in
    /// the settings.
    std::vector<dlib::frontal_face_detector> _faceDetectorHOG;

    /// \brief A pointer to the MMOD face detector.
    std::unique_ptr<MMODFaceDetector::Net> _faceDetectorMMOD = nullptr;

    /// \brief Our shape predictor.
    std::vector<dlib::shape_predictor> _faceShapePredictor;

    // The neural network that we will use to convert
    // each face to a "face code" (i.e. 128 numbers).
    std::unique_ptr<FaceRecognitionResnetModelV1::Net> _faceCoder;


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
};



class FaceDatabase
{
public:
    class Entry
    {
    public:
        Entry();

//        float distance(const Entry& entry);
        float distance(const Face& face);

        /// \brief Determine if two faces
//        bool matches(const Entry& entry, float maxDistance);
        bool matches(const Face& face, float maxDistance);

        void add(const Face& face);

        void add(const std::vector<Face>& faces);

        std::vector<float> meanFaceCode() const;

        void draw(float x, float y, float width, float height) const;

        void setLabel(const std::string& label);

        std::string getLabel() const;

    private:
        std::string _label;
        std::size_t _numSamples = 0;
        std::vector<dlib::running_stats<double>> _faceCode;

        mutable std::vector<float> _meanFaceCode;

        float _faceChipAlpha = 0.99f;
        ofFloatPixels _faceChip;
        mutable ofTexture _faceChipTexture;

    };

    FaceDatabase();

    void add(const FaceFinder::Faces& faces);

    std::vector<Entry> entries();

    void draw(float x,
              float y,
              float width,
              float height) const;

private:
    std::vector<Entry> _entries;

};



class FaceLandmarks
{
public:
    enum class Model
    {
        /// \brief The fast 5 landmark face shape predictor.
        MODEL_5_LANDMARKS,
        /// \brief The precise 68 landmark face shape predictor.
        MODEL_68_LANDMARKS
    };

   // enum class Model_5_Features


};

//
//
//class LandmarkMapper
//{
//public:
//    enum Feature
//    {
//        LEFT_EYE_TOP,
//        RIGHT_EYE_TOP,
//        LEFT_EYEBROW,
//        RIGHT_EYEBROW,
//        LEFT_EYE,
//        RIGHT_EYE,
//        LEFT_JAW,
//        RIGHT_JAW,
//        JAW,
//        OUTER_MOUTH,
//        INNER_MOUTH,
//        NOSE_BRIDGE,
//        NOSE_BASE,
//        FACE_OUTLINE,
//        ALL_FEATURES
//    };
//
//    std::vector<std::size_t> getFeatureIndices(Feature feature) {
//        switch(feature) {
//            case LEFT_EYE_TOP: return consecutive(36, 40);
//            case RIGHT_EYE_TOP: return consecutive(42, 46);
//            case LEFT_JAW: return consecutive(0, 9);
//            case RIGHT_JAW: return consecutive(8, 17);
//            case JAW: return consecutive(0, 17);
//            case LEFT_EYEBROW: return consecutive(17, 22);
//            case RIGHT_EYEBROW: return consecutive(22, 27);
//            case LEFT_EYE: return consecutive(36, 42);
//            case RIGHT_EYE: return consecutive(42, 48);
//            case OUTER_MOUTH: return consecutive(48, 60);
//            case INNER_MOUTH: return consecutive(60, 68);
//            case NOSE_BRIDGE: return consecutive(27, 31);
//            case NOSE_BASE: return consecutive(31, 36);
//            case FACE_OUTLINE: {
//                static int faceOutline[] = {17,18,19,20,21,22,23,24,25,26, 16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
//                return vector<int>(faceOutline, faceOutline + 27);
//            }
//            case ALL_FEATURES: return consecutive(0, 68);
//        }
//    }
//
//
//    template <class T>
//    ofPolyline ofxFaceTracker2Landmarks::getFeature(Feature feature, vector<T> points) const {
//        ofPolyline polyline;
//        vector<int> indices = getFeatureIndices(feature);
//        for(int i = 0; i < indices.size(); i++) {
//            int cur = indices[i];
//            glm::vec2 pt = toGlm(points[cur]);
//            polyline.addVertex(pt.x, pt.y, 0);
//        }
//        switch(feature) {
//            case LEFT_EYE:
//            case RIGHT_EYE:
//            case OUTER_MOUTH:
//            case INNER_MOUTH:
//            case FACE_OUTLINE:
//                polyline.close();
//                break;
//            default:
//                break;
//        }
//
//        return polyline;
//    }
//
//    std::vector<std::size_t> consecutive(std::size_t start, std::size_t end)
//    {
//        if (end < start)
//        {
//            ofLogWarning("ofxFaceTracker2Landmarks::consecutive") << "Start index is greater than end index.";
//            return std::vector<std::size_t>();
//        }
//
//        std::vector<std::size_t> l(end - start);
//        std::iota(l.begin(), l.end(), start);
//        return l;
//    }
//
//};
//



} } // namespace ofx::Dlib
