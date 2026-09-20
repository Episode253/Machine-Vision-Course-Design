

#ifndef OPENCV_CORE_TYPES_H
#define OPENCV_CORE_TYPES_H

#if !defined(__OPENCV_BUILD) && !defined(CV__DISABLE_C_API_CTORS)
#define CV__ENABLE_C_API_CTORS
#endif



#ifdef __cplusplus

#ifdef CV__VALIDATE_UNUNITIALIZED_VARS
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#define CV_STRUCT_INITIALIZER {0,}
#else
#if defined(__GNUC__) && __GNUC__ == 4
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
#define CV_STRUCT_INITIALIZER {}
#endif

#else
#define CV_STRUCT_INITIALIZER {0}
#endif


#ifdef HAVE_IPL
#  ifndef __IPL_H__
#    if defined _WIN32
#      include <ipl.h>
#    else
#      include <ipl/ipl.h>
#    endif
#  endif
#elif defined __IPL_H__
#  define HAVE_IPL
#endif

#include "opencv2/core/cvdef.h"

#ifndef SKIP_INCLUDES
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#endif

#if defined _WIN32
#  define CV_CDECL __cdecl
#  define CV_STDCALL __stdcall
#else
#  define CV_CDECL
#  define CV_STDCALL
#endif

#ifndef CV_DEFAULT
#  ifdef __cplusplus
#    define CV_DEFAULT(val) = val
#  else
#    define CV_DEFAULT(val)
#  endif
#endif

#ifndef CV_EXTERN_C_FUNCPTR
#  ifdef __cplusplus
#    define CV_EXTERN_C_FUNCPTR(x) extern "C" { typedef x; }
#  else
#    define CV_EXTERN_C_FUNCPTR(x) typedef x
#  endif
#endif

#ifndef CVAPI
#  define CVAPI(rettype) CV_EXTERN_C CV_EXPORTS rettype CV_CDECL
#endif

#ifndef CV_IMPL
#  define CV_IMPL CV_EXTERN_C
#endif

#ifdef __cplusplus
#  include "opencv2/core.hpp"
#endif




typedef void CvArr;

typedef int CVStatus;


enum {
 CV_StsOk=                       0,
 CV_StsBackTrace=               -1,
 CV_StsError=                   -2,
 CV_StsInternal=                -3,
 CV_StsNoMem=                   -4,
 CV_StsBadArg=                  -5,
 CV_StsBadFunc=                 -6,
 CV_StsNoConv=                  -7,
 CV_StsAutoTrace=               -8,
 CV_HeaderIsNull=               -9,
 CV_BadImageSize=              -10,
 CV_BadOffset=                 -11,
 CV_BadDataPtr=                -12,
 CV_BadStep=                   -13,
 CV_BadModelOrChSeq=           -14,
 CV_BadNumChannels=            -15,
 CV_BadNumChannel1U=           -16,
 CV_BadDepth=                  -17,
 CV_BadAlphaChannel=           -18,
 CV_BadOrder=                  -19,
 CV_BadOrigin=                 -20,
 CV_BadAlign=                  -21,
 CV_BadCallBack=               -22,
 CV_BadTileSize=               -23,
 CV_BadCOI=                    -24,
 CV_BadROISize=                -25,
 CV_MaskIsTiled=               -26,
 CV_StsNullPtr=                -27,
 CV_StsVecLengthErr=           -28,
 CV_StsFilterStructContentErr= -29,
 CV_StsKernelStructContentErr= -30,
 CV_StsFilterOffsetErr=        -31,
 CV_StsBadSize=                -201,
 CV_StsDivByZero=              -202,
 CV_StsInplaceNotSupported=    -203,
 CV_StsObjectNotFound=         -204,
 CV_StsUnmatchedFormats=       -205,
 CV_StsBadFlag=                -206,
 CV_StsBadPoint=               -207,
 CV_StsBadMask=                -208,
 CV_StsUnmatchedSizes=         -209,
 CV_StsUnsupportedFormat=      -210,
 CV_StsOutOfRange=             -211,
 CV_StsParseError=             -212,
 CV_StsNotImplemented=         -213,
 CV_StsBadMemBlock=            -214,
 CV_StsAssert=                 -215,
 CV_GpuNotSupported=           -216,
 CV_GpuApiCallError=           -217,
 CV_OpenGlNotSupported=        -218,
 CV_OpenGlApiCallError=        -219,
 CV_OpenCLApiCallError=        -220,
 CV_OpenCLDoubleNotSupported=  -221,
 CV_OpenCLInitError=           -222,
 CV_OpenCLNoAMDBlasFft=        -223
};



#define CV_SWAP(a,b,t) ((t) = (a), (a) = (b), (b) = (t))


#define  CV_IMIN(a, b)  ((a) ^ (((a)^(b)) & (((a) < (b)) - 1)))

#define  CV_IMAX(a, b)  ((a) ^ (((a)^(b)) & (((a) > (b)) - 1)))


#ifndef __cplusplus
#  define  CV_IABS(a)     (((a) ^ ((a) < 0 ? -1 : 0)) - ((a) < 0 ? -1 : 0))
#else
#  define  CV_IABS(a)     abs(a)
#endif
#define  CV_CMP(a,b)    (((a) > (b)) - ((a) < (b)))
#define  CV_SIGN(a)     CV_CMP((a),0)

#define cvInvSqrt(value) ((float)(1./sqrt(value)))
#define cvSqrt(value)  ((float)sqrt(value))




typedef uint64 CvRNG;

#define CV_RNG_COEFF 4164903690U


CV_INLINE CvRNG cvRNG( int64 seed CV_DEFAULT(-1))
{
    CvRNG rng = seed ? (uint64)seed : (uint64)(int64)-1;
    return rng;
}


CV_INLINE unsigned cvRandInt( CvRNG* rng )
{
    uint64 temp = *rng;
    temp = (uint64)(unsigned)temp*CV_RNG_COEFF + (temp >> 32);
    *rng = temp;
    return (unsigned)temp;
}


CV_INLINE double cvRandReal( CvRNG* rng )
{
    return cvRandInt(rng)*2.3283064365386962890625e-10 ;
}



#ifndef HAVE_IPL


#define IPL_DEPTH_SIGN 0x80000000

#define IPL_DEPTH_1U     1
#define IPL_DEPTH_8U     8
#define IPL_DEPTH_16U   16
#define IPL_DEPTH_32F   32

#define IPL_DEPTH_8S  (IPL_DEPTH_SIGN| 8)
#define IPL_DEPTH_16S (IPL_DEPTH_SIGN|16)
#define IPL_DEPTH_32S (IPL_DEPTH_SIGN|32)

#define IPL_DATA_ORDER_PIXEL  0
#define IPL_DATA_ORDER_PLANE  1

#define IPL_ORIGIN_TL 0
#define IPL_ORIGIN_BL 1

#define IPL_ALIGN_4BYTES   4
#define IPL_ALIGN_8BYTES   8
#define IPL_ALIGN_16BYTES 16
#define IPL_ALIGN_32BYTES 32

#define IPL_ALIGN_DWORD   IPL_ALIGN_4BYTES
#define IPL_ALIGN_QWORD   IPL_ALIGN_8BYTES

#define IPL_BORDER_CONSTANT   0
#define IPL_BORDER_REPLICATE  1
#define IPL_BORDER_REFLECT    2
#define IPL_BORDER_WRAP       3

#ifdef __cplusplus
typedef struct _IplImage IplImage;
CV_EXPORTS _IplImage cvIplImage(const cv::Mat& m);
#endif


typedef struct
_IplImage
{
    int  nSize;
    int  ID;
    int  nChannels;
    int  alphaChannel;
    int  depth;
    char colorModel[4];
    char channelSeq[4];
    int  dataOrder;
    int  origin;
    int  align;
    int  width;
    int  height;
    struct _IplROI *roi;
    struct _IplImage *maskROI;
    void  *imageId;
    struct _IplTileInfo *tileInfo;
    int  imageSize;
    char *imageData;
    int  widthStep;
    int  BorderMode[4];
    int  BorderConst[4];
    char *imageDataOrigin;

#if defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus)
    _IplImage() {}
    _IplImage(const cv::Mat& m) { *this = cvIplImage(m); }
#endif
}
IplImage;

