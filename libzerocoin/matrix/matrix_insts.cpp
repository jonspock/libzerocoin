// Copyright (c) 2018 Jon Spock
#include "bignum.h"
#include "matrix.h"

namespace matrix
{
template matrix<CBigNum> operator+(const matrix<CBigNum>& m1, const matrix<CBigNum>& m2);
template matrix<CBigNum> operator+(const matrix<CBigNum>& m, CBigNum t);
template matrix<CBigNum> operator+(CBigNum t, const matrix<CBigNum>& m);
template matrix<CBigNum> operator-(const matrix<CBigNum>& m1, const matrix<CBigNum>& m2);
template matrix<CBigNum> operator-(const matrix<CBigNum>& m, CBigNum t);

template matrix<CBigNum> operator-(const matrix<CBigNum>& m);
template matrix<CBigNum> operator*(const matrix<CBigNum>& m, CBigNum t);
template matrix<CBigNum> operator*(CBigNum t, const matrix<CBigNum>& m);
} // namespace matrix
