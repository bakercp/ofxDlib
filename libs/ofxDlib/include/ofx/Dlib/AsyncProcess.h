//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "dlib/pipe.h"
#include "dlib/threads.h"
//#include "ofThreadChannel.h"


namespace ofx {
namespace Dlib {


/// \brief An AsyncProcess_.
///
/// This takes inputs and proceses outputs with a thread. It can only handle
/// one input at a time.
///
/// \note I'm sure this was directly influenced by the StoryBots song.
template <typename InputType, typename OutputType>
class AsyncProcess_: public dlib::threaded_object
{
public:
    /// \brief Create an Async Process.
    ///
    /// \param procesInput The input processor.
    /// \param processOutput The output processor.
    AsyncProcess_(std::function<bool(const InputType& input, OutputType& output)> processInput,
                  std::function<void(const OutputType& output)> processOutput):
        _processInput(processInput),
        _processOutput(processOutput),
        _updateListener(ofEvents().update.newListener(this, &AsyncProcess_::_update,
                                                      OF_EVENT_ORDER_AFTER_APP)), // Use OF_EVENT_ORDER_AFTER_APP to potentially reduce latency.
        _exitListener(ofEvents().exit.newListener(this, &AsyncProcess_::_exit)),
        _inputPipe(1),
        _outputPipe(1)
    {
        start();
    }

    /// \brief Destroy the async process, stopping and waiting for the thread.
    ~AsyncProcess_() override
    {
        _inputPipe.disable();
        _outputPipe.disable();
        wait();
    }

    /// \brief Try to submit intput for processing.
    /// \param input The intput the process.
    /// \returns true if the input was accepted. False if the input was dropped.
    bool tryProcess(InputType& input)
    {
        if (!_inputPipe.enqueue_or_timeout(input, 0))
            return false;
        return true;
    }

    /// \returns the fps measured by delivered output events.
    double fps() const
    {
        return _fpsCounter.getFps();
    }

private:
    /// \brief The input pipe.
    dlib::pipe<InputType> _inputPipe;

    /// \brief The output channel.
    dlib::pipe<OutputType> _outputPipe;

    /// \brief The update callback.
    ///
    /// This will call processOuput from the main thread.
    void _update(ofEventArgs&)
    {
        OutputType output;
        while (_outputPipe.dequeue_or_timeout(output, 0))
        {
            _processOutput(output);
            _fpsCounter.newFrame();
        }
    }

    /// \brief The exit callback.
    void _exit(ofEventArgs&)
    {
        _inputPipe.disable();
        _outputPipe.disable();
        wait();
    }

    /// \brief The update listener.
    ofEventListener _updateListener;

    /// \brief The exit listener.
    ofEventListener _exitListener;

    /// \brief an FPS counter.
    ofFpsCounter _fpsCounter;

    /// \brief The input process function.
    std::function<bool(const InputType& input, OutputType& output)> _processInput;

    /// \brief The output process function.
    std::function<void(const OutputType& output)> _processOutput;

    void thread() override
    {
        InputType lastInput;
        OutputType lastOutput;

        while (_inputPipe.dequeue(lastInput))
        {
            if (_processInput(lastInput, lastOutput))
                _outputPipe.enqueue(lastOutput);
        }
    }

};


} } // namespace ofx::Dlib
