// Copyright (c) 2018 Jon Spock
#pragma once
#include <vector>

namespace matrix

{
template <class T>
class vector_zero
{
public:
    vector_zero() { datasize = 0; }
    vector_zero(int start, int end)
    {
        datasize = 0;
        int in_size = end - start + 1;
        positive_offset = -start;
        resize(in_size);
    }
    vector_zero(const vector_zero<T>& m)
    {
        datasize = 0;
        resize(m.datasize);
        positive_offset = m.positive_offset;
        for (size_t i = 0; i < data.size(); i++)
            data[i] = m.data[i];
    }
    ~vector_zero() { ; }
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
    inline T operator()(int index) const { return data[index + positive_offset]; }
    inline T operator[](int index) const { return data[index + positive_offset]; }

    inline T& operator()(int index) { return data[index + positive_offset]; }
    inline T& operator[](int index) { return data[index + positive_offset]; }

    void operator=(T t)
    {
        for (int i = 0; i < datasize; i++)
            data[i] = t;
    }
    void operator=(const vector_zero<T>& m)
    {
        resize(m.datasize);
        positive_offset = m.positive_offset;
        if (m.datasize == 0)
            return;
        for (size_t i = 0; i < data.size(); i++)
            data[i] = m.data[i];
    }
    void operator=(const std::vector<T>& v)
    {
        resize(v.size());
        for (int i = 0; i < data.size(); i++)
            data[i] = v[i];
    }

    void resize(int r)
    {
        if (datasize == r) {
            return;
        }
        if (r == 0) return;
        datasize = r;
        data.resize(datasize);
    }

protected:
    int datasize;
    int positive_offset;
    std::vector<T> data;
};

} // namespace matrix
