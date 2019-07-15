//
// Copyright (c) 2012 Kyle McDonald <https://github.com/kylemcdonald/ofxCv/blob/master/libs/ofxCv/include/ofxCv/Tracker.h>
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//

#pragma once


//#include <algorithm>
//#include <cstdint>
//#include <limits>
//#include <unordered_map>
//#include <vector>
//#include "ofMath.h"
//#include "ofRectangle.h"
//#include "ofVectorMath.h"
#include "ofx/Dlib/ObjectDetection.h"


namespace ofx {
namespace Dlib {


/// \brief Event arguments that are sent with tracking events.
class TrackerEventArgs
{
public:
    /// \brief The tracking states.
    enum class State
    {
        /// \brief The state when a new track begins.
        TRACK_BEGIN,
        /// \brief The state when a track is updated. This may be a stale track.
        TRACK_UPDATE,
        /// \brief The state when a track expires, has an error or is otherwise lost.
        TRACK_END,
        /// \brief The state when an event is not tracking.
        TRACK_NONE
    };

    /// \brief The tracker state.
    State state = State::TRACK_NONE;

    /// \brief The frame id used to synchronize frames.
    std::size_t frameId = 0;

    /// \brief The tracker label associated with this event.
    std::size_t label = static_cast<std::size_t>(-1);

    /// \brief The age of the track.
    uint64_t age = 0;

    /// \brief The time since last seen.
    uint64_t lastSeen = 0;

    /// \brief The detection associated with this event, if any.
    ///
    /// This will only be valid for state == TRACK_BEGIN and
    /// state == TRACK_UPDATE.
    ObjectDetection detection;

};


} } // namespace ofx::Dlib
