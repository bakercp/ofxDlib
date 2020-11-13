//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


// A class representing a yawn.
class Yawn
{
public:
    struct MouthShape
    {
        uint64_t timestamp;
        ofPolyline innerMouth;
        ofPolyline outerMouth;
    };

    Yawn(): playbackColor(ofColor::fromHsb(ofRandom(255), 255, 255))
    {
    }

    bool tryAddFaceShape(const ofxDlib::FaceShape& faceShape)
    {
        float yawnFactor = faceShape.getMeasurement(ofxDlib::FaceShape::Measurement::YAWN_FACTOR);

        if (yawnFactor > minYawnFactor)
        {
            MouthShape shape;
            shape.innerMouth = faceShape.getFeatureAsPolyline(ofxDlib::FaceShape::Feature::INNER_MOUTH);
            shape.outerMouth = faceShape.getFeatureAsPolyline(ofxDlib::FaceShape::Feature::OUTER_MOUTH);
            shape.timestamp = ofGetElapsedTimeMillis();
            mouthShapes.push_back(shape);
            return true;
        }

        // Failed.
        return false;
    }

    bool isLongEnough() const
    {
        return !mouthShapes.empty()
            &&  mouthShapes.back().timestamp - mouthShapes.front().timestamp > minYawnDurationMillis;
    }

    void update()
    {
        // Play in palindrome mode for better looping.
        shapePlaybackIndex += shapePlaybackIndexIncrement;
        if (shapePlaybackIndex - 1 < 0 || shapePlaybackIndex + 1 == mouthShapes.size())
            shapePlaybackIndexIncrement *= -1;
    }

    void draw() const
    {
        ofPushStyle();
        ofSetColor(playbackColor);
        mouthShapes[shapePlaybackIndex].innerMouth.draw();
        mouthShapes[shapePlaybackIndex].outerMouth.draw();
        ofPopStyle();
    }

    // The id of the Yawner.
    std::size_t id = 0;

    // The minimum duration required for a yawn.
    uint64_t minYawnDurationMillis = 4000;

    // The minimum yawn factor required for a yawn.
    float minYawnFactor = 10.0;

    // The timestamp, inner and outer polyline shapes that comprised the yawn.
    std::vector<MouthShape> mouthShapes;

    // The index used for playback.
    int shapePlaybackIndex = 0;

    // Playback increment.
    int shapePlaybackIndexIncrement = 1;

    // The color we'll use during playback.
    ofColor playbackColor;
};


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;

    void onTrackBegin(ofxDlib::FaceTrackerEventArgs& evt);
    void onTrackUpdate(ofxDlib::FaceTrackerEventArgs& evt);
    void onTrackEnd(ofxDlib::FaceTrackerEventArgs& evt);

    // The video grabber.
    ofVideoGrabber video;

    // The face detector.
    ofxDlib::FaceTracker faceTracker;

    // The yawns we are tracking.
    std::map<std::size_t, Yawn> trackedYawns;

    // The finished yawns.
    std::vector<Yawn> yawns;

};
