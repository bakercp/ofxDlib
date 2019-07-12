//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "ofEvents.h"
#include "ofx/Dlib/AsyncProcess.h"
#include "ofx/Dlib/FaceDetector.h"
#include "ofx/Dlib/ObjectDetection.h"
#include "ofx/Dlib/Tracker.h"


namespace ofx {
namespace Dlib {


/// \brief Event arguments that are sent with tracking events.
class FaceTrackerEventArgs
{
public:
    /// \brief The tracking states.
    enum class State
    {
        /// \brief The state when a new track begins.
        TRACK_BEGIN,
        /// \brief The state when a track is updated. This may be a stale track.
        TRACK_UPDATE,
        /// \brief The state when a track expires and is lost.
        TRACK_END,
        /// \brief The state when an event is not tracking.
        TRACK_NONE
    };

    /// \brief The tracker state.
    State state = State::TRACK_NONE;

    /// \brief The tracker label associated with this event.
    std::size_t label = static_cast<std::size_t>(-1);

    /// \brief The age of the track.
    uint64_t age = 0;

    /// \brief The time since last seen.
    uint64_t lastSeen = 0;

    /// \brief The Face associated with this event, if any.
    ///
    /// This will only be valid for state == TRACK_BEGIN and
    /// state == TRACK_UPDATE.
    ObjectDetection face;

    /// \brief The previous detection associated with this event, if any.
    ///
    /// This will only be valid for state == TRACK_UPDATE and
    /// state == TRACK_END.
    ObjectDetection previousFace;

};


inline float distance(const ObjectDetection& _a, const ObjectDetection& _b)
{
    // Calculate the spatial "distance" between the objects.
    ofRectangle a = _a.rectangle;
    ofRectangle b = _b.rectangle;
    glm::vec2 dp = glm::vec2(a.getCenter()) - glm::vec2(b.getCenter());
    glm::vec2 ds = { a.width - b.width, a.height - b.height };

    // Distance between the centers.
    float centerDistance = glm::length(dp);

    // Difference between the sizes.
    float sizeDistance = glm::length(ds);

    // The combined distance.
    return centerDistance + sizeDistance;
}


/// \brief The FaceTracker.
class FaceTracker
{
public:
    typedef dlib::matrix<dlib::rgb_pixel> InputType;
    typedef std::vector<FaceTrackerEventArgs> OutputType;
    typedef AsyncProcess_<InputType, OutputType> AsyncProcess;

    struct Settings;

    /// \brief Create a default FaceTracker.
    FaceTracker();

    /// \brief Create a FaceTracker with the given settings.
    /// \param settings The Settings to use.
    FaceTracker(const Settings& settings);

    /// \brief Set up the FaceTracker.
    ///
    /// Calling setup will reset the FaceTracker completely.
    ///
    /// \param settings The Settings to use.
    bool setup(const Settings& settings);

    /// \returns true if the FaceTracker has been set up successfully.
    bool isLoaded() const;

    /// \brief Update the tracker state.
    /// \param pixels The pixels to search.
    /// \todo Make accept dlib generic image types.
    template<typename image_type>
    void track(const image_type& image)
    {
        if (isLoaded())
        {
            if (!_settings.async)
            {
                OutputType output;
                if (_processInput(dlib::mat(image), output))
                {
                    _processOutput(output);
                    _fpsCounter.newFrame();
                }
            }
            else
            {
                dlib::matrix<dlib::rgb_pixel> _image = dlib::mat(image);
                _asyncProcess->tryProcess(_image);
            }
        }
    }

    /// \brief Get the process
    double fps() const;

    /// \returns the current tracks.
    std::map<std::size_t, ObjectDetection> tracks() const;

    /// \returns the current settings.
    Settings settings() const;

    struct Settings: public FaceDetector::Settings
    {
        /// \brief True if a seperate thread should be used for tracking.
        bool async = true;

        /// \brief How many frames to persist the tracked object.
        std::size_t trackerPersistence = 15;

        /// \brief The maximum distance that a tracked object can move.
        float trackerMaximumDistance = 200;
    };

    /// \brief Register tracker event listeners.
    ///
    /// Event listeners registered via this function must have the following
    /// ofEvent callbacks defined:
    ///
    ///     `void onTrackBegin(ofxDlib::FaceTrackerEventArgs& evt)`
    ///     `void onTrackUpdate(ofxDlib::FaceTrackerEventArgs& evt)`
    ///     `void onTrackEnd(ofxDlib::FaceTrackerEventArgs& evt)`
    ///
    /// Other method signatures event signatures are also supported.
    ///
    /// \tparam ListenerClass The class of the listener.
    /// \param listener A pointer to the listener class.
    /// \param prio The event priority.
    template <class ListenerClass>
    void registerEvents(ListenerClass* listener,
                        int prio = OF_EVENT_ORDER_AFTER_APP)
    {
        ofAddListener(trackBegin, listener, &ListenerClass::onTrackBegin, prio);
        ofAddListener(trackUpdate, listener, &ListenerClass::onTrackUpdate, prio);
        ofAddListener(trackEnd, listener, &ListenerClass::onTrackEnd, prio);
    }

    /// \brief Unregister tracker event listener.
    ///
    /// Event listeners unregistered via this function must have the following
    /// ofEvent callbacks defined:
    ///
    ///     `void onTrackBegin(ofxDlib::FaceTrackerEventArgs& evt)`
    ///     `void onTrackUpdate(ofxDlib::FaceTrackerEventArgs& evt)`
    ///     `void onTrackEnd(ofxDlib::FaceTrackerEventArgs& evt)`
    ///
    /// Other method signatures event signatures are also supported.
    ///
    /// \tparam ListenerClass The class of the listener.
    /// \param listener A pointer to the listener class.
    /// \param prio The event priority.
    template <class ListenerClass>
    void unregisterEvents(ListenerClass* listener,
                          int prio = OF_EVENT_ORDER_AFTER_APP)
    {
        ofRemoveListener(trackBegin, listener, &ListenerClass::onTrackBegin, prio);
        ofRemoveListener(trackUpdate, listener, &ListenerClass::onTrackUpdate, prio);
        ofRemoveListener(trackEnd, listener, &ListenerClass::onTrackEnd, prio);
    }

    /// \brief Called for all track events.
    ofEvent<FaceTrackerEventArgs> trackEvent;

    /// \brief Called for all TRACK_BEGIN events.
    ofEvent<FaceTrackerEventArgs> trackBegin;

    /// \brief Called for all TRACK_UPDATE events.
    ofEvent<FaceTrackerEventArgs> trackUpdate;

    /// \brief Called for all TRACK_END events.
    ofEvent<FaceTrackerEventArgs> trackEnd;

private:
    /// \brief The update listener.
    ofEventListener _updateListener;

    /// \brief The current Settings.
    Settings _settings;

    /// \brief True if the settings were loaded correctly.
    bool _isLoaded = false;

    /// \breif An FPS counter for sync processing.
    ofFpsCounter _fpsCounter;

    /// \brief The current tracks.
    std::map<std::size_t, ObjectDetection> _tracks;

    /// \brief The async processor.
    std::unique_ptr<AsyncProcess> _asyncProcess = nullptr;

    bool _processInput(const InputType& input, OutputType& output);
    void _processOutput(const OutputType& output);

    /// \brief The face detector.
    FaceDetector _detector;

    /// \brief The bounding box tracker.
    Tracker<ObjectDetection> _tracker;


};


} } // namespace ofx::Dlib
