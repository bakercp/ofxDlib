//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/FaceFinder.h"


namespace ofx {
namespace Dlib {


FaceFinder::FaceFinder()
{
    setup(Settings());
}


FaceFinder::FaceFinder(const Settings& settings)
{
    setup(settings);
}


bool FaceFinder::setup(const Settings& settings)
{
#if !defined(__OPTIMIZE__)
    ofLogWarning("FaceFinder::setup") << "FaceFinder runs much faster in RELEASE Mode. http://dlib.net/faq.html#Whyisdlibslow";
#endif

    _isLoaded = false;

    if (_settings.numThreads == 0)
        _effectiveNumThreads = std::thread::hardware_concurrency();

    if (_effectiveNumThreads == 0)
    {
        ofLogWarning("FaceFinder::setup") << "Unable to detect number of threads, setting manually. Defaulting to 1 thread.";
        _effectiveNumThreads = 1;
    }

    _faceDetectorHOG.reset();
    _faceDetectorMMOD.reset();

    if (settings.detectorType == FaceDetector::FACE_DETECTOR_HOG)
    {
        _faceDetectorHOG = std::make_unique<dlib::frontal_face_detector>(dlib::get_frontal_face_detector());
    }
    else if (settings.detectorType == FaceDetector::FACE_DETECTOR_MMOD)
    {
        std::filesystem::path path = ofToDataPath(_settings.modelsPath, true);
        path /= "mmod_human_face_detector.dat";

        if (std::filesystem::exists(path))
        {
            _faceDetectorMMOD = std::make_unique<MMODFaceDetector::Net>();
            dlib::deserialize(path.string()) >> (*_faceDetectorMMOD);
        }
        else
        {
            ofLogError("FaceFinder::setup") << "Unable to find: " << path.string() << ".  Modify your settings.modelsPath and/or make sure the file exists at that location.";
            return false;
        }
    }

    _faceShapePredictor.clear();

    std::filesystem::path path = ofToDataPath(_settings.modelsPath, true);

    if (settings.shapePredictorType == FaceShapePredictor::FACE_SHAPE_PREDICTOR_5_LANDMARKS)
        path /= "shape_predictor_5_face_landmarks.dat";
    else if (settings.shapePredictorType == FaceShapePredictor::FACE_SHAPE_PREDICTOR_68_LANDMARKS)
        path /= "shape_predictor_68_face_landmarks.dat";

    if (std::filesystem::exists(path))
    {
        dlib::shape_predictor shapePredictor;
        dlib::deserialize(path.string()) >> (shapePredictor);
        for (std::size_t i = 0; i < _effectiveNumThreads; ++i)
            _faceShapePredictor.push_back(shapePredictor);

    }
    else
    {
        ofLogError("FaceFinder::setup") << "Unable to find: " << path.string() << ".  Modify your settings.modelsPath and/or make sure the file exists at that location.";
        return false;
    }


    _faceCoder.clear();

    path = ofToDataPath(_settings.modelsPath, true);
    path /= "dlib_face_recognition_resnet_model_v1.dat";

    if (std::filesystem::exists(path))
    {
        auto faceCoder = FaceRecognitionResnetModelV1::Net();
        dlib::deserialize(path.string()) >> faceCoder;
        for (std::size_t i = 0; i < _effectiveNumThreads; ++i)
            _faceCoder.push_back(faceCoder);
    }
    else
    {
        ofLogError("FaceFinder::setup") << "Unable to find: " << path.string() << ".  Modify your settings.modelsPath and/or make sure the file exists at that location.";
        return false;
    }

    // If we made it to here, everything is a success.
    _settings = settings;
    _settings.inputROI.standardize();
    _isLoaded = true;
    return _isLoaded;
}


bool FaceFinder::isLoaded() const
{
    return _isLoaded;
}


FaceFinder::Settings FaceFinder::settings() const
{
    return _settings;
}


const ofPixels& FaceFinder::pixels() const
{
    return _pixels;
}


} } // namespace ofx::Dlib