CV_INLINE IplImage cvIplImage()
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    IplImage self = CV_STRUCT_INITIALIZER; self.nSize = sizeof(IplImage); return self;
#else
    return _IplImage();
#endif
}

typedef struct _IplTileInfo IplTileInfo;

typedef struct _IplROI
{
    int  coi;
    int  xOffset;
    int  yOffset;
    int  width;
    int  height;
}
IplROI;

typedef struct _IplConvKernel
{
    int  nCols;
    int  nRows;
    int  anchorX;
    int  anchorY;
    int *values;
    int  nShiftR;
}
IplConvKernel;

typedef struct _IplConvKernelFP
{
    int  nCols;
    int  nRows;
    int  anchorX;
    int  anchorY;
    float *values;
}
IplConvKernelFP;

#define IPL_IMAGE_HEADER 1
#define IPL_IMAGE_DATA   2
#define IPL_IMAGE_ROI    4

#endif


#define IPL_BORDER_REFLECT_101    4
#define IPL_BORDER_TRANSPARENT    5

#define IPL_IMAGE_MAGIC_VAL  ((int)sizeof(IplImage))
#define CV_TYPE_NAME_IMAGE "opencv-image"

#define CV_IS_IMAGE_HDR(img) \
    ((img) != NULL && ((const IplImage*)(img))->nSize == sizeof(IplImage))

#define CV_IS_IMAGE(img) \
    (CV_IS_IMAGE_HDR(img) && ((IplImage*)img)->imageData != NULL)


#define IPL_DEPTH_64F  64


#define CV_IMAGE_ELEM( image, elemtype, row, col )       \
    (((elemtype*)((image)->imageData + (image)->widthStep*(row)))[(col)])



#define CV_AUTO_STEP  0x7fffffff
#define CV_WHOLE_ARR  cvSlice( 0, 0x3fffffff )

#define CV_MAGIC_MASK       0xFFFF0000
#define CV_MAT_MAGIC_VAL    0x42420000
#define CV_TYPE_NAME_MAT    "opencv-matrix"

#ifdef __cplusplus
typedef struct CvMat CvMat;
CV_INLINE CvMat cvMat(const cv::Mat& m);
#endif


typedef struct CvMat
{
    int type;
    int step;


    int* refcount;
    int hdr_refcount;

    union
    {
        uchar* ptr;
        short* s;
        int* i;
        float* fl;
        double* db;
    } data;

#ifdef __cplusplus
    union
    {
        int rows;
        int height;
    };

    union
    {
        int cols;
        int width;
    };
#else
    int rows;
    int cols;
#endif

#if defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus)
    CvMat() {}
    CvMat(const cv::Mat& m) { *this = cvMat(m); }
#endif
}
CvMat;


#define CV_IS_MAT_HDR(mat) \
    ((mat) != NULL && \
    (((const CvMat*)(mat))->type & CV_MAGIC_MASK) == CV_MAT_MAGIC_VAL && \
    ((const CvMat*)(mat))->cols > 0 && ((const CvMat*)(mat))->rows > 0)

#define CV_IS_MAT_HDR_Z(mat) \
    ((mat) != NULL && \
    (((const CvMat*)(mat))->type & CV_MAGIC_MASK) == CV_MAT_MAGIC_VAL && \
    ((const CvMat*)(mat))->cols >= 0 && ((const CvMat*)(mat))->rows >= 0)

#define CV_IS_MAT(mat) \
    (CV_IS_MAT_HDR(mat) && ((const CvMat*)(mat))->data.ptr != NULL)

#define CV_IS_MASK_ARR(mat) \
    (((mat)->type & (CV_MAT_TYPE_MASK & ~CV_8SC1)) == 0)

#define CV_ARE_TYPES_EQ(mat1, mat2) \
    ((((mat1)->type ^ (mat2)->type) & CV_MAT_TYPE_MASK) == 0)

#define CV_ARE_CNS_EQ(mat1, mat2) \
    ((((mat1)->type ^ (mat2)->type) & CV_MAT_CN_MASK) == 0)

#define CV_ARE_DEPTHS_EQ(mat1, mat2) \
    ((((mat1)->type ^ (mat2)->type) & CV_MAT_DEPTH_MASK) == 0)

#define CV_ARE_SIZES_EQ(mat1, mat2) \
    ((mat1)->rows == (mat2)->rows && (mat1)->cols == (mat2)->cols)

#define CV_IS_MAT_CONST(mat)  \
    (((mat)->rows|(mat)->cols) == 1)

#define IPL2CV_DEPTH(depth) \
    ((((CV_8U)+(CV_16U<<4)+(CV_32F<<8)+(CV_64F<<16)+(CV_8S<<20)+ \
    (CV_16S<<24)+(CV_32S<<28)) >> ((((depth) & 0xF0) >> 2) + \
    (((depth) & IPL_DEPTH_SIGN) ? 20 : 0))) & 15)


CV_INLINE CvMat cvMat( int rows, int cols, int type, void* data CV_DEFAULT(NULL))
{
    CvMat m;

    assert( (unsigned)CV_MAT_DEPTH(type) <= CV_64F );
    type = CV_MAT_TYPE(type);
    m.type = CV_MAT_MAGIC_VAL | CV_MAT_CONT_FLAG | type;
    m.cols = cols;
    m.rows = rows;
    m.step = m.cols*CV_ELEM_SIZE(type);
    m.data.ptr = (uchar*)data;
    m.refcount = NULL;
    m.hdr_refcount = 0;

    return m;
}

#ifdef __cplusplus

CV_INLINE CvMat cvMat(const cv::Mat& m)
{
    CvMat self;
    CV_DbgAssert(m.dims <= 2);
    self = cvMat(m.rows, m.dims == 1 ? 1 : m.cols, m.type(), m.data);
    self.step = (int)m.step[0];
    self.type = (self.type & ~cv::Mat::CONTINUOUS_FLAG) | (m.flags & cv::Mat::CONTINUOUS_FLAG);
    return self;
}
CV_INLINE CvMat cvMat()
{
#if !defined(CV__ENABLE_C_API_CTORS)
    CvMat self = CV_STRUCT_INITIALIZER; return self;
#else
    return CvMat();
#endif
}
CV_INLINE CvMat cvMat(const CvMat& m)
{
#if !defined(CV__ENABLE_C_API_CTORS)
    CvMat self = CV_STRUCT_INITIALIZER; memcpy(&self, &m, sizeof(self)); return self;
#else
    return CvMat(m);
#endif
}

#endif


#define CV_MAT_ELEM_PTR_FAST( mat, row, col, pix_size )  \
    (assert( (unsigned)(row) < (unsigned)(mat).rows &&   \
             (unsigned)(col) < (unsigned)(mat).cols ),   \
     (mat).data.ptr + (size_t)(mat).step*(row) + (pix_size)*(col))

#define CV_MAT_ELEM_PTR( mat, row, col )                 \
    CV_MAT_ELEM_PTR_FAST( mat, row, col, CV_ELEM_SIZE((mat).type) )

#define CV_MAT_ELEM( mat, elemtype, row, col )           \
    (*(elemtype*)CV_MAT_ELEM_PTR_FAST( mat, row, col, sizeof(elemtype)))


CV_INLINE  double  cvmGet( const CvMat* mat, int row, int col )
{
    int type;

    type = CV_MAT_TYPE(mat->type);
    assert( (unsigned)row < (unsigned)mat->rows &&
            (unsigned)col < (unsigned)mat->cols );

    if( type == CV_32FC1 )
        return ((float*)(void*)(mat->data.ptr + (size_t)mat->step*row))[col];
    else
    {
        assert( type == CV_64FC1 );
        return ((double*)(void*)(mat->data.ptr + (size_t)mat->step*row))[col];
    }
}


