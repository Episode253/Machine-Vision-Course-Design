


#ifndef OPENCV_FLANN_SAMPLING_H_
#define OPENCV_FLANN_SAMPLING_H_

#include "matrix.h"
#include "random.h"

namespace cvflann
{

template<typename T>
Matrix<T> random_sample(Matrix<T>& srcMatrix, long size, bool remove = false)
{
    Matrix<T> newSet(new T[size * srcMatrix.cols], size,srcMatrix.cols);

    T* src,* dest;
    for (long i=0; i<size; ++i) {
        long r = rand_int((int)(srcMatrix.rows-i));
        dest = newSet[i];
        src = srcMatrix[r];
        std::copy(src, src+srcMatrix.cols, dest);
        if (remove) {
            src = srcMatrix[srcMatrix.rows-i-1];
            dest = srcMatrix[r];
            std::copy(src, src+srcMatrix.cols, dest);
        }
    }
    if (remove) {
        srcMatrix.rows -= size;
    }
    return newSet;
}

template<typename T>
Matrix<T> random_sample(const Matrix<T>& srcMatrix, size_t size)
{
    UniqueRandom rand((int)srcMatrix.rows);
    Matrix<T> newSet(new T[size * srcMatrix.cols], size,srcMatrix.cols);

    T* src,* dest;
    for (size_t i=0; i<size; ++i) {
        long r = rand.next();
        dest = newSet[i];
        src = srcMatrix[r];
        std::copy(src, src+srcMatrix.cols, dest);
    }
    return newSet;
}

}


#endif
