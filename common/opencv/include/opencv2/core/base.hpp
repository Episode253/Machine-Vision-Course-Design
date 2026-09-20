

#ifndef OPENCV_CORE_BASE_HPP
#define OPENCV_CORE_BASE_HPP

#ifndef __cplusplus
#  error base.hpp header must be compiled as C++
#endif

#include "opencv2/opencv_modules.hpp"

#include <climits>
#include <algorithm>

#include "opencv2/core/cvdef.h"
#include "opencv2/core/cvstd.hpp"

namespace cv
{




namespace Error {

enum Code {
    StsOk=                       0,
    StsBackTrace=               -1,
    StsError=                   -2,
    StsInternal=                -3,
    StsNoMem=                   -4,
    StsBadArg=                  -5,
    StsBadFunc=                 -6,
    StsNoConv=                  -7,
    StsAutoTrace=               -8,
    HeaderIsNull=               -9,
    BadImageSize=              -10,
    BadOffset=                 -11,
    BadDataPtr=                -12,
    BadStep=                   -13,
    BadModelOrChSeq=           -14,
    BadNumChannels=            -15,
    BadNumChannel1U=           -16,
    BadDepth=                  -17,
    BadAlphaChannel=           -18,
    BadOrder=                  -19,
    BadOrigin=                 -20,
    BadAlign=                  -21,
    BadCallBack=               -22,
    BadTileSize=               -23,
    BadCOI=                    -24,
    BadROISize=                -25,
    MaskIsTiled=               -26,
    StsNullPtr=                -27,
    StsVecLengthErr=           -28,
    StsFilterStructContentErr= -29,
    StsKernelStructContentErr= -30,
    StsFilterOffsetErr=        -31,
    StsBadSize=                -201,
    StsDivByZero=              -202,
    StsInplaceNotSupported=    -203,
    StsObjectNotFound=         -204,
    StsUnmatchedFormats=       -205,
    StsBadFlag=                -206,
    StsBadPoint=               -207,
    StsBadMask=                -208,
    StsUnmatchedSizes=         -209,
    StsUnsupportedFormat=      -210,
    StsOutOfRange=             -211,
    StsParseError=             -212,
    StsNotImplemented=         -213,
    StsBadMemBlock=            -214,
    StsAssert=                 -215,
    GpuNotSupported=           -216,
    GpuApiCallError=           -217,
    OpenGlNotSupported=        -218,
    OpenGlApiCallError=        -219,
    OpenCLApiCallError=        -220,
    OpenCLDoubleNotSupported=  -221,
    OpenCLInitError=           -222,
    OpenCLNoAMDBlasFft=        -223
};
}







enum DecompTypes {

    DECOMP_LU       = 0,

    DECOMP_SVD      = 1,

    DECOMP_EIG      = 2,

    DECOMP_CHOLESKY = 3,

    DECOMP_QR       = 4,

    DECOMP_NORMAL   = 16
};



enum NormTypes {

                NORM_INF       = 1,

                 NORM_L1        = 2,

                 NORM_L2        = 4,

                 NORM_L2SQR     = 5,

                 NORM_HAMMING   = 6,

                 NORM_HAMMING2  = 7,
                 NORM_TYPE_MASK = 7,
                 NORM_RELATIVE  = 8,
                 NORM_MINMAX    = 32
               };


enum CmpTypes { CMP_EQ = 0,
                CMP_GT = 1,
                CMP_GE = 2,
                CMP_LT = 3,
                CMP_LE = 4,
                CMP_NE = 5
              };


enum GemmFlags { GEMM_1_T = 1,
                 GEMM_2_T = 2,
                 GEMM_3_T = 4
               };

enum DftFlags {

    DFT_INVERSE        = 1,

    DFT_SCALE          = 2,

    DFT_ROWS           = 4,

    DFT_COMPLEX_OUTPUT = 16,

