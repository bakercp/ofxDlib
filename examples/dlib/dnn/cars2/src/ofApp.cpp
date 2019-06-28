//
// Copyright (c) 2018 Dario Longobardi
//
// License: MIT
//

/*
    This example shows how to run a CNN based vehicle detector using dlib.  The
    example loads a pretrained model and uses it to find the rear ends of cars in
    an image.  We will also visualize some of the detector's processing steps by
    plotting various intermediate images on the screen.  Viewing these can help
    you understand how the detector works.

    The model used by this example was trained by the dnn_mmod_train_find_cars_ex.cpp
    example.  Also, since this is a CNN, you really should use a GPU to get the
    best execution speed.  For instance, when run on a NVIDIA 1080ti, this detector
    runs at 98fps when run on the provided test image.  That's more than an order
    of magnitude faster than when run on the CPU.

    Users who are just learning about dlib's deep learning API should read
    the dnn_introduction_ex.cpp and dnn_introduction2_ex.cpp examples to learn
    how the API works.  For an introduction to the object detection method you
    should read dnn_mmod_ex.cpp.

    You can also see some videos of this vehicle detector running on YouTube:
        https://www.youtube.com/watch?v=4B3bzmxMAZU
        https://www.youtube.com/watch?v=bP2SUo5vSlc
*/

#include "ofApp.h"

void ofApp::setup()
{
	image.load("mmod_cars_test_image2.png");
        dlib::matrix<dlib::rgb_pixel> img = dlib::mat(image.getPixels());

        net_type net;
	shape_predictor sp;
    	// You can get this file from http://dlib.net/files/mmod_front_and_rear_end_vehicle_detector.dat.bz2
    	// This network was produced by the dnn_mmod_train_find_cars_ex.cpp example program.
    	// As you can see, the file also includes a separately trained shape_predictor.  To see
    	// a generic example of how to train those refer to train_shape_predictor_ex.cpp.
    	deserialize(ofToDataPath("mmod_front_and_rear_end_vehicle_detector.dat")) >> net >> sp;

	// Run the detector on the image and show us the output.
    	for (auto&& d : net(img))
    	{
        	// We use a shape_predictor to refine the exact shape and location of the detection
	        // box.  This shape_predictor is trained to simply output the 4 corner points of
	        // the box.  So all we do is make a rectangle that tightly contains those 4 points
	        // and that rectangle is our refined detection position.
	        auto fd = sp(img,d);
	        dlib::rectangle rect;
	        for (unsigned long j = 0; j < fd.num_parts(); ++j)
	            rect += fd.part(j);

		pair<string,ofRectangle> p_rect;
        	p_rect = std::make_pair(d.label,ofxDlib::toOf(rect));
    		rectof.push_back(p_rect);
    	}
}

void ofApp::draw()
{
	image.draw(0,0);
	ofPushStyle();
	ofNoFill();
	ofSetLineWidth(4);
	for(int i = 0; i < rectof.size(); i++) {
	        if (rectof[i].first == "rear")
			ofSetColor(ofColor::orange);
		else
			ofSetColor(ofColor::violet);
		ofDrawRectangle(rectof[i].second);
		ofDrawBitmapStringHighlight(rectof[i].first,rectof[i].second.x,rectof[i].second.y);
	}
	ofPopStyle();
}
