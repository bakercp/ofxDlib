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
#if !defined(__OPTIMIZE__)
        ofLogWarning() << "Dlib performs poorly when compiled in debug mode. Use Release Mode for speed!";
#endif

        ofPixels pixels;
        ofLoadImage(pixels, "faces/2007_007763.png");

        ofLogNotice() << "Pixel size: " << pixels.getWidth() << ", " << pixels.getHeight();

        resizeTest(1000, pixels);
        rotateTest(1000, pixels);

        ofxTestEq(true, true, "First Test.");
    }

    void resizeTest(std::size_t iterations,
                    const ofPixels& pixels)
    {
        auto copies = makeCopies(pixels, iterations);

        for (float scale = 0.25; scale < 1.5; scale += 1)
        {
            std::cout << "------------ SCALE " << scale << std::endl;
            std::size_t newWidth = std::round(pixels.getWidth() * scale);
            std::size_t newHeight = std::round(pixels.getHeight() * scale);

            ofPixels targetPix;
            targetPix.allocate(newWidth, newHeight, copies[0].getNumChannels());

            {
                auto _copies = copies;
                timerStart();
                for (auto&& p: _copies)
                    p.resizeTo(targetPix, OF_INTERPOLATE_NEAREST_NEIGHBOR);
                timerStop("OF_INTERPOLATE_NEAREST_NEIGHBOR");
            }
            {
                auto _copies = copies;
                timerStart();
                for (auto&& p: _copies)
                    p.resizeTo(targetPix, OF_INTERPOLATE_BICUBIC);
                timerStop("OF_INTERPOLATE_BICUBIC");
            }
            {
                dlib::interpolate_nearest_neighbor interp;
                auto _copies = copies;
                timerStart();
                for (auto&& p: _copies)
                    dlib::resize_image(p, targetPix, interp);
                timerStop("dlib::interpolate_nearest_neighbor");
            }
            {
                dlib::interpolate_bilinear interp;
                auto _copies = copies;
                timerStart();
                for (auto&& p: _copies)
                    dlib::resize_image(p, targetPix, interp);
                timerStop("dlib::interpolate_bilinear");
            }
            {
                dlib::interpolate_quadratic interp;
                auto _copies = copies;
                timerStart();
                for (auto&& p: _copies)
                    dlib::resize_image(p, targetPix, interp);
                timerStop("dlib::interpolate_quadratic");
            }
        }
    }

    void rotateTest(std::size_t iterations,
                    const ofPixels& pixels)
    {
        auto copies = makeCopies(pixels, iterations);

        for (int i = 0; i < 4; ++i)
        {
            ofPixels targetPix;

            if (i == 0 || i == 2)
                targetPix.allocate(pixels.getWidth(), pixels.getHeight(), copies[0].getNumChannels());
            else
                targetPix.allocate(pixels.getHeight(), pixels.getWidth(), copies[0].getNumChannels());

            {
                auto _copies = copies;
                timerStart();
                for (auto&& p: _copies)
                    p.rotate90To(targetPix, i);
                timerStop("OF_ROTATE_90 " + ofToString(i) + " times");
            }
            {
                auto _copies = copies;
                timerStart();
                for (auto&& p: _copies)
                    ofxDlib::rotate_image_90(p, targetPix, i);

                timerStop("rotate dlib::rotate90 " + ofToString(i) + " times");
            }
        }
    }


    void timerStart()
    {
        start = std::chrono::steady_clock::now();
    }

    void timerStop(const std::string& message)
    {
        end = std::chrono::steady_clock::now();
        ofLogNotice() << message << "\t" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms.";
    }

    std::vector<ofPixels> makeCopies(const ofPixels& pixels, std::size_t numCopies)
    {
        std::vector<ofPixels> copies;
        for (std::size_t i = 0; i < numCopies; ++i)
            copies.push_back(pixels);
        return copies;
    }

    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
};


int main()
{
    ofInit();
    auto window = make_shared<ofAppNoWindow>();
    auto app = make_shared<ofApp>();
    ofRunApp(window, app);
    return ofRunMainLoop();
}
