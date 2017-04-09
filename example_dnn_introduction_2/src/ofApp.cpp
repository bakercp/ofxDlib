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

    std::vector<matrix<unsigned char>> training_images;
    std::vector<unsigned long> training_labels;
    std::vector<matrix<unsigned char>> testing_images;
    std::vector<unsigned long> testing_labels;

    load_mnist_dataset(ofToDataPath("mnist/", true),
                       training_images,
                       training_labels,
                       testing_images,
                       testing_labels);

    // dlib uses cuDNN under the covers.  One of the features of cuDNN is the
    // option to use slower methods that use less RAM or faster methods that use
    // a lot of RAM.  If you find that you run out of RAM on your graphics card
    // then you can call this function and we will request the slower but more
    // RAM frugal cuDNN algorithms.
    set_dnn_prefer_smallest_algorithms();


    // Create a network as defined above.  This network will produce 10 outputs
    // because that's how we defined net_type.  However, fc layers can have the
    // number of outputs they produce changed at runtime.
    net_type net;
    // So if you wanted to use the same network but override the number of
    // outputs at runtime you can do so like this:
    net_type net2(num_fc_outputs(15));

    // Now, let's imagine we wanted to replace some of the relu layers with
    // prelu layers.  We might do it like this:
    using net_type2 = loss_multiclass_log<fc<number_of_classes,
    avg_pool_everything<
    pres<res<res<res_down< // 2 prelu layers here
    tag4<repeat<9,pres,    // 9 groups, each containing 2 prelu layers
    res_down<
    res<
    input<matrix<unsigned char>>
    >>>>>>>>>>>;

    // prelu layers have a floating point parameter.  If you want to set it to
    // something other than its default value you can do so like this:
    net_type2 pnet(prelu_(0.2),
                   prelu_(0.25),
                   repeat_group(prelu_(0.3),prelu_(0.4)) // Initialize all the prelu instances in the repeat
                   // layer.  repeat_group() is needed to group the
                   // things that are part of repeat's block.
                   );
    // As you can see, a network will greedily assign things given to its
    // constructor to the layers inside itself.  The assignment is done in the
    // order the layers are defined, but it will skip layers where the
    // assignment doesn't make sense.

    // Now let's print the details of the pnet to the screen and inspect it.
    cout << "The pnet has " << pnet.num_layers << " layers in it." << endl;
    cout << pnet << endl;
    // These print statements will output this (I've truncated it since it's
    // long, but you get the idea):
    /*
     The pnet has 131 layers in it.
     layer<0>    loss_multiclass_log
     layer<1>    fc       (num_outputs=10) learning_rate_mult=1 weight_decay_mult=1 bias_learning_rate_mult=1 bias_weight_decay_mult=0
     layer<2>    avg_pool (nr=0, nc=0, stride_y=1, stride_x=1, padding_y=0, padding_x=0)
     layer<3>    prelu    (initial_param_value=0.2)
     layer<4>    add_prev1
     layer<5>    bn_con   eps=1e-05 learning_rate_mult=1 weight_decay_mult=0 bias_learning_rate_mult=1 bias_weight_decay_mult=1
     layer<6>    con      (num_filters=8, nr=3, nc=3, stride_y=1, stride_x=1, padding_y=1, padding_x=1) learning_rate_mult=1 weight_decay_mult=1 bias_learning_rate_mult=1 bias_weight_decay_mult=0
     layer<7>    prelu    (initial_param_value=0.25)
     layer<8>    bn_con   eps=1e-05 learning_rate_mult=1 weight_decay_mult=0 bias_learning_rate_mult=1 bias_weight_decay_mult=1
     layer<9>    con      (num_filters=8, nr=3, nc=3, stride_y=1, stride_x=1, padding_y=1, padding_x=1) learning_rate_mult=1 weight_decay_mult=1 bias_learning_rate_mult=1 bias_weight_decay_mult=0
     layer<10>   tag1
     ...
     layer<34>   relu
     layer<35>   bn_con   eps=1e-05 learning_rate_mult=1 weight_decay_mult=0 bias_learning_rate_mult=1 bias_weight_decay_mult=1
     layer<36>   con      (num_filters=8, nr=3, nc=3, stride_y=2, stride_x=2, padding_y=0, padding_x=0) learning_rate_mult=1 weight_decay_mult=1 bias_learning_rate_mult=1 bias_weight_decay_mult=0
     layer<37>   tag1
     layer<38>   tag4
     layer<39>   prelu    (initial_param_value=0.3)
     layer<40>   add_prev1
     layer<41>   bn_con   eps=1e-05 learning_rate_mult=1 weight_decay_mult=0 bias_learning_rate_mult=1 bias_weight_decay_mult=1
     ...
     layer<118>  relu
     layer<119>  bn_con   eps=1e-05 learning_rate_mult=1 weight_decay_mult=0 bias_learning_rate_mult=1 bias_weight_decay_mult=1
     layer<120>  con      (num_filters=8, nr=3, nc=3, stride_y=2, stride_x=2, padding_y=0, padding_x=0) learning_rate_mult=1 weight_decay_mult=1 bias_learning_rate_mult=1 bias_weight_decay_mult=0
     layer<121>  tag1
     layer<122>  relu
     layer<123>  add_prev1
     layer<124>  bn_con   eps=1e-05 learning_rate_mult=1 weight_decay_mult=0 bias_learning_rate_mult=1 bias_weight_decay_mult=1
     layer<125>  con      (num_filters=8, nr=3, nc=3, stride_y=1, stride_x=1, padding_y=1, padding_x=1) learning_rate_mult=1 weight_decay_mult=1 bias_learning_rate_mult=1 bias_weight_decay_mult=0
     layer<126>  relu
     layer<127>  bn_con   eps=1e-05 learning_rate_mult=1 weight_decay_mult=0 bias_learning_rate_mult=1 bias_weight_decay_mult=1
     layer<128>  con      (num_filters=8, nr=3, nc=3, stride_y=1, stride_x=1, padding_y=1, padding_x=1) learning_rate_mult=1 weight_decay_mult=1 bias_learning_rate_mult=1 bias_weight_decay_mult=0
     layer<129>  tag1
     layer<130>  input<matrix>
     */

    // Now that we know the index numbers for each layer, we can access them
    // individually using layer<index>(pnet).  For example, to access the output
    // tensor for the first prelu layer we can say:
    layer<3>(pnet).get_output();
    // Or to print the prelu parameter for layer 7 we can say:
    cout << "prelu param: "<< layer<7>(pnet).layer_details().get_initial_param_value() << endl;

    // We can also access layers by their type.  This next statement finds the
    // first tag1 layer in pnet, and is therefore equivalent to calling
    // layer<10>(pnet):
    layer<tag1>(pnet);
    // The tag layers don't do anything at all and exist simply so you can tag
    // parts of your network and access them by layer<tag>().  You can also
    // index relative to a tag.  So for example, to access the layer immediately
    // after tag4 you can say:
    layer<tag4,1>(pnet); // Equivalent to layer<38+1>(pnet).

    // Or to access the layer 2 layers after tag4:
    layer<tag4,2>(pnet);
    // Tagging is a very useful tool for making complex network structures.  For
    // example, the add_prev1 layer is implemented internally by using a call to
    // layer<tag1>().



    // Ok, that's enough talk about defining and inspecting networks.  Let's
    // talk about training networks!

    // The dnn_trainer will use SGD by default, but you can tell it to use
    // different solvers like adam with a weight decay of 0.0005 and the given
    // momentum parameters.
    dnn_trainer<net_type,adam> trainer(net,adam(0.0005, 0.9, 0.999));
    // Also, if you have multiple graphics cards you can tell the trainer to use
    // them together to make the training faster.  For example, replacing the
    // above constructor call with this one would cause it to use GPU cards 0
    // and 1.
    //dnn_trainer<net_type,adam> trainer(net,adam(0.0005, 0.9, 0.999), {0,1});

    trainer.be_verbose();
    trainer.set_synchronization_file("mnist_resnet_sync", std::chrono::seconds(100));
    // While the trainer is running it keeps an eye on the training error.  If
    // it looks like the error hasn't decreased for the last 2000 iterations it
    // will automatically reduce the learning rate by 0.1.  You can change these
    // default parameters to some other values by calling these functions.  Or
    // disable the automatic shrinking entirely by setting the shrink factor to 1.
    trainer.set_iterations_without_progress_threshold(2000);
    trainer.set_learning_rate_shrink_factor(0.1);
    // The learning rate will start at 1e-3.
    trainer.set_learning_rate(1e-3);


    // Now, what if your training dataset is so big it doesn't fit in RAM?  You
    // make mini-batches yourself, any way you like, and you send them to the
    // trainer by repeatedly calling trainer.train_one_step().
    //
    // For example, the loop below stream MNIST data to out trainer.
    std::vector<matrix<unsigned char>> mini_batch_samples;
    std::vector<unsigned long> mini_batch_labels;
    dlib::rand rnd(time(0));
    // Loop until the trainer's automatic shrinking has shrunk the learning rate to 1e-6.
    // Given our settings, this means it will stop training after it has shrunk the
    // learning rate 3 times.
    while(trainer.get_learning_rate() >= 1e-6)
    {
        mini_batch_samples.clear();
        mini_batch_labels.clear();

        // make a 128 image mini-batch
        while(mini_batch_samples.size() < 128)
        {
            auto idx = rnd.get_random_32bit_number()%training_images.size();
            mini_batch_samples.push_back(training_images[idx]);
            mini_batch_labels.push_back(training_labels[idx]);
        }

        // Tell the trainer to update the network given this mini-batch
        trainer.train_one_step(mini_batch_samples, mini_batch_labels);

        // You can also feed validation data into the trainer by periodically
        // calling trainer.test_one_step(samples,labels).  Unlike train_one_step(),
        // test_one_step() doesn't modify the network, it only computes the testing
        // error which it records internally.  This testing error will then be print
        // in the verbose logging and will also determine when the trainer's
        // automatic learning rate shrinking happens.  Therefore, test_one_step()
        // can be used to perform automatic early stopping based on held out data.
    }

    // When you call train_one_step(), the trainer will do its processing in a
    // separate thread.  This allows the main thread to work on loading data
    // while the trainer is busy executing the mini-batches in parallel.
    // However, this also means we need to wait for any mini-batches that are
    // still executing to stop before we mess with the net object.  Calling
    // get_net() performs the necessary synchronization.
    trainer.get_net();


    net.clean();
    serialize(ofToDataPath("mnist_res_network.dat", true)) << net;


    // Now we have a trained network.  However, it has batch normalization
    // layers in it.  As is customary, we should replace these with simple
    // affine layers before we use the network.  This can be accomplished by
    // making a network type which is identical to net_type but with the batch
    // normalization layers replaced with affine.  For example:
    using test_net_type = loss_multiclass_log<fc<number_of_classes,
    avg_pool_everything<
    ares<ares<ares<ares_down<
    repeat<9,ares,
    ares_down<
    ares<
    input<matrix<unsigned char>>
    >>>>>>>>>>;
    // Then we can simply assign our trained net to our testing net.
    test_net_type tnet = net;
    // Or if you only had a file with your trained network you could deserialize
    // it directly into your testing network.
    deserialize(ofToDataPath("mnist_res_network.dat", true)) >> tnet;


    // And finally, we can run the testing network over our data.

    std::vector<unsigned long> predicted_labels = tnet(training_images);
    int num_right = 0;
    int num_wrong = 0;
    for (size_t i = 0; i < training_images.size(); ++i)
    {
        if (predicted_labels[i] == training_labels[i])
            ++num_right;
        else
            ++num_wrong;

    }
    cout << "training num_right: " << num_right << endl;
    cout << "training num_wrong: " << num_wrong << endl;
    cout << "training accuracy:  " << num_right/(double)(num_right+num_wrong) << endl;

    predicted_labels = tnet(testing_images);
    num_right = 0;
    num_wrong = 0;
    for (size_t i = 0; i < testing_images.size(); ++i)
    {
        if (predicted_labels[i] == testing_labels[i])
            ++num_right;
        else
            ++num_wrong;

    }
    cout << "testing num_right: " << num_right << endl;
    cout << "testing num_wrong: " << num_wrong << endl;
    cout << "testing accuracy:  " << num_right/(double)(num_right+num_wrong) << endl;

    // Finish benchmarking.
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "Finished computation at " << std::ctime(&end_time);
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s";
    std::cout << std::endl;

    // Took many many hours on MacBook Pro (15-inch, Mid 2012), no CUDA support.
    // 432 seconds on i7 7700 + Nvidia 1080, CUDA support + MKL libs.

    ofExit();
}
