

#ifndef OPENCV_CORE_CVSTDINL_HPP
#define OPENCV_CORE_CVSTDINL_HPP

#include <complex>
#include <ostream>



#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif

namespace cv
{

template<typename _Tp> class DataType< std::complex<_Tp> >
{
public:
    typedef std::complex<_Tp>  value_type;
    typedef value_type         work_type;
    typedef _Tp                channel_type;

    enum { generic_type = 0,
           depth        = DataType<channel_type>::depth,
           channels     = 2,
           fmt          = DataType<channel_type>::fmt + ((channels - 1) << 8),
           type         = CV_MAKETYPE(depth, channels) };

    typedef Vec<channel_type, channels> vec_type;
};

static inline
std::ostream& operator << (std::ostream& out, Ptr<Formatted> fmtd)
{
    fmtd->reset();
    for(const char* str = fmtd->next(); str; str = fmtd->next())
        out << str;
    return out;
}

static inline
std::ostream& operator << (std::ostream& out, const Mat& mtx)
{
    return out << Formatter::get()->format(mtx);
}

static inline
std::ostream& operator << (std::ostream& out, const UMat& m)
{
    return out << m.getMat(ACCESS_READ);
}

template<typename _Tp> static inline
std::ostream& operator << (std::ostream& out, const Complex<_Tp>& c)
{
    return out << "(" << c.re << "," << c.im << ")";
}

template<typename _Tp> static inline
std::ostream& operator << (std::ostream& out, const std::vector<Point_<_Tp> >& vec)
{
    return out << Formatter::get()->format(Mat(vec));
}


template<typename _Tp> static inline
std::ostream& operator << (std::ostream& out, const std::vector<Point3_<_Tp> >& vec)
{
    return out << Formatter::get()->format(Mat(vec));
}


template<typename _Tp, int m, int n> static inline
std::ostream& operator << (std::ostream& out, const Matx<_Tp, m, n>& matx)
{
    return out << Formatter::get()->format(Mat(matx));
}

template<typename _Tp> static inline
std::ostream& operator << (std::ostream& out, const Point_<_Tp>& p)
{
    out << "[" << p.x << ", " << p.y << "]";
    return out;
}

template<typename _Tp> static inline
std::ostream& operator << (std::ostream& out, const Point3_<_Tp>& p)
{
    out << "[" << p.x << ", " << p.y << ", " << p.z << "]";
    return out;
}

template<typename _Tp, int n> static inline
std::ostream& operator << (std::ostream& out, const Vec<_Tp, n>& vec)
{
    out << "[";
    if (cv::traits::Depth<_Tp>::value <= CV_32S)
    {
        for (int i = 0; i < n - 1; ++i) {
            out << (int)vec[i] << ", ";
        }
        out << (int)vec[n-1] << "]";
    }
    else
    {
        for (int i = 0; i < n - 1; ++i) {
            out << vec[i] << ", ";
        }
        out << vec[n-1] << "]";
    }

    return out;
}

template<typename _Tp> static inline
std::ostream& operator << (std::ostream& out, const Size_<_Tp>& size)
{
    return out << "[" << size.width << " x " << size.height << "]";
}

template<typename _Tp> static inline
std::ostream& operator << (std::ostream& out, const Rect_<_Tp>& rect)
{
    return out << "[" << rect.width << " x " << rect.height << " from (" << rect.x << ", " << rect.y << ")]";
}

static inline std::ostream& operator << (std::ostream& out, const MatSize& msize)
{
    int i, dims = msize.dims();
    for( i = 0; i < dims; i++ )
    {
        out << msize[i];
        if( i < dims-1 )
            out << " x ";
    }
    return out;
}

static inline std::ostream &operator<< (std::ostream &s, cv::Range &r)
{
    return s << "[" << r.start << " : " << r.end << ")";
}

}

#ifdef _MSC_VER
#pragma warning( pop )
#endif



#endif