CV_INLINE  void  cvmSet( CvMat* mat, int row, int col, double value )
{
    int type;
    type = CV_MAT_TYPE(mat->type);
    assert( (unsigned)row < (unsigned)mat->rows &&
            (unsigned)col < (unsigned)mat->cols );

    if( type == CV_32FC1 )
        ((float*)(void*)(mat->data.ptr + (size_t)mat->step*row))[col] = (float)value;
    else
    {
        assert( type == CV_64FC1 );
        ((double*)(void*)(mat->data.ptr + (size_t)mat->step*row))[col] = value;
    }
}


CV_INLINE int cvIplDepth( int type )
{
    int depth = CV_MAT_DEPTH(type);
    return CV_ELEM_SIZE1(depth)*8 | (depth == CV_8S || depth == CV_16S ||
           depth == CV_32S ? IPL_DEPTH_SIGN : 0);
}




#define CV_MATND_MAGIC_VAL    0x42430000
#define CV_TYPE_NAME_MATND    "opencv-nd-matrix"

#define CV_MAX_DIM            32

#ifdef __cplusplus
typedef struct CvMatND CvMatND;
CV_EXPORTS CvMatND cvMatND(const cv::Mat& m);
#endif


typedef struct
CvMatND
{
    int type;
    int dims;

    int* refcount;
    int hdr_refcount;

    union
    {
        uchar* ptr;
        float* fl;
        double* db;
        int* i;
        short* s;
    } data;

    struct
    {
        int size;
        int step;
    }
    dim[CV_MAX_DIM];

#if defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus)
    CvMatND() {}
    CvMatND(const cv::Mat& m) { *this = cvMatND(m); }
#endif
}
CvMatND;


CV_INLINE CvMatND cvMatND()
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvMatND self = CV_STRUCT_INITIALIZER; return self;
#else
    return CvMatND();
#endif
}

#define CV_IS_MATND_HDR(mat) \
    ((mat) != NULL && (((const CvMatND*)(mat))->type & CV_MAGIC_MASK) == CV_MATND_MAGIC_VAL)

#define CV_IS_MATND(mat) \
    (CV_IS_MATND_HDR(mat) && ((const CvMatND*)(mat))->data.ptr != NULL)




#define CV_SPARSE_MAT_MAGIC_VAL    0x42440000
#define CV_TYPE_NAME_SPARSE_MAT    "opencv-sparse-matrix"

struct CvSet;

typedef struct CvSparseMat
{
    int type;
    int dims;
    int* refcount;
    int hdr_refcount;

    struct CvSet* heap;
    void** hashtable;
    int hashsize;
    int valoffset;
    int idxoffset;
    int size[CV_MAX_DIM];

#ifdef __cplusplus
    CV_EXPORTS void copyToSparseMat(cv::SparseMat& m) const;
#endif
}
CvSparseMat;

#ifdef __cplusplus
CV_EXPORTS CvSparseMat* cvCreateSparseMat(const cv::SparseMat& m);
#endif

#define CV_IS_SPARSE_MAT_HDR(mat) \
    ((mat) != NULL && \
    (((const CvSparseMat*)(mat))->type & CV_MAGIC_MASK) == CV_SPARSE_MAT_MAGIC_VAL)

#define CV_IS_SPARSE_MAT(mat) \
    CV_IS_SPARSE_MAT_HDR(mat)



typedef struct CvSparseNode
{
    unsigned hashval;
    struct CvSparseNode* next;
}
CvSparseNode;

typedef struct CvSparseMatIterator
{
    CvSparseMat* mat;
    CvSparseNode* node;
    int curidx;
}
CvSparseMatIterator;

#define CV_NODE_VAL(mat,node)   ((void*)((uchar*)(node) + (mat)->valoffset))
#define CV_NODE_IDX(mat,node)   ((int*)((uchar*)(node) + (mat)->idxoffset))



typedef int CvHistType;

#define CV_HIST_MAGIC_VAL     0x42450000
#define CV_HIST_UNIFORM_FLAG  (1 << 10)


#define CV_HIST_RANGES_FLAG   (1 << 11)

#define CV_HIST_ARRAY         0
#define CV_HIST_SPARSE        1
#define CV_HIST_TREE          CV_HIST_SPARSE


#define CV_HIST_UNIFORM       1

typedef struct CvHistogram
{
    int     type;
    CvArr*  bins;
    float   thresh[CV_MAX_DIM][2];
    float** thresh2;
    CvMatND mat;
}
CvHistogram;

#define CV_IS_HIST( hist ) \
    ((hist) != NULL  && \
     (((CvHistogram*)(hist))->type & CV_MAGIC_MASK) == CV_HIST_MAGIC_VAL && \
     (hist)->bins != NULL)

#define CV_IS_UNIFORM_HIST( hist ) \
    (((hist)->type & CV_HIST_UNIFORM_FLAG) != 0)

#define CV_IS_SPARSE_HIST( hist ) \
    CV_IS_SPARSE_MAT((hist)->bins)

#define CV_HIST_HAS_RANGES( hist ) \
    (((hist)->type & CV_HIST_RANGES_FLAG) != 0)





typedef struct CvRect
{
    int x;
    int y;
    int width;
    int height;

#ifdef CV__VALIDATE_UNUNITIALIZED_VARS
    CvRect() __attribute__(( warning("Non-initialized variable") )) {};
    template<typename _Tp> CvRect(const std::initializer_list<_Tp> list)
    {
        CV_Assert(list.size() == 0 || list.size() == 4);
        x = y = width = height = 0;
        if (list.size() == 4)
        {
            x = list.begin()[0]; y = list.begin()[1]; width = list.begin()[2]; height = list.begin()[3];
        }
    };
#elif defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus)
    CvRect(int _x = 0, int _y = 0, int w = 0, int h = 0): x(_x), y(_y), width(w), height(h) {}
    template<typename _Tp>
    CvRect(const cv::Rect_<_Tp>& r): x(cv::saturate_cast<int>(r.x)), y(cv::saturate_cast<int>(r.y)), width(cv::saturate_cast<int>(r.width)), height(cv::saturate_cast<int>(r.height)) {}
#endif
#ifdef __cplusplus
    template<typename _Tp>
    operator cv::Rect_<_Tp>() const { return cv::Rect_<_Tp>((_Tp)x, (_Tp)y, (_Tp)width, (_Tp)height); }
#endif
}
CvRect;


CV_INLINE  CvRect  cvRect( int x, int y, int width, int height )
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvRect r = {x, y, width, height};
#else
    CvRect r(x, y , width, height);
#endif
    return r;
}
#ifdef __cplusplus
CV_INLINE CvRect cvRect(const cv::Rect& rc) { return cvRect(rc.x, rc.y, rc.width, rc.height); }
#endif

CV_INLINE  IplROI  cvRectToROI( CvRect rect, int coi )
{
    IplROI roi;
    roi.xOffset = rect.x;
    roi.yOffset = rect.y;
    roi.width = rect.width;
    roi.height = rect.height;
    roi.coi = coi;

    return roi;
}


CV_INLINE  CvRect  cvROIToRect( IplROI roi )
{
    return cvRect( roi.xOffset, roi.yOffset, roi.width, roi.height );
}



#define CV_TERMCRIT_ITER    1
#define CV_TERMCRIT_NUMBER  CV_TERMCRIT_ITER
#define CV_TERMCRIT_EPS     2


typedef struct CvTermCriteria
{
    int    type;
    int    max_iter;
    double epsilon;
#if defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus)
    CvTermCriteria(int _type = 0, int _iter = 0, double _eps = 0) : type(_type), max_iter(_iter), epsilon(_eps)  {}
    CvTermCriteria(const cv::TermCriteria& t) : type(t.type), max_iter(t.maxCount), epsilon(t.epsilon)  {}
#endif
#ifdef __cplusplus
    operator cv::TermCriteria() const { return cv::TermCriteria(type, max_iter, epsilon); }
#endif
}
CvTermCriteria;

