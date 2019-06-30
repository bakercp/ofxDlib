//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/FaceDetector.h"


namespace ofx {
namespace Dlib {


ObjectDetection::ObjectDetection()
{
}


ObjectDetection::ObjectDetection(const dlib::mmod_rect& detection):
    confidence(detection.detection_confidence),
    rectangle(toOf(detection.rect))
{
}


ObjectDetection::ObjectDetection(const std::pair<double,
                                 dlib::rectangle>& det):
    ObjectDetection(det.first, det.second)
{
}


ObjectDetection::ObjectDetection(double _confidence,
                                 const dlib::rectangle& _rectangle):
    confidence(_confidence),
    rectangle(toOf(_rectangle))
{
}

ObjectDetection::ObjectDetection(double _confidence,
                                 const ofRectangle& _rectangle):
    confidence(_confidence),
    rectangle(_rectangle)
{
}


FaceDetector::FaceDetector()
{
    setup(Settings());
}


FaceDetector::FaceDetector(const Settings& settings)
{
    setup(settings);
}


bool FaceDetector::setup(const Settings& settings)
{
#if !defined(__OPTIMIZE__)
    ofLogWarning("FaceDetector::setup") << "FaceDetector runs much faster in RELEASE Mode. http://dlib.net/faq.html#Whyisdlibslow";
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


//std::vector<ObjectDetection> FaceDetector::detect(const ofBaseHasPixels& image)
//{
////    dlib::of_image _image = toDlib(image.getPixels());
//    return detect(toDlib(image.getPixels()));
//}


const ofPixels& FaceDetector::pixels() const
{
    return _pixels;
}


} } // namespace ofx::Dlib
