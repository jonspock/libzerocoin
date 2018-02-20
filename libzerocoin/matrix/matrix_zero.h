// Copyright (c) 2018 Jon Spock
#pragma once
#include <vector>

namespace matrix

{
template <class T>
class matrix_zero
{
public:
    matrix_zero() { datasize = rows = cols = 0; }
    matrix_zero(int inrow, int incol, int offset)
    {
        positive_offset = offset;
        datasize = 0;
        resize(inrow, incol);
    }
    matrix_zero(int inrow, int incol)
    {
        datasize = 0;
        positive_offset = (inrow - 1) / 2;
        resize(inrow, incol);
    }
    matrix_zero(const matrix_zero<T>& m)
    {
        datasize = 0;
        resize(m.rows, m.cols);
        for (size_t i = 0; i < data.size(); i++)
            data[i] = m.data[i];
    }
    ~matrix_zero() { ; }
    int num_cols() const { return cols; }
    int num_rows() const { return rows; }
    int len() const { return datasize; }
    void reset()
    {
        for (int i = 0; i < datasize; i++)
            data[i] = T(0);
    }
    void copy_data(const std::vector<T>& d)
    {
        for (size_t i = 0; i < data.size(); i++)
            data[i] = d[i];
    }
    std::vector<T> get_data()
    {
        std::vector<T> d(data);
        return d;
    }
    inline T operator()(int R, int C) const { return data[(R + positive_offset) * cols + C]; }
    //    inline T operator()(int index) const { return data[index]; }
    //    inline T operator[](int index) const { return data[index]; }

    inline T& operator()(int R, int C) { return data[(R + positive_offset) * cols + C]; }
    //    inline T& operator()(int index) { return data[index]; }
    //inline T& operator[](int index) { return data[index]; }

    //inline T& col(int C) { return &data[C * cols]; } // TBD

    inline std::vector<T> get_row(int R) const
    {
        std::vector<T> r(cols);
        for (int i = 0; i < cols; i++)
            r[i] = data[(R + positive_offset) * cols + i];
        return r;
    }

    inline std::vector<T> get_iterated_row(int R) const
    {
        std::vector<T> r(cols);
        for (int i = 0; i < cols; i++)
            r[i] = data[R * cols + i];
        return r;
    }

    inline std::vector<T> get_col(int R) const
    {
        std::vector<T> r(rows);
        for (int i = 0; i < rows; i++)
            r[i] = data[(i + positive_offset) * cols + R];
        return r;
    }

    void operator=(T t)
    {
        for (int i = 0; i < datasize; i++)
            data[i] = t;
    }
    void operator=(const matrix_zero<T>& m)
    {
        resize(m.rows, m.cols);
        if (m.datasize == 0)
            return;
        for (size_t i = 0; i < data.size(); i++)
            data[i] = m.data[i];
    }
    void operator=(const std::vector<T>& v)
    {
        resize(v.size(), 1);
        for (int i = 0; i < data.size(); i++)
            data[i] = v[i];
    }

    void resize(int r, int c)
    {
        if (datasize == r * c) {
            rows = r;
            cols = c;
            return;
        }
        if (r == 0 || c == 0)
            return;
        datasize = r * c;
        data.resize(datasize);
        rows = r;
        cols = c;
    }

protected:
    int datasize, rows, cols;
    int positive_offset;
    std::vector<T> data;
};

} // namespace matrix