CV_INLINE  CvTermCriteria  cvTermCriteria( int type, int max_iter, double epsilon )
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvTermCriteria t = { type, max_iter, (float)epsilon};
#else
    CvTermCriteria t(type, max_iter, epsilon);
#endif
    return t;
}
#ifdef __cplusplus
CV_INLINE CvTermCriteria cvTermCriteria(const cv::TermCriteria& t) { return cvTermCriteria(t.type, t.maxCount, t.epsilon); }
#endif




typedef struct CvPoint
{
    int x;
    int y;

#ifdef CV__VALIDATE_UNUNITIALIZED_VARS
    CvPoint() __attribute__(( warning("Non-initialized variable") )) {}
    template<typename _Tp> CvPoint(const std::initializer_list<_Tp> list)
    {
        CV_Assert(list.size() == 0 || list.size() == 2);
        x = y = 0;
        if (list.size() == 2)
        {
            x = list.begin()[0]; y = list.begin()[1];
        }
    };
#elif defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus)
    CvPoint(int _x = 0, int _y = 0): x(_x), y(_y) {}
    template<typename _Tp>
    CvPoint(const cv::Point_<_Tp>& pt): x((int)pt.x), y((int)pt.y) {}
#endif
#ifdef __cplusplus
    template<typename _Tp>
    operator cv::Point_<_Tp>() const { return cv::Point_<_Tp>(cv::saturate_cast<_Tp>(x), cv::saturate_cast<_Tp>(y)); }
#endif
}
CvPoint;


CV_INLINE  CvPoint  cvPoint( int x, int y )
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvPoint p = {x, y};
#else
    CvPoint p(x, y);
#endif
    return p;
}
#ifdef __cplusplus
CV_INLINE CvPoint cvPoint(const cv::Point& pt) { return cvPoint(pt.x, pt.y); }
#endif

typedef struct CvPoint2D32f
{
    float x;
    float y;

#ifdef CV__VALIDATE_UNUNITIALIZED_VARS
    CvPoint2D32f() __attribute__(( warning("Non-initialized variable") )) {}
    template<typename _Tp> CvPoint2D32f(const std::initializer_list<_Tp> list)
    {
        CV_Assert(list.size() == 0 || list.size() == 2);
        x = y = 0;
        if (list.size() == 2)
        {
            x = list.begin()[0]; y = list.begin()[1];
        }
    };
#elif defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus)
    CvPoint2D32f(float _x = 0, float _y = 0): x(_x), y(_y) {}
    template<typename _Tp>
    CvPoint2D32f(const cv::Point_<_Tp>& pt): x((float)pt.x), y((float)pt.y) {}
#endif
#ifdef __cplusplus
    template<typename _Tp>
    operator cv::Point_<_Tp>() const { return cv::Point_<_Tp>(cv::saturate_cast<_Tp>(x), cv::saturate_cast<_Tp>(y)); }
#endif
}
CvPoint2D32f;


CV_INLINE  CvPoint2D32f  cvPoint2D32f( double x, double y )
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvPoint2D32f p = { (float)x, (float)y };
#else
    CvPoint2D32f p((float)x, (float)y);
#endif
    return p;
}

#ifdef __cplusplus
template<typename _Tp>
CvPoint2D32f cvPoint2D32f(const cv::Point_<_Tp>& pt)
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvPoint2D32f p = { (float)pt.x, (float)pt.y };
#else
    CvPoint2D32f p((float)pt.x, (float)pt.y);
#endif
    return p;
}
#endif


CV_INLINE  CvPoint2D32f  cvPointTo32f( CvPoint point )
{
    return cvPoint2D32f( (float)point.x, (float)point.y );
}


CV_INLINE  CvPoint  cvPointFrom32f( CvPoint2D32f point )
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvPoint ipt = { cvRound(point.x), cvRound(point.y) };
#else
    CvPoint ipt(cvRound(point.x), cvRound(point.y));
#endif
    return ipt;
}


typedef struct CvPoint3D32f
{
    float x;
    float y;
    float z;

#ifdef CV__VALIDATE_UNUNITIALIZED_VARS
    CvPoint3D32f() __attribute__(( warning("Non-initialized variable") )) {}
    template<typename _Tp> CvPoint3D32f(const std::initializer_list<_Tp> list)
    {
        CV_Assert(list.size() == 0 || list.size() == 3);
        x = y = z = 0;
        if (list.size() == 3)
        {
            x = list.begin()[0]; y = list.begin()[1]; z = list.begin()[2];
        }
    };
#elif defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus)
    CvPoint3D32f(float _x = 0, float _y = 0, float _z = 0): x(_x), y(_y), z(_z) {}
    template<typename _Tp>
    CvPoint3D32f(const cv::Point3_<_Tp>& pt): x((float)pt.x), y((float)pt.y), z((float)pt.z) {}
#endif
#ifdef __cplusplus
    template<typename _Tp>
    operator cv::Point3_<_Tp>() const { return cv::Point3_<_Tp>(cv::saturate_cast<_Tp>(x), cv::saturate_cast<_Tp>(y), cv::saturate_cast<_Tp>(z)); }
#endif
}
CvPoint3D32f;


CV_INLINE  CvPoint3D32f  cvPoint3D32f( double x, double y, double z )
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvPoint3D32f p = { (float)x, (float)y, (float)z };
#else
    CvPoint3D32f p((float)x, (float)y, (float)z);
#endif
    return p;
}

#ifdef __cplusplus
template<typename _Tp>
CvPoint3D32f cvPoint3D32f(const cv::Point3_<_Tp>& pt)
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvPoint3D32f p  = { (float)pt.x, (float)pt.y, (float)pt.z };
#else
    CvPoint3D32f p((float)pt.x, (float)pt.y, (float)pt.z);
#endif
    return p;
}
#endif


typedef struct CvPoint2D64f
{
    double x;
    double y;
#ifdef CV__VALIDATE_UNUNITIALIZED_VARS
    CvPoint2D64f() __attribute__(( warning("Non-initialized variable") )) {}
    template<typename _Tp> CvPoint2D64f(const std::initializer_list<_Tp> list)
    {
        CV_Assert(list.size() == 0 || list.size() == 2);
        x = y = 0;
        if (list.size() == 2)
        {
            x = list.begin()[0]; y = list.begin()[1];
        }
    };
#endif
}
CvPoint2D64f;


CV_INLINE  CvPoint2D64f  cvPoint2D64f( double x, double y )
{
    CvPoint2D64f p = { x, y };
    return p;
}


typedef struct CvPoint3D64f
{
    double x;
    double y;
    double z;
#ifdef CV__VALIDATE_UNUNITIALIZED_VARS
    CvPoint3D64f() __attribute__(( warning("Non-initialized variable") )) {}
    template<typename _Tp> CvPoint3D64f(const std::initializer_list<_Tp> list)
    {
        CV_Assert(list.size() == 0 || list.size() == 3);
        x = y = z = 0;
        if (list.size() == 3)
        {
            x = list.begin()[0]; y = list.begin()[1]; z = list.begin()[2];
        }
    };
#endif
}
CvPoint3D64f;


CV_INLINE  CvPoint3D64f  cvPoint3D64f( double x, double y, double z )
{
    CvPoint3D64f p = { x, y, z };
    return p;
}




