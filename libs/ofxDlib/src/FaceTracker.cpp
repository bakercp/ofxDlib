//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/FaceTracker.h"
#include "ofAppRunner.h"
#include "ofEvents.h"
#include "ofEventUtils.h"


namespace ofx {
namespace Dlib {


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
    ofLogWarning("FaceTracker::setup") << "FaceDetector runs much faster when compiled with optimizations (i.e. RELEASE Mode). http://dlib.net/faq.html#Whyisdlibslow";
#endif

    _isLoaded = false;
    _asyncProcess.reset();
    _tracks.clear();
    _tracker = Tracker<ObjectDetection>();
    if (_detector.setup(settings))
    {
        _tracker.setMaximumDistance(settings.trackerMaximumDistance);
        _tracker.setPersistence(settings.trackerPersistence);

        if (settings.async)
        {
            _asyncProcess = std::make_unique<AsyncProcess>([this](const InputType& input, OutputType& output) { return _processInput(input, output); },
                                                           [this](const OutputType& output) { _processOutput(output); }
                                                           );
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


double FaceTracker::fps() const
{
    if (_settings.async)
        return _asyncProcess->fps();

    return _fpsCounter.getFps();
}


std::map<std::size_t, ObjectDetection> FaceTracker::tracks() const
{
    return _tracks;
}


FaceTracker::Settings FaceTracker::settings() const
{
    return _settings;
}


bool FaceTracker::_processInput(const InputType& input,
                                OutputType& output)
{
    _tracker.track(_detector.detect(input));

    auto saveEvents = [&](const Tracker<ObjectDetection>::Labels& labels,
                          FaceTrackerEventArgs::State state)
    {
        for (std::size_t label: labels)
        {
            FaceTrackerEventArgs evt;
            evt.state = state;
            evt.label = label;

            if (_tracker.existsCurrent(label))
            {
                evt.face = _tracker.getCurrent(label);
                evt.age = _tracker.getAge(label);
                evt.lastSeen = _tracker.getLastSeen(label);
            }
            if (_tracker.existsPrevious(label))
            {
               evt.previousFace = _tracker.getPrevious(label);
            }

            output.push_back(evt);
        }
    };

    saveEvents(_tracker.deadLabels(), FaceTrackerEventArgs::State::TRACK_END);
    saveEvents(_tracker.newLabels(), FaceTrackerEventArgs::State::TRACK_BEGIN);
    saveEvents(_tracker.currentLabels(), FaceTrackerEventArgs::State::TRACK_UPDATE);

    // Relabel dead tracker events.
    for (auto& event: output)
    {
        if (event.state == FaceTrackerEventArgs::State::TRACK_END)
        {
            if (event.age != 0 || event.lastSeen != 0)
            {
                event.state = FaceTrackerEventArgs::State::TRACK_UPDATE;
            }
        }
    }

    return true;
}

void FaceTracker::_processOutput(const OutputType& output)
{
    _tracks.clear();

    OutputType events = output;

    for (auto&& event: events)
    {
        ofNotifyEvent(trackEvent, event, this);

        switch(event.state)
        {
            case FaceTrackerEventArgs::State::TRACK_BEGIN:
                _tracks[event.label] = event.face;
                ofNotifyEvent(trackBegin, event, this);
                break;
            case FaceTrackerEventArgs::State::TRACK_UPDATE:
                _tracks[event.label] = event.face;
                ofNotifyEvent(trackUpdate, event, this);
                break;
            case FaceTrackerEventArgs::State::TRACK_END:
                ofNotifyEvent(trackEnd, event, this);
                break;
            case FaceTrackerEventArgs::State::TRACK_NONE:
                break;
        }
    }
}


} } // namespace ofx::Dlib
