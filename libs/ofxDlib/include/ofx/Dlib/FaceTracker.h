//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "dlib/of_default_adapter.h"
#include "dlib/of_image.h"
#include "ofEvents.h"
#include "ofThreadChannel.h"
#include "ofx/Dlib/AsyncProcess.h"
#include "ofx/Dlib/Tracker.h"
#include "ofx/Dlib/FaceDetector.h"
#include "ofx/Dlib/FaceShapePredictor.h"
#include "ofx/Dlib/FilterGroup.h"
#include "ofx/Dlib/ObjectDetection.h"
#include "ofx/Dlib/ObjectTracker.h"


namespace ofx {
namespace Dlib {


class FaceTrackerEventArgs: public TrackerEventArgs
{
public:
    FaceTrackerEventArgs(const TrackerEventArgs& trackerEventArgs,
                         const FaceShape& faceShape);

    FaceShape faceShape;

};


/// \brief The FaceTracker.
class FaceTracker
{
public:
    typedef std::size_t FrameToken;
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
    ///
    /// The track function returns a frame id that can be used to associate
    /// the resulting event data with the frame that was passed. This is
    /// particularly useful when using the FaceTracker in asynchronous mode.
    ///
    /// \param frame The frame to track.
    /// \returns a frame id that will be passed with all events. Zero returned on failure.
    std::size_t track(const ofBaseHasPixels& frame);

    /// \brief Update the tracker state.
    ///
    /// The track function returns a frame id that can be used to associate
    /// the resulting event data with the frame that was passed. This is
    /// particularly useful when using the FaceTracker in asynchronous mode.
    ///
    /// \param pixels The pixels to search.
    /// \returns a frame id that will be passed with all events. Zero returned on failure.
    std::size_t track(const ofPixels& frame);

    /// \brief Get the process
    double fps() const;

    /// \returns the current tracks.
    const std::unordered_map<std::size_t, ObjectDetection>& tracks() const;

    /// \returns the current tracks.
    const std::unordered_map<std::size_t, FaceShape>& shapes() const;

    /// \returns the current settings.
    Settings settings() const;

    struct Settings
    {
        /// \brief True if a seperate thread should be used for tracking.
        bool async = true;

        /// \brief The tracker settings.
        Tracker_<ObjectDetection>::Settings trackerSettings;

        /// \brief Face detector settings.
        FaceDetector::Settings faceDetectorSettings;

        /// \brief Face shape
        FaceShapePredictor::Settings faceShapePredictorSettings;

        /// \brief Face detector filter alpha.
        ///
        /// A simple low-pass filter can be applied to the corners of the face
        /// detection bounding box. The filter value should be in the range
        /// [0, 1). A value of 0 means no smoothing. A value close to 1 means
        /// greater smoothing but more latency.
        double faceDetectorFilterSmoothness = 0.0;

        /// \brief Face landmark filter alpha.
        ///
        /// A simple low-pass filter can be applied to all detected face
        /// landmarks. The filter value should be in the range [0, 1).
        /// A value of 0 means no smoothing. A value close to 1 means greater
        /// smoothing but more latency.
        double faceShapeFilterSmoothness = 0.0;

        /// \brief The duration of the frame history buffer in microseconds.
        ///
        /// Default value is 30 seconds.
        uint64_t frameBufferDuration = 30 * 1000000;

    };

    /// \brief Register tracker event listeners.
    ///
    /// Event listeners registered via this function must have the following
    /// ofEvent callbacks defined:
    ///
    ///     `void onTrackBegin(ofxDlib::FaceTrackerEventArgs& evt)`
    ///     `void onTrackUpdate(ofxDlib::FaceTrackerEventArgs& evt)`
    ///     `void onTrackEnd(ofxDlib::FaceTrackerEventArgs& evt)`
    ///     `void onTrackError(ofxDlib::FaceTrackerEventArgs& evt)`
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
        ofAddListener(trackError, listener, &ListenerClass::onTrackError, prio);
    }

    /// \brief Unregister tracker event listener.
    ///
    /// Event listeners unregistered via this function must have the following
    /// ofEvent callbacks defined:
    ///
    ///     `void onTrackBegin(ofxDlib::FaceTrackerEventArgs& evt)`
    ///     `void onTrackUpdate(ofxDlib::FaceTrackerEventArgs& evt)`
    ///     `void onTrackEnd(ofxDlib::FaceTrackerEventArgs& evt)`
    ///     `void onTrackError(ofxDlib::FaceTrackerEventArgs& evt)`
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
        ofRemoveListener(trackError, listener, &ListenerClass::onTrackError, prio);
    }

    /// \brief Called for all track events.
    ofEvent<FaceTrackerEventArgs> trackEvent;

    /// \brief Called for all TRACK_BEGIN events.
    ofEvent<FaceTrackerEventArgs> trackBegin;

    /// \brief Called for all TRACK_UPDATE events.
    ofEvent<FaceTrackerEventArgs> trackUpdate;

    /// \brief Called for all TRACK_END events.
    ofEvent<FaceTrackerEventArgs> trackEnd;

    /// \brief Called for all TRACK_ERROR events.
    ofEvent<FaceTrackerEventArgs> trackError;

private:
    /// \brief The update function called when async != true;
    /// We use this function to make sure that the frame ids are received
    /// by the caller before the track event callbacks are called.
    /// \todo Collapse this all into the AsyncProcess function.
    void _syncUpdate(ofEventArgs&);

    /// \brief The update listener.
    ofEventListener _syncUpdateListener;

    /// \brief The current Settings.
    Settings _settings;

    /// \brief True if the settings were loaded correctly.
    bool _isLoaded = false;

    /// \brief An FPS counter for sync processing.
    ofFpsCounter _fpsCounter;

    /// \brief The current tracks.
    std::unordered_map<std::size_t, ObjectDetection> _tracks;

    /// \brief The current face shapes.
    std::unordered_map<std::size_t, FaceShape> _shapes;

    /// \brief Bounding box filter.
    FilterGroup_<std::vector<glm::vec2>> _boundingBoxFilter;

    /// \brief A shape filter.
    FilterGroup_<std::vector<glm::vec2>> _faceLandmarkFilter;

    /// \brief The async processor.
    std::unique_ptr<AsyncProcess> _asyncProcess = nullptr;

    bool _processInput(std::size_t frameId, const InputType& input, OutputType& output);
    void _processOutput(std::size_t frameId, const OutputType& output);

    /// \brief The face detector.
    FaceDetector _detector;

    /// \brief The face shape predictor.
    FaceShapePredictor _faceShapePredictor;

    /// \brief The bounding box tracker.
    Tracker_<ObjectDetection> _tracker;

    std::size_t _frameId = 1;

    /// \brief True if _syncOutput needs processing.
    bool _hasSyncOutput = false;

    /// \brief The output used during sync tracking;
    OutputType _syncOutput;

};


} } // namespace ofx::Dlib
