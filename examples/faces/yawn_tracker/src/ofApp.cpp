//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Set up the detector settings.
    // See the ofxDlib/examples/faces/tracker example for a description of these settings.
    ofxDlib::FaceTracker::Settings settings;

    // Use the GPU face detector.
    // settings.faceDetectorSettings.detectorType = ofxDlib::FaceDetector::Type::FACE_DETECTOR_MMOD;

    // Make a small ROI.
    settings.faceDetectorSettings.inputROI = ofRectangle(340, 60, 600, 600);

    // Smooth our face shapes because yawns are slow anyway.
    // This helps a lot when using the FACE_DETECTOR_HOG detector.
    settings.faceDetectorFilterSmoothness = 0.75;
    settings.faceShapeFilterSmoothness = 0.75;

    // Set up the detector.
    faceTracker.setup(settings);

    // Add tracking event listeners.
    faceTracker.registerEvents(this);

    // Set up the video input.
    video.setDeviceID(0);
    video.setup(1280, 720);
}


void ofApp::update()
{
    // Update the video.
    video.update();

    // Process a frame if it is new.
    if (video.isFrameNew())
    {
        // Track the faces.
        faceTracker.track(video);

        // We update our yawn videos at the same rate that the camera is updated.
        for (auto& yawn: yawns)
        {
            yawn.update();
        }
    }
}


void ofApp::draw()
{
    // Draw the input video.
    ofSetColor(ofColor::white);
    video.draw(0, 0);

    // Draw the ROI.
    ofSetColor(ofColor::white, 127);
    ofxDlib::draw(faceTracker.settings().faceDetectorSettings.inputROI, "ROI");

    // Draw the mouth shapes.
    ofSetColor(ofColor::yellow);
    for (auto& shape: faceTracker.shapes())
    {
        shape.second.getFeatureAsPolyline(ofxDlib::FaceShape::Feature::INNER_MOUTH).draw();
        shape.second.getFeatureAsPolyline(ofxDlib::FaceShape::Feature::OUTER_MOUTH).draw();
        float yawnFactor = shape.second.getMeasurement(ofx::Dlib::FaceShape::Measurement::YAWN_FACTOR);
        std::stringstream ss;
        ss << "Yawn Factor: " << ofToString(yawnFactor, 2) << std::endl;
        ss << "Yawning? " << (yawnFactor > 10 ? "YES" : "NO");
        ofDrawBitmapStringHighlight(ss.str(), shape.second.landmarks()[54] + glm::vec2(30, 0));
    }

    // Play the collected yawns.
    for (auto& yawn: yawns)
    {
        yawn.draw();
    }

    ofDrawBitmapStringHighlight("    App: " + ofToString(ofGetFrameRate(), 2) + " FPS", 20, 20);
    ofDrawBitmapStringHighlight("Tracker: " + ofToString(faceTracker.fps(), 2) + " FPS", 20, 40);
}


void ofApp::onTrackBegin(ofxDlib::FaceTrackerEventArgs& evt)
{
    // We send track begin events to the update because the are the same to us.
    onTrackUpdate(evt);
}

void ofApp::onTrackUpdate(ofxDlib::FaceTrackerEventArgs& evt)
{
    auto iter = trackedYawns.find(evt.label);
    if (iter == trackedYawns.end())
    {
        bool inserted;
        std::tie(iter, inserted) = trackedYawns.insert({ evt.label, Yawn() });
    }

    if (!iter->second.tryAddFaceShape(evt.faceShape))
    {
        if (iter->second.isLongEnough())
        {
            // Add our completed yawn.
            yawns.push_back(iter->second);

            // Make sure we don't have too many yawns.
            while (yawns.size() > 10)
            {
                yawns.erase(yawns.begin());
            }
        }

       trackedYawns.erase(iter);
    }
}

void ofApp::onTrackEnd(ofxDlib::FaceTrackerEventArgs& evt)
{
    // We don't save unfinished yawns.
    auto iter = trackedYawns.find(evt.label);
    if (iter != trackedYawns.end())
    {
        trackedYawns.erase(iter);
    }
}
