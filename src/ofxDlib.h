//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "dlib/of_default_adapter.h"
#include "dlib/of_image.h"
#include "dlib/to_of.h"
//#include "ofx/Dlib/Types.h"
#include "ofx/Dlib/Utils.h"
#include "ofx/Dlib/Network/LeNet.h"


#include <iostream>
#include <dlib/clustering.h>
#include <dlib/data_io.h>
#include <dlib/dnn.h>
#include <dlib/image_io.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/full_object_detection.h>
#if !defined(DLIB_NO_GUI_SUPPORT)
#include <dlib/gui_widgets.h>
#endif
#if defined(OPENCV)
#include <dlib/opencv.h>
#endif


namespace ofxDlib = ofx::Dlib;
