//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "dlib/pipe.h"
#include "dlib/threads.h"
#include "ofEvents.h"
#include "ofx/Dlib/FaceDetector.h"
#include "ofx/Dlib/ObjectDetection.h"
#include "ofx/Dlib/Tracker.h"


namespace ofx {
namespace Dlib {



/// \brief An AsyncProcess.
/// \note I'm sure this was directly influenced by the StoryBots song.
template <typename InputType, typename OutputType>
class AsyncProcess: public dlib::threaded_object
{
public:
    /// \brief Create an async process.
    ///
    /// To bind the finction, call
    ///     std::bind(&TheClass::function, pointerToInstanceOfTheClass)
    ///
    /// \param processFunction The threaded function.
    /// \param inputPipeMaxSize The maximum size of the input pipe.
    /// \param outputPipeMaxSize The maximum size of the output pipe.
    AsyncProcess(std::function<bool(const InputType& input, OutputType& output)> processFunction,
                 std::size_t inputPipeMaxSize,
                 std::size_t outputPipeMaxSize):
        _exitListener(ofEvents().exit.newListener(this, &AsyncProcess::_exit)),
        inputPipe(inputPipeMaxSize),
        outputPipe(outputPipeMaxSize),
        _processFunction(processFunction)
    {
        start();
    }

    /// \brief Destroy the async process, stopping and waiting for the thread.
    ~AsyncProcess() override
    {
        // We do control this.
        // inputPipe.wait_until_empty();

        // We don't control this.
        // outputPipe.wait_until_empty();

        // This causes inputPipe.dequeue() to unblock and return false.
        inputPipe.disable();
        // outputPipe.disable();

        // It is not required to call stop() as calling
        // inputPipe.disable() will have the same effect.

        // stop();
        wait();
    }

    /// \brief The input pipe.
    dlib::pipe<InputType> inputPipe;

    /// \brief The output pipe.
    dlib::pipe<OutputType> outputPipe;

    /// \brief Sets dopped input and output counts to zero.
    void resetCounts()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _droppedInputsCount = 0;
        _droppedOutputsCount = 0;
    }

    /// \returns the number of inputs that were not processed.
    std::size_t droppedInputsCount() const
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _droppedInputsCount;
    }

    /// \returns the number of outputs that were not recevied.
    std::size_t droppedOutputsCount() const
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _droppedOutputsCount;
    }

private:
    /// \brief The exit callback.
    void _exit(ofEventArgs&)
    {
        inputPipe.disable();
        wait();
    }

    /// \brief The exit listener.
    ofEventListener _exitListener;

    /// \brief Mutext to protect counts.
    mutable std::mutex _mutex;

    /// \brief The number of inputs that were dropped.
    std::size_t _droppedInputsCount = 0;

    /// \brief The number of outputs that were dropped.
    std::size_t _droppedOutputsCount = 0;

    /// \brief The process function.
    std::function<bool(const InputType& input, OutputType& output)> _processFunction;

    void thread() override
    {
        InputType lastInput;
        OutputType lastOutput;
        OutputType missedOutput;

        while (inputPipe.dequeue(lastInput))
        {
            // We need to get to the latest frame, so consume it and continue.
            if (inputPipe.size() != 0)
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _droppedInputsCount++;
                continue;
            }

            if (_processFunction(lastInput, lastOutput))
            {
                // Make space in the output pipe if needed.
                if (outputPipe.size() == outputPipe.max_size())
                {
                    outputPipe.dequeue(missedOutput);
                    std::unique_lock<std::mutex> lock(_mutex);
                    _droppedOutputsCount++;
                }

                outputPipe.enqueue(lastOutput);
            }
        }
    }

};


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
    typedef AsyncProcess<dlib::matrix<dlib::rgb_pixel>, std::vector<FaceTrackerEventArgs>> AsyncTracker;

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
            _asyncTracker->inputPipe.enqueue_or_timeout(dlib::mat(image), 0);
        }
    }

    /// \returns the current settings.
    Settings settings() const;

    /// \returns the tracker.
    const Tracker<ObjectDetection>& tracker() const;

    struct Settings: public FaceDetector::Settings
    {
        /// \brief How many frames to persist the tracked object.
        std::size_t trackerPersistence = 15;

        /// \brief The maximum distance that a tracked object can move.
        float trackerMaximumDistance = 200;

        /// \brief Thread input max queue size.
        std::size_t threadInputMaxQueueSize = 1;

        /// \brief Thread output max queue size.
        std::size_t threadOutputMaxQueueSize = 2048;
    };

    /// \brief Called for all track events.
    ofEvent<FaceTrackerEventArgs> trackEvent;

    /// \brief Called for all TRACK_BEGIN events.
    ofEvent<FaceTrackerEventArgs> trackBegin;

    /// \brief Called for all TRACK_UPDATE events.
    ofEvent<FaceTrackerEventArgs> trackUpdate;

    /// \brief Called for all TRACK_END events.
    ofEvent<FaceTrackerEventArgs> trackEnd;

    const AsyncTracker* t()
    {
        return _asyncTracker.get();
    }

private:
    /// \brief The update callback.
    void _update(ofEventArgs&);

    /// \brief The update listener.
    ofEventListener _updateListener;

    /// \brief The current Settings.
    Settings _settings;

    // Threaded components below.

    std::unique_ptr<AsyncTracker> _asyncTracker = nullptr;

    bool _process(const dlib::matrix<dlib::rgb_pixel>& input,
                  std::vector<FaceTrackerEventArgs>& output);

    /// \brief The face detector.
    FaceDetector _detector;

    /// \brief The bounding box tracker.
    Tracker<ObjectDetection> _tracker;

};


} } // namespace ofx::Dlib
