//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include "ofxDlib.h"


class ofApp: public ofxUnitTestsApp
{
    void run() override
    {
        ofPixels pixels;
        ofxTestEq(ofLoadImage(pixels, "Puppy.jpg"), true, "Load RGB.");

        ofImage image("Puppy.jpg");
        ofVideoPlayer player;
        player.load("Puppy.jpg");

        tests(image);
        tests(player);
    }

    template <typename image_type>
    void tests(image_type& pixels)
    {
        const image_type cPixels = pixels;

        ofxTestEq(dlib::num_columns(pixels), pixels.getWidth(), "Num Columns");
        ofxTestEq(dlib::num_columns(cPixels), cPixels.getWidth(), "Num Columns");

        ofxTestEq(dlib::num_rows(pixels), pixels.getHeight(), "Num Rows");
        ofxTestEq(dlib::num_rows(cPixels), cPixels.getHeight(), "Num Rows");

//        ofxTestEq(dlib::image_data(pixels), pixels.getData(), "Image Data");
//        ofxTestEq(dlib::image_data(cPixels), cPixels.getData(), "Image Data");


//        ofxTestEq(dlib::image_data(pixels), pixels.getData(), "Image Data");
//        ofxTestEq(dlib::image_data(cPixels), cPixels.getData(), "Image Data");

        dlib::rectangle roi(0, 0, 10, 10);

        dlib::sub_image_proxy<image_type> roiImage = dlib::sub_image(pixels, roi);
        dlib::const_sub_image_proxy<image_type> cRoiImage = dlib::sub_image(cPixels, roi);

        ofPixels tmpPixels;
        dlib::set_image_size(tmpPixels,
                             std::round(0.5 * dlib::num_rows(roiImage)),
                             std::round(0.5 * dlib::num_columns(roiImage)));

        dlib::resize_image(cRoiImage, tmpPixels);

        dlib::set_image_size(tmpPixels,
                             std::round(0.5 * dlib::num_rows(cRoiImage)),
                             std::round(0.5 * dlib::num_columns(cRoiImage)));

        dlib::resize_image(cRoiImage, tmpPixels);
    }


};


int main()
{
    ofInit();
    auto window = make_shared<ofAppNoWindow>();
    auto app = make_shared<ofApp>();
    ofRunApp(window, app);
    return ofRunMainLoop();
}
