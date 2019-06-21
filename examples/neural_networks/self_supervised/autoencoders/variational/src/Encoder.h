//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


namespace dlib {
    
    
class gaussian_sampler_
{
public:
    explicit gaussian_sampler_(float mean = 0.0, float stddev = 1.0): _mean(mean), _stddev(stddev)
    {
    }
    
    float get_mean () const { return _mean; }
    float get_stddev () const { return _stddev; }

    template <typename SUBNET>
    void setup (const SUBNET& /*sub*/)
    {
    }
    
    void forward_inplace(const tensor& input, tensor& output)
    {
       // tt::affine_transform(output, input, val);
    }
    
    inline dpoint map_input_to_output (const dpoint& p) const { return p; }
    inline dpoint map_output_to_input (const dpoint& p) const { return p; }
    
    void backward_inplace(const tensor& gradient_input,
                          tensor& data_grad,
                          tensor& /*params_grad*/
    )
    {
//        if (is_same_object(gradient_input, data_grad))
//            tt::affine_transform(data_grad, gradient_input, val);
//        else
//            tt::affine_transform(data_grad, data_grad, gradient_input, 1, val);
    }
    
    const tensor& get_layer_params() const { return params; }
    tensor& get_layer_params() { return params; }
    
    friend void serialize(const gaussian_sampler_& item, std::ostream& out)
    {
//        serialize("multiply_", out);
//        serialize(item.val, out);
    }
    
    friend void deserialize(gaussian_sampler_& item, std::istream& in)
    {
//        std::string version;
//        deserialize(version, in);
//        if (version == "dropout_")
//        {
//            // Since we can build a multiply_ from a dropout_ we check if that's what
//            // is in the stream and if so then just convert it right here.
//            unserialize sin(version, in);
//            dropout_ temp;
//            deserialize(temp, sin);
//            item = temp;
//            return;
//        }
//
//        if (version != "multiply_")
//            throw serialization_error("Unexpected version '"+version+"' found while deserializing dlib::multiply_.");
//        deserialize(item.val, in);
    }
    
    friend std::ostream& operator<<(std::ostream& out, const gaussian_sampler_& item)
    {
//        out << "	multiply ("
//        << "val="<<item.val
//        << ")";
        return out;
    }
    
    friend void to_xml(const gaussian_sampler_& item, std::ostream& out)
    {
//        out << "<multiply"
//        << " val='"<<item.val<<"'";
//        out << "/>\n";
    }
private:
    float _mean = 0.0;
    float _stddev = 1.0;

    resizable_tensor params; // unused
};

    
template <typename SUBNET>
using gaussian_sampler = add_layer<gaussian_sampler_, SUBNET>;

    
} // namespace dlib