typedef struct CvSize
{
    int width;
    int height;

#ifdef CV__VALIDATE_UNUNITIALIZED_VARS
    CvSize() __attribute__(( warning("Non-initialized variable") )) {}
    template<typename _Tp> CvSize(const std::initializer_list<_Tp> list)
    {
        CV_Assert(list.size() == 0 || list.size() == 2);
        width = 0; height = 0;
        if (list.size() == 2)
        {
            width = list.begin()[0]; height = list.begin()[1];
        }
    };
#elif defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus)
    CvSize(int w = 0, int h = 0): width(w), height(h) {}
    template<typename _Tp>
    CvSize(const cv::Size_<_Tp>& sz): width(cv::saturate_cast<int>(sz.width)), height(cv::saturate_cast<int>(sz.height)) {}
#endif
#ifdef __cplusplus
    template<typename _Tp>
    operator cv::Size_<_Tp>() const { return cv::Size_<_Tp>(cv::saturate_cast<_Tp>(width), cv::saturate_cast<_Tp>(height)); }
#endif
}
CvSize;


CV_INLINE  CvSize  cvSize( int width, int height )
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvSize s = { width, height };
#else
    CvSize s(width, height);
#endif
    return s;
}

#ifdef __cplusplus
CV_INLINE CvSize cvSize(const cv::Size& sz)
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvSize s = { sz.width, sz.height };
#else
    CvSize s(sz.width, sz.height);
#endif
    return s;
}
#endif

typedef struct CvSize2D32f
{
    float width;
    float height;

#ifdef CV__VALIDATE_UNUNITIALIZED_VARS
    CvSize2D32f() __attribute__(( warning("Non-initialized variable") )) {}
    template<typename _Tp> CvSize2D32f(const std::initializer_list<_Tp> list)
    {
        CV_Assert(list.size() == 0 || list.size() == 2);
        width = 0; height = 0;
        if (list.size() == 2)
        {
            width = list.begin()[0]; height = list.begin()[1];
        }
    };
#elif defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus)
    CvSize2D32f(float w = 0, float h = 0): width(w), height(h) {}
    template<typename _Tp>
    CvSize2D32f(const cv::Size_<_Tp>& sz): width(cv::saturate_cast<float>(sz.width)), height(cv::saturate_cast<float>(sz.height)) {}
#endif
#ifdef __cplusplus
    template<typename _Tp>
    operator cv::Size_<_Tp>() const { return cv::Size_<_Tp>(cv::saturate_cast<_Tp>(width), cv::saturate_cast<_Tp>(height)); }
#endif
}
CvSize2D32f;


CV_INLINE  CvSize2D32f  cvSize2D32f( double width, double height )
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvSize2D32f s = { (float)width, (float)height };
#else
    CvSize2D32f s((float)width, (float)height);
#endif
    return s;
}
#ifdef __cplusplus
template<typename _Tp>
CvSize2D32f cvSize2D32f(const cv::Size_<_Tp>& sz)
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvSize2D32f s = { (float)sz.width, (float)sz.height };
#else
    CvSize2D32f s((float)sz.width, (float)sz.height);
#endif
    return s;
}
#endif


typedef struct CvBox2D
{
    CvPoint2D32f center;
    CvSize2D32f  size;
    float angle;


#if defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus)
    CvBox2D(CvPoint2D32f c = CvPoint2D32f(), CvSize2D32f s = CvSize2D32f(), float a = 0) : center(c), size(s), angle(a) {}
    CvBox2D(const cv::RotatedRect& rr) : center(rr.center), size(rr.size), angle(rr.angle) {}
#endif
#ifdef __cplusplus
    operator cv::RotatedRect() const { return cv::RotatedRect(center, size, angle); }
#endif
}
CvBox2D;


#ifdef __cplusplus
CV_INLINE CvBox2D cvBox2D(CvPoint2D32f c = CvPoint2D32f(), CvSize2D32f s = CvSize2D32f(), float a = 0)
{
    CvBox2D self;
    self.center = c;
    self.size = s;
    self.angle = a;
    return self;
}
CV_INLINE CvBox2D cvBox2D(const cv::RotatedRect& rr)
{
    CvBox2D self;
    self.center = cvPoint2D32f(rr.center);
    self.size = cvSize2D32f(rr.size);
    self.angle = rr.angle;
    return self;
}
#endif



typedef struct CvLineIterator
{

    uchar* ptr;


    int  err;
    int  plus_delta;
    int  minus_delta;
    int  plus_step;
    int  minus_step;
}
CvLineIterator;




#define CV_WHOLE_SEQ_END_INDEX 0x3fffffff
#define CV_WHOLE_SEQ  cvSlice(0, CV_WHOLE_SEQ_END_INDEX)

typedef struct CvSlice
{
    int  start_index, end_index;

#ifdef CV__VALIDATE_UNUNITIALIZED_VARS
    CvSlice() __attribute__(( warning("Non-initialized variable") )) {}
    template<typename _Tp> CvSlice(const std::initializer_list<_Tp> list)
    {
        CV_Assert(list.size() == 0 || list.size() == 2);
        start_index = end_index = 0;
        if (list.size() == 2)
        {
            start_index = list.begin()[0]; end_index = list.begin()[1];
        }
    };
#endif
#if defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus) && !defined(__CUDACC__)
    CvSlice(int start = 0, int end = 0) : start_index(start), end_index(end) {}
    CvSlice(const cv::Range& r) { *this = (r.start != INT_MIN && r.end != INT_MAX) ? CvSlice(r.start, r.end) : CvSlice(0, CV_WHOLE_SEQ_END_INDEX); }
    operator cv::Range() const { return (start_index == 0 && end_index == CV_WHOLE_SEQ_END_INDEX ) ? cv::Range::all() : cv::Range(start_index, end_index); }
#endif
}
CvSlice;

CV_INLINE  CvSlice  cvSlice( int start, int end )
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus) && !defined(__CUDACC__))
    CvSlice slice = { start, end };
#else
    CvSlice slice(start, end);
#endif
    return slice;
}

#if defined(__cplusplus)
CV_INLINE  CvSlice  cvSlice(const cv::Range& r)
{
    CvSlice slice = (r.start != INT_MIN && r.end != INT_MAX) ? cvSlice(r.start, r.end) : cvSlice(0, CV_WHOLE_SEQ_END_INDEX);
    return slice;
}
#endif




typedef struct CvScalar
{
    double val[4];

#ifdef CV__VALIDATE_UNUNITIALIZED_VARS
    CvScalar() __attribute__(( warning("Non-initialized variable") )) {}
    CvScalar(const std::initializer_list<double> list)
    {
        CV_Assert(list.size() == 0 || list.size() == 4);
        val[0] = val[1] = val[2] = val[3] = 0;
        if (list.size() == 4)
        {
            val[0] = list.begin()[0]; val[1] = list.begin()[1]; val[2] = list.begin()[2]; val[3] = list.begin()[3];
        }
    };
#elif defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus)
    CvScalar() {}
    CvScalar(double d0, double d1 = 0, double d2 = 0, double d3 = 0) { val[0] = d0; val[1] = d1; val[2] = d2; val[3] = d3; }
    template<typename _Tp>
    CvScalar(const cv::Scalar_<_Tp>& s) { val[0] = s.val[0]; val[1] = s.val[1]; val[2] = s.val[2]; val[3] = s.val[3]; }
    template<typename _Tp, int cn>
    CvScalar(const cv::Vec<_Tp, cn>& v)
    {
        int i;
        for( i = 0; i < (cn < 4 ? cn : 4); i++ ) val[i] = v.val[i];
        for( ; i < 4; i++ ) val[i] = 0;
    }
#endif
#ifdef __cplusplus
    template<typename _Tp>
    operator cv::Scalar_<_Tp>() const { return cv::Scalar_<_Tp>(cv::saturate_cast<_Tp>(val[0]), cv::saturate_cast<_Tp>(val[1]), cv::saturate_cast<_Tp>(val[2]), cv::saturate_cast<_Tp>(val[3])); }
#endif
}
CvScalar;

CV_INLINE  CvScalar  cvScalar( double val0, double val1 CV_DEFAULT(0),
                               double val2 CV_DEFAULT(0), double val3 CV_DEFAULT(0))
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvScalar scalar = CV_STRUCT_INITIALIZER;
#else
    CvScalar scalar;
#endif
    scalar.val[0] = val0; scalar.val[1] = val1;
    scalar.val[2] = val2; scalar.val[3] = val3;
    return scalar;
}

