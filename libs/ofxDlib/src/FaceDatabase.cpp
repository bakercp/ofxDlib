//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/Dlib/FaceDatabase.h"


namespace ofx {
namespace Dlib {



FaceDatabase::FaceDatabase()
{
}


bool FaceDatabase::isLoaded() const
{
    return _isLoaded;
}


} } // namespace ofx::Dlib
