//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{

    {
        dlib::array2d<dlib::rgb_pixel> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::array2d<dlib::bgr_pixel> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::array2d<dlib::rgb_alpha_pixel> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::array2d<dlib::hsi_pixel> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::array2d<dlib::lab_pixel> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::array2d<unsigned char> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::array2d<float> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::array2d<unsigned short> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::matrix<dlib::rgb_pixel> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::matrix<dlib::bgr_pixel> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::matrix<dlib::rgb_alpha_pixel> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::matrix<dlib::hsi_pixel> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::matrix<dlib::lab_pixel> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::matrix<unsigned char> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }
    {
        dlib::matrix<float> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());

        auto i = ofxDlib::toOf(img);

        float mx = dlib::max(img);
        float mn = dlib::min(img);

        std::cout << mn << " / " << mx << std::endl;

        textures.back().loadData(i);
    }
    {
        dlib::matrix<unsigned short> img;
        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
        textures.push_back(ofTexture());
        textures.back().loadData(ofxDlib::toOf(img));
    }


    auto d_p = dlib::point(1, 2);
    auto d_dp = dlib::dpoint(1, 2);

    auto o_p = ofxDlib::toOf(d_p);
    auto o_dp = ofxDlib::toOf(d_dp);


/*
    try
    {

        // Here we declare an image object that can store rgb_pixels.  Note that in
        // dlib there is no explicit image object, just a 2D array and
        // various pixel types.
        //dlib::array2d<dlib::rgb_pixel> img;

        // Now load the image file into our image.  If something is wrong then
        // load_image() will throw an exception.  Also, if you linked with libpng
        // and libjpeg then load_image() can load PNG and JPEG files in addition
        // to BMP files.
//        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));
//        dlib::load_image(img, ofToDataPath("Puppy.jpg", true));

        ofImage img;
        img.load("Puppy.jpg");
        img.setImageType(OF_IMAGE_GRAYSCALE);
        ofPixels in = img.getPixels();;
        ofPixels out;

        //dlib::gaussian_blur(img, blurred_img);
        dlib::gaussian_blur(in, out, 1);


        // Now find the horizontal and vertical gradient images.
        ofFloatPixels horz_gradient, vert_gradient;
        ofPixels edge_image;

        dlib::sobel_edge_detector(out, horz_gradient, vert_gradient);

        // now we do the non-maximum edge suppression step so that our edges are nice and thin
        dlib::suppress_non_maximum_edges(horz_gradient, vert_gradient, edge_image);

        tex.loadData(out);
        edgeTex.loadData(edge_image);

        // Now we would like to see what our images look like.  So let's use a
        // window to display them on the screen.  (Note that you can zoom into
        // the window by holding CTRL and scrolling the mouse wheel)
        //image_window my_window(edge_image, "Normal Edge Image");

        // We can also easily display the edge_image as a heatmap or using the jet color
        // scheme like so.
        //image_window win_hot(heatmap(edge_image));

        auto heatMap = dlib::heatmap(edge_image);

//        heatMap

//        auto heatMap = ofxDlib::toOf(dlib::heatmap(edge_image));

        //image_window win_jet(jet(edge_image));

        auto jetImg = dlib::jet(edge_image);

        // also make a window to display the original image
        //image_window my_window2(img, "Original Image");

        // Sometimes you want to get input from the user about which pixels are important
        // for some task.  You can do this easily by trapping user clicks as shown below.
        // This loop executes every time the user double clicks on some image pixel and it
        // will terminate once the user closes the window.
//        dlib::point p;

//        while (my_window.get_next_double_click(p))
//        {
//            cout << "User double clicked on pixel:         " << p << endl;
//            cout << "edge pixel value at this location is: " << (int)edge_image[p.y()][p.x()] << endl;
//        }

        // wait until the user closes the windows before we let the program
        // terminate.
//        win_hot.wait_until_closed();
//        my_window2.wait_until_closed();


        // Finally, note that you can access the elements of an image using the normal [row][column]
        // operator like so:
//        cout << horz_gradient[0][3] << endl;
//        cout << "number of rows in image:    " << horz_gradient.nr() << endl;
//        cout << "number of columns in image: " << horz_gradient.nc() << endl;
    }
    catch (exception& e)
    {
        cout << "exception thrown: " << e.what() << endl;
    }

 */
 }



void ofApp::draw()
{
//    ofPixels in;
//
//    ofLoadImage(in, "Puppy.jpg");
//
//    std::cout << in.getWidth() << " / " << in.getHeight() << " / " << in.getPixelFormat() << std::endl;
//
//    // Now let's use some image functions.  First let's blur the image a little.
//    ofPixels out;
//
//    out.allocate(in.getWidth(), in.getHeight(), in.getPixelFormat());
//
//
//    std::cout << in.getWidth() << " / " << in.getHeight() << " / " << in.getPixelFormat() << std::endl;
//
//
//    //        dlib::array2d<unsigned char> blurred_img;
//
//    //dlib::gaussian_blur(img, blurred_img);
//
//    dlib::gaussian_blur(in, out);
//
//
//    //        // Now find the horizontal and vertical gradient images.
//    //        dlib::array2d<short> horz_gradient, vert_gradient;
//    //        dlib::array2d<unsigned char> edge_image;
//    //        dlib::sobel_edge_detector(blurred_img, horz_gradient, vert_gradient);
//    //
//    //        // now we do the non-maximum edge suppression step so that our edges are nice and thin
//    //        dlib::suppress_non_maximum_edges(horz_gradient, vert_gradient, edge_image);
//    //
//    //        ofPixels pixels;
//    //        ofxDlib::toOf(edge_image, pixels);
//    tex.loadData(out);

    float x = 0;
    float y = 0;

    for (auto& texture: textures)
    {
        texture.draw(x, y, texture.getWidth() * 0.5, texture.getHeight() * 0.5);

        if (x + texture.getWidth() * 0.5 > ofGetWidth())
        {
            x = 0;
            y += texture.getHeight() * 0.5;
        }
        else
        {
            x += texture.getWidth() * 0.5;
        }

    }
}

