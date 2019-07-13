//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/TrackerEventFilter.h"


namespace ofx {
namespace Dlib {


//TrackerEventFilter::TrackerEventFilter()
//{
//}
//
//
//TrackerEventFilter::TrackerEventFilter(const Settings& settings)
//{
//    setup(settings);
//}
//
//    
//bool TrackerEventFilter::setup(const Settings& settings)
//{
//    _isLoaded = false;
//    _lowPassSmoothed.clear();
//
//    _kalmanRectangleHistory.clear();
//    _kalmanFilters.clear();
//
//    _settings = settings;
//    _isLoaded = true;
//    return isLoaded();
//}
//
//
//bool TrackerEventFilter::isLoaded() const
//{
//    return _isLoaded;
//}
//
//
//bool TrackerEventFilter::filter(TrackerEventArgs& event)
//{
//    switch(event.state)
//    {
//        case TrackerEventArgs::State::TRACK_BEGIN:
//        {
//            if (_settings.type == Type::TYPE_KALMAN)
//            {
//                // No warnings here, just remove any leftovers.
//                auto filtersIter = _kalmanFilters.find(event.label);
//                if (filtersIter != _kalmanFilters.end())
//                    _kalmanFilters.erase(filtersIter);
//
//                auto rectangleHistoryIter = _kalmanRectangleHistory.find(event.label);
//                if (rectangleHistoryIter != _kalmanRectangleHistory.end())
//                    _kalmanRectangleHistory.erase(rectangleHistoryIter);
//
//                _kalmanRectangleHistory[event.label] = { toDlib(event.detection.rectangle) };
//            }
//            else if (_settings.type == Type::TYPE_LOW_PASS)
//            {
//                _lowPassSmoothed[event.label] = { event.detection.rectangle.getCenter(), { event.detection.rectangle.getWidth(), event.detection.rectangle.getHeight() } };
//            }
//
//            break;
//        }
//        case TrackerEventArgs::State::TRACK_UPDATE:
//        {
//            if (_settings.type == Type::TYPE_KALMAN)
//            {
//                auto filtersIter = _kalmanFilters.find(event.label);
//                if (filtersIter != _kalmanFilters.end())
//                {
//                    // Set the filtered rectangle.
//                    event.detection.filteredRectangle = toOf(filtersIter->second(toDlib(event.detection.rectangle)));
//                    event.detection.hasFiltered = true;
//                }
//                else
//                {
//                    std::size_t historyRequired = std::max(_settings.kalmanRequiredHistory, std::size_t(4));
//
//                    auto rectangleHistoryIter = _kalmanRectangleHistory.find(event.label);
//
//                    if (rectangleHistoryIter->second.size() < historyRequired)
//                    {
//                        rectangleHistoryIter->second.push_back(toDlib(event.detection.rectangle));
//                    }
//
//                    if (rectangleHistoryIter->second.size() == historyRequired)
//                    {
//                        _kalmanFilters[event.label] = dlib::find_optimal_rect_filter(rectangleHistoryIter->second, _settings.kalmanSmoothness);
//                        event.detection.filteredRectangle = toOf(_kalmanFilters[event.label](toDlib(event.detection.rectangle)));
//                        event.detection.hasFiltered = true;
//                    }
//                }
//            }
//            else if (_settings.type == Type::TYPE_LOW_PASS)
//            {
//                std::pair<glm::vec2, glm::vec2> unfilteredRectangle =
//                {
//                    event.detection.rectangle.getCenter(),
//                    {
//                        event.detection.rectangle.getWidth(),
//                        event.detection.rectangle.getHeight()
//                    }
//                };
//
//                std::pair<glm::vec2, glm::vec2> smoothRectangle = _lowPassSmoothed[event.label];
//
//                smoothRectangle.first = _settings.lowPassAlpha * smoothRectangle.first + (1 - _settings.lowPassAlpha) * unfilteredRectangle.first;
//                smoothRectangle.second = _settings.lowPassAlpha * smoothRectangle.second + (1 - _settings.lowPassAlpha) * unfilteredRectangle.second;
//
//                event.detection.filteredRectangle.setFromCenter(smoothRectangle.first, smoothRectangle.second.x, smoothRectangle.second.y);
//                event.detection.hasFiltered = true;
//
//                _lowPassSmoothed[event.label] = smoothRectangle;
//            }
//
//            break;
//        }
//        case TrackerEventArgs::State::TRACK_END:
//        {
//            if (_settings.type == Type::TYPE_KALMAN)
//            {
//                auto iter = _kalmanFilters.find(event.label);
//                if (iter != _kalmanFilters.end())
//                    _kalmanFilters.erase(iter);
//            }
//            else if (_settings.type == Type::TYPE_LOW_PASS)
//            {
//                auto iter = _lowPassSmoothed.find(event.label);
//                if (iter != _lowPassSmoothed.end())
//                    _lowPassSmoothed.erase(iter);
//            }
//
//            break;
//        }
//        case TrackerEventArgs::State::TRACK_NONE:
//        {
//            break;
//        }
//    }
//
//    return event.detection.hasFiltered;
//}


} } // namespace ofx::Dlib
