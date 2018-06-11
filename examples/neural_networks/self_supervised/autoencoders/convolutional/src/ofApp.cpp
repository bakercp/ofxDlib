//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Set up a timing mechanism for benchmarking.
    auto start = std::chrono::system_clock::now();

    // MNIST is broken into two parts, a training set of 60000 images and a test set of
    // 10000 images.  Each image is labeled so that we know what hand written digit is
    // depicted.  These next statements load the dataset into memory.
    std::vector<dlib::matrix<unsigned char>> training_images;
    std::vector<unsigned long> training_labels;
    std::vector<dlib::matrix<unsigned char>> testing_images;
    std::vector<unsigned long> testing_labels;

    dlib::load_mnist_dataset(ofToDataPath("mnist/", true),
                             training_images,
                             training_labels,
                             testing_images,
                             testing_labels);


    
    for (std::size_t i = 0; i < training_images.size(); ++i)
    {
        trainingImages.push_back(dlib::reshape(dlib::matrix_cast<float>(training_images[i]) / 255.0, training_images[i].size(), 1));
    }
    
    for (std::size_t i = 0; i < testing_images.size(); ++i)
    {
        testingImages.push_back(dlib::reshape(dlib::matrix_cast<float>(testing_images[i]) / 255.0, testing_images[i].size(), 1));
    }
    
    dlib::dnn_trainer<autoencoder> trainer(net, dlib::sgd());

    // If you have CUDA access, you can make specific CUDA devices do the work.
    // dlib::dnn_trainer<net_type,dlib::sgd> trainer(net, dlib::sgd(), {0, 1});

    trainer.set_learning_rate(0.01);
    trainer.set_min_learning_rate(0.00001);
    trainer.set_mini_batch_size(64);
    trainer.be_verbose();

    // Since DNN training can take a long time, we can ask the trainer to save its state to
    // a file named "mnist_sync" every 20 seconds.  This way, if we kill this program and
    // start it again it will begin where it left off rather than restarting the training
    // from scratch.  This is because, when the program restarts, this call to
    // set_synchronization_file() will automatically reload the settings from mnist_sync if
    // the file exists.
    // trainer.set_synchronization_file(ofToDataPath("mnist_ae_sync", true), std::chrono::seconds(20));

    // Finally, this line begins training.  By default, it runs SGD with our specified
    // learning rate until the loss stops decreasing.  Then it reduces the learning rate by
    // a factor of 10 and continues running until the loss stops decreasing again.  It will
    // keep doing this until the learning rate has dropped below the min learning rate
    // defined above or the maximum number of epochs as been executed (defaulted to 10000).
    trainer.train(trainingImages, trainingImages);

//    trainer.test_one_step(<#const std::vector<input_type> &data#>, <#const std::vector<training_label_type> &labels#>)
    
    // At this point our net object should have learned how to classify MNIST images.  But
    // before we try it out let's save it to disk.  Note that, since the trainer has been
    // running images through the network, net will have a bunch of state in it related to
    // the last batch of images it processed (e.g. outputs from each layer).  Since we
    // don't care about saving that kind of stuff to disk we can tell the network to forget
    // about that kind of transient data so that our file will be smaller.  We do this by
    // "cleaning" the network before saving it.
    //net.clean();
    //dlib::serialize(ofToDataPath("mnist_network.dat", true)) << net;

    
    //    // Finish benchmarking.
//    auto end = std::chrono::system_clock::now();
//    std::chrono::duration<double> elapsed_seconds = end - start;
//    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
//
//    std::cout << "Finished computation at " << std::ctime(&end_time);
//    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s";
//    std::cout << std::endl;
//
//    // Took many many hours on MacBook Pro (15-inch, Mid 2012), no CUDA support.
//    // Took 1909.17s on Macbook Pro (15-inch, Mid 2017), no CUDA, with Intel MKL.
//    // 7 seconds on i7 7700 + Nvidia 1080, CUDA support + MKL libs.
//
     
}


void ofApp::update()
{
    auto now = ofGetElapsedTimeMillis();
    
    if (now > lastUpdate + delay)
    {
        lastUpdate = now;
        testingTex.clear();
        encodedTex.clear();
        
        for (int i = 0; i < 10; ++i)
        {
            testingImageOffset = (testingImageOffset + 1) % testingImages.size();
            
            dlib::matrix<float> encoded = net(testingImages[testingImageOffset]);
            
            testingTex.push_back(ofxDlib::toTexture(testingImages[testingImageOffset], 28, 28, OF_PIXELS_GRAY));
            encodedTex.push_back(ofxDlib::toTexture(encoded, 28, 28, OF_PIXELS_GRAY));
        }
    }
}


void ofApp::draw()
{
    float scale = 4;
    float w = 28 * scale;
    float h = w;
    for (std::size_t i = 0; i < testingTex.size(); ++i)
    {
        testingTex[i].draw(0, i * h, w, h);
        encodedTex[i].draw(w, i * h, w, h);
    }
}

