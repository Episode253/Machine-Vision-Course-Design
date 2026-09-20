


#ifndef OPENCV_CORE_C_H
#define OPENCV_CORE_C_H

#include "opencv2/core/types_c.h"

#ifdef __cplusplus
#  ifdef _MSC_VER

#    pragma warning(disable:4190)
#  elif defined __clang__ && __clang_major__ >= 3
#    pragma GCC diagnostic ignored "-Wreturn-type-c-linkage"
#  endif
#endif

#ifdef __cplusplus
extern "C" {
#endif






CVAPI(void*)  cvAlloc( size_t size );


CVAPI(void)   cvFree_( void* ptr );
#define cvFree(ptr) (cvFree_(*(ptr)), *(ptr)=0)


CVAPI(IplImage*)  cvCreateImageHeader( CvSize size, int depth, int channels );


CVAPI(IplImage*) cvInitImageHeader( IplImage* image, CvSize size, int depth,
                                   int channels, int origin CV_DEFAULT(0),
                                   int align CV_DEFAULT(4));


CVAPI(IplImage*)  cvCreateImage( CvSize size, int depth, int channels );


CVAPI(void)  cvReleaseImageHeader( IplImage** image );


CVAPI(void)  cvReleaseImage( IplImage** image );


CVAPI(IplImage*) cvCloneImage( const IplImage* image );


CVAPI(void)  cvSetImageCOI( IplImage* image, int coi );


CVAPI(int)  cvGetImageCOI( const IplImage* image );


CVAPI(void)  cvSetImageROI( IplImage* image, CvRect rect );


CVAPI(void)  cvResetImageROI( IplImage* image );


CVAPI(CvRect) cvGetImageROI( const IplImage* image );


CVAPI(CvMat*)  cvCreateMatHeader( int rows, int cols, int type );

#define CV_AUTOSTEP  0x7fffffff


CVAPI(CvMat*) cvInitMatHeader( CvMat* mat, int rows, int cols,
                              int type, void* data CV_DEFAULT(NULL),
                              int step CV_DEFAULT(CV_AUTOSTEP) );


CVAPI(CvMat*)  cvCreateMat( int rows, int cols, int type );


CVAPI(void)  cvReleaseMat( CvMat** mat );


CV_INLINE  void  cvDecRefData( CvArr* arr )
{
    if( CV_IS_MAT( arr ))
    {
        CvMat* mat = (CvMat*)arr;
        mat->data.ptr = NULL;
        if( mat->refcount != NULL && --*mat->refcount == 0 )
            cvFree( &mat->refcount );
        mat->refcount = NULL;
    }
    else if( CV_IS_MATND( arr ))
    {
        CvMatND* mat = (CvMatND*)arr;
        mat->data.ptr = NULL;
        if( mat->refcount != NULL && --*mat->refcount == 0 )
            cvFree( &mat->refcount );
        mat->refcount = NULL;
    }
}


CV_INLINE  int  cvIncRefData( CvArr* arr )
{
    int refcount = 0;
    if( CV_IS_MAT( arr ))
    {
        CvMat* mat = (CvMat*)arr;
        if( mat->refcount != NULL )
            refcount = ++*mat->refcount;
    }
    else if( CV_IS_MATND( arr ))
    {
        CvMatND* mat = (CvMatND*)arr;
        if( mat->refcount != NULL )
            refcount = ++*mat->refcount;
    }
    return refcount;
}



CVAPI(CvMat*) cvCloneMat( const CvMat* mat );



CVAPI(CvMat*) cvGetSubRect( const CvArr* arr, CvMat* submat, CvRect rect );
#define cvGetSubArr cvGetSubRect


CVAPI(CvMat*) cvGetRows( const CvArr* arr, CvMat* submat,
                        int start_row, int end_row,
                        int delta_row CV_DEFAULT(1));


CV_INLINE  CvMat*  cvGetRow( const CvArr* arr, CvMat* submat, int row )
{
    return cvGetRows( arr, submat, row, row + 1, 1 );
}



CVAPI(CvMat*) cvGetCols( const CvArr* arr, CvMat* submat,
                        int start_col, int end_col );


CV_INLINE  CvMat*  cvGetCol( const CvArr* arr, CvMat* submat, int col )
{
    return cvGetCols( arr, submat, col, col + 1 );
}


CVAPI(CvMat*) cvGetDiag( const CvArr* arr, CvMat* submat,
                            int diag CV_DEFAULT(0));


CVAPI(void) cvScalarToRawData( const CvScalar* scalar, void* data, int type,
                              int extend_to_12 CV_DEFAULT(0) );

CVAPI(void) cvRawDataToScalar( const void* data, int type, CvScalar* scalar );


CVAPI(CvMatND*)  cvCreateMatNDHeader( int dims, const int* sizes, int type );


CVAPI(CvMatND*)  cvCreateMatND( int dims, const int* sizes, int type );


CVAPI(CvMatND*)  cvInitMatNDHeader( CvMatND* mat, int dims, const int* sizes,
                                    int type, void* data CV_DEFAULT(NULL) );


CV_INLINE  void  cvReleaseMatND( CvMatND** mat )
{
    cvReleaseMat( (CvMat**)mat );
}


CVAPI(CvMatND*) cvCloneMatND( const CvMatND* mat );


CVAPI(CvSparseMat*)  cvCreateSparseMat( int dims, const int* sizes, int type );


CVAPI(void)  cvReleaseSparseMat( CvSparseMat** mat );


CVAPI(CvSparseMat*) cvCloneSparseMat( const CvSparseMat* mat );


CVAPI(CvSparseNode*) cvInitSparseMatIterator( const CvSparseMat* mat,
                                              CvSparseMatIterator* mat_iterator );


CV_INLINE CvSparseNode* cvGetNextSparseNode( CvSparseMatIterator* mat_iterator )
{
    if( mat_iterator->node->next )
        return mat_iterator->node = mat_iterator->node->next;
    else
    {
        int idx;
        for( idx = ++mat_iterator->curidx; idx < mat_iterator->mat->hashsize; idx++ )
        {
            CvSparseNode* node = (CvSparseNode*)mat_iterator->mat->hashtable[idx];
            if( node )
            {
                mat_iterator->curidx = idx;
                return mat_iterator->node = node;
            }
        }
        return NULL;
    }
}


#define CV_MAX_ARR 10


typedef struct CvNArrayIterator
{
    int count;
    int dims;
    CvSize size;
    uchar* ptr[CV_MAX_ARR];
    int stack[CV_MAX_DIM];
    CvMatND* hdr[CV_MAX_ARR];
}
CvNArrayIterator;

#define CV_NO_DEPTH_CHECK     1
#define CV_NO_CN_CHECK        2
#define CV_NO_SIZE_CHECK      4


CVAPI(int) cvInitNArrayIterator( int count, CvArr** arrs,
                                 const CvArr* mask, CvMatND* stubs,
                                 CvNArrayIterator* array_iterator,
                                 int flags CV_DEFAULT(0) );


CVAPI(int) cvNextNArraySlice( CvNArrayIterator* array_iterator );



CVAPI(int) cvGetElemType( const CvArr* arr );


CVAPI(int) cvGetDims( const CvArr* arr, int* sizes CV_DEFAULT(NULL) );



CVAPI(int) cvGetDimSize( const CvArr* arr, int index );



CVAPI(uchar*) cvPtr1D( const CvArr* arr, int idx0, int* type CV_DEFAULT(NULL));

CVAPI(uchar*) cvPtr2D( const CvArr* arr, int idx0, int idx1, int* type CV_DEFAULT(NULL) );

CVAPI(uchar*) cvPtr3D( const CvArr* arr, int idx0, int idx1, int idx2,
                      int* type CV_DEFAULT(NULL));

CVAPI(uchar*) cvPtrND( const CvArr* arr, const int* idx, int* type CV_DEFAULT(NULL),
                      int create_node CV_DEFAULT(1),
                      unsigned* precalc_hashval CV_DEFAULT(NULL));


CVAPI(CvScalar) cvGet1D( const CvArr* arr, int idx0 );

CVAPI(CvScalar) cvGet2D( const CvArr* arr, int idx0, int idx1 );

CVAPI(CvScalar) cvGet3D( const CvArr* arr, int idx0, int idx1, int idx2 );

CVAPI(CvScalar) cvGetND( const CvArr* arr, const int* idx );


CVAPI(double) cvGetReal1D( const CvArr* arr, int idx0 );

CVAPI(double) cvGetReal2D( const CvArr* arr, int idx0, int idx1 );

CVAPI(double) cvGetReal3D( const CvArr* arr, int idx0, int idx1, int idx2 );

CVAPI(double) cvGetRealND( const CvArr* arr, const int* idx );


CVAPI(void) cvSet1D( CvArr* arr, int idx0, CvScalar value );

CVAPI(void) cvSet2D( CvArr* arr, int idx0, int idx1, CvScalar value );

CVAPI(void) cvSet3D( CvArr* arr, int idx0, int idx1, int idx2, CvScalar value );

CVAPI(void) cvSetND( CvArr* arr, const int* idx, CvScalar value );


CVAPI(void) cvSetReal1D( CvArr* arr, int idx0, double value );

CVAPI(void) cvSetReal2D( CvArr* arr, int idx0, int idx1, double value );

CVAPI(void) cvSetReal3D( CvArr* arr, int idx0,
                        int idx1, int idx2, double value );

CVAPI(void) cvSetRealND( CvArr* arr, const int* idx, double value );


CVAPI(void) cvClearND( CvArr* arr, const int* idx );


CVAPI(CvMat*) cvGetMat( const CvArr* arr, CvMat* header,
                       int* coi CV_DEFAULT(NULL),
                       int allowND CV_DEFAULT(0));


CVAPI(IplImage*) cvGetImage( const CvArr* arr, IplImage* image_header );



CVAPI(CvArr*) cvReshapeMatND( const CvArr* arr,
                             int sizeof_header, CvArr* header,
                             int new_cn, int new_dims, int* new_sizes );

#define cvReshapeND( arr, header, new_cn, new_dims, new_sizes )   \
      cvReshapeMatND( (arr), sizeof(*(header)), (header),         \
                      (new_cn), (new_dims), (new_sizes))


CVAPI(CvMat*) cvReshape( const CvArr* arr, CvMat* header,
                        int new_cn, int new_rows CV_DEFAULT(0) );


CVAPI(void) cvRepeat( const CvArr* src, CvArr* dst );


CVAPI(void)  cvCreateData( CvArr* arr );


CVAPI(void)  cvReleaseData( CvArr* arr );


CVAPI(void)  cvSetData( CvArr* arr, void* data, int step );


CVAPI(void) cvGetRawData( const CvArr* arr, uchar** data,
                         int* step CV_DEFAULT(NULL),
                         CvSize* roi_size CV_DEFAULT(NULL));


CVAPI(CvSize) cvGetSize( const CvArr* arr );


CVAPI(void)  cvCopy( const CvArr* src, CvArr* dst,
                     const CvArr* mask CV_DEFAULT(NULL) );


CVAPI(void)  cvSet( CvArr* arr, CvScalar value,
                    const CvArr* mask CV_DEFAULT(NULL) );


CVAPI(void)  cvSetZero( CvArr* arr );
#define cvZero  cvSetZero



CVAPI(void)  cvSplit( const CvArr* src, CvArr* dst0, CvArr* dst1,
                      CvArr* dst2, CvArr* dst3 );


CVAPI(void)  cvMerge( const CvArr* src0, const CvArr* src1,
                      const CvArr* src2, const CvArr* src3,
                      CvArr* dst );


CVAPI(void)  cvMixChannels( const CvArr** src, int src_count,
                            CvArr** dst, int dst_count,
                            const int* from_to, int pair_count );


CVAPI(void)  cvConvertScale( const CvArr* src, CvArr* dst,
                             double scale CV_DEFAULT(1),
                             double shift CV_DEFAULT(0) );
#define cvCvtScale cvConvertScale
#define cvScale  cvConvertScale
#define cvConvert( src, dst )  cvConvertScale( (src), (dst), 1, 0 )



CVAPI(void)  cvConvertScaleAbs( const CvArr* src, CvArr* dst,
                                double scale CV_DEFAULT(1),
                                double shift CV_DEFAULT(0) );
#define cvCvtScaleAbs  cvConvertScaleAbs



CVAPI(CvTermCriteria) cvCheckTermCriteria( CvTermCriteria criteria,
                                           double default_eps,
                                           int default_max_iters );




CVAPI(void)  cvAdd( const CvArr* src1, const CvArr* src2, CvArr* dst,
                    const CvArr* mask CV_DEFAULT(NULL));


CVAPI(void)  cvAddS( const CvArr* src, CvScalar value, CvArr* dst,
                     const CvArr* mask CV_DEFAULT(NULL));


CVAPI(void)  cvSub( const CvArr* src1, const CvArr* src2, CvArr* dst,
                    const CvArr* mask CV_DEFAULT(NULL));


CV_INLINE  void  cvSubS( const CvArr* src, CvScalar value, CvArr* dst,
                         const CvArr* mask CV_DEFAULT(NULL))
{
    cvAddS( src, cvScalar( -value.val[0], -value.val[1], -value.val[2], -value.val[3]),
            dst, mask );
}


CVAPI(void)  cvSubRS( const CvArr* src, CvScalar value, CvArr* dst,
                      const CvArr* mask CV_DEFAULT(NULL));


CVAPI(void)  cvMul( const CvArr* src1, const CvArr* src2,
                    CvArr* dst, double scale CV_DEFAULT(1) );


CVAPI(void)  cvDiv( const CvArr* src1, const CvArr* src2,
                    CvArr* dst, double scale CV_DEFAULT(1));


CVAPI(void)  cvScaleAdd( const CvArr* src1, CvScalar scale,
                         const CvArr* src2, CvArr* dst );
#define cvAXPY( A, real_scalar, B, C ) cvScaleAdd(A, cvRealScalar(real_scalar), B, C)


CVAPI(void)  cvAddWeighted( const CvArr* src1, double alpha,
                            const CvArr* src2, double beta,
                            double gamma, CvArr* dst );


CVAPI(double)  cvDotProduct( const CvArr* src1, const CvArr* src2 );


CVAPI(void) cvAnd( const CvArr* src1, const CvArr* src2,
                  CvArr* dst, const CvArr* mask CV_DEFAULT(NULL));


CVAPI(void) cvAndS( const CvArr* src, CvScalar value,
                   CvArr* dst, const CvArr* mask CV_DEFAULT(NULL));


CVAPI(void) cvOr( const CvArr* src1, const CvArr* src2,
                 CvArr* dst, const CvArr* mask CV_DEFAULT(NULL));


CVAPI(void) cvOrS( const CvArr* src, CvScalar value,
                  CvArr* dst, const CvArr* mask CV_DEFAULT(NULL));


CVAPI(void) cvXor( const CvArr* src1, const CvArr* src2,
                  CvArr* dst, const CvArr* mask CV_DEFAULT(NULL));


CVAPI(void) cvXorS( const CvArr* src, CvScalar value,
                   CvArr* dst, const CvArr* mask CV_DEFAULT(NULL));


CVAPI(void) cvNot( const CvArr* src, CvArr* dst );


CVAPI(void) cvInRange( const CvArr* src, const CvArr* lower,
                      const CvArr* upper, CvArr* dst );


CVAPI(void) cvInRangeS( const CvArr* src, CvScalar lower,
                       CvScalar upper, CvArr* dst );

#define CV_CMP_EQ   0
#define CV_CMP_GT   1
#define CV_CMP_GE   2
#define CV_CMP_LT   3
#define CV_CMP_LE   4
#define CV_CMP_NE   5




CVAPI(void) cvCmp( const CvArr* src1, const CvArr* src2, CvArr* dst, int cmp_op );


CVAPI(void) cvCmpS( const CvArr* src, double value, CvArr* dst, int cmp_op );


CVAPI(void) cvMin( const CvArr* src1, const CvArr* src2, CvArr* dst );


CVAPI(void) cvMax( const CvArr* src1, const CvArr* src2, CvArr* dst );


CVAPI(void) cvMinS( const CvArr* src, double value, CvArr* dst );


CVAPI(void) cvMaxS( const CvArr* src, double value, CvArr* dst );


CVAPI(void) cvAbsDiff( const CvArr* src1, const CvArr* src2, CvArr* dst );


CVAPI(void) cvAbsDiffS( const CvArr* src, CvArr* dst, CvScalar value );
#define cvAbs( src, dst ) cvAbsDiffS( (src), (dst), cvScalarAll(0))




CVAPI(void)  cvCartToPolar( const CvArr* x, const CvArr* y,
                            CvArr* magnitude, CvArr* angle CV_DEFAULT(NULL),
                            int angle_in_degrees CV_DEFAULT(0));


CVAPI(void)  cvPolarToCart( const CvArr* magnitude, const CvArr* angle,
                            CvArr* x, CvArr* y,
                            int angle_in_degrees CV_DEFAULT(0));


CVAPI(void)  cvPow( const CvArr* src, CvArr* dst, double power );


CVAPI(void)  cvExp( const CvArr* src, CvArr* dst );


CVAPI(void)  cvLog( const CvArr* src, CvArr* dst );


CVAPI(float) cvFastArctan( float y, float x );


CVAPI(float)  cvCbrt( float value );

#define  CV_CHECK_RANGE    1
#define  CV_CHECK_QUIET    2

CVAPI(int)  cvCheckArr( const CvArr* arr, int flags CV_DEFAULT(0),
                        double min_val CV_DEFAULT(0), double max_val CV_DEFAULT(0));
#define cvCheckArray cvCheckArr

#define CV_RAND_UNI      0
#define CV_RAND_NORMAL   1


CVAPI(void) cvRandArr( CvRNG* rng, CvArr* arr, int dist_type,
                      CvScalar param1, CvScalar param2 );

CVAPI(void) cvRandShuffle( CvArr* mat, CvRNG* rng,
                           double iter_factor CV_DEFAULT(1.));

#define CV_SORT_EVERY_ROW 0
#define CV_SORT_EVERY_COLUMN 1
#define CV_SORT_ASCENDING 0
#define CV_SORT_DESCENDING 16

CVAPI(void) cvSort( const CvArr* src, CvArr* dst CV_DEFAULT(NULL),
                    CvArr* idxmat CV_DEFAULT(NULL),
                    int flags CV_DEFAULT(0));


CVAPI(int) cvSolveCubic( const CvMat* coeffs, CvMat* roots );


CVAPI(void) cvSolvePoly(const CvMat* coeffs, CvMat *roots2,
      int maxiter CV_DEFAULT(20), int fig CV_DEFAULT(100));




CVAPI(void)  cvCrossProduct( const CvArr* src1, const CvArr* src2, CvArr* dst );


#define cvMatMulAdd( src1, src2, src3, dst ) cvGEMM( (src1), (src2), 1., (src3), 1., (dst), 0 )
#define cvMatMul( src1, src2, dst )  cvMatMulAdd( (src1), (src2), NULL, (dst))

#define CV_GEMM_A_T 1
#define CV_GEMM_B_T 2
#define CV_GEMM_C_T 4

CVAPI(void)  cvGEMM( const CvArr* src1, const CvArr* src2, double alpha,
                     const CvArr* src3, double beta, CvArr* dst,
                     int tABC CV_DEFAULT(0));
#define cvMatMulAddEx cvGEMM


CVAPI(void)  cvTransform( const CvArr* src, CvArr* dst,
                          const CvMat* transmat,
                          const CvMat* shiftvec CV_DEFAULT(NULL));
#define cvMatMulAddS cvTransform


CVAPI(void)  cvPerspectiveTransform( const CvArr* src, CvArr* dst,
                                     const CvMat* mat );


CVAPI(void) cvMulTransposed( const CvArr* src, CvArr* dst, int order,
                             const CvArr* delta CV_DEFAULT(NULL),
                             double scale CV_DEFAULT(1.) );


CVAPI(void)  cvTranspose( const CvArr* src, CvArr* dst );
#define cvT cvTranspose


CVAPI(void)  cvCompleteSymm( CvMat* matrix, int LtoR CV_DEFAULT(0) );


CVAPI(void)  cvFlip( const CvArr* src, CvArr* dst CV_DEFAULT(NULL),
                     int flip_mode CV_DEFAULT(0));
#define cvMirror cvFlip


#define CV_SVD_MODIFY_A   1
#define CV_SVD_U_T        2
#define CV_SVD_V_T        4


CVAPI(void)   cvSVD( CvArr* A, CvArr* W, CvArr* U CV_DEFAULT(NULL),
                     CvArr* V CV_DEFAULT(NULL), int flags CV_DEFAULT(0));


CVAPI(void)   cvSVBkSb( const CvArr* W, const CvArr* U,
                        const CvArr* V, const CvArr* B,
                        CvArr* X, int flags );

#define CV_LU  0
#define CV_SVD 1
#define CV_SVD_SYM 2
#define CV_CHOLESKY 3
#define CV_QR  4
#define CV_NORMAL 16


CVAPI(double)  cvInvert( const CvArr* src, CvArr* dst,
                         int method CV_DEFAULT(CV_LU));
#define cvInv cvInvert


CVAPI(int)  cvSolve( const CvArr* src1, const CvArr* src2, CvArr* dst,
                     int method CV_DEFAULT(CV_LU));


CVAPI(double) cvDet( const CvArr* mat );


CVAPI(CvScalar) cvTrace( const CvArr* mat );


CVAPI(void)  cvEigenVV( CvArr* mat, CvArr* evects, CvArr* evals,
                        double eps CV_DEFAULT(0),
                        int lowindex CV_DEFAULT(-1),
                        int highindex CV_DEFAULT(-1));






CVAPI(void)  cvSetIdentity( CvArr* mat, CvScalar value CV_DEFAULT(cvRealScalar(1)) );


CVAPI(CvArr*)  cvRange( CvArr* mat, double start, double end );




#define CV_COVAR_SCRAMBLED 0


#define CV_COVAR_NORMAL    1


#define CV_COVAR_USE_AVG   2


#define CV_COVAR_SCALE     4


#define CV_COVAR_ROWS      8


#define CV_COVAR_COLS     16




CVAPI(void)  cvCalcCovarMatrix( const CvArr** vects, int count,
                                CvArr* cov_mat, CvArr* avg, int flags );

#define CV_PCA_DATA_AS_ROW 0
#define CV_PCA_DATA_AS_COL 1
#define CV_PCA_USE_AVG 2
CVAPI(void)  cvCalcPCA( const CvArr* data, CvArr* mean,
                        CvArr* eigenvals, CvArr* eigenvects, int flags );

CVAPI(void)  cvProjectPCA( const CvArr* data, const CvArr* mean,
                           const CvArr* eigenvects, CvArr* result );

CVAPI(void)  cvBackProjectPCA( const CvArr* proj, const CvArr* mean,
                               const CvArr* eigenvects, CvArr* result );


CVAPI(double)  cvMahalanobis( const CvArr* vec1, const CvArr* vec2, const CvArr* mat );
#define cvMahalonobis  cvMahalanobis




CVAPI(CvScalar)  cvSum( const CvArr* arr );


CVAPI(int)  cvCountNonZero( const CvArr* arr );


CVAPI(CvScalar)  cvAvg( const CvArr* arr, const CvArr* mask CV_DEFAULT(NULL) );


CVAPI(void)  cvAvgSdv( const CvArr* arr, CvScalar* mean, CvScalar* std_dev,
                       const CvArr* mask CV_DEFAULT(NULL) );


CVAPI(void)  cvMinMaxLoc( const CvArr* arr, double* min_val, double* max_val,
                          CvPoint* min_loc CV_DEFAULT(NULL),
                          CvPoint* max_loc CV_DEFAULT(NULL),
                          const CvArr* mask CV_DEFAULT(NULL) );


#define CV_C            1
#define CV_L1           2
#define CV_L2           4
#define CV_NORM_MASK    7
#define CV_RELATIVE     8
#define CV_DIFF         16
#define CV_MINMAX       32

#define CV_DIFF_C       (CV_DIFF | CV_C)
#define CV_DIFF_L1      (CV_DIFF | CV_L1)
#define CV_DIFF_L2      (CV_DIFF | CV_L2)
#define CV_RELATIVE_C   (CV_RELATIVE | CV_C)
#define CV_RELATIVE_L1  (CV_RELATIVE | CV_L1)
#define CV_RELATIVE_L2  (CV_RELATIVE | CV_L2)



CVAPI(double)  cvNorm( const CvArr* arr1, const CvArr* arr2 CV_DEFAULT(NULL),
                       int norm_type CV_DEFAULT(CV_L2),
                       const CvArr* mask CV_DEFAULT(NULL) );


CVAPI(void)  cvNormalize( const CvArr* src, CvArr* dst,
                          double a CV_DEFAULT(1.), double b CV_DEFAULT(0.),
                          int norm_type CV_DEFAULT(CV_L2),
                          const CvArr* mask CV_DEFAULT(NULL) );


#define CV_REDUCE_SUM 0
#define CV_REDUCE_AVG 1
#define CV_REDUCE_MAX 2
#define CV_REDUCE_MIN 3



CVAPI(void)  cvReduce( const CvArr* src, CvArr* dst, int dim CV_DEFAULT(-1),
                       int op CV_DEFAULT(CV_REDUCE_SUM) );




#define CV_DXT_FORWARD  0
#define CV_DXT_INVERSE  1
#define CV_DXT_SCALE    2
#define CV_DXT_INV_SCALE (CV_DXT_INVERSE + CV_DXT_SCALE)
#define CV_DXT_INVERSE_SCALE CV_DXT_INV_SCALE
#define CV_DXT_ROWS     4
#define CV_DXT_MUL_CONJ 8



CVAPI(void)  cvDFT( const CvArr* src, CvArr* dst, int flags,
                    int nonzero_rows CV_DEFAULT(0) );
#define cvFFT cvDFT


CVAPI(void)  cvMulSpectrums( const CvArr* src1, const CvArr* src2,
                             CvArr* dst, int flags );


CVAPI(int)  cvGetOptimalDFTSize( int size0 );


CVAPI(void)  cvDCT( const CvArr* src, CvArr* dst, int flags );




CVAPI(int) cvSliceLength( CvSlice slice, const CvSeq* seq );



CVAPI(CvMemStorage*)  cvCreateMemStorage( int block_size CV_DEFAULT(0));



CVAPI(CvMemStorage*)  cvCreateChildMemStorage( CvMemStorage* parent );



CVAPI(void)  cvReleaseMemStorage( CvMemStorage** storage );



CVAPI(void)  cvClearMemStorage( CvMemStorage* storage );


CVAPI(void)  cvSaveMemStoragePos( const CvMemStorage* storage, CvMemStoragePos* pos );


CVAPI(void)  cvRestoreMemStoragePos( CvMemStorage* storage, CvMemStoragePos* pos );


CVAPI(void*) cvMemStorageAlloc( CvMemStorage* storage, size_t size );






CVAPI(CvSeq*)  cvCreateSeq( int seq_flags, size_t header_size,
                            size_t elem_size, CvMemStorage* storage );


CVAPI(void)  cvSetSeqBlockSize( CvSeq* seq, int delta_elems );



CVAPI(schar*)  cvSeqPush( CvSeq* seq, const void* element CV_DEFAULT(NULL));



CVAPI(schar*)  cvSeqPushFront( CvSeq* seq, const void* element CV_DEFAULT(NULL));



CVAPI(void)  cvSeqPop( CvSeq* seq, void* element CV_DEFAULT(NULL));



CVAPI(void)  cvSeqPopFront( CvSeq* seq, void* element CV_DEFAULT(NULL));


#define CV_FRONT 1
#define CV_BACK 0

CVAPI(void)  cvSeqPushMulti( CvSeq* seq, const void* elements,
                             int count, int in_front CV_DEFAULT(0) );


CVAPI(void)  cvSeqPopMulti( CvSeq* seq, void* elements,
                            int count, int in_front CV_DEFAULT(0) );


CVAPI(schar*)  cvSeqInsert( CvSeq* seq, int before_index,
                            const void* element CV_DEFAULT(NULL));


CVAPI(void)  cvSeqRemove( CvSeq* seq, int index );



CVAPI(void)  cvClearSeq( CvSeq* seq );



CVAPI(schar*)  cvGetSeqElem( const CvSeq* seq, int index );


CVAPI(int)  cvSeqElemIdx( const CvSeq* seq, const void* element,
                         CvSeqBlock** block CV_DEFAULT(NULL) );


CVAPI(void)  cvStartAppendToSeq( CvSeq* seq, CvSeqWriter* writer );



CVAPI(void)  cvStartWriteSeq( int seq_flags, int header_size,
                              int elem_size, CvMemStorage* storage,
                              CvSeqWriter* writer );


CVAPI(CvSeq*)  cvEndWriteSeq( CvSeqWriter* writer );



CVAPI(void)   cvFlushSeqWriter( CvSeqWriter* writer );



CVAPI(void) cvStartReadSeq( const CvSeq* seq, CvSeqReader* reader,
                           int reverse CV_DEFAULT(0) );



CVAPI(int)  cvGetSeqReaderPos( CvSeqReader* reader );



CVAPI(void)   cvSetSeqReaderPos( CvSeqReader* reader, int index,
                                 int is_relative CV_DEFAULT(0));


CVAPI(void*)  cvCvtSeqToArray( const CvSeq* seq, void* elements,
                               CvSlice slice CV_DEFAULT(CV_WHOLE_SEQ) );


CVAPI(CvSeq*) cvMakeSeqHeaderForArray( int seq_type, int header_size,
                                       int elem_size, void* elements, int total,
                                       CvSeq* seq, CvSeqBlock* block );


CVAPI(CvSeq*) cvSeqSlice( const CvSeq* seq, CvSlice slice,
                         CvMemStorage* storage CV_DEFAULT(NULL),
                         int copy_data CV_DEFAULT(0));

CV_INLINE CvSeq* cvCloneSeq( const CvSeq* seq, CvMemStorage* storage CV_DEFAULT(NULL))
{
    return cvSeqSlice( seq, CV_WHOLE_SEQ, storage, 1 );
}


CVAPI(void)  cvSeqRemoveSlice( CvSeq* seq, CvSlice slice );


CVAPI(void)  cvSeqInsertSlice( CvSeq* seq, int before_index, const CvArr* from_arr );


typedef int (CV_CDECL* CvCmpFunc)(const void* a, const void* b, void* userdata );


CVAPI(void) cvSeqSort( CvSeq* seq, CvCmpFunc func, void* userdata CV_DEFAULT(NULL) );


CVAPI(schar*) cvSeqSearch( CvSeq* seq, const void* elem, CvCmpFunc func,
                           int is_sorted, int* elem_idx,
                           void* userdata CV_DEFAULT(NULL) );


CVAPI(void) cvSeqInvert( CvSeq* seq );


CVAPI(int)  cvSeqPartition( const CvSeq* seq, CvMemStorage* storage,
                            CvSeq** labels, CvCmpFunc is_equal, void* userdata );


CVAPI(void)  cvChangeSeqBlock( void* reader, int direction );
CVAPI(void)  cvCreateSeqBlock( CvSeqWriter* writer );



CVAPI(CvSet*)  cvCreateSet( int set_flags, int header_size,
                            int elem_size, CvMemStorage* storage );


CVAPI(int)  cvSetAdd( CvSet* set_header, CvSetElem* elem CV_DEFAULT(NULL),
                      CvSetElem** inserted_elem CV_DEFAULT(NULL) );


CV_INLINE  CvSetElem* cvSetNew( CvSet* set_header )
{
    CvSetElem* elem = set_header->free_elems;
    if( elem )
    {
        set_header->free_elems = elem->next_free;
        elem->flags = elem->flags & CV_SET_ELEM_IDX_MASK;
        set_header->active_count++;
    }
    else
        cvSetAdd( set_header, NULL, &elem );
    return elem;
}


CV_INLINE  void cvSetRemoveByPtr( CvSet* set_header, void* elem )
{
    CvSetElem* _elem = (CvSetElem*)elem;
    assert( _elem->flags >= 0  );
    _elem->next_free = set_header->free_elems;
    _elem->flags = (_elem->flags & CV_SET_ELEM_IDX_MASK) | CV_SET_ELEM_FREE_FLAG;
    set_header->free_elems = _elem;
    set_header->active_count--;
}


CVAPI(void)   cvSetRemove( CvSet* set_header, int index );


CV_INLINE CvSetElem* cvGetSetElem( const CvSet* set_header, int idx )
{
    CvSetElem* elem = (CvSetElem*)(void *)cvGetSeqElem( (CvSeq*)set_header, idx );
    return elem && CV_IS_SET_ELEM( elem ) ? elem : 0;
}


CVAPI(void)  cvClearSet( CvSet* set_header );


CVAPI(CvGraph*)  cvCreateGraph( int graph_flags, int header_size,
                                int vtx_size, int edge_size,
                                CvMemStorage* storage );


CVAPI(int)  cvGraphAddVtx( CvGraph* graph, const CvGraphVtx* vtx CV_DEFAULT(NULL),
                           CvGraphVtx** inserted_vtx CV_DEFAULT(NULL) );



CVAPI(int)  cvGraphRemoveVtx( CvGraph* graph, int index );
CVAPI(int)  cvGraphRemoveVtxByPtr( CvGraph* graph, CvGraphVtx* vtx );



CVAPI(int)  cvGraphAddEdge( CvGraph* graph,
                            int start_idx, int end_idx,
                            const CvGraphEdge* edge CV_DEFAULT(NULL),
                            CvGraphEdge** inserted_edge CV_DEFAULT(NULL) );

CVAPI(int)  cvGraphAddEdgeByPtr( CvGraph* graph,
                               CvGraphVtx* start_vtx, CvGraphVtx* end_vtx,
                               const CvGraphEdge* edge CV_DEFAULT(NULL),
                               CvGraphEdge** inserted_edge CV_DEFAULT(NULL) );


CVAPI(void)  cvGraphRemoveEdge( CvGraph* graph, int start_idx, int end_idx );
CVAPI(void)  cvGraphRemoveEdgeByPtr( CvGraph* graph, CvGraphVtx* start_vtx,
                                     CvGraphVtx* end_vtx );


CVAPI(CvGraphEdge*)  cvFindGraphEdge( const CvGraph* graph, int start_idx, int end_idx );
CVAPI(CvGraphEdge*)  cvFindGraphEdgeByPtr( const CvGraph* graph,
                                           const CvGraphVtx* start_vtx,
                                           const CvGraphVtx* end_vtx );
#define cvGraphFindEdge cvFindGraphEdge
#define cvGraphFindEdgeByPtr cvFindGraphEdgeByPtr


CVAPI(void)  cvClearGraph( CvGraph* graph );



CVAPI(int)  cvGraphVtxDegree( const CvGraph* graph, int vtx_idx );
CVAPI(int)  cvGraphVtxDegreeByPtr( const CvGraph* graph, const CvGraphVtx* vtx );



#define cvGetGraphVtx( graph, idx ) (CvGraphVtx*)cvGetSetElem((CvSet*)(graph), (idx))


#define cvGraphVtxIdx( graph, vtx ) ((vtx)->flags & CV_SET_ELEM_IDX_MASK)


#define cvGraphEdgeIdx( graph, edge ) ((edge)->flags & CV_SET_ELEM_IDX_MASK)

#define cvGraphGetVtxCount( graph ) ((graph)->active_count)
#define cvGraphGetEdgeCount( graph ) ((graph)->edges->active_count)

#define  CV_GRAPH_VERTEX        1
#define  CV_GRAPH_TREE_EDGE     2
#define  CV_GRAPH_BACK_EDGE     4
#define  CV_GRAPH_FORWARD_EDGE  8
#define  CV_GRAPH_CROSS_EDGE    16
#define  CV_GRAPH_ANY_EDGE      30
#define  CV_GRAPH_NEW_TREE      32
#define  CV_GRAPH_BACKTRACKING  64
#define  CV_GRAPH_OVER          -1

#define  CV_GRAPH_ALL_ITEMS    -1


#define  CV_GRAPH_ITEM_VISITED_FLAG  (1 << 30)
#define  CV_IS_GRAPH_VERTEX_VISITED(vtx) \
    (((CvGraphVtx*)(vtx))->flags & CV_GRAPH_ITEM_VISITED_FLAG)
#define  CV_IS_GRAPH_EDGE_VISITED(edge) \
    (((CvGraphEdge*)(edge))->flags & CV_GRAPH_ITEM_VISITED_FLAG)
#define  CV_GRAPH_SEARCH_TREE_NODE_FLAG   (1 << 29)
#define  CV_GRAPH_FORWARD_EDGE_FLAG       (1 << 28)

typedef struct CvGraphScanner
{
    CvGraphVtx* vtx;
    CvGraphVtx* dst;
    CvGraphEdge* edge;

    CvGraph* graph;
    CvSeq*   stack;
    int      index;
    int      mask;
}
CvGraphScanner;


CVAPI(CvGraphScanner*)  cvCreateGraphScanner( CvGraph* graph,
                                             CvGraphVtx* vtx CV_DEFAULT(NULL),
                                             int mask CV_DEFAULT(CV_GRAPH_ALL_ITEMS));


CVAPI(void) cvReleaseGraphScanner( CvGraphScanner** scanner );


CVAPI(int)  cvNextGraphItem( CvGraphScanner* scanner );


CVAPI(CvGraph*) cvCloneGraph( const CvGraph* graph, CvMemStorage* storage );



CVAPI(void) cvLUT( const CvArr* src, CvArr* dst, const CvArr* lut );



typedef struct CvTreeNodeIterator
{
    const void* node;
    int level;
    int max_level;
}
CvTreeNodeIterator;

CVAPI(void) cvInitTreeNodeIterator( CvTreeNodeIterator* tree_iterator,
                                   const void* first, int max_level );
CVAPI(void*) cvNextTreeNode( CvTreeNodeIterator* tree_iterator );
CVAPI(void*) cvPrevTreeNode( CvTreeNodeIterator* tree_iterator );


CVAPI(void) cvInsertNodeIntoTree( void* node, void* parent, void* frame );


CVAPI(void) cvRemoveNodeFromTree( void* node, void* frame );


CVAPI(CvSeq*) cvTreeToNodeSeq( const void* first, int header_size,
                              CvMemStorage* storage );


#define CV_KMEANS_USE_INITIAL_LABELS    1
CVAPI(int) cvKMeans2( const CvArr* samples, int cluster_count, CvArr* labels,
                      CvTermCriteria termcrit, int attempts CV_DEFAULT(1),
                      CvRNG* rng CV_DEFAULT(0), int flags CV_DEFAULT(0),
                      CvArr* _centers CV_DEFAULT(0), double* compactness CV_DEFAULT(0) );




CVAPI(int)  cvUseOptimized( int on_off );

typedef IplImage* (CV_STDCALL* Cv_iplCreateImageHeader)
                            (int,int,int,char*,char*,int,int,int,int,int,
                            IplROI*,IplImage*,void*,IplTileInfo*);
typedef void (CV_STDCALL* Cv_iplAllocateImageData)(IplImage*,int,int);
typedef void (CV_STDCALL* Cv_iplDeallocate)(IplImage*,int);
typedef IplROI* (CV_STDCALL* Cv_iplCreateROI)(int,int,int,int,int);
typedef IplImage* (CV_STDCALL* Cv_iplCloneImage)(const IplImage*);


CVAPI(void) cvSetIPLAllocators( Cv_iplCreateImageHeader create_header,
                               Cv_iplAllocateImageData allocate_data,
                               Cv_iplDeallocate deallocate,
                               Cv_iplCreateROI create_roi,
                               Cv_iplCloneImage clone_image );

#define CV_TURN_ON_IPL_COMPATIBILITY()                                  \
    cvSetIPLAllocators( iplCreateImageHeader, iplAllocateImage,         \
                        iplDeallocate, iplCreateROI, iplCloneImage )



#if 0



CVAPI(CvFileStorage*)  cvOpenFileStorage( const char* filename, CvMemStorage* memstorage,
                                          int flags, const char* encoding CV_DEFAULT(NULL) );


CVAPI(void) cvReleaseFileStorage( CvFileStorage** fs );


CVAPI(const char*) cvAttrValue( const CvAttrList* attr, const char* attr_name );


CVAPI(void) cvStartWriteStruct( CvFileStorage* fs, const char* name,
                                int struct_flags, const char* type_name CV_DEFAULT(NULL),
                                CvAttrList attributes CV_DEFAULT(cvAttrList()));


CVAPI(void) cvEndWriteStruct( CvFileStorage* fs );


CVAPI(void) cvWriteInt( CvFileStorage* fs, const char* name, int value );


CVAPI(void) cvWriteReal( CvFileStorage* fs, const char* name, double value );


CVAPI(void) cvWriteString( CvFileStorage* fs, const char* name,
                           const char* str, int quote CV_DEFAULT(0) );


CVAPI(void) cvWriteComment( CvFileStorage* fs, const char* comment,
                            int eol_comment );


CVAPI(void) cvWrite( CvFileStorage* fs, const char* name, const void* ptr,
                         CvAttrList attributes CV_DEFAULT(cvAttrList()));


CVAPI(void) cvStartNextStream( CvFileStorage* fs );


CVAPI(void) cvWriteRawData( CvFileStorage* fs, const void* src,
                                int len, const char* dt );


CVAPI(void) cvWriteRawDataBase64( CvFileStorage* fs, const void* src,
                                 int len, const char* dt );


CVAPI(CvStringHashNode*) cvGetHashedKey( CvFileStorage* fs, const char* name,
                                        int len CV_DEFAULT(-1),
                                        int create_missing CV_DEFAULT(0));


CVAPI(CvFileNode*) cvGetRootFileNode( const CvFileStorage* fs,
                                     int stream_index CV_DEFAULT(0) );


CVAPI(CvFileNode*) cvGetFileNode( CvFileStorage* fs, CvFileNode* map,
                                 const CvStringHashNode* key,
                                 int create_missing CV_DEFAULT(0) );


CVAPI(CvFileNode*) cvGetFileNodeByName( const CvFileStorage* fs,
                                       const CvFileNode* map,
                                       const char* name );


CV_INLINE int cvReadInt( const CvFileNode* node, int default_value CV_DEFAULT(0) )
{
    return !node ? default_value :
        CV_NODE_IS_INT(node->tag) ? node->data.i :
        CV_NODE_IS_REAL(node->tag) ? cvRound(node->data.f) : 0x7fffffff;
}


CV_INLINE int cvReadIntByName( const CvFileStorage* fs, const CvFileNode* map,
                         const char* name, int default_value CV_DEFAULT(0) )
{
    return cvReadInt( cvGetFileNodeByName( fs, map, name ), default_value );
}


CV_INLINE double cvReadReal( const CvFileNode* node, double default_value CV_DEFAULT(0.) )
{
    return !node ? default_value :
        CV_NODE_IS_INT(node->tag) ? (double)node->data.i :
        CV_NODE_IS_REAL(node->tag) ? node->data.f : 1e300;
}


CV_INLINE double cvReadRealByName( const CvFileStorage* fs, const CvFileNode* map,
                        const char* name, double default_value CV_DEFAULT(0.) )
{
    return cvReadReal( cvGetFileNodeByName( fs, map, name ), default_value );
}


CV_INLINE const char* cvReadString( const CvFileNode* node,
                        const char* default_value CV_DEFAULT(NULL) )
{
    return !node ? default_value : CV_NODE_IS_STRING(node->tag) ? node->data.str.ptr : 0;
}


CV_INLINE const char* cvReadStringByName( const CvFileStorage* fs, const CvFileNode* map,
                        const char* name, const char* default_value CV_DEFAULT(NULL) )
{
    return cvReadString( cvGetFileNodeByName( fs, map, name ), default_value );
}



CVAPI(void*) cvRead( CvFileStorage* fs, CvFileNode* node,
                        CvAttrList* attributes CV_DEFAULT(NULL));


CV_INLINE void* cvReadByName( CvFileStorage* fs, const CvFileNode* map,
                              const char* name, CvAttrList* attributes CV_DEFAULT(NULL) )
{
    return cvRead( fs, cvGetFileNodeByName( fs, map, name ), attributes );
}



CVAPI(void) cvStartReadRawData( const CvFileStorage* fs, const CvFileNode* src,
                               CvSeqReader* reader );


CVAPI(void) cvReadRawDataSlice( const CvFileStorage* fs, CvSeqReader* reader,
                               int count, void* dst, const char* dt );


CVAPI(void) cvReadRawData( const CvFileStorage* fs, const CvFileNode* src,
                          void* dst, const char* dt );


CVAPI(void) cvWriteFileNode( CvFileStorage* fs, const char* new_node_name,
                            const CvFileNode* node, int embed );


CVAPI(const char*) cvGetFileNodeName( const CvFileNode* node );




CVAPI(void) cvRegisterType( const CvTypeInfo* info );


CVAPI(void) cvUnregisterType( const char* type_name );


CVAPI(CvTypeInfo*) cvFirstType(void);


CVAPI(CvTypeInfo*) cvFindType( const char* type_name );


CVAPI(CvTypeInfo*) cvTypeOf( const void* struct_ptr );

#endif


CVAPI(void) cvRelease( void** struct_ptr );


CVAPI(void*) cvClone( const void* struct_ptr );




CVAPI(int64)  cvGetTickCount( void );
CVAPI(double) cvGetTickFrequency( void );



CVAPI(int) cvCheckHardwareSupport(int feature);




CVAPI(int)  cvGetNumThreads( void );
CVAPI(void) cvSetNumThreads( int threads CV_DEFAULT(0) );

CVAPI(int)  cvGetThreadNum( void );





CVAPI(int) cvGetErrStatus( void );


CVAPI(void) cvSetErrStatus( int status );

#define CV_ErrModeLeaf     0
#define CV_ErrModeParent   1
#define CV_ErrModeSilent   2


CVAPI(int)  cvGetErrMode( void );


CVAPI(int) cvSetErrMode( int mode );


CVAPI(void) cvError( int status, const char* func_name,
                    const char* err_msg, const char* file_name, int line );


CVAPI(const char*) cvErrorStr( int status );


CVAPI(int) cvGetErrInfo( const char** errcode_desc, const char** description,
                        const char** filename, int* line );


CVAPI(int) cvErrorFromIppStatus( int ipp_status );

typedef int (CV_CDECL *CvErrorCallback)( int status, const char* func_name,
                                        const char* err_msg, const char* file_name, int line, void* userdata );


CVAPI(CvErrorCallback) cvRedirectError( CvErrorCallback error_handler,
                                       void* userdata CV_DEFAULT(NULL),
                                       void** prev_userdata CV_DEFAULT(NULL) );


CVAPI(int) cvNulDevReport( int status, const char* func_name, const char* err_msg,
                          const char* file_name, int line, void* userdata );


CVAPI(int) cvStdErrReport( int status, const char* func_name, const char* err_msg,
                          const char* file_name, int line, void* userdata );


CVAPI(int) cvGuiBoxReport( int status, const char* func_name, const char* err_msg,
                          const char* file_name, int line, void* userdata );

#define OPENCV_ERROR(status,func,context)                           \
cvError((status),(func),(context),__FILE__,__LINE__)

#define OPENCV_ASSERT(expr,func,context)                            \
{if (! (expr))                                      \
{OPENCV_ERROR(CV_StsInternal,(func),(context));}}

#define OPENCV_CALL( Func )                                         \
{                                                                   \
Func;                                                           \
}



#ifdef CV_NO_FUNC_NAMES
#define CV_FUNCNAME( Name )
#define cvFuncName ""
#else
#define CV_FUNCNAME( Name )  \
static char cvFuncName[] = Name
#endif



#define CV_ERROR( Code, Msg )                                       \
{                                                                   \
    cvError( (Code), cvFuncName, Msg, __FILE__, __LINE__ );        \
    __CV_EXIT__;                                                   \
}


#define CV_CHECK()                                                  \
{                                                                   \
    if( cvGetErrStatus() < 0 )                                      \
        CV_ERROR( CV_StsBackTrace, "Inner function failed." );      \
}



#define CV_CALL( Func )                                             \
{                                                                   \
    Func;                                                           \
    CV_CHECK();                                                     \
}



#define CV_ASSERT( Condition )                                          \
{                                                                       \
    if( !(Condition) )                                                  \
        CV_ERROR( CV_StsInternal, "Assertion: " #Condition " failed" ); \
}

#define __CV_BEGIN__       {
#define __CV_END__         goto exit; exit: ; }
#define __CV_EXIT__        goto exit



#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include "opencv2/core/utility.hpp"

namespace cv
{







CV_EXPORTS Mat cvarrToMat(const CvArr* arr, bool copyData=false,
                          bool allowND=true, int coiMode=0,
                          AutoBuffer<double>* buf=0);

static inline Mat cvarrToMatND(const CvArr* arr, bool copyData=false, int coiMode=0)
{
    return cvarrToMat(arr, copyData, true, coiMode);
}



CV_EXPORTS void extractImageCOI(const CvArr* arr, OutputArray coiimg, int coi=-1);

CV_EXPORTS void insertImageCOI(InputArray coiimg, CvArr* arr, int coi=-1);





template<> struct DefaultDeleter<CvMat>{ CV_EXPORTS void operator ()(CvMat* obj) const; };
template<> struct DefaultDeleter<IplImage>{ CV_EXPORTS void operator ()(IplImage* obj) const; };
template<> struct DefaultDeleter<CvMatND>{ CV_EXPORTS void operator ()(CvMatND* obj) const; };
template<> struct DefaultDeleter<CvSparseMat>{ CV_EXPORTS void operator ()(CvSparseMat* obj) const; };
template<> struct DefaultDeleter<CvMemStorage>{ CV_EXPORTS void operator ()(CvMemStorage* obj) const; };



template<typename _Tp> class SeqIterator;

typedef Ptr<CvMemStorage> MemStorage;


template<typename _Tp> class Seq
{
public:
    typedef SeqIterator<_Tp> iterator;
    typedef SeqIterator<_Tp> const_iterator;


    Seq();

    Seq(const CvSeq* seq);

    Seq(MemStorage& storage, int headerSize = sizeof(CvSeq));

    _Tp& operator [](int idx);

    const _Tp& operator[](int idx) const;

    SeqIterator<_Tp> begin() const;

    SeqIterator<_Tp> end() const;

    size_t size() const;

    int type() const;

    int depth() const;

    int channels() const;

    size_t elemSize() const;

    size_t index(const _Tp& elem) const;

    void push_back(const _Tp& elem);

    void push_front(const _Tp& elem);

    void push_back(const _Tp* elems, size_t count);

    void push_front(const _Tp* elems, size_t count);

    void insert(int idx, const _Tp& elem);

    void insert(int idx, const _Tp* elems, size_t count);

    void remove(int idx);

    void remove(const Range& r);


    _Tp& front();

    const _Tp& front() const;

    _Tp& back();

    const _Tp& back() const;

    bool empty() const;


    void clear();

    void pop_front();

    void pop_back();

    void pop_front(_Tp* elems, size_t count);

    void pop_back(_Tp* elems, size_t count);


    void copyTo(std::vector<_Tp>& vec, const Range& range=Range::all()) const;

    operator std::vector<_Tp>() const;

    CvSeq* seq;
};



template<typename _Tp> class SeqIterator : public CvSeqReader
{
public:

    SeqIterator();

    SeqIterator(const Seq<_Tp>& seq, bool seekEnd=false);

    void seek(size_t pos);

    size_t tell() const;

    _Tp& operator *();

    const _Tp& operator *() const;

    SeqIterator& operator ++();

    SeqIterator operator ++(int) const;

    SeqIterator& operator --();

    SeqIterator operator --(int) const;


    SeqIterator& operator +=(int);

    SeqIterator& operator -=(int);



    int index;
};




CV_EXPORTS schar*  seqPush( CvSeq* seq, const void* element=0);
CV_EXPORTS schar*  seqPushFront( CvSeq* seq, const void* element=0);
CV_EXPORTS void  seqPop( CvSeq* seq, void* element=0);
CV_EXPORTS void  seqPopFront( CvSeq* seq, void* element=0);
CV_EXPORTS void  seqPopMulti( CvSeq* seq, void* elements,
                              int count, int in_front=0 );
CV_EXPORTS void  seqRemove( CvSeq* seq, int index );
CV_EXPORTS void  clearSeq( CvSeq* seq );
CV_EXPORTS schar*  getSeqElem( const CvSeq* seq, int index );
CV_EXPORTS void  seqRemoveSlice( CvSeq* seq, CvSlice slice );
CV_EXPORTS void  seqInsertSlice( CvSeq* seq, int before_index, const CvArr* from_arr );

template<typename _Tp> inline Seq<_Tp>::Seq() : seq(0) {}
template<typename _Tp> inline Seq<_Tp>::Seq( const CvSeq* _seq ) : seq((CvSeq*)_seq)
{
    CV_Assert(!_seq || _seq->elem_size == sizeof(_Tp));
}

template<typename _Tp> inline Seq<_Tp>::Seq( MemStorage& storage,
                                             int headerSize )
{
    CV_Assert(headerSize >= (int)sizeof(CvSeq));
    seq = cvCreateSeq(DataType<_Tp>::type, headerSize, sizeof(_Tp), storage);
}

template<typename _Tp> inline _Tp& Seq<_Tp>::operator [](int idx)
{ return *(_Tp*)getSeqElem(seq, idx); }

template<typename _Tp> inline const _Tp& Seq<_Tp>::operator [](int idx) const
{ return *(_Tp*)getSeqElem(seq, idx); }

template<typename _Tp> inline SeqIterator<_Tp> Seq<_Tp>::begin() const
{ return SeqIterator<_Tp>(*this); }

template<typename _Tp> inline SeqIterator<_Tp> Seq<_Tp>::end() const
{ return SeqIterator<_Tp>(*this, true); }

template<typename _Tp> inline size_t Seq<_Tp>::size() const
{ return seq ? seq->total : 0; }

template<typename _Tp> inline int Seq<_Tp>::type() const
{ return seq ? CV_MAT_TYPE(seq->flags) : 0; }

template<typename _Tp> inline int Seq<_Tp>::depth() const
{ return seq ? CV_MAT_DEPTH(seq->flags) : 0; }

template<typename _Tp> inline int Seq<_Tp>::channels() const
{ return seq ? CV_MAT_CN(seq->flags) : 0; }

template<typename _Tp> inline size_t Seq<_Tp>::elemSize() const
{ return seq ? seq->elem_size : 0; }

template<typename _Tp> inline size_t Seq<_Tp>::index(const _Tp& elem) const
{ return cvSeqElemIdx(seq, &elem); }

template<typename _Tp> inline void Seq<_Tp>::push_back(const _Tp& elem)
{ cvSeqPush(seq, &elem); }

template<typename _Tp> inline void Seq<_Tp>::push_front(const _Tp& elem)
{ cvSeqPushFront(seq, &elem); }

template<typename _Tp> inline void Seq<_Tp>::push_back(const _Tp* elem, size_t count)
{ cvSeqPushMulti(seq, elem, (int)count, 0); }

template<typename _Tp> inline void Seq<_Tp>::push_front(const _Tp* elem, size_t count)
{ cvSeqPushMulti(seq, elem, (int)count, 1); }

template<typename _Tp> inline _Tp& Seq<_Tp>::back()
{ return *(_Tp*)getSeqElem(seq, -1); }

template<typename _Tp> inline const _Tp& Seq<_Tp>::back() const
{ return *(const _Tp*)getSeqElem(seq, -1); }

template<typename _Tp> inline _Tp& Seq<_Tp>::front()
{ return *(_Tp*)getSeqElem(seq, 0); }

template<typename _Tp> inline const _Tp& Seq<_Tp>::front() const
{ return *(const _Tp*)getSeqElem(seq, 0); }

template<typename _Tp> inline bool Seq<_Tp>::empty() const
{ return !seq || seq->total == 0; }

template<typename _Tp> inline void Seq<_Tp>::clear()
{ if(seq) clearSeq(seq); }

template<typename _Tp> inline void Seq<_Tp>::pop_back()
{ seqPop(seq); }

template<typename _Tp> inline void Seq<_Tp>::pop_front()
{ seqPopFront(seq); }

template<typename _Tp> inline void Seq<_Tp>::pop_back(_Tp* elem, size_t count)
{ seqPopMulti(seq, elem, (int)count, 0); }

template<typename _Tp> inline void Seq<_Tp>::pop_front(_Tp* elem, size_t count)
{ seqPopMulti(seq, elem, (int)count, 1); }

template<typename _Tp> inline void Seq<_Tp>::insert(int idx, const _Tp& elem)
{ seqInsert(seq, idx, &elem); }

template<typename _Tp> inline void Seq<_Tp>::insert(int idx, const _Tp* elems, size_t count)
{
    CvMat m = cvMat(1, count, DataType<_Tp>::type, elems);
    seqInsertSlice(seq, idx, &m);
}

template<typename _Tp> inline void Seq<_Tp>::remove(int idx)
{ seqRemove(seq, idx); }

template<typename _Tp> inline void Seq<_Tp>::remove(const Range& r)
{ seqRemoveSlice(seq, cvSlice(r.start, r.end)); }

template<typename _Tp> inline void Seq<_Tp>::copyTo(std::vector<_Tp>& vec, const Range& range) const
{
    size_t len = !seq ? 0 : range == Range::all() ? seq->total : range.end - range.start;
    vec.resize(len);
    if( seq && len )
        cvCvtSeqToArray(seq, &vec[0], cvSlice(range));
}

template<typename _Tp> inline Seq<_Tp>::operator std::vector<_Tp>() const
{
    std::vector<_Tp> vec;
    copyTo(vec);
    return vec;
}

template<typename _Tp> inline SeqIterator<_Tp>::SeqIterator()
{ memset(this, 0, sizeof(*this)); }

template<typename _Tp> inline SeqIterator<_Tp>::SeqIterator(const Seq<_Tp>& _seq, bool seekEnd)
{
    cvStartReadSeq(_seq.seq, this);
    index = seekEnd ? _seq.seq->total : 0;
}

template<typename _Tp> inline void SeqIterator<_Tp>::seek(size_t pos)
{
    cvSetSeqReaderPos(this, (int)pos, false);
    index = pos;
}

template<typename _Tp> inline size_t SeqIterator<_Tp>::tell() const
{ return index; }

template<typename _Tp> inline _Tp& SeqIterator<_Tp>::operator *()
{ return *(_Tp*)ptr; }

template<typename _Tp> inline const _Tp& SeqIterator<_Tp>::operator *() const
{ return *(const _Tp*)ptr; }

template<typename _Tp> inline SeqIterator<_Tp>& SeqIterator<_Tp>::operator ++()
{
    CV_NEXT_SEQ_ELEM(sizeof(_Tp), *this);
    if( ++index >= seq->total*2 )
        index = 0;
    return *this;
}

template<typename _Tp> inline SeqIterator<_Tp> SeqIterator<_Tp>::operator ++(int) const
{
    SeqIterator<_Tp> it = *this;
    ++*this;
    return it;
}

template<typename _Tp> inline SeqIterator<_Tp>& SeqIterator<_Tp>::operator --()
{
    CV_PREV_SEQ_ELEM(sizeof(_Tp), *this);
    if( --index < 0 )
        index = seq->total*2-1;
    return *this;
}

template<typename _Tp> inline SeqIterator<_Tp> SeqIterator<_Tp>::operator --(int) const
{
    SeqIterator<_Tp> it = *this;
    --*this;
    return it;
}

template<typename _Tp> inline SeqIterator<_Tp>& SeqIterator<_Tp>::operator +=(int delta)
{
    cvSetSeqReaderPos(this, delta, 1);
    index += delta;
    int n = seq->total*2;
    if( index < 0 )
        index += n;
    if( index >= n )
        index -= n;
    return *this;
}

template<typename _Tp> inline SeqIterator<_Tp>& SeqIterator<_Tp>::operator -=(int delta)
{
    return (*this += -delta);
}

template<typename _Tp> inline ptrdiff_t operator - (const SeqIterator<_Tp>& a,
                                                    const SeqIterator<_Tp>& b)
{
    ptrdiff_t delta = a.index - b.index, n = a.seq->total;
    if( delta > n || delta < -n )
        delta += delta < 0 ? n : -n;
    return delta;
}

template<typename _Tp> inline bool operator == (const SeqIterator<_Tp>& a,
                                                const SeqIterator<_Tp>& b)
{
    return a.seq == b.seq && a.index == b.index;
}

template<typename _Tp> inline bool operator != (const SeqIterator<_Tp>& a,
                                                const SeqIterator<_Tp>& b)
{
    return !(a == b);
}



}

#endif

#endif
