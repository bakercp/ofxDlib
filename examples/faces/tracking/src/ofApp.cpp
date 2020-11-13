//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Set up the detector settings.
    ofxDlib::FaceTracker::Settings settings;

    // By default the detector uses the standard dlib HOG detector.
    // It can also use the more accurate (and computationally expensive)
    // MMOD detector. The MMOD detector benefits greatly from CUDA hardware
    // acceleration.
    //
    // Find a useful a discussion of the various methods here:
    // https://www.learnopencv.com/face-detection-opencv-dlib-and-deep-learning-c-python/
    //
    // settings.faceDetectorSettings.detectorType = ofxDlib::FaceDetector::Type::FACE_DETECTOR_MMOD;

    // If you are using the MMOD detector and have multiple GPUS, you can select
    // the gpu device to use. See also http://dlib.net/dlib/dnn/cuda_dlib.h.html
    //
    // settings.faceDetectorSettings.gpuDevice = 0;

    // We can scale the image DOWN to increase detection speed.
    // But this makes smaller faces more difficult to detect.
    //
    settings.faceDetectorSettings.inputScale = 0.5;

    // We can scale the image UP to detect smaller faces. But this may result
    // in slower detection speeds.
    //
    // settings.faceDetectorSettings.inputScale = 1.5;

    // By default, scaling interpolation is done using a
    // OF_INTERPOLATE_NEAREST_NEIGHBOR, which is very fast. In some cases
    // it might be useful to use more complex interpolation, like
    // OF_INTERPOLATE_BILINEAR. This will be slower.
    //
    // settings.faceDetectorSettings.inputScaleInterpolation = OF_INTERPOLATE_BILINEAR;

    // If you know what region of the image will contain faces, you can set
    // region of interest (ROI). This will increase the speed by reducing the
    // number of pixels that need to be processed.
    //
    settings.faceDetectorSettings.inputROI = ofRectangle(200, 200, 800, 400);

    // For low resolution or noisy cameras, the face detection and shape
    // prediction can be pretty jittery. To avoid this jitter, use a higher
    // resolution camera with more light or try these smoothing factors.
    // They are simple low-pass filters. Values should be set in the range
    // [0, 1). Values close to zero yield minimal smoothing and lower latency.
    // Values closer to 1 yeild maximal smoothing and larger latency.
    //
    // The smoothing factor for the detected face bounding boxes.
    //
    // settings.faceDetectorFilterSmoothness = 0.75;
    //
    // The smoothing factor for the face shape landmarks.
    //
    // settings.faceShapeFilterSmoothness = 0.75;

    // By default, the tracker runs asynchronously in a background thread. It
    // emits tracker events from the main thread. If you want the tracking to
    // also run in the main thread, set this value to false.
    //
    // settings.async = false;

    // Set up the detector.
    tracker.setup(settings);

    // Add tracking event listeners.
    tracker.registerEvents(this);

    // Set up the video input.
    video.setDeviceID(2);
    video.setup(1280, 720);

}


void ofApp::update()
{
    video.update();

    if (video.isFrameNew())
    {
        tracker.track(video);
    }
}


void ofApp::draw()
{
    // Draw the input video.
    ofSetColor(ofColor::white);
    video.draw(0, 0);

    // Draw the ROI.
    ofSetColor(ofColor::white, 127);
    ofxDlib::draw(tracker.settings().faceDetectorSettings.inputROI, "ROI");

    // Draw the tracks.
    ofSetColor(ofColor::yellow);
    ofxDlib::draw(tracker.tracks());

    // Draw the shapes.
    ofSetColor(ofColor::green);
    ofxDlib::draw(tracker.shapes());

    ofDrawBitmapStringHighlight("    App: " + ofToString(ofGetFrameRate(), 2) + " FPS", 20, 20);
    ofDrawBitmapStringHighlight("Tracker: " + ofToString(tracker.fps(), 2) + " FPS", 20, 40);
}


void ofApp::onTrackBegin(ofxDlib::FaceTrackerEventArgs& evt)
{
    std::cout << "Track begin: ID:" << evt.label << std::endl;
}


void ofApp::onTrackUpdate(ofxDlib::FaceTrackerEventArgs& evt)
{
    if (evt.lastSeen != 0)
        std::cout << "Track Update (Lost): ID: " << evt.label << " Age: " << evt.age << " Last Seen: " << evt.lastSeen << std::endl;
    else
        std::cout << "Track Update: ID: " << evt.label << " Age: " << evt.age << std::endl;
}


void ofApp::onTrackEnd(ofxDlib::FaceTrackerEventArgs& evt)
{
    std::cout << "Track end: " << evt.label << std::endl;
}
