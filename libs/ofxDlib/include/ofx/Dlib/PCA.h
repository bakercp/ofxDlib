//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


namespace ofx {
namespace Dlib {


/// \brief A class for caculating PCA.
///
/// The class calculates the principal components of a given 2D data matrix. By
/// default, data matrix is comprised of M columns and N rows. Each column
/// represents a data feature. Each row represents a single data sample.
template<typename T>
class PCA
{
public:
    struct Settings;

    /// \brief Create an empty PCA.
    PCA();

    /// \brief Create a PCA with the given parameters.
    ///
    /// \param data The data matrix. Features are in columns. Samples are in rows.
    /// \param targetOutputDimensions The desired number of output dimensions.
    ///        If zero, the minimum of data rows or columns will be used.
    PCA(const dlib::matrix<T>& data, std::size_t targetOutputDimensions = 0);

    /// \brief Load the PCA from the given path.
    /// \param path The path to load from.
    ///
    /// Check for a successful load by calling isLoaded().
    PCA(const std::string& path);

    /// \brief Destroy the PCA.
    ~PCA();

    /// \brief Project the given row matrix onto the PCA representation.
    /// \param data The matrix to project.
    /// \returns the projection of the given mat onto the PCA.
    dlib::matrix<T> project(const dlib::matrix<T>& data) const;

    /// \brief Unproject the given projection using the PCA representation.
    /// \param mat The matrix to unproject.
    /// \returns the unprojection of the given mat from the PCA.
    dlib::matrix<T> unproject(const dlib::matrix<T>& mat) const;

    /// \brief Calculate the PCA for the given matrix.
    ///
    /// \param data The data matrix. Features are in columns. Samples are in rows.
    /// \param targetOutputDimensions The desired number of output dimensions.
    ///        If zero, the minimum of data rows or columns will be used.
    void train(const dlib::matrix<T>& data, std::size_t targetOutputDimensions);

    /// \brief Calculate the PCA for the given matrix using the given settings.
    /// \param data The data matrix. Features are in columns. Samples are in rows.
    /// \param settings The settings to use.
    void train(const dlib::matrix<T>& data, Settings settings = Settings());

    /// \brief Save the PCA to the given path.
    /// \param path The path to save to.
    /// \returns true if the save was successful.
    bool save(const std::string& path) const;

    /// \brief Load the PCA from the given path.
    /// \param path The path to load from.
    /// \returns true if the load was successful.
    bool load(const std::string& path);

    enum class Method: int
    {
        /// \brief Compute PCA using the correlation matrix.
        ///
        /// If the number of samples is less than the number of features,
        /// the transposed (aka "scrambled") correlation will be used to
        /// significantly increase computation speed.
        EIGEN = 0,
        /// \brief Compute the PCA using the SVD of the data matrix.
        ///
        /// No correlation matrix will be calculated.
        SVD = 1,
        /// \brief Compute the PCA using a faster, less accurate SVD of the data matrix.
        ///
        /// No correlation matrix will be calculated.
        SVD_FAST = 2,
        /// \brief The default method.
        DEFAULT = EIGEN
    };

    struct Settings
    {
        /// \brief The computation method.
        Method method = Method::DEFAULT;

        /// \brief Subtract the mean from the input data.
        ///
        /// Set to false if the data is already centered.
        bool centerData = true;

        /// \brief Divide the data by its standard deviation.
        ///
        /// Set to false if the data is pre-scaled or shares a common scale.
        bool scaleData = true;

        /// \brief The default number of output dimensions.
        std::size_t targetOutputDimensions = 0;

        /// \brief The target amount of variance to capture (0 - 1).
        ///
        //  e.g. 0.99 will attempt to use enough output dimensions to capture
        /// 99% of the variance of the input data.
        /// If <= 0, the minimum of data rows or columns will be used.
        double targetVariance = 1.00;
    };

    /// \brief Clear all data.
    void clear();

    /// \returns true if the data is loaded.
    bool isLoaded() const;

    /// \returns the current settings.
    Settings settings() const;

    /// \returns the mean row matrix.
    const dlib::matrix<T>& mean() const;

    /// \returns the real eigenvectors, sorted by descending eigenvalue.
    const dlib::matrix<T>& eigenvectors() const;