    DFT_REAL_OUTPUT    = 32,

    DFT_COMPLEX_INPUT  = 64,

    DCT_INVERSE        = DFT_INVERSE,

    DCT_ROWS           = DFT_ROWS
};



enum BorderTypes {
    BORDER_CONSTANT    = 0,
    BORDER_REPLICATE   = 1,
    BORDER_REFLECT     = 2,
    BORDER_WRAP        = 3,
    BORDER_REFLECT_101 = 4,
    BORDER_TRANSPARENT = 5,

    BORDER_REFLECT101  = BORDER_REFLECT_101,
    BORDER_DEFAULT     = BORDER_REFLECT_101,
    BORDER_ISOLATED    = 16
};







CV_EXPORTS CV_NORETURN void error(int _code, const String& _err, const char* _func, const char* _file, int _line);

#ifdef CV_STATIC_ANALYSIS



#define CV_Error(code, msg) do { (void)(code); (void)(msg); abort(); } while (0)
#define CV_Error_(code, args) do { (void)(code); (void)(cv::format args); abort(); } while (0)
#define CV_Assert( expr ) do { if (!(expr)) abort(); } while (0)

#else


#define CV_Error( code, msg ) cv::error( code, msg, CV_Func, __FILE__, __LINE__ )


#define CV_Error_( code, args ) cv::error( code, cv::format args, CV_Func, __FILE__, __LINE__ )


#define CV_Assert( expr ) do { if(!!(expr)) ; else cv::error( cv::Error::StsAssert, #expr, CV_Func, __FILE__, __LINE__ ); } while(0)

#endif


#if !defined(__OPENCV_BUILD)
#ifndef CV_ErrorNoReturn
#define CV_ErrorNoReturn CV_Error
#endif
#ifndef CV_ErrorNoReturn_
#define CV_ErrorNoReturn_ CV_Error_
#endif
#endif

#define CV_Assert_1 CV_Assert
#define CV_Assert_2( expr, ... ) CV_Assert_1(expr); __CV_EXPAND(CV_Assert_1( __VA_ARGS__ ))
#define CV_Assert_3( expr, ... ) CV_Assert_1(expr); __CV_EXPAND(CV_Assert_2( __VA_ARGS__ ))
#define CV_Assert_4( expr, ... ) CV_Assert_1(expr); __CV_EXPAND(CV_Assert_3( __VA_ARGS__ ))
#define CV_Assert_5( expr, ... ) CV_Assert_1(expr); __CV_EXPAND(CV_Assert_4( __VA_ARGS__ ))
#define CV_Assert_6( expr, ... ) CV_Assert_1(expr); __CV_EXPAND(CV_Assert_5( __VA_ARGS__ ))
#define CV_Assert_7( expr, ... ) CV_Assert_1(expr); __CV_EXPAND(CV_Assert_6( __VA_ARGS__ ))
#define CV_Assert_8( expr, ... ) CV_Assert_1(expr); __CV_EXPAND(CV_Assert_7( __VA_ARGS__ ))
#define CV_Assert_9( expr, ... ) CV_Assert_1(expr); __CV_EXPAND(CV_Assert_8( __VA_ARGS__ ))
#define CV_Assert_10( expr, ... ) CV_Assert_1(expr); __CV_EXPAND(CV_Assert_9( __VA_ARGS__ ))

#define CV_Assert_N(...) do { __CV_EXPAND(__CV_CAT(CV_Assert_, __CV_VA_NUM_ARGS(__VA_ARGS__)) (__VA_ARGS__)); } while(0)



#if defined _DEBUG || defined CV_STATIC_ANALYSIS
#  define CV_DbgAssert(expr) CV_Assert(expr)
#else

#  define CV_DbgAssert(expr)
#endif


struct CV_EXPORTS Hamming
{
    static const NormTypes normType = NORM_HAMMING;
    typedef unsigned char ValueType;
    typedef int ResultType;


