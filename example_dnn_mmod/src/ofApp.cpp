//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // In this example we are going to train a face detector based on the
    // small faces dataset in the examples/faces directory.  So the first
    // thing we do is load that dataset.  This means you need to supply the
    // path to this faces folder.

    const std::string faces_directory = ofToDataPath("faces", true);
    // The faces directory contains a training dataset and a separate
    // testing dataset.  The training data consists of 4 images, each
    // annotated with rectangles that bound each human face.  The idea is
    // to use this training data to learn to identify human faces in new
    // images.
    //
    // Once you have trained an object detector it is always important to
    // test it on data it wasn't trained on.  Therefore, we will also load
    // a separate testing set of 5 images.  Once we have a face detector
    // created from the training data we will see how well it works by
    // running it on the testing images.
    //
    // So here we create the variables that will hold our dataset.
    // images_train will hold the 4 training images and face_boxes_train
    // holds the locations of the faces in the training images.  So for
    // example, the image images_train[0] has the faces given by the
    // rectangles in face_boxes_train[0].
    std::vector<matrix<rgb_pixel>> images_train, images_test;
    std::vector<std::vector<mmod_rect>> face_boxes_train, face_boxes_test;

    // Now we load the data.  These XML files list the images in each dataset
    // and also contain the positions of the face boxes.  Obviously you can use
    // any kind of input format you like so long as you store the data into
    // images_train and face_boxes_train.  But for convenience dlib comes with
    // tools for creating and loading XML image datasets.  Here you see how to
    // load the data.  To create the XML files you can use the imglab tool which
    // can be found in the tools/imglab folder.  It is a simple graphical tool
    // for labeling objects in images with boxes.  To see how to use it read the
    // tools/imglab/README.txt file.
    load_image_dataset(images_train, face_boxes_train, faces_directory + "/training.xml");
    load_image_dataset(images_test, face_boxes_test, faces_directory + "/testing.xml");

    cout << "num training images: " << images_train.size() << endl;
    cout << "num testing images:  " << images_test.size() << endl;

    // The MMOD algorithm has some options you can set to control its behavior.  However,
    // you can also call the constructor with your training annotations and a "target
    // object size" and it will automatically configure itself in a reasonable way for your
    // problem.  Here we are saying that faces are still recognizably faces when they are
    // 40x40 pixels in size.  You should generally pick the smallest size where this is
    // true.  Based on this information the mmod_options constructor will automatically
    // pick a good sliding window width and height.  It will also automatically set the
    // non-max-suppression parameters to something reasonable.  For further details see the
    // mmod_options documentation.
    mmod_options options(face_boxes_train, 40,40);
    // The detector will automatically decide to use multiple sliding windows if needed.
    // For the face data, only one is needed however.
    cout << "num detector windows: "<< options.detector_windows.size() << endl;
    for (auto& w : options.detector_windows)
        cout << "detector window width by height: " << w.width << " x " << w.height << endl;
    cout << "overlap NMS IOU thresh:             " << options.overlaps_nms.get_iou_thresh() << endl;
    cout << "overlap NMS percent covered thresh: " << options.overlaps_nms.get_percent_covered_thresh() << endl;

    // Now we are ready to create our network and trainer.
    net_type net(options);
    // The MMOD loss requires that the number of filters in the final network layer equal
    // options.detector_windows.size().  So we set that here as well.
    net.subnet().layer_details().set_num_filters(options.detector_windows.size());
    dnn_trainer<net_type> trainer(net);
    trainer.set_learning_rate(0.1);
    trainer.be_verbose();
    trainer.set_synchronization_file(ofToDataPath("mmod_sync", true), std::chrono::minutes(5));
    trainer.set_iterations_without_progress_threshold(300);


    // Now let's train the network.  We are going to use mini-batches of 150
    // images.   The images are random crops from our training set (see
    // random_cropper_ex.cpp for a discussion of the random_cropper).
    std::vector<matrix<rgb_pixel>> mini_batch_samples;
    std::vector<std::vector<mmod_rect>> mini_batch_labels;
    random_cropper cropper;
    cropper.set_chip_dims(200, 200);
    // Usually you want to give the cropper whatever min sizes you passed to the
    // mmod_options constructor, which is what we do here.
    cropper.set_min_object_size(40,40);
    dlib::rand rnd;
    // Run the trainer until the learning rate gets small.  This will probably take several
    // hours.
    while(trainer.get_learning_rate() >= 1e-4)
    {
        cropper(150, images_train, face_boxes_train, mini_batch_samples, mini_batch_labels);
        // We can also randomly jitter the colors and that often helps a detector
        // generalize better to new images.
        for (auto&& img : mini_batch_samples)
            disturb_colors(img, rnd);

        trainer.train_one_step(mini_batch_samples, mini_batch_labels);
    }
    // wait for training threads to stop
    trainer.get_net();
    cout << "done training" << endl;

    // Save the network to disk
    net.clean();
    serialize(ofToDataPath("mmod_network.dat", true)) << net;


    // Now that we have a face detector we can test it.  The first statement tests it
    // on the training data.  It will print the precision, recall, and then average precision.
    // This statement should indicate that the network works perfectly on the
    // training data.
    cout << "training results: " << test_object_detection_function(net, images_train, face_boxes_train) << endl;
    // However, to get an idea if it really worked without overfitting we need to run
    // it on images it wasn't trained on.  The next line does this.   Happily,
    // this statement indicates that the detector finds most of the faces in the
    // testing data.
    cout << "testing results:  " << test_object_detection_function(net, images_test, face_boxes_test) << endl;


    // If you are running many experiments, it's also useful to log the settings used
    // during the training experiment.  This statement will print the settings we used to
    // the screen.
    cout << trainer << cropper << endl;


    for (auto&& img : images_test)
    {
        pyramid_up(img);
        auto dets = net(img);

        TestImage i;
        i.image = ofxDlib::toOf(img);
        for (auto&& d : dets)
            i.faceRects.push_back(d);
    }



}


void ofApp::draw()
{
    ofBackground(0);
    ofNoFill();
    ofSetColor(ofColor::white);

//    image.draw(0, 0);

//    for (auto& faceRect: faceRects)
//    {
//        ofSetColor(ofColor::yellow);
//        ofDrawRectangle(ofxDlib::toOf(faceRect));
//    }

//    ofDrawBitmapStringHighlight("Num. faces detected: " + ofToString(faceRects.size()), 14, 20);
}

