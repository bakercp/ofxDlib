//
// Copyright (c) 2019 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <vector>
#include "dlib/lsh/hashes.h"
#include "dlib/graph_utils/find_k_nearest_neighbors_lsh.h"
#include "dlib/graph_utils/function_objects.h"
#include "ofLog.h"


namespace ofx {
namespace Dlib {


/// \brief A locality sensitive hash.
/// \tparam SampleType The sample type of the data being hashed.
/// \tparam The locality sensitive hash function. Conforms to dlib/lsh/hashes.h.
template <typename SampleType,
          typename DistanceFunctionType = dlib::squared_euclidean_distance,
          typename LSHashType = dlib::hash_similar_angles_64>
class LSHash
{
public:
    struct Settings;

    /// \brief Create an LSHash with default Settings.
    LSHash(): LSHash(Settings())
    {
    }

    /// \brief Create an LSHash with the given Settings.
    /// \param settings The settings to load.
    LSHash(const Settings& settings)
    {
        setup(Settings());
    }

    /// \brief Load new settings.
    ///
    /// This will clear all current samples and hashes.
    ///
    /// \param settings The settings to load.
    /// \returns true of the settings loaded correctly.
    bool setup(const Settings& settings)
    {
        _isLoaded = false;
        _samples.clear();
        _hashes.clear();
        _settings = settings;

        if (_settings.numThreads == 0)
            _effectiveNumThreads = std::thread::hardware_concurrency();

        if (_effectiveNumThreads == 0)
        {
            ofLogWarning("LSHash::setup") << "Unable to detect number of threads, setting manually. Defaulting to 1 thread.";
            _effectiveNumThreads = 1;
        }

        _isLoaded = true;
        return isLoaded();
    }

    /// \returns true if valid settings have been loaded.
    bool isLoaded() const
    {
        return _isLoaded;
    }

    std::vector<dlib::sample_pair> findKNearestNeighbors(std::size_t k,
                                                         std::size_t k_oversample = 20)
    {
        std::vector<dlib::sample_pair, std::allocator<dlib::sample_pair>> edges;

        if (_samples.size() <= 1)
            return edges;

        typedef dlib::impl::scan_find_k_nearest_neighbors_lsh<SampleType,
                                                              DistanceFunctionType,
                                                              LSHashType,
                                                              std::allocator<dlib::sample_pair>
        > scan_type;

//        scan_find_k_nearest_neighbors_lsh (
//                                           const vector_type& samples_,
//                                           const distance_function_type& dist_funct_,
//                                           const hash_function_type& hash_funct_,
//                                           const unsigned long k_,
//                                           std::vector<sample_pair, alloc>& edges_,
//                                           const unsigned long k_oversample_,
//                                           const std::vector<typename hash_function_type::result_type>& hashes_
//

        scan_type temp(_samples,
                       DistanceFunctionType(),
                       LSHashType(_settings.hashSeed),
                       k,
                       edges,
                       k_oversample,
                       _hashes);

        dlib::parallel_for(_effectiveNumThreads, 0, _hashes.size(), temp);

        dlib::remove_duplicate_edges(edges);

        return edges;
    }

    /// \brief Add and hash samples.
    /// \param samples The samples to add.
    void add(const std::vector<SampleType>& samples)
    {
        std::vector<typename LSHashType::result_type> hashes;
        hash(samples, hashes);

        _samples.insert(_samples.end(), samples.begin(), samples.end());
        _hashes.insert(_hashes.end(), hashes.begin(), hashes.end());
    }

    /// \brief Add samples to the hash space.
    /// \param samples The samples to add.
    /// \param hashes The pre-computed hashes to add.
    void add(const std::vector<SampleType>& samples,
             std::vector<typename LSHashType::result_type> hashes)
    {
        if (samples.size() != hashes.size())
        {
            ofLogError("LSHash::add") << "Precomputed samples.size() != hashes.size(), recomputing hashes.";
            add(samples);
            return;
        }

        _samples.insert(_samples.end(), samples.begin(), samples.end());
        _hashes.insert(_hashes.end(), hashes.begin(), hashes.end());
    }

    /// \brief Update all hashes.
    void rehash()
    {
        _hash(_samples, _hashes);
    }

    /// \returns the current samples.
    const std::vector<SampleType>& samples() const
    {
        return _samples;
    }

    /// \returns the current hashes.
    const std::vector<typename LSHashType::result_type>& hashes() const
    {
        return _hashes;
    }

    /// \brief The hash settings.
    struct Settings
    {
        /// \brief The number of threads to use.
        ///
        /// A value of 0 queries the hardware for maximum concurrent threads.
        std::size_t numThreads = 0;

        /// \brief The seed used for random locality sensitive hashing.
        std::size_t hashSeed = 0;
    };

    /// \brief Compute the hashes for the given samples.
    ///
    /// Does not add the samples to the sample instance.
    ///
    /// \param samples The samples to hash.
    /// \param hashes The results of the hash operation.
    void hash(const std::vector<SampleType>& samples,
              std::vector<typename LSHashType::result_type>& hashes)
    {
        // Call the multi-threaded hasher.
        dlib::hash_samples(*reinterpret_cast<const std::vector<dlib::vector<float, 2>>*>(&samples),
                           LSHashType(_settings.hashSeed),
                           _effectiveNumThreads,
                           hashes);
    }

private:
    /// \brief The settings for this object.
    Settings _settings;

    /// \brief The number of threads to use.
    std::size_t _effectiveNumThreads = 0;

    /// \brief True if the settings were loaded correctly.
    bool _isLoaded = false;

    /// \brief The data samples.
    std::vector<SampleType> _samples;

    /// \brief The calculated hases for the samples.
    mutable std::vector<typename LSHashType::result_type> _hashes;

};


} } // namespace ofx::Dlib
