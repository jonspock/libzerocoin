// Copyright (c) 2018 Jon Spock
#pragma once
#include <vector>

namespace tensor
{
template <class T>
class tensor
{
public:
    tensor() { datasize = x = y = z = 0; }
    tensor(int in_x, int in_y, int in_z)
    {
        datasize = 0;
        resize(in_x, in_y, in_z);
    }
    tensor(const tensor<T>& m)
    {
        datasize = 0;
        resize(m.x, m.y, m.z);
        for (size_t i = 0; i < data.size(); i++)
            data[i] = m.data[i];
    }
    ~tensor() { ; }
    int size_x() const { return x; }
    int size_y() const { return y; }
    int size_z() const { return z; }
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
    inline T operator()(int X, int Y, int Z) const { return data[X * y * z + Y * z + Z]; }
    inline T& operator()(int X, int Y, int Z) { return data[X * y * z + Y * z + Z]; }

    void operator=(T t)
    {
        for (int i = 0; i < datasize; i++)
            data[i] = t;
    }
    void operator=(const tensor<T>& m)
    {
        resize(m.x, m.y, m.z);
        if (m.datasize == 0)
            return;
        for (size_t i = 0; i < data.size(); i++)
            data[i] = m.data[i];
    }
    void operator=(const std::vector<T>& v)
    {
        resize(v.size(), 1, 1);
        for (int i = 0; i < data.size(); i++)
            data[i] = v[i];
    }

    void resize(int a, int b, int c)
    {
        if (datasize == a * b * c) {
            x = a;
            y = b;
            z = c;
            return;
        }
        if (a == 0 || b == 0 || c == 0)
            return;
        datasize = a * b * c;
        data.resize(datasize);
        x = a;
        y = b;
        z = c;
    }

protected:
    int datasize, x, y, z;
    std::vector<T> data;
};

} // namespace tensor
