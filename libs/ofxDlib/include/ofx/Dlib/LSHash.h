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
#include "ofVectorMath.h"
#include "ofLog.h"


namespace ofx {
namespace Dlib {


/// \brief Helpers for getting data pointer.
template <typename VectorType, typename FloatType>
const FloatType* VectorDataPointer(const VectorType& v) { return v.data(); }

template <>
inline const float* VectorDataPointer(const ofVec2f& v) { return v.getPtr(); }

template <>
inline const float* VectorDataPointer(const ofVec3f& v) { return v.getPtr(); }

template <>
inline const float* VectorDataPointer(const ofVec4f& v) { return v.getPtr(); }

template <>
inline const float* VectorDataPointer(const glm::vec2& v) { return &v[0]; }

template <>
inline const float* VectorDataPointer(const glm::vec3& v) { return &v[0]; }

template <>
inline const float* VectorDataPointer(const glm::vec4& v) { return &v[0]; }


/// \brief Helpers for getting data vector dimensions.
template <typename VectorType>
struct VectorDataDim { static const int DIM = -1; };

template <>
struct VectorDataDim<ofVec2f> { static const int DIM = ofVec2f::DIM; };

template <>
struct VectorDataDim<ofVec3f> { static const int DIM = ofVec3f::DIM; };

template <>
struct VectorDataDim<ofVec4f> { static const int DIM = ofVec4f::DIM; };

template <>
struct VectorDataDim<glm::vec2> { static const int DIM = 2; };//glm::vec2::components; };

template <>
struct VectorDataDim<glm::vec3> { static const int DIM = 3; };//glm::vec3::components; };

template <>
struct VectorDataDim<glm::vec4> { static const int DIM = 4; };//glm::vec4::components; };

template <typename FloatType, std::size_t N>
struct VectorDataDim<std::array<FloatType, N>> { static const int DIM = N; };


/// \brief A locality sensitive hash.
/// \tparam VectorType The sample type of the data being hashed.
/// \tparam SampleTypeDimension The number of dimensions in the sample vectors.
/// \tparam DistanceFunctionType The distance function.
/// \tparam The locality sensitive hash function. Conforms to dlib/lsh/hashes.h.
template <typename VectorType,
          int VectorDimension = VectorDataDim<VectorType>::DIM,
          typename FloatType = float,
          typename IndexType = std::size_t,
          typename DistanceFunctionType = dlib::squared_euclidean_distance,
          typename LSHashType = dlib::hash_similar_angles_64>
class LSHash
{
public:
    typedef dlib::vector<FloatType, VectorDimension> DlibVectorType;
    typedef std::vector<DlibVectorType> DlibSamplesType;

    struct Settings;

    /// \brief Create an unloaded LSHash.
    LSHash()
    {
    }

    /// \brief Create an LSHash with the given Settings.
    /// \param settings The settings to load.
    LSHash(const Settings& settings)
    {
        setup(settings);
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

        if (settings.numThreads == 0)
            _effectiveNumThreads = std::thread::hardware_concurrency();

        if (_effectiveNumThreads == 0)
        {
            ofLogWarning("LSHash::setup") << "Unable to detect number of threads, setting manually. Defaulting to 1 thread.";
            _effectiveNumThreads = 1;
        }

        _hasher = LSHashType(_settings.hashSeed);

        _settings = settings;
        _isLoaded = true;
        return isLoaded();
    }

    /// \returns true if valid settings have been loaded.
    bool isLoaded() const
    {
        return _isLoaded;
    }

