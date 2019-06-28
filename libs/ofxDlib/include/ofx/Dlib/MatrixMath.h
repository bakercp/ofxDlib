//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include "dlib/matrix.h"


namespace ofx {
namespace Dlib {


///// \brief Calculate the column mean.
///// \param matrix The input dlib vector.
///// \tparam T vector data type.
///// \returns the column mean.
//template<typename T>
//inline const glm::tvec4<T>& toOf(const dlib::vector<T, 4>& v)
//{
//    return *reinterpret_cast<const glm::tvec4<T>*>(&v);
//}
//    dlib::matrix<double, 1, 0> mean(A.nc());
//    for (std::size_t col = 0; col < A.nc(); ++col)
//        mean(col) = dlib::mean(dlib::colm(A, col));

/// \brief Calculate the column mean.
/// \param matrix The input dlib matrix.
/// \tparam T matrix data type.
/// \returns the column mean.
template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, dlib::matrix<T, 1, 0>>::type row_mean(const dlib::matrix<T>& a)
{
    return dlib::sum_rows(a) / a.nr();

//    dlib::matrix<T, 1, 0> mean(a.nc());
//    for (std::size_t col = 0; col < a.nc(); ++col)
//        mean(col) = dlib::mean(dlib::colm(a, col));
//    return mean;
}


/// \brief Calculate the row mean.
/// \param matrix The input dlib matrix.
/// \tparam T matrix data type.
/// \returns the row mean.
template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, dlib::matrix<T, 0, 1>>::type col_mean(const dlib::matrix<T>& a)
{
//    dlib::matrix<T, 0, 1> mean(a.nr());
//    for (std::size_t row = 0; row < a.nr(); ++row)
//        mean(row) = dlib::mean(dlib::rowm(a, row));
//    return mean;

    return dlib::sum_cols(a) / a.nc();
}


template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, dlib::matrix<T>>::type center(const dlib::matrix<T>& _X)
{
    dlib::matrix<T> X = _X;

    // Calculate the row mean.
    dlib::matrix<T> mean = row_mean(X);

    // Remove mean from each row.
    for (std::size_t row = 0; row < X.nr(); ++row)
        dlib::set_rowm(X, row) -= mean;

    return X;
}


/// \brief Calculate the covariance matrix.
///
/// The input matrix must have each sample on as a row, and each feature as a column.
///
/// \param matrix The input dlib matrix.
/// \tparam T matrix data type.
/// \returns the covariance matrix.
/// \sa http://www.fil.ion.ucl.ac.uk/~wpenny/course/matrices.pdf
/// \sa https://www.itl.nist.gov/div898/handbook/pmc/section5/pmc541.htm
/// \sa https://stackoverflow.com/questions/26444525/how-do-i-calculate-the-covariance-matrix-without-any-built-in-functions-or-loops
/// \sa https://www.mathworks.com/help/matlab/ref/cov.html
template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, dlib::matrix<T>>::type cov(const dlib::matrix<T>& _X)
{
    dlib::matrix<T> X = _X;

    // Calculate the row mean.
    dlib::matrix<T> mean = row_mean(X);

    // Remove mean from each row.
    for (std::size_t row = 0; row < X.nr(); ++row)
        dlib::set_rowm(X, row) -= mean;

    // Caculate the covariance of X.
    return dlib::trans(X) * X / (X.nr() - 1);
}


} } // namespace ofx::Dlib