#ifdef __cplusplus
CV_INLINE CvScalar cvScalar()
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvScalar scalar = CV_STRUCT_INITIALIZER;
#else
    CvScalar scalar;
#endif
    scalar.val[0] = scalar.val[1] = scalar.val[2] = scalar.val[3] = 0;
    return scalar;
}
CV_INLINE CvScalar cvScalar(const cv::Scalar& s)
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvScalar scalar = CV_STRUCT_INITIALIZER;
#else
    CvScalar scalar;
#endif
    scalar.val[0] = s.val[0];
    scalar.val[1] = s.val[1];
    scalar.val[2] = s.val[2];
    scalar.val[3] = s.val[3];
    return scalar;
}
#endif

CV_INLINE  CvScalar  cvRealScalar( double val0 )
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvScalar scalar = CV_STRUCT_INITIALIZER;
#else
    CvScalar scalar;
#endif
    scalar.val[0] = val0;
    scalar.val[1] = scalar.val[2] = scalar.val[3] = 0;
    return scalar;
}

CV_INLINE  CvScalar  cvScalarAll( double val0123 )
{
#if !(defined(CV__ENABLE_C_API_CTORS) && defined(__cplusplus))
    CvScalar scalar = CV_STRUCT_INITIALIZER;
#else
    CvScalar scalar;
#endif
    scalar.val[0] = val0123;
    scalar.val[1] = val0123;
    scalar.val[2] = val0123;
    scalar.val[3] = val0123;
    return scalar;
}





typedef struct CvMemBlock
{
    struct CvMemBlock*  prev;
    struct CvMemBlock*  next;
}
CvMemBlock;

#define CV_STORAGE_MAGIC_VAL    0x42890000

typedef struct CvMemStorage
{
    int signature;
    CvMemBlock* bottom;
    CvMemBlock* top;
    struct  CvMemStorage* parent;
    int block_size;
    int free_space;
}
CvMemStorage;

#define CV_IS_STORAGE(storage)  \
    ((storage) != NULL &&       \
    (((CvMemStorage*)(storage))->signature & CV_MAGIC_MASK) == CV_STORAGE_MAGIC_VAL)


typedef struct CvMemStoragePos
{
    CvMemBlock* top;
    int free_space;
}
CvMemStoragePos;




typedef struct CvSeqBlock
{
    struct CvSeqBlock*  prev;
    struct CvSeqBlock*  next;
    int    start_index;

    int    count;
    schar* data;
}
CvSeqBlock;


#define CV_TREE_NODE_FIELDS(node_type)                               \
    int       flags;                   \
    int       header_size;             \
    struct    node_type* h_prev;       \
    struct    node_type* h_next;       \
    struct    node_type* v_prev;       \
    struct    node_type* v_next


#define CV_SEQUENCE_FIELDS()                                              \
    CV_TREE_NODE_FIELDS(CvSeq);                                           \
    int       total;            \
    int       elem_size;        \
    schar*    block_max;        \
    schar*    ptr;              \
    int       delta_elems;      \
    CvMemStorage* storage;      \
    CvSeqBlock* free_blocks;    \
    CvSeqBlock* first;

typedef struct CvSeq
{
    CV_SEQUENCE_FIELDS()
}
CvSeq;

#define CV_TYPE_NAME_SEQ             "opencv-sequence"
#define CV_TYPE_NAME_SEQ_TREE        "opencv-sequence-tree"



#define CV_SET_ELEM_FIELDS(elem_type)   \
    int  flags;                         \
    struct elem_type* next_free;

typedef struct CvSetElem
{
    CV_SET_ELEM_FIELDS(CvSetElem)
}
CvSetElem;

#define CV_SET_FIELDS()      \
    CV_SEQUENCE_FIELDS()     \
    CvSetElem* free_elems;   \
    int active_count;

typedef struct CvSet
{
    CV_SET_FIELDS()
}
CvSet;


#define CV_SET_ELEM_IDX_MASK   ((1 << 26) - 1)
#define CV_SET_ELEM_FREE_FLAG  (1 << (sizeof(int)*8-1))


#define CV_IS_SET_ELEM( ptr )  (((CvSetElem*)(ptr))->flags >= 0)




#define CV_GRAPH_EDGE_FIELDS()      \
    int flags;                      \
    float weight;                   \
    struct CvGraphEdge* next[2];    \
    struct CvGraphVtx* vtx[2];


#define CV_GRAPH_VERTEX_FIELDS()    \
    int flags;                      \
    struct CvGraphEdge* first;


typedef struct CvGraphEdge
{
    CV_GRAPH_EDGE_FIELDS()
}
CvGraphEdge;

typedef struct CvGraphVtx
{
    CV_GRAPH_VERTEX_FIELDS()
}
CvGraphVtx;

typedef struct CvGraphVtx2D
{
    CV_GRAPH_VERTEX_FIELDS()
    CvPoint2D32f* ptr;
}
CvGraphVtx2D;


#define  CV_GRAPH_FIELDS()   \
    CV_SET_FIELDS()          \
    CvSet* edges;

typedef struct CvGraph
{
    CV_GRAPH_FIELDS()
}
CvGraph;

#define CV_TYPE_NAME_GRAPH "opencv-graph"





typedef struct CvChain
{
    CV_SEQUENCE_FIELDS()
    CvPoint  origin;
}
CvChain;

#define CV_CONTOUR_FIELDS()  \
    CV_SEQUENCE_FIELDS()     \
    CvRect rect;             \
    int color;               \
    int reserved[3];

typedef struct CvContour
{
    CV_CONTOUR_FIELDS()
}
CvContour;

typedef CvContour CvPoint2DSeq;



#define CV_SEQ_MAGIC_VAL             0x42990000

#define CV_IS_SEQ(seq) \
    ((seq) != NULL && (((CvSeq*)(seq))->flags & CV_MAGIC_MASK) == CV_SEQ_MAGIC_VAL)

#define CV_SET_MAGIC_VAL             0x42980000
#define CV_IS_SET(set) \
    ((set) != NULL && (((CvSeq*)(set))->flags & CV_MAGIC_MASK) == CV_SET_MAGIC_VAL)

#define CV_SEQ_ELTYPE_BITS           12
#define CV_SEQ_ELTYPE_MASK           ((1 << CV_SEQ_ELTYPE_BITS) - 1)

#define CV_SEQ_ELTYPE_POINT          CV_32SC2
#define CV_SEQ_ELTYPE_CODE           CV_8UC1
#define CV_SEQ_ELTYPE_GENERIC        0
#define CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 )
#define CV_SEQ_ELTYPE_PPOINT         CV_SEQ_ELTYPE_PTR
#define CV_SEQ_ELTYPE_INDEX          CV_32SC1
#define CV_SEQ_ELTYPE_GRAPH_EDGE     0
#define CV_SEQ_ELTYPE_GRAPH_VERTEX   0
#define CV_SEQ_ELTYPE_TRIAN_ATR      0
#define CV_SEQ_ELTYPE_CONNECTED_COMP 0
#define CV_SEQ_ELTYPE_POINT3D        CV_32FC3

#define CV_SEQ_KIND_BITS        2
#define CV_SEQ_KIND_MASK        (((1 << CV_SEQ_KIND_BITS) - 1)<<CV_SEQ_ELTYPE_BITS)


#define CV_SEQ_KIND_GENERIC     (0 << CV_SEQ_ELTYPE_BITS)
#define CV_SEQ_KIND_CURVE       (1 << CV_SEQ_ELTYPE_BITS)
#define CV_SEQ_KIND_BIN_TREE    (2 << CV_SEQ_ELTYPE_BITS)


#define CV_SEQ_KIND_GRAPH       (1 << CV_SEQ_ELTYPE_BITS)
#define CV_SEQ_KIND_SUBDIV2D    (2 << CV_SEQ_ELTYPE_BITS)

