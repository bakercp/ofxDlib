//
// Copyright (c) 2012 Kyle McDonald <https://github.com/kylemcdonald/ofxCv/blob/master/libs/ofxCv/include/ofxCv/Tracker.h>
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//

#pragma once


#include <algorithm>
#include <cstdint>
#include <limits>
#include <map>
#include <vector>
#include "glm.hpp"
#include "ofMath.h"
#include "ofRectangle.h"


namespace ofx {
namespace Dlib {


inline glm::vec2 position(const ofRectangle& item)
{
    return item.getCenter();
}

inline ofRectangle lerp(const ofRectangle& a,
                 const ofRectangle& b,
                 float smoothingRate)
{
    ofRectangle result;
    result.x = ofLerp(a.x, b.x, smoothingRate);
    result.y = ofLerp(a.y, b.y, smoothingRate);
    result.width = ofLerp(a.width, b.width, smoothingRate);
    result.height = ofLerp(a.height, b.height, smoothingRate);
    return result;
}

inline float distance(const ofRectangle& a, const ofRectangle& b)
{
    glm::vec2 dp = a.getCenter() - b.getCenter();
    glm::vec2 ds = { a.width - b.width, a.height - b.height };
    return glm::length(dp) + glm::length(ds);
}


/// \tparam T The type of object being tracked.
template <class T>
class TrackedObject {
public:
    TrackedObject(const T& object, std::size_t label, std::size_t index):
        _object(object),
        _label(label),
        _index(index)
    {
    }

    TrackedObject(const T& object,
                  const TrackedObject<T>& previous,
                  std::size_t index):
        _object(object),
        _label(previous._label),
        _index(index),
        _age(previous._age)
    {
    }

    TrackedObject(const TrackedObject<T>& old):
        _object(old._object),
        _label(old._label),
        _age(old._age),
        _lastSeen(old._lastSeen)
    {
    }

    // TODO write implicit copy assignment operator since we do it here.

    /// \returns a reference to the tracked object.
    T& object()
    {
        return _object;
    }

    /// \returns a const reference to the tracked object.
    const T& object() const
    {
        return _object;
    }

    /// \returns the tracked object label.
    std::size_t label() const
    {
        return _label;
    }

    /// \returns the tracked object index.
    std::size_t index() const
    {
        return _index;
    }

    /// \returns the tracked object age.
    uint64_t age() const
    {
        return _age;
    }

    /// \returns the tracked object last seen age.
    uint64_t lastSeen() const
    {
        return _lastSeen;
    }

    void timeStep(bool visible)
    {
        _age++;

        if (!visible)
            _lastSeen++;
    }

protected:
    T _object;

    std::size_t _label = static_cast<std::size_t>(-1);
    std::size_t _index = static_cast<std::size_t>(-1);

    uint64_t _age = 0;
    uint64_t _lastSeen = 0;

};


template <class T>
class Tracker
{
public:
    typedef std::vector<std::size_t> Labels;

    Tracker()
    {
    }

    virtual ~Tracker()
    {
    }

    void setPersistence(std::size_t persistence)
    {
        _persistence = persistence;
    }

    void setMaximumDistance(float maximumDistance)
    {
        _maximumDistance = maximumDistance;
    }

    // organized in the order received by track()
    Labels currentLabels() const
    {
        return _currentLabels;
    }

    Labels previousLabels() const
    {
        return _previousLabels;
    }

    Labels newLabels() const
    {
        return _newLabels;
    }

    Labels deadLabels() const
    {
        return _deadLabels;
    }

    std::size_t getLabelFromIndex(std::size_t index) const
    {
        return _currentLabels[index];
    }

    // organized by label
    std::size_t getIndexFromLabel(std::size_t label) const
    {
        return _currentLabelMap.find(label)->second->getIndex();
    }

    T getPrevious(std::size_t label) const
    {
        return _previousLabelMap.find(label)->second->object();
    }

    T getCurrent(std::size_t label) const
    {
        return _currentLabelMap.find(label)->second->object();
    }

    bool existsCurrent(std::size_t label) const
    {
        return _currentLabelMap.count(label) > 0;
    }

    bool existsPrevious(std::size_t label) const
    {
        return _previousLabelMap.count(label) > 0;
    }

