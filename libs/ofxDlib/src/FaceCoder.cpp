//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/FaceCoder.h"


namespace ofx {
namespace Dlib {


FaceCoder::FaceCoder()
{
}


FaceCoder::FaceCoder(const Settings& settings)
{
    setup(settings);
}


bool FaceCoder::setup(const Settings& settings)
{
#if !defined(__OPTIMIZE__)
    ofLogWarning("FaceCoder::setup") << "FaceCoder runs much faster when compiled with optimizations (i.e. RELEASE Mode). http://dlib.net/faq.html#Whyisdlibslow";
#endif

    _isLoaded = false;

    _faceCoder.reset();

    std::filesystem::path path;
    path = ofToDataPath(settings.modelsPath, true);
    path /= "dlib_face_recognition_resnet_model_v1.dat";

    if (std::filesystem::exists(path))
    {
        _faceCoder = std::make_shared<FaceRecognitionResnetModelV1::Net>();
        dlib::deserialize(path.string()) >> *_faceCoder;
    }
    else
    {
        ofLogError("FaceCoder::setup") << "Unable to find: " << path.string() << ".  Modify your settings.modelsPath and/or make sure the file exists at that location.";
        return false;
    }

    // If we made it to here, everything is a success.
    _settings = settings;
    _isLoaded = true;
    return _isLoaded;
}


bool FaceCoder::isLoaded() const
{
    return _isLoaded;
}


FaceCoder::Settings FaceCoder::settings() const
{
    return _settings;
}


} } // namespace ofx::Dlib
