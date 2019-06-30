//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <dlib/pipe.h>
#include <dlib/threads.h>
#include "ofEvents.h"
#include "ofx/Dlib/Tracker.h"
#include "ofx/Dlib/FaceDetector.h"


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


class FaceTrackerEventArgs
{
public:
    enum class State
    {
        TRACK_BEGIN,
        TRACK_UPDATE,
        TRACK_END,
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

    /// \brief The object detection associated with this event, if any.
    ///
    /// This will only be valid for state == TRACK_BEGIN and
    /// state == TRACK_UPDATE.
    ObjectDetection detection;

    /// \brief The previous detection associated with this event, if any.
    ///
    /// This will only be valid for state == TRACK_UPDATE and
    /// state == TRACK_END.
    ObjectDetection previousDetection;

};


inline float distance(const ObjectDetection& _a, const ObjectDetection& _b)
{
    ofRectangle a = _a.rectangle;
    ofRectangle b = _b.rectangle;
    glm::vec2 dp = glm::vec2(a.getCenter()) - glm::vec2(b.getCenter());
    glm::vec2 ds = { a.width - b.width, a.height - b.height };
    return glm::length(dp) + glm::length(ds);
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

    /// \returns the current set of tracks.
    std::map<std::size_t, ObjectDetection> tracks() const;

    struct Settings: public FaceDetector::Settings
    {
        std::size_t trackerPersistence = 15;
        float trackerMaximumDistance = 64;
        // bool useThreaded = true;
        std::size_t threadInputMaxQueueSize = 1;
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

private:
    void _update(ofEventArgs&);

    /// \brief The update listener.
    ofEventListener _updateListener;

    /// \brief The current Settings.
    Settings _settings;

    /// \brief The last set of detections.
    std::map<std::size_t, ObjectDetection> _tracks;

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
