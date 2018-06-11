//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxDlib.h"


// These just allow us to print our type names as strings for easier reading during testing.
template <typename T> struct TypeName { static const char* Get() { return typeid(T).name(); }};
template <> struct TypeName<int> { static const char* Get() { return "int"; }};
template <> struct TypeName<uint8_t> { static const char* Get() { return "uint8_t"; }};
template <> struct TypeName<uint16_t> { static const char* Get() { return "uint16_t"; }};
template <> struct TypeName<float> { static const char* Get() { return "float"; }};
template <> struct TypeName<int16_t> { static const char* Get() { return "int16_t"; }};
template <> struct TypeName<dlib::rgb_alpha_pixel> { static const char* Get() { return "dlib::rgb_alpha_pixel"; }};
template <> struct TypeName<dlib::rgb_pixel> { static const char* Get() { return "dlib::rgb_pixel"; }};
template <> struct TypeName<dlib::bgr_pixel> { static const char* Get() { return "dlib::bgr_pixel"; }};
template <> struct TypeName<dlib::hsi_pixel> { static const char* Get() { return "dlib::hsi_pixel"; }};
template <> struct TypeName<dlib::lab_pixel> { static const char* Get() { return "dlib::lab_pixel"; }};


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;

    std::vector<std::pair<std::string, ofTexture>> textures;

    template<typename of_pixel_type>
    void test_load_of_image_auto(ofPixels_<of_pixel_type>& pixels, bool normalize = false)
    {
        dlib::load_image(pixels, ofToDataPath("Ada.png", true));
        std::stringstream ss;
        ss << "ofPixels<" << std::endl;
        ss << "  " << TypeName<of_pixel_type>::Get() << std::endl;
        ss << ">" << std::endl;

        add(ss.str(), pixels, normalize);
    }


    template<typename dlib_pixel_type>
    void test_display_dlib_image(dlib::array2d<dlib_pixel_type>& dlib_img, bool normalize = false)
    {
        dlib::load_image(dlib_img, ofToDataPath("Ada.png", true));
        auto p = ofxDlib::toOf(dlib_img);

        std::stringstream ss;
        ss << "dlib::array2d<" << std::endl;
        ss << "  " << TypeName<dlib_pixel_type>::Get() << "," << std::endl;
        ss << ">" << std::endl;
        ss << "as " << ofToString(p.getPixelFormat()) << std::endl;

        add(ss.str(), p, normalize);
    }

    template<typename dlib_pixel_type>
    void test_display_dlib_image(dlib::matrix<dlib_pixel_type>& dlib_img, bool normalize = false)
    {
        dlib::load_image(dlib_img, ofToDataPath("Ada.png", true));
        auto p = ofxDlib::toOf(dlib_img);

        std::stringstream ss;
        ss << "dlib::matrix<" << std::endl;
        ss << "  " << TypeName<dlib_pixel_type>::Get() << "," << std::endl;
        ss << ">" << std::endl;
        ss << "as " << ofToString(p.getPixelFormat()) << std::endl;

        add(ss.str(), p, normalize);
    }


    template<typename dlib_pixel_type, typename of_pixel_type>
    void test_load_of_image(ofPixels_<of_pixel_type>& pixels, bool normalize = false)
    {
        auto dlib_img = ofxDlib::toDlib<dlib_pixel_type>(pixels);
        dlib::load_image(dlib_img, ofToDataPath("Ada.png", true));
        std::stringstream ss;
        ss << "of_image<" << std::endl;
        ss << "  " << TypeName<dlib_pixel_type>::Get() << "," << std::endl;;
        ss << "  " << TypeName<of_pixel_type>::Get() << std::endl;
        ss << ">" << std::endl;;

        add(ss.str(), pixels, normalize);
    }


    template<typename of_pixel_type>
    void add(const std::string& title, const ofPixels_<of_pixel_type>& pixels, bool normalize)
    {
        ofPixels_<of_pixel_type> _pixels = pixels;

        // Normalize non 8-bit types.
        if (normalize)
        {
            ofxDlib::map(_pixels, of_pixel_type(0), of_pixel_type(255));
        }

        textures.push_back({ title, ofTexture() });
        textures.back().second.loadData(_pixels);
    }
};
