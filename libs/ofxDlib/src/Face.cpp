//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/Face.h"
#include "ofx/Dlib/Utils.h"
#include "ofGraphics.h"
#include "ofMesh.h"
#include <thread>


namespace ofx {
namespace Dlib {



Face::Face()
{
}


Face::Face(const std::pair<double, dlib::rectangle>& face,
           const dlib::full_object_detection& shape,
           const ofPixels& faceChip,
           const std::vector<float>& faceCode):
    _isLoaded(true),
    _confidence(face.first),
    _rectangle(toOf(face.second)),
    _shape(shape),
    _faceChip(faceChip),
    _faceCode(faceCode)
{
}


bool Face::isLoaded() const
{
    return _isLoaded;
}


double Face::confidence() const
{
    return _confidence;
}


ofRectangle Face::rectangle() const
{
    return _rectangle;
}


std::vector<glm::vec2> Face::landmarks() const
{
    std::vector<glm::vec2> landmarks;
    for (std::size_t i = 0; i < _shape.num_parts(); ++i)
    {
        landmarks.push_back(toOf(_shape.part(i)));
    }
    return landmarks;
}


dlib::full_object_detection Face::shape() const
{
    return _shape;
}


const ofPixels& Face::faceChip() const
{
    return _faceChip;
}


const std::vector<float>& Face::faceCode() const
{
    return _faceCode;
}


//void Face::draw() const
//{
//    if (!_isLoaded)
//        return;
//
//    ofPushStyle();
//    ofNoFill();
//    ofSetColor(ofColor::yellow);
//    ofDrawRectangle(_rectangle);
//    ofDrawBitmapStringHighlight(ofToString(_confidence * 100, 2),
//                                _rectangle.getPosition() + glm::vec3(4, 13, 0),
//                                ofColor::yellow,
//                                ofColor::black);
//
//    ofSetColor(ofColor::green);
//    ofDrawRectangle(toOf(_shape.get_rect()));
//    ofSetColor(ofColor::blue);
//
//
//    if (!_hasShapePath && _shape.num_parts() > 0)
//    {
//        for (std::size_t i = 0; i < _shape.num_parts(); ++i)
//        {
//            auto p = toOf(_shape.part(i));
//            _shapePath.circle(glm::vec3(p.x, p.y, 0), 3);
//        }
//
//        _hasShapePath = true;
//    }
//
//    if (!_faceChipTexture.isAllocated())
//        _faceChipTexture.loadData(_faceChip);
//
//    _shapePath.draw();
//
//    ofSetColor(255);
//    ofRectangle chipRectangle = rectangle();
//    chipRectangle.x += chipRectangle.getWidth();
//    _faceChipTexture.draw(chipRectangle);
//
//    ofNoFill();
//    ofSetColor(ofColor::orange);
//    ofDrawRectangle(chipRectangle);
//    ofDrawBitmapStringHighlight("Aligned",
//                                chipRectangle.getPosition() + glm::vec3(4, 13, 0),
//                                ofColor::yellow,
//                                ofColor::black);
//
//
//    ofPopStyle();
//}


} } // namespace ofx::Dlib