    uint64_t getAge(std::size_t label) const
    {
        return _currentLabelMap.find(label)->second->age();
    }

    uint64_t getLastSeen(std::size_t label) const
    {
        return _currentLabelMap.find(label)->second->lastSeen();
    }

    virtual Labels track(const std::vector<T>& objects)
    {
        _previous = _current;

        std::size_t n = objects.size();
        std::size_t m = _previous.size();

        // build NxM distance matrix
        typedef std::pair<std::size_t, std::size_t> MatchPair;
        typedef std::pair<MatchPair, float> MatchDistancePair;

        std::vector<MatchDistancePair> all;

        for (std::size_t i = 0; i < n; ++i)
        {
            for (std::size_t j = 0; j < m; ++j)
            {
                float curDistance = distance(objects[i], _previous[j].object());

                if (curDistance < _maximumDistance)
                {
                    all.push_back(MatchDistancePair(MatchPair(i, j),
                                                    curDistance));
                }
            }
        }

        // sort all possible matches by distance
        std::sort(all.begin(), all.end(), [](const MatchDistancePair& a,
                                             const MatchDistancePair& b) {
            return a.second < b.second;
        });

        _previousLabels = _currentLabels;
        _currentLabels.clear();
        _currentLabels.resize(n);
        _current.clear();

        std::vector<bool> matchedObjects(n, false);
        std::vector<bool> matchedPrevious(m, false);

        // walk through matches in order
        for (std::size_t k = 0; k < all.size(); ++k)
        {
            MatchPair& match = all[k].first;
            std::size_t i = match.first;
            std::size_t j = match.second;

            // only use match if both objects are unmatched, lastSeen is set to 0
            if (!matchedObjects[i] && !matchedPrevious[j])
            {
                matchedObjects[i] = true;
                matchedPrevious[j] = true;
                std::size_t index = _current.size();
                _current.push_back(TrackedObject<T>(objects[i],
                                                    _previous[j],
                                                    index));
                _current.back().timeStep(true);
                _currentLabels[i] = _current.back().label();
            }
        }

        // create new labels for new unmatched objects, lastSeen is set to 0
        _newLabels.clear();

        for (std::size_t i = 0; i < n; ++i)
        {
            if (!matchedObjects[i])
            {
                std::size_t curLabel = _curLabel++;
                std::size_t index = _current.size();
                _current.push_back(TrackedObject<T>(objects[i],
                                                    curLabel,
                                                    index));
                _current.back().timeStep(true);
                _currentLabels[i] = curLabel;
                _newLabels.push_back(curLabel);
            }
        }

        // copy old unmatched objects if young enough, lastSeen is increased

        _deadLabels.clear();

        for (std::size_t j = 0; j < m; ++j)
        {
            if (!matchedPrevious[j])
            {
                if (_previous[j].lastSeen() < _persistence)
                {
                    _current.push_back(_previous[j]);
                    _current.back().timeStep(false);
                }

                _deadLabels.push_back(_previous[j].label());
            }
        }

        // build label maps
        _currentLabelMap.clear();

        for(std::size_t i = 0; i < _current.size(); ++i)
        {
            _currentLabelMap[_current[i].label()] = &(_current[i]);
        }

        _previousLabelMap.clear();

        for (std::size_t i = 0; i < _previous.size(); ++i)
        {
            _previousLabelMap[_previous[i].label()] = &(_previous[i]);
        }

        return _currentLabels;
    }

protected:
    std::vector<TrackedObject<T>> _previous;
    std::vector<TrackedObject<T>> _current;

    Labels _currentLabels;
    Labels _previousLabels;
    Labels _newLabels;
    Labels _deadLabels;

    std::map<std::size_t, TrackedObject<T>*> _previousLabelMap;
    std::map<std::size_t, TrackedObject<T>*> _currentLabelMap;

    std::size_t _persistence = 15;
    float _maximumDistance = 64;
    std::size_t _curLabel = 0;

};


template <class T>
class SmoothTracker: public Tracker<T>
{
public:
    SmoothTracker()
    {
    }

    virtual ~SmoothTracker() override
    {
    }

    virtual glm::vec2 position(const T& item) = 0;

    virtual T lerp(const T& from, const T& to, float smoothingRate) = 0;

