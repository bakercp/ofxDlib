//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/FaceTracker.h"
#include "ofAppRunner.h"
#include "ofEvents.h"
#include "ofEventUtils.h"
#include "ofx/Dlib/Utils.h"


namespace ofx {
namespace Dlib {


FaceTrackerEventArgs::FaceTrackerEventArgs(const TrackerEventArgs& trackerEventArgs,
                                           const FaceShape& faceShape_,
                                           const std::vector<float>& faceCode_):
    TrackerEventArgs(trackerEventArgs),
    faceShape(faceShape_),
    faceCode(faceCode_)
{
}


FaceTracker::FaceTracker()
{
}


FaceTracker::FaceTracker(const Settings& settings)
{
    setup(settings);
}


bool FaceTracker::setup(const Settings& settings)
{
#if !defined(__OPTIMIZE__)
    ofLogWarning("FaceTracker::setup") << "FaceTracker runs much faster when compiled with optimizations (i.e. RELEASE Mode). http://dlib.net/faq.html#Whyisdlibslow";
#endif

    _frameId = 1;
    _isLoaded = false;
    _asyncProcess.reset();
    _syncUpdateListener.unsubscribe();
    _tracks.clear();
    _shapes.clear();
    _tracker = Tracker_<ObjectDetection>();
    _faceShapePredictor = FaceShapePredictor();
    _faceCoder = FaceCoder();

    _boundingBoxFilter.clear();
    _faceLandmarkFilter.clear();

    if (_detector.setup(settings.faceDetectorSettings)
    && _tracker.setup(settings.trackerSettings)
    && _faceShapePredictor.setup(settings.faceShapePredictorSettings)
    && _faceCoder.setup(settings.faceCoderSettings))
    {
        _boundingBoxFilter.setAlpha(settings.faceDetectorFilterSmoothness);
        _faceLandmarkFilter.setAlpha(settings.faceShapeFilterSmoothness);

        if (settings.async)
        {
            _asyncProcess = std::make_unique<AsyncProcess>([this](std::size_t frameId, const InputType& input, OutputType& output) { return _processInput(frameId, input, output); },
                                                           [this](std::size_t frameId, const OutputType& output) { _processOutput(frameId, output); }
                                                           );
        }
        else
        {
            _syncUpdateListener = ofEvents().update.newListener(this, &FaceTracker::_syncUpdate, OF_EVENT_ORDER_AFTER_APP);
        }

        _settings = settings;
        _isLoaded = true;
    }

    return _isLoaded;
}


bool FaceTracker::isLoaded() const
{
    return _isLoaded;
}


std::size_t FaceTracker::track(const ofBaseHasPixels& frame)
{
    return track(frame.getPixels());
}


std::size_t FaceTracker::track(const ofPixels& frame)
{
    if (isLoaded())
    {
        // Increment the frameId.
        _frameId++;

        if (!_settings.async)
        {
            _processInput(_frameId, dlib::mat(frame), _syncOutput);
            _hasSyncOutput = true;
            _fpsCounter.newFrame();
            return _frameId;
        }
        else if (_asyncProcess->tryProcess(_frameId, dlib::mat(frame)))
        {
            _fpsCounter.newFrame();
            return _frameId;
        }

        // Failed, so decrement.
        _frameId--;
    }

    return 0;
}



double FaceTracker::fps() const
{
    return _fpsCounter.getFps();
}


const std::unordered_map<std::size_t, ObjectDetection>& FaceTracker::tracks() const
{
    return _tracks;
}


const std::unordered_map<std::size_t, FaceShape>& FaceTracker::shapes() const
{
    return _shapes;
}


FaceTracker::Settings FaceTracker::settings() const
{
    return _settings;
}


bool FaceTracker::_processInput(std::size_t frameId,
                                const InputType& input,
                                OutputType& output)
{
    _tracker.track(_detector.detect(input));

    std::vector<TrackerEventArgs> trackerEvents;

    auto saveEvents = [&](const Tracker_<ObjectDetection>::Labels& labels,
                          TrackerEventArgs::State state)
    {
        for (std::size_t label: labels)
        {
            TrackerEventArgs evt;
            evt.frameId = frameId;
            evt.state = state;
            evt.label = label;

            if (_tracker.existsCurrent(label))
            {
                evt.detection = _tracker.getCurrent(label);
                evt.age = _tracker.getAge(label);
                evt.lastSeen = _tracker.getLastSeen(label);
            }

            trackerEvents.push_back(evt);
        }
    };

    saveEvents(_tracker.deadLabels(), TrackerEventArgs::State::TRACK_END);
    saveEvents(_tracker.newLabels(), TrackerEventArgs::State::TRACK_BEGIN);
    saveEvents(_tracker.currentLabels(), TrackerEventArgs::State::TRACK_UPDATE);

    // Relabel dead tracker events and filter if needed.
    for (auto& event: trackerEvents)
    {
        // Adjust our events to turn dead labels in to updates if they are lost
        // but not forgotten.
        if (event.state == TrackerEventArgs::State::TRACK_END)
        {
            if (event.age != 0 || event.lastSeen != 0)
            {
                event.state = TrackerEventArgs::State::TRACK_UPDATE;
            }
        }

        // Filter any bounding boxes if needed.
        if (_settings.faceDetectorFilterSmoothness > 0)
        {
            std::vector<glm::vec2> filteredBoundingBox =
            {
                event.detection.rectangle.getMin(),
                event.detection.rectangle.getMax()
            };

            switch(event.state)
            {
                case TrackerEventArgs::State::TRACK_BEGIN:
                case TrackerEventArgs::State::TRACK_UPDATE:
                {
                    filteredBoundingBox = _boundingBoxFilter.filter(event.label, filteredBoundingBox);
                    break;
                }
                case TrackerEventArgs::State::TRACK_END:
                {
                    _boundingBoxFilter.end(event.label);
                    break;
                }
                case TrackerEventArgs::State::TRACK_NONE:
                    break;
            }

            event.detection.rectangle = ofRectangle(filteredBoundingBox[0], filteredBoundingBox[1]);
        }
    }

    // Face shape analysis.
    // calculate optical flow.

    output.clear();
    output.reserve(trackerEvents.size());

    for (auto& event: trackerEvents)
    {
        FaceShape shape = _faceShapePredictor.predict(input,
                                                      event.detection.rectangle);
        std::vector<float> faceCode;
        
        if (_settings.faceCoderEnabled)
        {
            faceCode = _faceCoder.code(shape.alignedFace());
        }

        // Do filtering if possible.
        if (_settings.faceShapeFilterSmoothness > 0)
        {
            std::vector<glm::vec2> filteredLandmarks = shape.landmarks();

            switch(event.state)
            {
                case TrackerEventArgs::State::TRACK_BEGIN:
                case TrackerEventArgs::State::TRACK_UPDATE:
                {
                    filteredLandmarks = _faceLandmarkFilter.filter(event.label, filteredLandmarks);
                    break;
                }
                case TrackerEventArgs::State::TRACK_END:
                {
                    _faceLandmarkFilter.end(event.label);
                    break;
                }
                case TrackerEventArgs::State::TRACK_NONE:
                    break;
            }

            output.push_back(FaceTrackerEventArgs(event,
                                                  FaceShape(shape,
                                                            filteredLandmarks),
                                                  faceCode));
        }
        else
        {
            output.push_back(FaceTrackerEventArgs(event, shape, faceCode));
        }
    }

    return true;
}


void FaceTracker::_processOutput(std::size_t frameId, const OutputType& output)
{
    _tracks.clear();
    _shapes.clear();

    OutputType events = output;

    for (auto&& event: events)
    {
        ofNotifyEvent(trackEvent, event, this);

        switch(event.state)
        {
            case TrackerEventArgs::State::TRACK_BEGIN:
            {
                _tracks[event.label] = event.detection;
                _shapes[event.label] = event.faceShape;
                ofNotifyEvent(trackBegin, event, this);
                break;
            }
            case TrackerEventArgs::State::TRACK_UPDATE:
            {
                _tracks[event.label] = event.detection;
                _shapes[event.label] = event.faceShape;
                ofNotifyEvent(trackUpdate, event, this);
                break;
            }
            case TrackerEventArgs::State::TRACK_END:
            {
                ofNotifyEvent(trackEnd, event, this);
                break;
            }
            case TrackerEventArgs::State::TRACK_NONE:
                break;
        }
    }
}


void FaceTracker::_syncUpdate(ofEventArgs&)
{
    if (_hasSyncOutput)
    {
        _processOutput(_frameId, _syncOutput);
        _hasSyncOutput = false;
    }
}


} } // namespace ofx::Dlib
