//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


namespace dlib {
    


class ELBO_
{
public:
    /// \brief The genrated data in flattened matrix form.
    typedef matrix<float> training_label_type;
    /// \brief The training data in flattened matrix form.
    typedef matrix<float> output_label_type;
    
    template <typename SUB_TYPE, typename label_iterator>
    void to_label (const tensor& input_tensor,
                   const SUB_TYPE& sub,
                   label_iterator iter
                   ) const
    {
        DLIB_CASSERT(sub.sample_expansion_factor() == 1);
        
        const tensor& output_tensor = sub.get_output();

        DLIB_CASSERT(output_tensor.nr() == 1 & output_tensor.nc() == 1)
        DLIB_CASSERT(input_tensor.num_samples() == output_tensor.num_samples());
        
        const float* out_data = output_tensor.host();
        for (long i = 0; i < output_tensor.num_samples(); ++i)
        {
            //
            *iter++ = mat(out_data, output_tensor.k(), 1);
            // Skip pointer forward k positions.
            out_data += output_tensor.k();
        }
    }
    
    
    template <typename const_label_iterator, typename SUBNET>
    double compute_loss_value_and_gradient (
                                            const tensor& input_tensor,
                                            const_label_iterator truth,
                                            SUBNET& sub
                                            ) const
    {
        const tensor& output_tensor = sub.get_output();
        tensor& grad = sub.get_gradient_input();
        
        // Require
        DLIB_CASSERT(sub.sample_expansion_factor() == 1);
        DLIB_CASSERT(input_tensor.num_samples() != 0);
        DLIB_CASSERT(input_tensor.num_samples()%sub.sample_expansion_factor() == 0);
        DLIB_CASSERT(input_tensor.num_samples() == grad.num_samples());
        DLIB_CASSERT(input_tensor.num_samples() == output_tensor.num_samples());
        DLIB_CASSERT(output_tensor.nr() == 1 &&
                     output_tensor.nc() == 1);
        DLIB_CASSERT(grad.nr() == 1 &&
                     grad.nc() == 1);
        DLIB_CASSERT(grad.k() == output_tensor.k());
        const long k = output_tensor.k();
        for (long idx = 0; idx < output_tensor.num_samples(); ++idx)
        {
            const_label_iterator truth_matrix_ptr = (truth + idx);
            DLIB_CASSERT((*truth_matrix_ptr).nr() == k &&
                         (*truth_matrix_ptr).nc() == 1);
        }
        
        // The loss we output is the average loss over the mini-batch.
        const double scale = 1.0/output_tensor.num_samples();
        
        double loss = 0;
        
        float* g = grad.host_write_only();
        
        const float* out_data = output_tensor.host();
        
        matrix<float> ytrue;
        
        for (long i = 0; i < output_tensor.num_samples(); ++i)
        {
            ytrue = *truth++;
            for (long j = 0; j < output_tensor.k(); ++j)
            {
                const float y = ytrue(j, 0);
                const float temp1 = y - *out_data++; // diff
                const float temp2 = scale*temp1; // scaled diff
                loss += temp2*temp1; // loss
                *g = -temp2;
                ++g;
            }
            
        }
        return loss;
    }
    
    friend void serialize(const loss_mean_squared_multioutput_& , std::ostream& out)
    {
        serialize("loss_mean_squared_multioutput_", out);
    }
    
    friend void deserialize(loss_mean_squared_multioutput_& , std::istream& in)
    {
        std::string version;
        deserialize(version, in);
        if (version != "loss_mean_squared_multioutput_")
            throw serialization_error("Unexpected version found while deserializing dlib::loss_mean_squared_.");
    }
    
    friend std::ostream& operator<<(std::ostream& out, const loss_mean_squared_multioutput_& )
    {
        out << "loss_mean_squared_multioutput";
        return out;
    }
    
    friend void to_xml(const loss_mean_squared_multioutput_& /*item*/, std::ostream& out)
    {
        out << "<loss_mean_squared_multioutput/>";
    }
    
};

template <typename SUBNET>
using loss_mean_squared_multioutput = add_loss_layer<loss_mean_squared_multioutput_, SUBNET>;

    
} // namespace dlib
