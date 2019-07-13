//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


//#include <algorithm>
#include <unordered_map>


namespace ofx {
namespace Dlib {


/// \brief
template<typename DataType>
class FilterGroup_
{
public:
    /// \brief Filter the values for the given id.
    /// \param id The id of the values to filter.
    /// \param values The latest raw values to filter.
    /// \returns the filtered values for the given id.
    DataType filter(std::size_t id, const DataType& values)
    {
        auto smoothedIter = _smoothed.find(id);

        if (smoothedIter != _smoothed.end())
        {
            assert(values.size() == smoothedIter->second.size());

            double one_minus_alpha = 1.0 - _alpha;

            for (std::size_t i = 0; i < values.size(); ++i)
                smoothedIter->second[i] = _alpha * smoothedIter->second[i] + one_minus_alpha * values[i];

            return smoothedIter->second;
        }

        _smoothed[id] = values;
        return values;
    }

    /// \brief End smoothing calculations for id.
    /// \param id The id of the data to stop smoothing.
    void end(std::size_t id)
    {
        auto smoothedIter = _smoothed.find(id);

        if (smoothedIter != _smoothed.end())
            _smoothed.erase(smoothedIter);
    }

    /// \brief Set the alpha value.
    ///
    /// The value is clamped to [0, 1].
    ///
    /// \param alpha The smoothing factor.
    void setAlpha(double alpha)
    {
        _alpha = std::max(0.0, std::min(alpha, 1.0));
    }

    /// \returns the alpha value.
    double getAlpha() const
    {
        return _alpha;
    }

    /// \brief Clear all smoothed values.
    void clear()
    {
        _smoothed.clear();
    }

private:
    double _alpha = 0.9;

    std::unordered_map<std::size_t, DataType> _smoothed;

};


} } // namespace ofx::Dlib
