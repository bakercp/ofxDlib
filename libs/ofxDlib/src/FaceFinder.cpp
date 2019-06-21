//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/FaceFinder.h"
#include "ofGraphics.h"
#include "ofMesh.h"
#include <thread>


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
    _isLoaded = false;

    std::size_t numThreads = _settings.numThreads;

    if (numThreads == 0) numThreads = std::thread::hardware_concurrency();

    _faceDetectorHOG.clear();
    _faceDetectorMMOD.reset();

    if (settings.detectorType == FaceDetector::FACE_DETECTOR_HOG)
    {
        for (std::size_t i = 0; i < numThreads; ++i)
            _faceDetectorHOG.push_back(dlib::get_frontal_face_detector());
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
        for (std::size_t i = 0; i < numThreads; ++i)
            _faceShapePredictor.push_back(shapePredictor);

    }
    else
    {
        ofLogError("FaceFinder::setup") << "Unable to find: " << path.string() << ".  Modify your settings.modelsPath and/or make sure the file exists at that location.";
        return false;
    }


    _faceCoder.reset();
    path = ofToDataPath(_settings.modelsPath, true);
    path /= "dlib_face_recognition_resnet_model_v1.dat";

    if (std::filesystem::exists(path))
    {
        _faceCoder = std::make_unique<FaceRecognitionResnetModelV1::Net>();
        dlib::deserialize(path.string()) >> (*_faceCoder);
    }
    else
    {
        ofLogError("FaceFinder::setup") << "Unable to find: " << path.string() << ".  Modify your settings.modelsPath and/or make sure the file exists at that location.";
        return false;
    }

    // If we made it to here, everything is a success.
    _settings = settings;
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


FaceDatabase::Entry::Entry()
{
}


//float FaceDatabase::Entry::distance(const Entry& entry)
//{
//    dlib::matrix<float> f0 = dlib::mat(entry.meanFaceCode());
//    dlib::matrix<float> f1 = dlib::mat(meanFaceCode());
//    return dlib::length(f1 - f0);
//}


float FaceDatabase::Entry::distance(const Face& face)
{
    dlib::matrix<float> f0 = dlib::mat(face.faceCode());
    dlib::matrix<float> f1 = dlib::mat(meanFaceCode());
    return dlib::length(f1 - f0);
}


//bool FaceDatabase::Entry::matches(const Entry& entry, float maxDistance)
//{
//    dlib::matrix<float> f0 = dlib::mat(entry.meanFaceCode());
//    dlib::matrix<float> f1 = dlib::mat(meanFaceCode());
//    return dlib::length(f1 - f0);
//}


bool FaceDatabase::Entry::matches(const Face& face, float maxDistance)
{
    return distance(face) < maxDistance;
}


std::vector<float> FaceDatabase::Entry::meanFaceCode() const
{
    if (_meanFaceCode.empty())
    {
        _meanFaceCode.resize(_faceCode.size());
        for (std::size_t i = 0; i < _meanFaceCode.size(); ++i)
            _meanFaceCode[i] = _faceCode[i].mean();
    }

    return _meanFaceCode;
}


void FaceDatabase::Entry::add(const Face& face)
{
    if (!_faceChip.isAllocated())
        _faceChip = face.faceChip();
    else if (face.faceChip().size() == _faceChip.size())
    {
//        for (std::size_t i = 0; i < face.faceChip().size(); ++i)
//            _faceChip[i] = (_faceChipAlpha) * _faceChip[i] + (1 - _faceChipAlpha) * face.faceChip()[i];
        _faceChip = face.faceChip();
        _faceChipTexture.clear();
    }
    else ofLogWarning("FaceDatabase::Entry::add") << "Face chip size mismatch: " << face.faceChip().size() << " being added vs. " << _faceChip.size() << " in the database.";

    if (_faceCode.empty())
        for (std::size_t i = 0; i < face.faceCode().size(); ++i)
            _faceCode.push_back(dlib::running_stats<double>());

    for (std::size_t i = 0; i < face.faceCode().size(); ++i)
        _faceCode[i].add(face.faceCode()[i]);

    _numSamples++;
    _meanFaceCode.clear();
}


void FaceDatabase::Entry::add(const std::vector<Face>& faces)
{
    for (auto&& face: faces) add(face);
}


void FaceDatabase::Entry::draw(float x, float y, float width, float height) const
{
    ofRectangle drawRectangle(x, y, width, height);

    if (!_faceChipTexture.isAllocated())
        _faceChipTexture.loadData(_faceChip);

    _faceChipTexture.draw(drawRectangle);

    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::orange);
    ofDrawRectangle(drawRectangle);
//    if (!_label.empty())
        ofDrawBitmapStringHighlight(_label + ": " + ofToString(_numSamples),
                                    drawRectangle.getPosition() + glm::vec3(4, 13, 0),
                                    ofColor::yellow,
                                    ofColor::black);
    ofPopStyle();
}


void FaceDatabase::Entry::setLabel(const std::string& label)
{
    _label = label;
}


std::string FaceDatabase::Entry::getLabel() const
{
    return _label;
}


FaceDatabase::FaceDatabase()
{
}


void FaceDatabase::add(const std::vector<Face>& faces)
{
    for (auto&& face: faces)
    {
        float _minDistance = std::numeric_limits<float>::max();
        int minDistanceEntry = -1;
        for (int i = 0; i < _entries.size(); ++i)
        {
            float distance = _entries[i].distance(face);

            if (distance < _minDistance)
            {
                _minDistance = distance;
                minDistanceEntry = i;
            }
        }

        if (_minDistance < 0.6)
        {
            _entries[minDistanceEntry].add(face);
        }
        else
        {
            _entries.push_back(Entry());
            _entries.back().add(face);
        }
    }
}


std::vector<FaceDatabase::Entry> FaceDatabase::entries()
{
    return _entries;
}


void FaceDatabase::draw(float x,
                        float y,
                        float width,
                        float height) const
{
    ofRectangle drawRectangle(x, y, width, height);
    float _x = drawRectangle.x;
    float _y = drawRectangle.x;
    float _width = 100;
    float _height = 100;
    for (auto&& entry: _entries)
    {
        entry.draw(_x, _y, _width, _height);
        if (_x + _width > drawRectangle.getMaxX())
        {
            _x = drawRectangle.getMinX();
            _y = _height;
        }
        else
        {
            _x += _width;
        }
    }
}


} } // namespace ofx::Dlib