    /// \returns the real eigenvectors, sorted by descending eigenvalue.
    const dlib::matrix<T>& eigenvalues() const;

private:
    /// \brief The settings used to create this analysis.
    Settings _settings;

    /// \brief The mean of the training data.
    dlib::matrix<T> _mean;

    /// \brief The real eigenvectors, sorted by descending eigenvalue.
    dlib::matrix<T> _eigenvectors;

    /// \brief The eigenvalues of the correlation matrix, in descending order.
    dlib::matrix<T> _eigenvalues;

};


template<typename T>
PCA<T>::PCA()
{
}


template<typename T>
PCA<T>::PCA(const dlib::matrix<T>& data, std::size_t targetOutputDimensions)
{
    train(data, targetOutputDimensions);
}


template<typename T>
PCA<T>::PCA(const std::string& path)
{
    load(path);
}


template<typename T>
PCA<T>::~PCA()
{
}


template<typename T>
dlib::matrix<T> PCA<T>::project(const dlib::matrix<T>& data) const
{
    dlib::matrix<T> result;
    return result;
}


template<typename T>
dlib::matrix<T> PCA<T>::unproject(const dlib::matrix<T>& mat) const
{
    dlib::matrix<T> result;
    return result;
}


template<typename T>
void PCA<T>::train(const dlib::matrix<T>& data, std::size_t targetOutputDimensions)
{
    Settings settings;
    settings.targetOutputDimensions = targetOutputDimensions;
    train(data, settings);
}


template<typename T>
void PCA<T>::train(const dlib::matrix<T>& _data, Settings settings)
{
    dlib::matrix<T> data = _data;

    std::size_t numSamples = data.nr();
    std::size_t dimensionsIn = data.nc();
    std::size_t dimensionsOut = 0;

    bool useScrambled = data.nc() > data.nr();

    if (settings.centerData)
    {
        // First, we center and normalize our data. For a more discussion see:
        // See Part XI: Principal components analysis
        //    http://cs229.stanford.edu/notes/cs229-notes10.pdf

        // First we "center" the data by subtracting its mean. For data that has
        // a known mean of zero (e.g. most audio signals) this can be skipped.
        // 8-bit images, for example have values that range from 0-255, thus they
        // likely don't have a mean of zero and should be centered.

        // Calculate the sum, divide it by the number of rows.
        _mean = dlib::sum_rows(data) / data.nr();

        // Center the data by subtracting the mean.
        for (std::size_t row = 0; row < data.nr(); ++row)
            dlib::set_rowm(data, row) -= _mean;
    }

    if (settings.scaleData)
    {
        // If your vector features are measured using different units (i.e. they
        // exist on different scales), it is important to normalize your data,
        // otherwise sorting the eigen vectors by the eigen values will yield an
        // order that is biased by the units (scaling). If the units (scaling) are
        // the same (e.g. most digital images channel values fall between 0 - 255)
        // then you can skip normalization. If not you can normalize your data
        // by dividing each value by the standard deviaion (or another appropriate
        // measure of variance (e.g. max - min).
        dlib::matrix<T> stdev = dlib::zeros_matrix<T>(dimensionsIn, 0);

        // Calculate the mean of xs. First calculate the sums.
        // We have already subtracted the means in this case (i.e. the mean
        // is already zero, so we just square values).
        for (std::size_t row = 0; row < data.nr(); ++row)
        {
            // Add the square of the current row to the variance.
            stdev += dlib::squared(dlib::rowm(data, row));
        }

        // Divide the sum by the number of rows to get the variance.
        stdev /= data.nr();

        // Take the square root to get the standard deviation.
        stdev = dlib::sqrt(stdev);

        // Normalize the data by dividing by the standard deviation.
        for (std::size_t row = 0; row < data.nr(); ++row)
            set_rowm(data, row) = dlib::pointwise_multiply(dlib::rowm(data, row), 1.0 / stdev);
    }

    // Caculate the correlation matrix.
    dlib::matrix<T> cor;

    if (useScrambled)
        cor = data * dlib::trans(data) / (data.nr() - 1);
    else
        cor = dlib::trans(data) * data / (data.nr() - 1);

    // Create the decomposition object.
    // We ensure that the correlation is symmetric by calling dlib::make_ymmetric.
    dlib::eigenvalue_decomposition<dlib::matrix<T>> E(dlib::make_symmetric(cor));

    // Get the real eigen vectors.
    _eigenvectors = E.get_pseudo_v();

    // Get the real eigenvalues * an identity matrix.
    _eigenvalues = E.get_real_eigenvalues();

    // Sort the eigenvectors and values by eigenvalue size.
    dlib::rsort_columns(_eigenvectors, _eigenvalues);

    // No determine the number of output dimensions.
    if (settings.targetOutputDimensions != 0)
    {
        dimensionsOut = settings.targetOutputDimensions;
    }
    else if (settings.targetVariance > 0 && settings.targetVariance < 1)
    {
        // Some of the eigenvalues might be negative. So first lets zero those
        // out so they won't get considered.
        _eigenvalues = dlib::pointwise_multiply(_eigenvalues > 0, _eigenvalues);

        // Figure out how many eigenvectors we want.
        const double thresh = dlib::sum(_eigenvalues) * settings.targetVariance;

        double total = 0;

        for (std::size_t r = 0; r < _eigenvalues.size() && total < thresh; ++r)
        {
            // Don't even think about looking at eigenvalues that are 0.
            // If we go this far then we have all we need.
            if (_eigenvalues(r) == 0)
                break;

            ++dimensionsOut;
            total += _eigenvalues(r);
        }

        if (dimensionsOut == 0)
            ofLogError() << "While performing pca, all eigenvalues were negative or 0.";
    }
    else
    {
        dimensionsOut = std::min(data.nr(), data.nc());
    }

    // Unscramble the data if needed.
    if (useScrambled)
        _eigenvectors = dlib::trans(data) * _eigenvectors;

    // So now we know we want to use num_vectors of the first eigenvectors.  So
    // pull those out and discard the rest.
    _eigenvectors = dlib::trans(dlib::colm(_eigenvectors, dlib::range(0, dimensionsOut - 1)));

    // also clip off the eigenvalues we aren't using
    _eigenvalues = dlib::rowm(_eigenvalues, dlib::range(0, dimensionsOut - 1));

    // Normalize the eigenvectors.
    for (std::size_t row = 0; row < _eigenvectors.nr(); ++row)
        dlib::set_rowm(_eigenvectors, row) = dlib::normalize(dlib::rowm(_eigenvectors, row));

    // Save the settings.
    _settings = settings;

}


template<typename T>
bool PCA<T>::save(const std::string& path) const
{
    try
    {
        typedef typename std::underlying_type<Method>::type safe_type;

        dlib::serialize(ofToDataPath(path, true))
        << _mean
        << _eigenvectors
        << _eigenvalues
        << static_cast<safe_type>(_settings.method)
        << _settings.centerData
        << _settings.scaleData
        << _settings.targetOutputDimensions
        << _settings.targetVariance;
    }
    catch (const std::exception& exc)
    {
        ofLogError("PCA::save") << "Error loading: " << path << ", " << exc.what();
        return false;
    }

    return true;
}


template<typename T>
bool PCA<T>::load(const std::string& path)
{
    try
    {
        typedef typename std::underlying_type<Method>::type safe_type;
        safe_type method;

        dlib::deserialize(ofToDataPath(path, true))
        >> _mean
        >> _eigenvectors
        >> _eigenvalues
        >> method
        >> _settings.centerData
        >> _settings.scaleData
        >> _settings.targetOutputDimensions
        >> _settings.targetVariance;

        _settings.method = static_cast<Method>(method);

    }
    catch (const std::exception& exc)
    {
        ofLogError("PCA::load") << "Error loading: " << path << ", " << exc.what();
        return false;
    }

    return true;
}


template<typename T>
void PCA<T>::clear()
{
    _mean.clear();
    _eigenvectors.clear();
    _eigenvalues.clear();
}


template<typename T>
bool PCA<T>::isLoaded() const
{
    return _mean.size() > 0 && _eigenvectors.size() > 0 && _eigenvalues.size() > 0;
}


template<typename T>
typename PCA<T>::Settings PCA<T>::settings() const
{
    return _settings;
}


template<typename T>
const dlib::matrix<T>& PCA<T>::mean() const
{
    return _mean;
}


template<typename T>
const dlib::matrix<T>& PCA<T>::eigenvectors() const
{
    return _eigenvectors;
}


template<typename T>
const dlib::matrix<T>& PCA<T>::eigenvalues() const
{
    return _eigenvalues;
}


} } // namespace ofx::Dlib