#define CV_SEQ_FLAG_SHIFT       (CV_SEQ_KIND_BITS + CV_SEQ_ELTYPE_BITS)


#define CV_SEQ_FLAG_CLOSED     (1 << CV_SEQ_FLAG_SHIFT)
#define CV_SEQ_FLAG_SIMPLE     (0 << CV_SEQ_FLAG_SHIFT)
#define CV_SEQ_FLAG_CONVEX     (0 << CV_SEQ_FLAG_SHIFT)
#define CV_SEQ_FLAG_HOLE       (2 << CV_SEQ_FLAG_SHIFT)


#define CV_GRAPH_FLAG_ORIENTED (1 << CV_SEQ_FLAG_SHIFT)

#define CV_GRAPH               CV_SEQ_KIND_GRAPH
#define CV_ORIENTED_GRAPH      (CV_SEQ_KIND_GRAPH|CV_GRAPH_FLAG_ORIENTED)


#define CV_SEQ_POINT_SET       (CV_SEQ_KIND_GENERIC| CV_SEQ_ELTYPE_POINT)
#define CV_SEQ_POINT3D_SET     (CV_SEQ_KIND_GENERIC| CV_SEQ_ELTYPE_POINT3D)
#define CV_SEQ_POLYLINE        (CV_SEQ_KIND_CURVE  | CV_SEQ_ELTYPE_POINT)
#define CV_SEQ_POLYGON         (CV_SEQ_FLAG_CLOSED | CV_SEQ_POLYLINE )
#define CV_SEQ_CONTOUR         CV_SEQ_POLYGON
#define CV_SEQ_SIMPLE_POLYGON  (CV_SEQ_FLAG_SIMPLE | CV_SEQ_POLYGON  )


#define CV_SEQ_CHAIN           (CV_SEQ_KIND_CURVE  | CV_SEQ_ELTYPE_CODE)
#define CV_SEQ_CHAIN_CONTOUR   (CV_SEQ_FLAG_CLOSED | CV_SEQ_CHAIN)


#define CV_SEQ_POLYGON_TREE    (CV_SEQ_KIND_BIN_TREE  | CV_SEQ_ELTYPE_TRIAN_ATR)


#define CV_SEQ_CONNECTED_COMP  (CV_SEQ_KIND_GENERIC  | CV_SEQ_ELTYPE_CONNECTED_COMP)


#define CV_SEQ_INDEX           (CV_SEQ_KIND_GENERIC  | CV_SEQ_ELTYPE_INDEX)

#define CV_SEQ_ELTYPE( seq )   ((seq)->flags & CV_SEQ_ELTYPE_MASK)
#define CV_SEQ_KIND( seq )     ((seq)->flags & CV_SEQ_KIND_MASK )


#define CV_IS_SEQ_INDEX( seq )      ((CV_SEQ_ELTYPE(seq) == CV_SEQ_ELTYPE_INDEX) && \
                                     (CV_SEQ_KIND(seq) == CV_SEQ_KIND_GENERIC))

#define CV_IS_SEQ_CURVE( seq )      (CV_SEQ_KIND(seq) == CV_SEQ_KIND_CURVE)
#define CV_IS_SEQ_CLOSED( seq )     (((seq)->flags & CV_SEQ_FLAG_CLOSED) != 0)
#define CV_IS_SEQ_CONVEX( seq )     0
#define CV_IS_SEQ_HOLE( seq )       (((seq)->flags & CV_SEQ_FLAG_HOLE) != 0)
#define CV_IS_SEQ_SIMPLE( seq )     1


#define CV_IS_SEQ_POINT_SET( seq ) \
    ((CV_SEQ_ELTYPE(seq) == CV_32SC2 || CV_SEQ_ELTYPE(seq) == CV_32FC2))

#define CV_IS_SEQ_POINT_SUBSET( seq ) \
    (CV_IS_SEQ_INDEX( seq ) || CV_SEQ_ELTYPE(seq) == CV_SEQ_ELTYPE_PPOINT)

#define CV_IS_SEQ_POLYLINE( seq )   \
    (CV_SEQ_KIND(seq) == CV_SEQ_KIND_CURVE && CV_IS_SEQ_POINT_SET(seq))

#define CV_IS_SEQ_POLYGON( seq )   \
    (CV_IS_SEQ_POLYLINE(seq) && CV_IS_SEQ_CLOSED(seq))

#define CV_IS_SEQ_CHAIN( seq )   \
    (CV_SEQ_KIND(seq) == CV_SEQ_KIND_CURVE && (seq)->elem_size == 1)

#define CV_IS_SEQ_CONTOUR( seq )   \
    (CV_IS_SEQ_CLOSED(seq) && (CV_IS_SEQ_POLYLINE(seq) || CV_IS_SEQ_CHAIN(seq)))

#define CV_IS_SEQ_CHAIN_CONTOUR( seq ) \
    (CV_IS_SEQ_CHAIN( seq ) && CV_IS_SEQ_CLOSED( seq ))

#define CV_IS_SEQ_POLYGON_TREE( seq ) \
    (CV_SEQ_ELTYPE (seq) ==  CV_SEQ_ELTYPE_TRIAN_ATR &&    \
    CV_SEQ_KIND( seq ) ==  CV_SEQ_KIND_BIN_TREE )

#define CV_IS_GRAPH( seq )    \
    (CV_IS_SET(seq) && CV_SEQ_KIND((CvSet*)(seq)) == CV_SEQ_KIND_GRAPH)

#define CV_IS_GRAPH_ORIENTED( seq )   \
    (((seq)->flags & CV_GRAPH_FLAG_ORIENTED) != 0)

#define CV_IS_SUBDIV2D( seq )  \
    (CV_IS_SET(seq) && CV_SEQ_KIND((CvSet*)(seq)) == CV_SEQ_KIND_SUBDIV2D)





#define CV_SEQ_WRITER_FIELDS()                                     \
    int          header_size;                                      \
    CvSeq*       seq;                    \
    CvSeqBlock*  block;                         \
    schar*       ptr;                   \
    schar*       block_min;  \
    schar*       block_max;

typedef struct CvSeqWriter
{
    CV_SEQ_WRITER_FIELDS()
}
CvSeqWriter;


#define CV_SEQ_READER_FIELDS()                                      \
    int          header_size;                                       \
    CvSeq*       seq;                     \
    CvSeqBlock*  block;                          \
    schar*       ptr;          \
    schar*       block_min;  \
    schar*       block_max;        \
    int          delta_index;      \
    schar*       prev_elem;

typedef struct CvSeqReader
{
    CV_SEQ_READER_FIELDS()
}
CvSeqReader;





#define  CV_SEQ_ELEM( seq, elem_type, index )                    \
  \
(   assert(sizeof((seq)->first[0]) == sizeof(CvSeqBlock) &&      \
    (seq)->elem_size == sizeof(elem_type)),                      \
    (elem_type*)((seq)->first && (unsigned)index <               \
    (unsigned)((seq)->first->count) ?                            \
    (seq)->first->data + (index) * sizeof(elem_type) :           \
    cvGetSeqElem( (CvSeq*)(seq), (index) )))
#define CV_GET_SEQ_ELEM( elem_type, seq, index ) CV_SEQ_ELEM( (seq), elem_type, (index) )


#define CV_WRITE_SEQ_ELEM_VAR( elem_ptr, writer )     \
{                                                     \
    if( (writer).ptr >= (writer).block_max )          \
    {                                                 \
        cvCreateSeqBlock( &writer);                   \
    }                                                 \
    memcpy((writer).ptr, elem_ptr, (writer).seq->elem_size);\
    (writer).ptr += (writer).seq->elem_size;          \
}

#define CV_WRITE_SEQ_ELEM( elem, writer )             \
{                                                     \
    assert( (writer).seq->elem_size == sizeof(elem)); \
    if( (writer).ptr >= (writer).block_max )          \
    {                                                 \
        cvCreateSeqBlock( &writer);                   \
    }                                                 \
    assert( (writer).ptr <= (writer).block_max - sizeof(elem));\
    memcpy((writer).ptr, &(elem), sizeof(elem));      \
    (writer).ptr += sizeof(elem);                     \
}



