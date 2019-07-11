//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <regex>
#include <thread>
#include "FreeImage.h"
#include "ofLog.h"
#include "ofThreadChannel.h"
#include "ofPixels.h"
#include "ofImage.h"


namespace ofx {
namespace Dlib {


/// \brief A class representing a collection of images with like dimensions.
template<typename PixelType>
class ImageCollection
{
public:
    /// \brief Image parameters.
    struct ImageHeader
    {
        std::size_t width = 0;
        std::size_t height = 0;
        std::size_t bpp = 0;
    };

    struct Settings;

    ImageCollection()
    {
    }

    ImageCollection(const std::string& directoryPath)
    {
        load(directoryPath);
    }

    ~ImageCollection()
    {
    }

    std::size_t width() const
    {
        return _width;
    }

    std::size_t height() const
    {
        return _height;
    }

    ofPixelFormat pixelFormat() const
    {
        return _pixelFormat;
    }

    std::size_t size() const
    {
        return _pixels.size();
    }

    void clear()
    {
        _settings = Settings();
        _paths.clear();
        _pixels.clear();
    }

    bool load(const std::string& directoryPath)
    {
        Settings settings;
        settings.directoryPath = directoryPath;
        return load(settings);
    }

    /// \returns true if at least one image was loaded.
    bool load(const Settings& settings)
    {
        clear();

        _settings = settings;

        std::unique_ptr<std::regex> regex = nullptr;

        ofThreadChannel<std::filesystem::path> paths;

        if (_settings.filenameRegex.size() > 0)
        {
            try
            {
                regex = std::make_unique<std::regex>(_settings.filenameRegex);
            }
            catch (const std::regex_error& e)
            {
                ofLogError("ImageCollection::load") << "regex_error caught: " << e.what();
                return false;
            }
        }

        if (_settings.recursiveSearch)
        {
            for(auto&& p: std::filesystem::recursive_directory_iterator(ofToDataPath(_settings.directoryPath, true)))
                if (!regex || std::regex_match(p.path().string(), *regex.get()))
                    paths.send(p.path());
        }
        else
        {
            for(auto&& p: std::filesystem::directory_iterator(ofToDataPath(_settings.directoryPath, true)))
                if (!regex || std::regex_match(p.path().string(), *regex.get()))
                    paths.send(p.path());
        }

        ofThreadChannel<std::pair<std::filesystem::path, ofPixels_<PixelType>>> pixelChannel;

        auto imageLoader = [&]()
        {
            while (!paths.empty())
            {
                std::filesystem::path path;
                if (paths.receive(path))
                {
                    ofPixels_<PixelType> pixels;
                    ofLoadImage(pixels, path);
                    pixelChannel.send(std::make_pair(std::move(path), std::move(pixels)));
                }
            }
        };

        if (settings.numThreads == 0)
            _effectiveNumThreads = std::thread::hardware_concurrency();

        if (_effectiveNumThreads == 0)
        {
            ofLogWarning("ImageCollection::load") << "Unable to detect number of threads, setting manually. Defaulting to 0 additional thread.";
            _effectiveNumThreads = 0;
        }

        if (_effectiveNumThreads > 0)
        {
            std::vector<std::thread> threads(_effectiveNumThreads);
            for (auto& t: threads) t = std::thread(imageLoader);
            for (auto& t: threads) t.join();
        }
        else imageLoader();

        while (!pixelChannel.empty())
        {
            std::pair<std::filesystem::path, ofPixels_<PixelType>> p;
            pixelChannel.receive(p);

            // Did the image get loaded correctly?
            if (p.second.isAllocated())
            {
                if (!isLoaded())
                {
                    _width = p.second.getWidth();
                    _height = p.second.getHeight();
                    _pixelFormat = p.second.getPixelFormat();
                }

                if (_width == p.second.getWidth()
                && _height == p.second.getHeight()
                && _pixelFormat == p.second.getPixelFormat())
                {
                    _paths.emplace_back(std::move(p.first));
                    _pixels.emplace_back(std::move(p.second));
                }
                else  ofLogWarning("ImageCollection::load") << "Invalid dimensions: " << p.first;
            }
            else  ofLogWarning("ImageCollection::load") << "Unable to load: " << p.first;
        }

        return isLoaded();
    }


