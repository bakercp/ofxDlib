//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/FaceShapePredictor.h"


namespace ofx {
namespace Dlib {


FaceShapePredictor::FaceShapePredictor()
{
}


FaceShapePredictor::FaceShapePredictor(const Settings& settings)
{
    setup(settings);
}


bool FaceShapePredictor::setup(const Settings& settings)
{
#if !defined(__OPTIMIZE__)
    ofLogWarning("FaceShapePredictor::setup") << "FaceDetector runs much faster when compiled with optimizations (i.e. RELEASE Mode). http://dlib.net/faq.html#Whyisdlibslow";
#endif

    _isLoaded = false;

    std::filesystem::path path = ofToDataPath(settings.modelsPath, true);

    if (settings.faceShapeType == FaceShape::Type::FACE_SHAPE_5_LANDMARKS)
        path /= "shape_predictor_5_face_landmarks.dat";
    else if (settings.faceShapeType == FaceShape::Type::FACE_SHAPE_68_LANDMARKS)
        path /= "shape_predictor_68_face_landmarks.dat";

    if (std::filesystem::exists(path))
    {
        dlib::deserialize(path.string()) >> _faceShapePredictor;
    }
    else
    {
        ofLogError("FaceShapePredictor::setup") << "Unable to find: " << path.string() << ".  Modify your settings.modelsPath and/or make sure the file exists at that location.";
        return false;
    }

    // If we made it to here, everything is a success.
    _settings = settings;
    _isLoaded = true;
    return _isLoaded;
}


bool FaceShapePredictor::isLoaded() const
{
    return _isLoaded;
}


FaceShapePredictor::Settings FaceShapePredictor::settings() const
{
    return _settings;
}


} } // namespace ofx::Dlib