    std::vector<std::pair<std::size_t, float>> findKNearestLabels(const VectorType& target,
                                                                  std::size_t k,
                                                                  std::size_t k_oversample = 20)
    {
//        std::priority_queue<std::pair<unsigned long, unsigned long>> best_hashes;
//        std::priority_queue<dlib::sample_pair, std::vector<dlib::sample_pair>, dlib::impl::compare_sample_pair_with_distance> best_samples;




        typename LSHashType::result_type targetHash = hash(target);

        const unsigned long k_hash = k*k_oversample;

        std::vector<dlib::sample_pair, std::allocator<dlib::sample_pair>> edges;

        std::priority_queue<std::pair<unsigned long, unsigned long>> best_hashes;
        std::priority_queue<dlib::sample_pair, std::vector<dlib::sample_pair>, dlib::impl::compare_sample_pair_with_distance> best_samples;

        unsigned long worst_distance = std::numeric_limits<unsigned long>::max();

        const DlibVectorType* pTarget = reinterpret_cast<const DlibVectorType*>(&target);
        const DlibSamplesType* pSamples = reinterpret_cast<const DlibSamplesType*>(&_samples);

        // scan over the hashes and find the best matches for hashes[i]
        for (unsigned long j = 0; j < _hashes.size(); ++j)
        {
            const unsigned long dist = _hasher.distance(targetHash, _hashes[j]);

            if (dist < worst_distance || best_hashes.size() < k_hash)
            {
                if (best_hashes.size() >= k_hash)
                    best_hashes.pop();
                best_hashes.push(std::make_pair(dist, j));
                worst_distance = best_hashes.top().first;
            }
        }

        DistanceFunctionType dist_funct;

        // Now figure out which of the best_hashes are actually the k best matches
        // according to dist_funct()
        while (best_hashes.size() != 0)
        {
            const unsigned long j = best_hashes.top().second;
            best_hashes.pop();

            const double dist = dist_funct(*pTarget, (*pSamples)[j]);
            if (dist < std::numeric_limits<double>::infinity())
            {
                if (best_samples.size() >= k)
                    best_samples.pop();
                best_samples.push(dlib::sample_pair(-1,j,dist));
            }
        }


        std::vector<std::pair<std::size_t, float>> results;

        while (best_samples.size() != 0)
        {
            auto top = best_samples.top();
            //edges.push_back(best_samples.top());
            results.push_back(std::make_pair(top.index1(), top.distance()));
            best_samples.pop();
        }

//        for (auto&& sample: best_samples)
//        {
//            labels.push_back(_labels[index1]);
//        }





        return results;



//        // Finally, now put the k best matches according to dist_funct() into edges
//        auto_mutex lock(m);
//        while (best_samples.size() != 0)
//        {
//            edges.push_back(best_samples.top());
//            best_samples.pop();
//        }
//





























//        std::size_t sampleSize = k_oversample * k_oversample;
//
//        bool multipleLabels = false;
//
//        unsigned int bestDistance = std::numeric_limits<unsigned int>::max();
//        unsigned int worstDistance = std::numeric_limits<unsigned int>::lowest();

//        for (std::size_t i = 0; i < _hashes.size(); ++i)
//        {
//            unsigned int distance = _hasher.distance(targetHash, _hashes[i]);
//            if (distance <)
//        }


        // if hashes.size <= k_oversample*k_oversample



//        std::vector<dlib::sample_pair, std::allocator<dlib::sample_pair>> edges;
//
//        typedef dlib::impl::scan_find_k_nearest_neighbors_lsh<DlibVectorType,
//                                                              DistanceFunctionType,
//                                                              LSHashType,
//                                                              std::allocator<dlib::sample_pair>
//        > scan_type;
//
//        std::vector<VectorType> samples = { target };
//
//        scan_type temp(*reinterpret_cast<const DlibVectorType*>(&samples),
//                       DistanceFunctionType(),
//                       LSHashType(_settings.hashSeed),
//                       k,
//                       edges,
//                       k_oversample,
//                       _hashes);
//
//        dlib::parallel_for(_effectiveNumThreads, 0, _hashes.size(), temp);

//        dlib::remove_duplicate_edges(edges);

//        for (auto&& edge: edges)
//        {
//            std::cout << edge.index1() << " " << edge.index2() << " " << edge.distance() << std::endl;
//        }
//        std::cout << "--" << std::endl;

       // return labels;
    }

    /// \brief Add and hash samples.
    /// \param samples The samples to add.
    /// \param labels The labels for each sample.
    void add(const std::vector<VectorType>& samples,
             const std::vector<std::size_t>& labels)
    {
        if (samples.size() != labels.size())
        {
            ofLogError("LSHash::add") << "samples.size() != labels.size(), rejecting.";
            return;
        }

        std::vector<typename LSHashType::result_type> hashes;
        hash(samples, hashes);

        _samples.insert(_samples.end(), samples.begin(), samples.end());
        _labels.insert(_labels.end(), labels.begin(), labels.end());
        _hashes.insert(_hashes.end(), hashes.begin(), hashes.end());
    }

    /// \brief Add samples to the hash space.
    /// \param samples The samples to add.
    /// \param labels The labels for each sample.
    /// \param hashes The pre-computed hashes to add.
    void add(const std::vector<VectorType>& samples,
             const std::vector<std::size_t>& labels,
             std::vector<typename LSHashType::result_type> hashes)
    {
        if (samples.size() != labels.size())
        {
            ofLogError("LSHash::add") << "samples.size() != labels.size(), rejecting.";
            return;
        }

        if (samples.size() != hashes.size())
        {
            ofLogError("LSHash::add") << "Precomputed samples.size() != hashes.size(), recomputing hashes.";
            add(samples);
            return;
        }

        _samples.insert(_samples.end(), samples.begin(), samples.end());
        _labels.insert(_labels.end(), labels.begin(), labels.end());
        _hashes.insert(_hashes.end(), hashes.begin(), hashes.end());
    }

    /// \brief Update all hashes.
    void rehash()
    {
        _hash(_samples, _hashes);
    }

    /// \returns the current samples.
    const std::vector<VectorType>& samples() const
    {
        return _samples;
    }

    /// \returns the current samples.
    const std::vector<std::size_t>& labels() const
    {
        return _labels;
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


    /// \brief Compute the hash for one samples.
    ///
    /// Does not add the samples to the sample instance.
    ///
    /// \param sample The sample to hash.
    /// \returns the hashed value.
    typename LSHashType::result_type hash(const VectorType& sample)
    {
        return _hasher(*reinterpret_cast<const DlibVectorType*>(&sample));
    }



    void hash(const std::vector<VectorType>& samples,
              std::vector<typename LSHashType::result_type>& hashes)
    {
        // Call the multi-threaded hasher.
        dlib::hash_samples(*reinterpret_cast<const DlibSamplesType*>(&samples),
                           _hasher,
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
    std::vector<VectorType> _samples;

    /// \brief The data labels.
    std::vector<std::size_t> _labels;

    /// \brief The calculated hases for the samples.
    mutable std::vector<typename LSHashType::result_type> _hashes;

    /// \brief The hasher object used by this object.
    LSHashType _hasher;

};


} } // namespace ofx::Dlib
