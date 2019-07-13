//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/FaceDetector.h"


namespace ofx {
namespace Dlib {


FaceDetector::FaceDetector()
{
}


FaceDetector::FaceDetector(const Settings& settings)
{
    setup(settings);
}


bool FaceDetector::setup(const Settings& settings)
{
#if !defined(__OPTIMIZE__)
    ofLogWarning("FaceDetector::setup") << "FaceDetector runs much faster when compiled with optimizations (i.e. RELEASE Mode). http://dlib.net/faq.html#Whyisdlibslow";
#endif

    _isLoaded = false;

    _faceDetectorHOG.reset();
    _faceDetectorMMOD.reset();

    if (settings.detectorType == Type::FACE_DETECTOR_HOG)
    {
        _faceDetectorHOG = std::make_unique<dlib::frontal_face_detector>(dlib::get_frontal_face_detector());
    }
    else if (settings.detectorType == Type::FACE_DETECTOR_MMOD)
    {
        std::filesystem::path path = ofToDataPath(settings.modelsPath, true);
        path /= "mmod_human_face_detector.dat";

        if (std::filesystem::exists(path))
        {
            dlib::cuda::raii_set_device gpuDevice(settings.gpuDevice);
            _faceDetectorMMOD = std::make_unique<MMODFaceDetector::Net>();
            dlib::deserialize(path.string()) >> (*_faceDetectorMMOD);
        }
        else
        {
            ofLogError("FaceDetector::setup") << "Unable to find: " << path.string() << ".  Modify your settings.modelsPath and/or make sure the file exists at that location.";
            return false;
        }
    }


    // If we made it to here, everything is a success.
    _settings = settings;
    _settings.inputROI.standardize();
    _isLoaded = true;
    return _isLoaded;
}


bool FaceDetector::isLoaded() const
{
    return _isLoaded;
}


FaceDetector::Settings FaceDetector::settings() const
{
    return _settings;
}


const ofPixels& FaceDetector::pixels() const
{
    return _pixels;
}


} } // namespace ofx::Dlib