#define CV_NEXT_SEQ_ELEM( elem_size, reader )                 \
{                                                             \
    if( ((reader).ptr += (elem_size)) >= (reader).block_max ) \
    {                                                         \
        cvChangeSeqBlock( &(reader), 1 );                     \
    }                                                         \
}



#define CV_PREV_SEQ_ELEM( elem_size, reader )                \
{                                                            \
    if( ((reader).ptr -= (elem_size)) < (reader).block_min ) \
    {                                                        \
        cvChangeSeqBlock( &(reader), -1 );                   \
    }                                                        \
}


#define CV_READ_SEQ_ELEM( elem, reader )                       \
{                                                              \
    assert( (reader).seq->elem_size == sizeof(elem));          \
    memcpy( &(elem), (reader).ptr, sizeof((elem)));            \
    CV_NEXT_SEQ_ELEM( sizeof(elem), reader )                   \
}


#define CV_REV_READ_SEQ_ELEM( elem, reader )                     \
{                                                                \
    assert( (reader).seq->elem_size == sizeof(elem));            \
    memcpy(&(elem), (reader).ptr, sizeof((elem)));               \
    CV_PREV_SEQ_ELEM( sizeof(elem), reader )                     \
}


#define CV_READ_CHAIN_POINT( _pt, reader )                              \
{                                                                       \
    (_pt) = (reader).pt;                                                \
    if( (reader).ptr )                                                  \
    {                                                                   \
        CV_READ_SEQ_ELEM( (reader).code, (reader));                     \
        assert( ((reader).code & ~7) == 0 );                            \
        (reader).pt.x += (reader).deltas[(int)(reader).code][0];        \
        (reader).pt.y += (reader).deltas[(int)(reader).code][1];        \
    }                                                                   \
}

#define CV_CURRENT_POINT( reader )  (*((CvPoint*)((reader).ptr)))
#define CV_PREV_POINT( reader )     (*((CvPoint*)((reader).prev_elem)))

#define CV_READ_EDGE( pt1, pt2, reader )               \
{                                                      \
    assert( sizeof(pt1) == sizeof(CvPoint) &&          \
            sizeof(pt2) == sizeof(CvPoint) &&          \
            reader.seq->elem_size == sizeof(CvPoint)); \
    (pt1) = CV_PREV_POINT( reader );                   \
    (pt2) = CV_CURRENT_POINT( reader );                \
    (reader).prev_elem = (reader).ptr;                 \
    CV_NEXT_SEQ_ELEM( sizeof(CvPoint), (reader));      \
}




#define  CV_NEXT_GRAPH_EDGE( edge, vertex )                              \
     (assert((edge)->vtx[0] == (vertex) || (edge)->vtx[1] == (vertex)),  \
      (edge)->next[(edge)->vtx[1] == (vertex)])





#if 0


typedef struct CvFileStorage CvFileStorage;


#define CV_STORAGE_READ          0
#define CV_STORAGE_WRITE         1
#define CV_STORAGE_WRITE_TEXT    CV_STORAGE_WRITE
#define CV_STORAGE_WRITE_BINARY  CV_STORAGE_WRITE
#define CV_STORAGE_APPEND        2
#define CV_STORAGE_MEMORY        4
#define CV_STORAGE_FORMAT_MASK   (7<<3)
#define CV_STORAGE_FORMAT_AUTO   0
#define CV_STORAGE_FORMAT_XML    8
#define CV_STORAGE_FORMAT_YAML  16
#define CV_STORAGE_FORMAT_JSON  24
#define CV_STORAGE_BASE64       64
#define CV_STORAGE_WRITE_BASE64  (CV_STORAGE_BASE64 | CV_STORAGE_WRITE)


typedef struct CvAttrList
{
    const char** attr;
    struct CvAttrList* next;
}
CvAttrList;


CV_INLINE CvAttrList cvAttrList( const char** attr CV_DEFAULT(NULL),
                                 CvAttrList* next CV_DEFAULT(NULL) )
{
    CvAttrList l;
    l.attr = attr;
    l.next = next;

    return l;
}

struct CvTypeInfo;

#define CV_NODE_NONE        0
#define CV_NODE_INT         1
#define CV_NODE_INTEGER     CV_NODE_INT
#define CV_NODE_REAL        2
#define CV_NODE_FLOAT       CV_NODE_REAL
#define CV_NODE_STR         3
#define CV_NODE_STRING      CV_NODE_STR
#define CV_NODE_REF         4
#define CV_NODE_SEQ         5
#define CV_NODE_MAP         6
#define CV_NODE_TYPE_MASK   7

#define CV_NODE_TYPE(flags)  ((flags) & CV_NODE_TYPE_MASK)


#define CV_NODE_FLOW        8
#define CV_NODE_USER        16
#define CV_NODE_EMPTY       32
#define CV_NODE_NAMED       64

#define CV_NODE_IS_INT(flags)        (CV_NODE_TYPE(flags) == CV_NODE_INT)
#define CV_NODE_IS_REAL(flags)       (CV_NODE_TYPE(flags) == CV_NODE_REAL)
#define CV_NODE_IS_STRING(flags)     (CV_NODE_TYPE(flags) == CV_NODE_STRING)
#define CV_NODE_IS_SEQ(flags)        (CV_NODE_TYPE(flags) == CV_NODE_SEQ)
#define CV_NODE_IS_MAP(flags)        (CV_NODE_TYPE(flags) == CV_NODE_MAP)
#define CV_NODE_IS_COLLECTION(flags) (CV_NODE_TYPE(flags) >= CV_NODE_SEQ)
#define CV_NODE_IS_FLOW(flags)       (((flags) & CV_NODE_FLOW) != 0)
#define CV_NODE_IS_EMPTY(flags)      (((flags) & CV_NODE_EMPTY) != 0)
#define CV_NODE_IS_USER(flags)       (((flags) & CV_NODE_USER) != 0)
#define CV_NODE_HAS_NAME(flags)      (((flags) & CV_NODE_NAMED) != 0)

#define CV_NODE_SEQ_SIMPLE 256
#define CV_NODE_SEQ_IS_SIMPLE(seq) (((seq)->flags & CV_NODE_SEQ_SIMPLE) != 0)

typedef struct CvString
{
    int len;
    char* ptr;
}
CvString;


typedef struct CvStringHashNode
{
    unsigned hashval;
    CvString str;
    struct CvStringHashNode* next;
}
CvStringHashNode;

typedef struct CvGenericHash CvFileNodeHash;


typedef struct CvFileNode
{
    int tag;
    struct CvTypeInfo* info;
    union
    {
        double f;
        int i;
        CvString str;
        CvSeq* seq;
        CvFileNodeHash* map;
    } data;
}
CvFileNode;

#ifdef __cplusplus
extern "C" {
#endif
typedef int (CV_CDECL *CvIsInstanceFunc)( const void* struct_ptr );
typedef void (CV_CDECL *CvReleaseFunc)( void** struct_dblptr );
typedef void* (CV_CDECL *CvReadFunc)( CvFileStorage* storage, CvFileNode* node );
typedef void (CV_CDECL *CvWriteFunc)( CvFileStorage* storage, const char* name,
                                      const void* struct_ptr, CvAttrList attributes );
typedef void* (CV_CDECL *CvCloneFunc)( const void* struct_ptr );
#ifdef __cplusplus
}
#endif


typedef struct CvTypeInfo
{
    int flags;
    int header_size;
    struct CvTypeInfo* prev;
    struct CvTypeInfo* next;
    const char* type_name;
    CvIsInstanceFunc is_instance;
    CvReleaseFunc release;
    CvReadFunc read;
    CvWriteFunc write;
    CvCloneFunc clone;
}
CvTypeInfo;
#endif



#endif