    bool isLoaded() const
    {
        return !_pixels.empty() && !_paths.empty();
    }

    Settings settings() const
    {
        return _settings;
    }

    /// \brief Returns a matrix representation.
    ///
    /// Each row is an image and each column is a collection of pixel values.
//    const dlib::matrix<PixelType> as_matrix() const
//    {
//
//
//
//        return _pixels;
//    }

    const std::vector<std::filesystem::path> paths() const
    {
        return _paths;
    }


    const std::vector<ofPixels_<PixelType>> pixels() const
    {
        return _pixels;
    }

    struct Settings
    {
//        /// \brief The desired width.
//        ///
//        /// If set to zero, this will be set to the width of the first image
//        /// found.
//        std::size_t width = 0;
//
//        /// \brief The desired width.
//        ///
//        /// If set to zero, this will be set to the height of the first image
//        /// found.
//        std::size_t height = 0;
//
        /// \brief The desired pixel type.
        ///
        /// If set to OF_PIXELS_UNKNOWN, this will be set to the pixel format of
        /// the first image found.
        ofPixelFormat pixelFormat = OF_PIXELS_UNKNOWN;

        /// \brief If this is set to true, images will be resized.
        ///
        /// If the width / height are not specified, then all images will be
        /// resized to the size of the first image found.
        //bool resizeImages = false;

        /// \brief If this is set to true, images will be reformatted.
        ///
        /// If the pixelFormat is not specified, then all images will be
        /// reformatted to the format of the first image found.
        //bool reformatImage = false;

        /// \brief The folder to search.
        std::string directoryPath;

        /// \brief Search the given folder recursively.
        bool recursiveSearch = false;

        /// \brief The filename regular expression used to match files.
        ///
        /// This can be used to match any part of a path. If it matches, the
        /// image will be loaded.
        std::string filenameRegex = ".*(png|jpg)$";

        /// \brief Set the maximum number of images to load.
        /// If the maximum number of images is loaded, stop searching and
        /// return. If set to 0, all images will be returned.
        //std::size_t maximumImages = 0;

        /// \brief Use multiple threads to load images.
        ///
        /// If numThreads is equal to 0, the number of threads will be chosen
        /// automatically, usually based on the number of processor cores.
        std::size_t numThreads = 0;
    };

    /// \brief Load image header information without loading pixels data.
    /// \param header The header data to fill.
    /// \param path the path of the image.
    /// \param loadPixelsIfRequired If header loading is not supported, load the full image.
    /// \returns true if successful.
    static bool loadHeader(ImageHeader& header,
                           const std::filesystem::path& path,
                           bool loadPixelsIfRequired = true)
    {
        int flags = FIF_LOAD_NOPIXELS;

        auto fif = FreeImage_GetFIFFromFilename(path.string().c_str());

        if (FreeImage_FIFSupportsNoPixels(fif))
        {
            FIBITMAP* dib = FreeImage_Load(fif, path.string().c_str(), flags);

            if (dib)
            {
                header.width = FreeImage_GetWidth(dib);
                header.height = FreeImage_GetHeight(dib);
                header.bpp = FreeImage_GetBPP(dib);

                FreeImage_Unload(dib);
                return true;
            }
        }


        if (loadPixelsIfRequired)
        {
            ofLogWarning("ImageUtils::loadHeader") << "Loading full pixels, may be slow: " << path;
            ofPixels pixels;
            if (ofLoadImage(pixels, path))
            {
                header.width = pixels.getWidth();
                header.height = pixels.getHeight();
                header.bpp = pixels.getBitsPerPixel();
                return true;
            }
        }

        return false;
    }

private:
    /// \brief The search settings used.
    Settings _settings;

    std::size_t _effectiveNumThreads = 0;

    /// \brief The width of the images in the collection.
    std::size_t _width = 0;

    /// \brief The height of the images in the collection.
    std::size_t _height = 0;

    /// \brief The pixel type of the images in the collection.
    ofPixelFormat _pixelFormat = OF_PIXELS_UNKNOWN;

    /// \brief A list of the paths that were loaded.
    std::vector<std::filesystem::path> _paths;

    /// \brief A list of the pixels that where loaded.
    std::vector<ofPixels_<PixelType>> _pixels;

};


} } // namespace ofx::Dlib
