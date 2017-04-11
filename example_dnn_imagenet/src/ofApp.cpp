//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


dlib::rectangle make_random_cropping_rect_resnet(const dlib::matrix<dlib::rgb_pixel>& img,
                                                 dlib::rand& rnd)
{
    // figure out what rectangle we want to crop from the image
    double mins = 0.466666666, maxs = 0.875;
    auto scale = mins + rnd.get_random_double()*(maxs-mins);
    auto size = scale*std::min(img.nr(), img.nc());
    dlib::rectangle rect(size, size);
    // randomly shift the box around
    dlib::point offset(rnd.get_random_32bit_number()%(img.nc()-rect.width()),
                       rnd.get_random_32bit_number()%(img.nr()-rect.height()));
    return move_rect(rect, offset);
}


void randomly_crop_images(const dlib::matrix<dlib::rgb_pixel>& img,
                          dlib::array<dlib::matrix<dlib::rgb_pixel>>& crops,
                          dlib::rand& rnd,
                          long num_crops)
{
    std::vector<dlib::chip_details> dets;
    
    for (long i = 0; i < num_crops; ++i)
    {
        auto rect = make_random_cropping_rect_resnet(img, rnd);
        dets.push_back(dlib::chip_details(rect, dlib::chip_dims(227,227)));
    }

    dlib::extract_image_chips(img, dets, crops);

    for (auto&& img : crops)
    {
        // Also randomly flip the image
        if (rnd.get_random_double() > 0.5)
            img = dlib::fliplr(img);
        
        // And then randomly adjust the colors.
        dlib::apply_random_color_offset(img, rnd);
    }
}


void ofApp::setup()
{
//    // Set up a timing mechanism for benchmarking.
//    auto start = std::chrono::system_clock::now();
//
//    // Finish benchmarking.
//    auto end = std::chrono::system_clock::now();
//    std::chrono::duration<double> elapsed_seconds = end - start;
//    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
//
//    std::cout << "Finished computation at " << std::ctime(&end_time);
//    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s";
//    std::cout << std::endl;

    dlib::deserialize(ofToDataPath("resnet34_1000_imagenet_classifier.dnn", true)) >> net >> labels;

    // Make a network with softmax as the final layer.  We don't have to do this
    // if we just want to output the single best prediction, since the anet_type
    // already does this.  But if we instead want to get the probability of each
    // class as output we need to replace the last layer of the network with a
    // softmax layer, which we do as follows:
    //snet.subnet() = net.subnet();

//    dlib::array<dlib::matrix<dlib::rgb_pixel>> images;
//    dlib::matrix<dlib::rgb_pixel> img, crop;
//
//    dlib::rand rnd;
//    image_window win;
//
//    // Read images from the command prompt and print the top 5 best labels for each.
//    for (int i = 1; i < 1; ++i)
//    {
//        load_image(img, argv[i]);
//        const int num_crops = 16;
//        // Grab 16 random crops from the image.  We will run all of them through the
//        // network and average the results.
//        randomly_crop_images(img, images, rnd, num_crops);
//        // p(i) == the probability the image contains object of class i.
//        dlib::matrix<float,1,1000> p = sum_rows(mat(snet(images.begin(), images.end())))/num_crops;
//
//        win.set_image(img);
//        // Print the 5 most probable labels
//        for (int k = 0; k < 5; ++k)
//        {
//            unsigned long predicted_label = index_of_max(p);
//            cout << p(predicted_label) << ": " << labels[predicted_label] << endl;
//            p(predicted_label) = 0;
//        }
//
//        cout << "Hit enter to process the next image";
//        cin.get();
//    }


}


void ofApp::update()
{
    if (loadNext)
    {
        dlib::array<dlib::matrix<dlib::rgb_pixel>> images;
        dlib::matrix<dlib::rgb_pixel> img, crop;

        load_image(img, "img.png");
        const int num_crops = 16;
        // Grab 16 random crops from the image.  We will run all of them through the
        // network and average the results.
        randomly_crop_images(img, images, rnd, num_crops);
        // p(i) == the probability the image contains object of class i.
        dlib::matrix<float,1,1000> p = sum_rows(mat(snet(images.begin(), images.end())))/num_crops;

        //win.set_image(img);
        // Print the 5 most probable labels
        for (int k = 0; k < 5; ++k)
        {
            unsigned long predicted_label = index_of_max(p);
            cout << p(predicted_label) << ": " << labels[predicted_label] << endl;
            p(predicted_label) = 0;
        }

        loadNext = false;
    }
}


void ofApp::draw()
{

}


void ofApp::keyPressed(int key)
{
    loadNext = true;
}
