

#ifndef OPENCV_FLANN_DATASET_H_
#define OPENCV_FLANN_DATASET_H_

#include <stdio.h>

#include "general.h"

namespace cvflann
{


template <typename T>
class Matrix
{
public:
    typedef T type;

    size_t rows;
    size_t cols;
    size_t stride;
    T* data;

    Matrix() : rows(0), cols(0), stride(0), data(NULL)
    {
    }

    Matrix(T* data_, size_t rows_, size_t cols_, size_t stride_ = 0) :
        rows(rows_), cols(cols_),  stride(stride_), data(data_)
    {
        if (stride==0) stride = cols;
    }


    CV_DEPRECATED void free()
    {
        fprintf(stderr, "The cvflann::Matrix<T>::free() method is deprecated "
                "and it does not do any memory deallocation any more.  You are"
                "responsible for deallocating the matrix memory (by doing"
                "'delete[] matrix.data' for example)");
    }


    T* operator[](size_t index) const
    {
        return data+index*stride;
    }
};


class UntypedMatrix
{
public:
    size_t rows;
    size_t cols;
    void* data;
    flann_datatype_t type;

    UntypedMatrix(void* data_, long rows_, long cols_) :
        rows(rows_), cols(cols_), data(data_)
    {
    }

    ~UntypedMatrix()
    {
    }


    template<typename T>
    Matrix<T> as()
    {
        return Matrix<T>((T*)data, rows, cols);
    }
};



}

#endif
