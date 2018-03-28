//
// Copyright (c) 2018 Dario Longobardi 
//
// License: MIT
//

#include "ofApp.h"
#include "segNet.h"

anet_type net;

void ofApp::setup()
{
	image.load("test.jpg");
//      dlib::load_image(input_image, ofToDataPath("test.jpg", true));
	dlib::deserialize(ofToDataPath("semantic_segmentation_voc2012net.dnn")) >> net;
//	image = ofxDlib::toOf(input_image);
	
        dlib::matrix<dlib::rgb_pixel> input_image = dlib::mat(image.getPixels());

        // Create predictions for each pixel. At this point, the type of each prediction
        // is an index (a value between 0 and 20). Note that the net may return an image
        // that is not exactly the same size as the input.
        const dlib::matrix<uint16_t> temp = net(input_image);
        // Crop the returned image to be exactly the same size as the input.
        const dlib::chip_details chip_details(
            dlib::centered_rect(temp.nc() / 2, temp.nr() / 2, input_image.nc(), input_image.nr()),
            dlib::chip_dims(input_image.nr(), input_image.nc())
        );
        dlib::extract_image_chip(temp, chip_details, index_label_image, dlib::interpolate_nearest_neighbor());
        // Convert the indexes to RGB values.
        index_label_image_to_rgb_label_image(index_label_image, rgb_label_image);

	// dlib rgb_pixel segment -> ofimage
	seg = ofxDlib::toOf(rgb_label_image);

	// get label detect
	classlabel = get_most_prominent_non_background_classlabel(index_label_image);

	//alpha image 
	seg_path.allocate(seg.getWidth(),seg.getHeight(),OF_IMAGE_COLOR_ALPHA);
	for(int h = 0; h < seg.getHeight(); h++) {
		for(int w = 0; w < seg.getWidth(); w++) {
			if(seg.getColor(w,h) != ofColor::black) {
				seg_path.setColor(w,h,seg.getColor(w,h));
			}
			else{
				seg_path.setColor(w,h,ofColor(0,0,0,0));
			}
		}
	}
	seg_path.update();
}

void ofApp::draw()
{
	image.draw(0,0);
	seg_path.draw(0,0);
	seg.draw(0,image.getHeight());
	ofDrawBitmapStringHighlight(classlabel, ofPoint(20,20), ofColor::red, ofColor::white);
}