    ResultType operator()( const unsigned char* a, const unsigned char* b, int size ) const;
};

typedef Hamming HammingLUT;



template<typename _Tp> inline _Tp cv_abs(_Tp x) { return std::abs(x); }
inline int cv_abs(uchar x) { return x; }
inline int cv_abs(schar x) { return std::abs(x); }
inline int cv_abs(ushort x) { return x; }
inline int cv_abs(short x) { return std::abs(x); }

template<typename _Tp, typename _AccTp> static inline
_AccTp normL2Sqr(const _Tp* a, int n)
{
    _AccTp s = 0;
    int i=0;
#if CV_ENABLE_UNROLLED
    for( ; i <= n - 4; i += 4 )
    {
        _AccTp v0 = a[i], v1 = a[i+1], v2 = a[i+2], v3 = a[i+3];
        s += v0*v0 + v1*v1 + v2*v2 + v3*v3;
    }
#endif
    for( ; i < n; i++ )
    {
        _AccTp v = a[i];
        s += v*v;
    }
    return s;
}

template<typename _Tp, typename _AccTp> static inline
_AccTp normL1(const _Tp* a, int n)
{
    _AccTp s = 0;
    int i = 0;
#if CV_ENABLE_UNROLLED
    for(; i <= n - 4; i += 4 )
    {
        s += (_AccTp)cv_abs(a[i]) + (_AccTp)cv_abs(a[i+1]) +
            (_AccTp)cv_abs(a[i+2]) + (_AccTp)cv_abs(a[i+3]);
    }
#endif
    for( ; i < n; i++ )
        s += cv_abs(a[i]);
    return s;
}

template<typename _Tp, typename _AccTp> static inline
_AccTp normInf(const _Tp* a, int n)
{
    _AccTp s = 0;
    for( int i = 0; i < n; i++ )
        s = std::max(s, (_AccTp)cv_abs(a[i]));
    return s;
}

template<typename _Tp, typename _AccTp> static inline
_AccTp normL2Sqr(const _Tp* a, const _Tp* b, int n)
{
    _AccTp s = 0;
    int i= 0;
#if CV_ENABLE_UNROLLED
    for(; i <= n - 4; i += 4 )
    {
        _AccTp v0 = _AccTp(a[i] - b[i]), v1 = _AccTp(a[i+1] - b[i+1]), v2 = _AccTp(a[i+2] - b[i+2]), v3 = _AccTp(a[i+3] - b[i+3]);
        s += v0*v0 + v1*v1 + v2*v2 + v3*v3;
    }
#endif
    for( ; i < n; i++ )
    {
        _AccTp v = _AccTp(a[i] - b[i]);
        s += v*v;
    }
    return s;
}

static inline float normL2Sqr(const float* a, const float* b, int n)
{
    float s = 0.f;
    for( int i = 0; i < n; i++ )
    {
        float v = a[i] - b[i];
        s += v*v;
    }
    return s;
}

template<typename _Tp, typename _AccTp> static inline
_AccTp normL1(const _Tp* a, const _Tp* b, int n)
{
    _AccTp s = 0;
    int i= 0;
#if CV_ENABLE_UNROLLED
    for(; i <= n - 4; i += 4 )
    {
        _AccTp v0 = _AccTp(a[i] - b[i]), v1 = _AccTp(a[i+1] - b[i+1]), v2 = _AccTp(a[i+2] - b[i+2]), v3 = _AccTp(a[i+3] - b[i+3]);
        s += std::abs(v0) + std::abs(v1) + std::abs(v2) + std::abs(v3);
    }
#endif
    for( ; i < n; i++ )
    {
        _AccTp v = _AccTp(a[i] - b[i]);
        s += std::abs(v);
    }
    return s;
}

inline float normL1(const float* a, const float* b, int n)
{
    float s = 0.f;
    for( int i = 0; i < n; i++ )
    {
        s += std::abs(a[i] - b[i]);
    }
    return s;
}

inline int normL1(const uchar* a, const uchar* b, int n)
{
    int s = 0;
    for( int i = 0; i < n; i++ )
    {
        s += std::abs(a[i] - b[i]);
    }
    return s;
}

template<typename _Tp, typename _AccTp> static inline
_AccTp normInf(const _Tp* a, const _Tp* b, int n)
{
    _AccTp s = 0;
    for( int i = 0; i < n; i++ )
    {
        _AccTp v0 = a[i] - b[i];
        s = std::max(s, std::abs(v0));
    }
    return s;
}


CV_EXPORTS_W float cubeRoot(float val);


CV_EXPORTS_W float fastAtan2(float y, float x);


CV_EXPORTS int LU(float* A, size_t astep, int m, float* b, size_t bstep, int n);

CV_EXPORTS int LU(double* A, size_t astep, int m, double* b, size_t bstep, int n);

CV_EXPORTS bool Cholesky(float* A, size_t astep, int m, float* b, size_t bstep, int n);

CV_EXPORTS bool Cholesky(double* A, size_t astep, int m, double* b, size_t bstep, int n);





template<typename _Tp, int cn> class Vec;
template<typename _Tp, int m, int n> class Matx;

template<typename _Tp> class Complex;
template<typename _Tp> class Point_;
template<typename _Tp> class Point3_;
template<typename _Tp> class Size_;
template<typename _Tp> class Rect_;
template<typename _Tp> class Scalar_;

class CV_EXPORTS RotatedRect;
class CV_EXPORTS Range;
class CV_EXPORTS TermCriteria;
class CV_EXPORTS KeyPoint;
class CV_EXPORTS DMatch;
class CV_EXPORTS RNG;

class CV_EXPORTS Mat;
class CV_EXPORTS MatExpr;

class CV_EXPORTS UMat;

class CV_EXPORTS SparseMat;
typedef Mat MatND;

template<typename _Tp> class Mat_;
template<typename _Tp> class SparseMat_;

class CV_EXPORTS MatConstIterator;
class CV_EXPORTS SparseMatIterator;
class CV_EXPORTS SparseMatConstIterator;
template<typename _Tp> class MatIterator_;
template<typename _Tp> class MatConstIterator_;
template<typename _Tp> class SparseMatIterator_;
template<typename _Tp> class SparseMatConstIterator_;

namespace ogl
{
    class CV_EXPORTS Buffer;
    class CV_EXPORTS Texture2D;
    class CV_EXPORTS Arrays;
}

namespace cuda
{
    class CV_EXPORTS GpuMat;
    class CV_EXPORTS HostMem;
    class CV_EXPORTS Stream;
    class CV_EXPORTS Event;
}

namespace cudev
{
    template <typename _Tp> class GpuMat_;
}

namespace ipp
{
CV_EXPORTS   unsigned long long getIppFeatures();
CV_EXPORTS   void setIppStatus(int status, const char * const funcname = NULL, const char * const filename = NULL,
                             int line = 0);
CV_EXPORTS   int getIppStatus();
CV_EXPORTS   String getIppErrorLocation();
CV_EXPORTS_W bool   useIPP();
CV_EXPORTS_W void   setUseIPP(bool flag);
CV_EXPORTS_W String getIppVersion();



CV_EXPORTS_W bool useIPP_NotExact();
CV_EXPORTS_W void setUseIPP_NotExact(bool flag);
#ifndef DISABLE_OPENCV_3_COMPATIBILITY
static inline bool useIPP_NE() { return useIPP_NotExact(); }
static inline void setUseIPP_NE(bool flag) { setUseIPP_NotExact(flag); }
#endif

}








}

#include "opencv2/core/neon_utils.hpp"
#include "opencv2/core/vsx_utils.hpp"
#include "opencv2/core/check.hpp"

#endif
