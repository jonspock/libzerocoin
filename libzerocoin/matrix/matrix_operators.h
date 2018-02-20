// Copyright (c) 2018 Jon Spock
#pragma once
#include <vector>

namespace matrix
{
//-------------------- Templated functions --------------------------
// Add a scalar to each element
template <class T>
inline matrix<T> operator+(const matrix<T>& m, T t)
{
    matrix<T> r(m.num_rows(), m.num_cols());
    for (int i = 0; i < r.len(); i++)
        r[i] = m[i] + t;
    return r;
}
template <class T>
inline matrix<T> operator+(T t, const matrix<T>& m)
{
    return (m + t);
}

// Subtract a scalar from each element
template <class T>
inline matrix<T> operator-(const matrix<T>& m, T t)
{
    matrix<T> r(m.num_rows(), m.num_cols());
    for (int i = 0; i < r.len(); i++)
        r[i] = m[i] - t;
    return r;
}

template <class T>
inline matrix<T> operator+(const matrix<T>& m1, const matrix<T>& m2)
{
    matrix<T> r(m1.num_rows(), m1.num_cols());

    for (int i = 0; i < r.num_cols(); i++) {
        for (int j = 0; j < r.num_rows(); j++)
            r(j, i) = m1(j, i) + m2(j, i);
    }
    return r;
}
template <class T>
inline matrix<T> operator-(const matrix<T>& m1, const matrix<T>& m2)
{
    matrix<T> r(m1.num_rows(), m1.num_cols());

    for (int i = 0; i < r.num_cols(); i++) {
        for (int j = 0; j < r.num_rows(); j++)
            r(j, i) = m1(j, i) - m2(j, i);
    }
    return r;
}

// Negate a Matrix
template <class T>
inline matrix<T> operator-(const matrix<T>& m)
{
    matrix<T> r(m.num_rows(), m.num_cols());
    for (int i = 0; i < r.len(); i++)
        r[i] = -m[i];
    return r;
}

template <class T>
inline matrix<T> operator*(const matrix<T>& m1, const matrix<T>& m2)
{
    matrix<T> res(m1.num_rows(), m2.num_cols());
    for (int r = 0; r < m1.num_rows(); r++) {
        for (int c = 0; c < m2.num_cols(); c++) {
            T sum = 0;
            for (int i = 0; i < m1.num_cols(); i++) {
                sum += m1(r, i) * m2(i, c);
            }
            res(r, c) = sum;
        }
    }
    return res;
}

// Multiply matrix by a scalar
template <class T>
inline matrix<T> operator*(const matrix<T>& m, T t)
{
    matrix<T> r(m.num_rows(), m.num_cols());
    for (int i = 0; i < r.len(); i++)
        r[i] = m[i] * t;
    return r;
}
template <class T>
inline matrix<T> operator*(T t, const matrix<T>& m)
{
    return (m * t);
}

template <class T>
inline std::vector<T> operator*(const std::vector<T>& m1, const matrix<T>& m2)
{
    std::vector<T> res(m2.num_cols(), (T)0);
    for (int c = 0; c < m2.num_cols(); c++) {
        for (size_t i = 0; i < m1.size(); i++)
            res[c] += m1[i] * m2(i, c);
    }
    return res;
}


} // namespace matrix
