//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/FaceTracker.h"
#include "ofEvents.h"
#include "ofEventUtils.h"


namespace ofx {
namespace Dlib {


FaceTracker::FaceTracker():
    _updateListener(ofEvents().update.newListener(this, &FaceTracker::_update))
{
    setup(Settings());
}


FaceTracker::FaceTracker(const Settings& settings)
{
    setup(settings);
}


bool FaceTracker::setup(const Settings& settings)
{
#if !defined(__OPTIMIZE__)
    ofLogWarning("FaceTracker::setup") << "FaceTracker runs much faster in RELEASE Mode. http://dlib.net/faq.html#Whyisdlibslow";
#endif

    _asyncTracker.reset();

    if (_detector.setup(settings))
    {
        // TODO: the internet is out and I can't find documentation on std::bind, so it's in here for a minute.
        auto process = [this](const dlib::matrix<dlib::rgb_pixel>& input,
                              std::vector<FaceTrackerEventArgs>& output) -> bool {
            return this->_process(input, output);
        };

        _asyncTracker = std::make_unique<AsyncTracker>(process,
                                                       settings.threadInputMaxQueueSize,
                                                       settings.threadOutputMaxQueueSize);
        _settings = settings;
    }

    return isLoaded();

}


bool FaceTracker::isLoaded() const
{
    return _asyncTracker != nullptr;
}


FaceTracker::Settings FaceTracker::settings() const
{
    return _settings;
}


std::map<std::size_t, ObjectDetection> FaceTracker::tracks() const
{
    return _tracks;
}


void FaceTracker::_update(ofEventArgs&)
{
    if (isLoaded())
    {
        std::vector<FaceTrackerEventArgs> events;
        if (_asyncTracker->outputPipe.dequeue_or_timeout(events, 0))
        {
            _tracks.clear();

            for (auto&& event: events)
            {
                ofNotifyEvent(trackEvent, event, this);

                switch(event.state)
                {
                    case FaceTrackerEventArgs::State::TRACK_BEGIN:
                        ofNotifyEvent(trackBegin, event, this);
                        _tracks[event.label] = event.detection;
                        break;
                    case FaceTrackerEventArgs::State::TRACK_UPDATE:
                        ofNotifyEvent(trackUpdate, event, this);
                        _tracks[event.label] = event.detection;
                        break;
                    case FaceTrackerEventArgs::State::TRACK_END:
                        ofNotifyEvent(trackEnd, event, this);
                        break;
                    case FaceTrackerEventArgs::State::TRACK_NONE:
                        break;
                }
            }
        }
    }
}


bool FaceTracker::_process(const dlib::matrix<dlib::rgb_pixel>& input,
                           std::vector<FaceTrackerEventArgs>& output)
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
                evt.detection = _tracker.getCurrent(label);
                evt.age = _tracker.getAge(label);
                evt.lastSeen = _tracker.getLastSeen(label);
            }
            if (_tracker.existsPrevious(label))
               evt.previousDetection = _tracker.getPrevious(label);

            output.push_back(evt);
        }
    };

    saveEvents(_tracker.deadLabels(), FaceTrackerEventArgs::State::TRACK_END);
    saveEvents(_tracker.newLabels(), FaceTrackerEventArgs::State::TRACK_BEGIN);
    saveEvents(_tracker.currentLabels(), FaceTrackerEventArgs::State::TRACK_UPDATE);

    return true;
}


} } // namespace ofx::Dlib