    typename Tracker<T>::Labels track(const std::vector<ofRectangle>& objects) override
    {
        auto labels = Tracker<T>::track(objects);

        // add new objects, update old objects
        for (std::size_t i = 0; i < labels.size(); ++i)
        {
            std::size_t label = labels[i];

            T cur = Tracker<T>::getCurrent(label);

            if  (_smoothed.count(label) > 0)
            {
                _smoothed[label] = lerp(_smoothed[label], cur, _smoothingRate);
            }
            else
            {
                _smoothed[label] = cur;
            }
        }

        auto smoothedItr = _smoothed.begin();

        while (smoothedItr != _smoothed.end())
        {
            auto label = smoothedItr->first;

            if (!Tracker<T>::existsCurrent(label))
            {
                smoothedItr = _smoothed.erase(smoothedItr);
            }
            else
            {
                ++smoothedItr;
            }
        }

        return labels;
    }

    T getSmoothed(std::size_t label) const
    {
        return _smoothed.find(label)->second;
    }

    glm::vec2 getVelocity(std::size_t index) const
    {
        std::size_t label = Tracker<T>::getLabelFromIndex(index);

        if (Tracker<T>::existsPrevious(label))
        {
            return position(Tracker<T>::getCurrent(label)) - position(Tracker<T>::getPrevious(label));
        }

        return glm::vec2(0, 0);
    }

    void setSmoothingRate(float smoothingRate)
    {
        _smoothingRate = smoothingRate;
    }

    float getSmoothingRate() const
    {
        return _smoothingRate;
    }

protected:
    float _smoothingRate = 0.5;

    std::map<std::size_t, T> _smoothed;

};


//class RectSmoothTracker: public SmoothTracker<ofRectangle>
//{
//public:
//    RectSmoothTracker()
//    {
//    }

//    virtual ~RectSmoothTracker() override
//    {
//    }


//};


template <class T>
class Follower
{
public:
    Follower()
    {
    }

    virtual ~Follower()
    {
    }

    virtual void setup(const T&)
    {
    }

    virtual void update(const T&)
    {
    }

    virtual void kill()
    {
        _dead = true;
    }

    void setLabel(std::size_t label)
    {
        _label = label;
    }

    std::size_t getLabel() const
    {
        return _label;
    }

    bool isDead() const
    {
        return _dead;
    }

protected:
    bool _dead = false;

    std::size_t _label = std::numeric_limits<std::size_t>::max();

};


/// The Follower class type should be default constructable and implement an
/// Follower::isDead() const method that returns true when the follower is
/// "dead".
///
/// \tparam T The Tracker class type.
/// \tparam F The Follower class type. The follower must be default constructable.
template <class T, class F>
class TrackerFollower: public Tracker<T>
{
public:
    typename Tracker<T>::Labels track(const std::vector<T>& objects) override
    {
        // Do the standard tracking.
        Tracker<T>::track(objects);

        // kill missing, update old
        for (std::size_t i = 0; i < _labels.size(); ++i)
        {
            std::size_t curLabel = _labels[i];
            F& curFollower = _followers[i];

            if (!Tracker<T>::existsCurrent(curLabel))
            {
                curFollower.kill();
            }
            else
            {
                curFollower.update(Tracker<T>::getCurrent(curLabel));
            }
        }

        // add new
        for (std::size_t i = 0; i < Tracker<T>::_newLabels.size(); ++i)
        {
            std::size_t curLabel = Tracker<T>::_newLabels[i];
            _labels.push_back(curLabel);

            _followers.push_back(F());
            _followers.back().setup(Tracker<T>::getCurrent(curLabel));
            _followers.back().setLabel(curLabel);
        }

        // remove dead
        for (std::size_t i = _labels.size() - 1; i != static_cast<std::size_t>(-1); --i)
        {
            if (_followers[i].isDead())
            {
                _followers.erase(_followers.begin() + i);
                _labels.erase(_labels.begin() + i);
            }
        }

        return _labels;
    }

    std::vector<F>& followers()
    {
        return _followers;
    }

    std::vector<F> followers() const
    {
        return _followers;
    }

protected:
    typename Tracker<T>::Labels _labels;

    std::vector<F> _followers;

};


} } // namespace ofx::Dlib
