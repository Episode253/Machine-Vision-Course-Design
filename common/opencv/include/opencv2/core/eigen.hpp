


#ifndef OPENCV_CORE_EIGEN_HPP
#define OPENCV_CORE_EIGEN_HPP

#include "opencv2/core.hpp"

#if defined _MSC_VER && _MSC_VER >= 1200
#pragma warning( disable: 4714 )
#pragma warning( disable: 4127 )
#pragma warning( disable: 4244 )
#endif

namespace cv
{




template<typename _Tp, int _rows, int _cols, int _options, int _maxRows, int _maxCols> static inline
void eigen2cv( const Eigen::Matrix<_Tp, _rows, _cols, _options, _maxRows, _maxCols>& src, OutputArray dst )
{
    if( !(src.Flags & Eigen::RowMajorBit) )
    {
        Mat _src(src.cols(), src.rows(), traits::Type<_Tp>::value,
              (void*)src.data(), src.outerStride()*sizeof(_Tp));
        transpose(_src, dst);
    }
    else
    {
        Mat _src(src.rows(), src.cols(), traits::Type<_Tp>::value,
                 (void*)src.data(), src.outerStride()*sizeof(_Tp));
        _src.copyTo(dst);
    }
}


template<typename _Tp, int _rows, int _cols, int _options, int _maxRows, int _maxCols> static inline
void eigen2cv( const Eigen::Matrix<_Tp, _rows, _cols, _options, _maxRows, _maxCols>& src,
               Matx<_Tp, _rows, _cols>& dst )
{
    if( !(src.Flags & Eigen::RowMajorBit) )
    {
        dst = Matx<_Tp, _cols, _rows>(static_cast<const _Tp*>(src.data())).t();
    }
    else
    {
        dst = Matx<_Tp, _rows, _cols>(static_cast<const _Tp*>(src.data()));
    }
}

template<typename _Tp, int _rows, int _cols, int _options, int _maxRows, int _maxCols> static inline
void cv2eigen( const Mat& src,
               Eigen::Matrix<_Tp, _rows, _cols, _options, _maxRows, _maxCols>& dst )
{
    CV_DbgAssert(src.rows == _rows && src.cols == _cols);
    if( !(dst.Flags & Eigen::RowMajorBit) )
    {
        const Mat _dst(src.cols, src.rows, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        if( src.type() == _dst.type() )
            transpose(src, _dst);
        else if( src.cols == src.rows )
        {
            src.convertTo(_dst, _dst.type());
            transpose(_dst, _dst);
        }
        else
            Mat(src.t()).convertTo(_dst, _dst.type());
    }
    else
    {
        const Mat _dst(src.rows, src.cols, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        src.convertTo(_dst, _dst.type());
    }
}


template<typename _Tp, int _rows, int _cols, int _options, int _maxRows, int _maxCols> static inline
void cv2eigen( const Matx<_Tp, _rows, _cols>& src,
               Eigen::Matrix<_Tp, _rows, _cols, _options, _maxRows, _maxCols>& dst )
{
    if( !(dst.Flags & Eigen::RowMajorBit) )
    {
        const Mat _dst(_cols, _rows, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        transpose(src, _dst);
    }
    else
    {
        const Mat _dst(_rows, _cols, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        Mat(src).copyTo(_dst);
    }
}

template<typename _Tp>  static inline
void cv2eigen( const Mat& src,
               Eigen::Matrix<_Tp, Eigen::Dynamic, Eigen::Dynamic>& dst )
{
    dst.resize(src.rows, src.cols);
    if( !(dst.Flags & Eigen::RowMajorBit) )
    {
        const Mat _dst(src.cols, src.rows, traits::Type<_Tp>::value,
             dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        if( src.type() == _dst.type() )
            transpose(src, _dst);
        else if( src.cols == src.rows )
        {
            src.convertTo(_dst, _dst.type());
            transpose(_dst, _dst);
        }
        else
            Mat(src.t()).convertTo(_dst, _dst.type());
    }
    else
    {
        const Mat _dst(src.rows, src.cols, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        src.convertTo(_dst, _dst.type());
    }
}


template<typename _Tp, int _rows, int _cols> static inline
void cv2eigen( const Matx<_Tp, _rows, _cols>& src,
               Eigen::Matrix<_Tp, Eigen::Dynamic, Eigen::Dynamic>& dst )
{
    dst.resize(_rows, _cols);
    if( !(dst.Flags & Eigen::RowMajorBit) )
    {
        const Mat _dst(_cols, _rows, traits::Type<_Tp>::value,
             dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        transpose(src, _dst);
    }
    else
    {
        const Mat _dst(_rows, _cols, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        Mat(src).copyTo(_dst);
    }
}

template<typename _Tp> static inline
void cv2eigen( const Mat& src,
               Eigen::Matrix<_Tp, Eigen::Dynamic, 1>& dst )
{
    CV_Assert(src.cols == 1);
    dst.resize(src.rows);

    if( !(dst.Flags & Eigen::RowMajorBit) )
    {
        const Mat _dst(src.cols, src.rows, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        if( src.type() == _dst.type() )
            transpose(src, _dst);
        else
            Mat(src.t()).convertTo(_dst, _dst.type());
    }
    else
    {
        const Mat _dst(src.rows, src.cols, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        src.convertTo(_dst, _dst.type());
    }
}


template<typename _Tp, int _rows> static inline
void cv2eigen( const Matx<_Tp, _rows, 1>& src,
               Eigen::Matrix<_Tp, Eigen::Dynamic, 1>& dst )
{
    dst.resize(_rows);

    if( !(dst.Flags & Eigen::RowMajorBit) )
    {
        const Mat _dst(1, _rows, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        transpose(src, _dst);
    }
    else
    {
        const Mat _dst(_rows, 1, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        src.copyTo(_dst);
    }
}


template<typename _Tp> static inline
void cv2eigen( const Mat& src,
               Eigen::Matrix<_Tp, 1, Eigen::Dynamic>& dst )
{
    CV_Assert(src.rows == 1);
    dst.resize(src.cols);
    if( !(dst.Flags & Eigen::RowMajorBit) )
    {
        const Mat _dst(src.cols, src.rows, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        if( src.type() == _dst.type() )
            transpose(src, _dst);
        else
            Mat(src.t()).convertTo(_dst, _dst.type());
    }
    else
    {
        const Mat _dst(src.rows, src.cols, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        src.convertTo(_dst, _dst.type());
    }
}


template<typename _Tp, int _cols> static inline
void cv2eigen( const Matx<_Tp, 1, _cols>& src,
               Eigen::Matrix<_Tp, 1, Eigen::Dynamic>& dst )
{
    dst.resize(_cols);
    if( !(dst.Flags & Eigen::RowMajorBit) )
    {
        const Mat _dst(_cols, 1, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        transpose(src, _dst);
    }
    else
    {
        const Mat _dst(1, _cols, traits::Type<_Tp>::value,
                 dst.data(), (size_t)(dst.outerStride()*sizeof(_Tp)));
        Mat(src).copyTo(_dst);
    }
}



}

#endif
