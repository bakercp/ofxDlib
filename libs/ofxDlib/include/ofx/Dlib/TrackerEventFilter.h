//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "dlib/filtering/kalman_filter.h"
#include "ofx/Dlib/ObjectTracker.h"


namespace ofx {
namespace Dlib {


///// \brief A class for filtering object tracker events.
/////
///// This should stabilize tracking rectangles.
//class TrackerEventFilter
//{
//public:
//    struct Settings;
//
//    /// \brief The type of filter.
//    enum class Type
//    {
//        TYPE_KALMAN,
//        TYPE_LOW_PASS
//    };
//
//    /// \brief Create a default TrackerEventFilter.
//    TrackerEventFilter();
//
//    /// \brief Create a TrackerEventFilter with the given settings.
//    /// \param settings The Settings to use.
//    TrackerEventFilter(const Settings& settings);
//
//    /// \brief Set up the TrackerFilter.
//    ///
//    /// Calling setup will reset the TrackerFilter completely.
//    ///
//    /// \param settings The Settings to use.
//    bool setup(const Settings& settings);
//
//    /// \returns true if the TrackerFilter has been set up successfully.
//    bool isLoaded() const;
//
//    /// \brief This will claculated a filtered object detection rectangle.
//    /// \param event The event to filter.
//    /// \returns true if a filtered value was set.
//    bool filter(TrackerEventArgs& event);
//
//    struct Settings
//    {
//        /// \brief The filter type.
//        Type type = Type::TYPE_LOW_PASS;
//
//        /// \brief The low pass alpha mix value.
//        ///
//        /// smooth = lowPassAlpha * smooth + (1 - lowPassAlpha) * raw;
//        float lowPassAlpha = 0.75;
//
//        /// \brief The amount of history required before filtering begins.
//        ///
//        /// More history should produce better tracks.
//        ///
//        /// \sa http://dlib.net/dlib/filtering/kalman_filter_abstract.h.html
//        std::size_t kalmanRequiredHistory = 240;
//
//        /// \brief The filter smoothness.
//        ///
//        /// The larger the value of smoothness the less jittery the filter
//        /// outputs will be, but they might become biased or laggy if smoothness
//        /// is set really high.
//        float kalmanSmoothness = 1;
//
//    };
//
//private:
//    /// \brief The current Settings.
//    Settings _settings;
//
//    /// \brief True if the settings were loaded correctly.
//    bool _isLoaded = false;
//
//    /// \brief The low pass smoothed history.
//    std::unordered_map<std::size_t, std::pair<glm::vec2, glm::vec2>> _lowPassSmoothed;
//
//    /// \brief Rectangle filters.
//    ///
//    /// These are used to smooth out the detected faces rectangles.
//    std::unordered_map<std::size_t, dlib::rect_filter> _kalmanFilters;
//
//    /// \brief The rectangle history to find the optimal filter.
//    std::unordered_map<std::size_t, std::vector<dlib::rectangle>> _kalmanRectangleHistory;
//
//};


} } // namespace ofx::Dlib
