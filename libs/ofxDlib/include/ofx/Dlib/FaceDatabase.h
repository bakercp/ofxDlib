//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <vector>
#include "ofx/Dlib/Face.h"


namespace ofx {
namespace Dlib {


/// \brief A structure representing an online Face Database.
class FaceDatabase
{
public:
    struct Settings;

    FaceDatabase();

    std::size_t getId(const Face& face) const;

    /// \returns true if a face has been loaded.
    bool isLoaded() const;

    struct Settings
    {

    };

private:
    /// \brief True if the settings were loaded correctly.
    bool _isLoaded = false;

};


} } // namespace ofx::Dlib
