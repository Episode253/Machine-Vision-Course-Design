

#ifndef OPENCV_IMGPROC_IMGPROC_C_H
#define OPENCV_IMGPROC_IMGPROC_C_H

#include "opencv2/imgproc/types_c.h"

#ifdef __cplusplus
extern "C" {
#endif






CVAPI(void)  cvAcc( const CvArr* image, CvArr* sum,
                   const CvArr* mask CV_DEFAULT(NULL) );


CVAPI(void)  cvSquareAcc( const CvArr* image, CvArr* sqsum,
                         const CvArr* mask CV_DEFAULT(NULL) );


CVAPI(void)  cvMultiplyAcc( const CvArr* image1, const CvArr* image2, CvArr* acc,
                           const CvArr* mask CV_DEFAULT(NULL) );


CVAPI(void)  cvRunningAvg( const CvArr* image, CvArr* acc, double alpha,
                          const CvArr* mask CV_DEFAULT(NULL) );




CVAPI(void) cvCopyMakeBorder( const CvArr* src, CvArr* dst, CvPoint offset,
                              int bordertype, CvScalar value CV_DEFAULT(cvScalarAll(0)));


CVAPI(void) cvSmooth( const CvArr* src, CvArr* dst,
                      int smoothtype CV_DEFAULT(CV_GAUSSIAN),
                      int size1 CV_DEFAULT(3),
                      int size2 CV_DEFAULT(0),
                      double sigma1 CV_DEFAULT(0),
                      double sigma2 CV_DEFAULT(0));


CVAPI(void) cvFilter2D( const CvArr* src, CvArr* dst, const CvMat* kernel,
                        CvPoint anchor CV_DEFAULT(cvPoint(-1,-1)));


CVAPI(void) cvIntegral( const CvArr* image, CvArr* sum,
                       CvArr* sqsum CV_DEFAULT(NULL),
                       CvArr* tilted_sum CV_DEFAULT(NULL));


CVAPI(void)  cvPyrDown( const CvArr* src, CvArr* dst,
                        int filter CV_DEFAULT(CV_GAUSSIAN_5x5) );


CVAPI(void)  cvPyrUp( const CvArr* src, CvArr* dst,
                      int filter CV_DEFAULT(CV_GAUSSIAN_5x5) );


CVAPI(CvMat**) cvCreatePyramid( const CvArr* img, int extra_layers, double rate,
                                const CvSize* layer_sizes CV_DEFAULT(0),
                                CvArr* bufarr CV_DEFAULT(0),
                                int calc CV_DEFAULT(1),
                                int filter CV_DEFAULT(CV_GAUSSIAN_5x5) );


CVAPI(void)  cvReleasePyramid( CvMat*** pyramid, int extra_layers );



CVAPI(void) cvPyrMeanShiftFiltering( const CvArr* src, CvArr* dst,
    double sp, double sr, int max_level CV_DEFAULT(1),
    CvTermCriteria termcrit CV_DEFAULT(cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,5,1)));


CVAPI(void) cvWatershed( const CvArr* image, CvArr* markers );


CVAPI(void) cvSobel( const CvArr* src, CvArr* dst,
                    int xorder, int yorder,
                    int aperture_size CV_DEFAULT(3));


CVAPI(void) cvLaplace( const CvArr* src, CvArr* dst,
                      int aperture_size CV_DEFAULT(3) );


CVAPI(void)  cvCvtColor( const CvArr* src, CvArr* dst, int code );



CVAPI(void)  cvResize( const CvArr* src, CvArr* dst,
                       int interpolation CV_DEFAULT( CV_INTER_LINEAR ));


CVAPI(void)  cvWarpAffine( const CvArr* src, CvArr* dst, const CvMat* map_matrix,
                           int flags CV_DEFAULT(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS),
                           CvScalar fillval CV_DEFAULT(cvScalarAll(0)) );


CVAPI(CvMat*) cvGetAffineTransform( const CvPoint2D32f * src,
                                    const CvPoint2D32f * dst,
                                    CvMat * map_matrix );


CVAPI(CvMat*)  cv2DRotationMatrix( CvPoint2D32f center, double angle,
                                   double scale, CvMat* map_matrix );


CVAPI(void)  cvWarpPerspective( const CvArr* src, CvArr* dst, const CvMat* map_matrix,
                                int flags CV_DEFAULT(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS),
                                CvScalar fillval CV_DEFAULT(cvScalarAll(0)) );


CVAPI(CvMat*) cvGetPerspectiveTransform( const CvPoint2D32f* src,
                                         const CvPoint2D32f* dst,
                                         CvMat* map_matrix );


CVAPI(void)  cvRemap( const CvArr* src, CvArr* dst,
                      const CvArr* mapx, const CvArr* mapy,
                      int flags CV_DEFAULT(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS),
                      CvScalar fillval CV_DEFAULT(cvScalarAll(0)) );


CVAPI(void)  cvConvertMaps( const CvArr* mapx, const CvArr* mapy,
                            CvArr* mapxy, CvArr* mapalpha );


CVAPI(void)  cvLogPolar( const CvArr* src, CvArr* dst,
                         CvPoint2D32f center, double M,
                         int flags CV_DEFAULT(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS));


CVAPI(void)  cvLinearPolar( const CvArr* src, CvArr* dst,
                         CvPoint2D32f center, double maxRadius,
                         int flags CV_DEFAULT(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS));


 CVAPI(IplConvKernel*)  cvCreateStructuringElementEx(
            int cols, int  rows, int  anchor_x, int  anchor_y,
            int shape, int* values CV_DEFAULT(NULL) );


CVAPI(void)  cvReleaseStructuringElement( IplConvKernel** element );


CVAPI(void)  cvErode( const CvArr* src, CvArr* dst,
                      IplConvKernel* element CV_DEFAULT(NULL),
                      int iterations CV_DEFAULT(1) );


CVAPI(void)  cvDilate( const CvArr* src, CvArr* dst,
                       IplConvKernel* element CV_DEFAULT(NULL),
                       int iterations CV_DEFAULT(1) );


CVAPI(void)  cvMorphologyEx( const CvArr* src, CvArr* dst,
                             CvArr* temp, IplConvKernel* element,
                             int operation, int iterations CV_DEFAULT(1) );


CVAPI(void) cvMoments( const CvArr* arr, CvMoments* moments, int binary CV_DEFAULT(0));


CVAPI(double)  cvGetSpatialMoment( CvMoments* moments, int x_order, int y_order );

CVAPI(double)  cvGetCentralMoment( CvMoments* moments, int x_order, int y_order );

CVAPI(double)  cvGetNormalizedCentralMoment( CvMoments* moments,
                                             int x_order, int y_order );


CVAPI(void) cvGetHuMoments( CvMoments*  moments, CvHuMoments*  hu_moments );




CVAPI(int)  cvSampleLine( const CvArr* image, CvPoint pt1, CvPoint pt2, void* buffer,
                          int connectivity CV_DEFAULT(8));


CVAPI(void)  cvGetRectSubPix( const CvArr* src, CvArr* dst, CvPoint2D32f center );



CVAPI(void)  cvGetQuadrangleSubPix( const CvArr* src, CvArr* dst,
                                    const CvMat* map_matrix );


CVAPI(void)  cvMatchTemplate( const CvArr* image, const CvArr* templ,
                              CvArr* result, int method );


CVAPI(float)  cvCalcEMD2( const CvArr* signature1,
                          const CvArr* signature2,
                          int distance_type,
                          CvDistanceFunction distance_func CV_DEFAULT(NULL),
                          const CvArr* cost_matrix CV_DEFAULT(NULL),
                          CvArr* flow CV_DEFAULT(NULL),
                          float* lower_bound CV_DEFAULT(NULL),
                          void* userdata CV_DEFAULT(NULL));




CVAPI(int)  cvFindContours( CvArr* image, CvMemStorage* storage, CvSeq** first_contour,
                            int header_size CV_DEFAULT(sizeof(CvContour)),
                            int mode CV_DEFAULT(CV_RETR_LIST),
                            int method CV_DEFAULT(CV_CHAIN_APPROX_SIMPLE),
                            CvPoint offset CV_DEFAULT(cvPoint(0,0)));


CVAPI(CvContourScanner)  cvStartFindContours( CvArr* image, CvMemStorage* storage,
                            int header_size CV_DEFAULT(sizeof(CvContour)),
                            int mode CV_DEFAULT(CV_RETR_LIST),
                            int method CV_DEFAULT(CV_CHAIN_APPROX_SIMPLE),
                            CvPoint offset CV_DEFAULT(cvPoint(0,0)));


CVAPI(CvSeq*)  cvFindNextContour( CvContourScanner scanner );



CVAPI(void)   cvSubstituteContour( CvContourScanner scanner, CvSeq* new_contour );



CVAPI(CvSeq*)  cvEndFindContours( CvContourScanner* scanner );


CVAPI(CvSeq*) cvApproxChains( CvSeq* src_seq, CvMemStorage* storage,
                            int method CV_DEFAULT(CV_CHAIN_APPROX_SIMPLE),
                            double parameter CV_DEFAULT(0),
                            int  minimal_perimeter CV_DEFAULT(0),
                            int  recursive CV_DEFAULT(0));


CVAPI(void) cvStartReadChainPoints( CvChain* chain, CvChainPtReader* reader );


CVAPI(CvPoint) cvReadChainPoint( CvChainPtReader* reader );





CVAPI(CvSeq*)  cvApproxPoly( const void* src_seq,
                             int header_size, CvMemStorage* storage,
                             int method, double eps,
                             int recursive CV_DEFAULT(0));


CVAPI(double)  cvArcLength( const void* curve,
                            CvSlice slice CV_DEFAULT(CV_WHOLE_SEQ),
                            int is_closed CV_DEFAULT(-1));


CV_INLINE double cvContourPerimeter( const void* contour )
{
    return cvArcLength( contour, CV_WHOLE_SEQ, 1 );
}



CVAPI(CvRect)  cvBoundingRect( CvArr* points, int update CV_DEFAULT(0) );


CVAPI(double)  cvContourArea( const CvArr* contour,
                              CvSlice slice CV_DEFAULT(CV_WHOLE_SEQ),
                              int oriented CV_DEFAULT(0));


CVAPI(CvBox2D)  cvMinAreaRect2( const CvArr* points,
                                CvMemStorage* storage CV_DEFAULT(NULL));


CVAPI(int)  cvMinEnclosingCircle( const CvArr* points,
                                  CvPoint2D32f* center, float* radius );


CVAPI(double)  cvMatchShapes( const void* object1, const void* object2,
                              int method, double parameter CV_DEFAULT(0));


CVAPI(CvSeq*) cvConvexHull2( const CvArr* input,
                             void* hull_storage CV_DEFAULT(NULL),
                             int orientation CV_DEFAULT(CV_CLOCKWISE),
                             int return_points CV_DEFAULT(0));


CVAPI(int)  cvCheckContourConvexity( const CvArr* contour );



CVAPI(CvSeq*)  cvConvexityDefects( const CvArr* contour, const CvArr* convexhull,
                                   CvMemStorage* storage CV_DEFAULT(NULL));


CVAPI(CvBox2D) cvFitEllipse2( const CvArr* points );


CVAPI(CvRect)  cvMaxRect( const CvRect* rect1, const CvRect* rect2 );


CVAPI(void) cvBoxPoints( CvBox2D box, CvPoint2D32f pt[4] );


CVAPI(CvSeq*) cvPointSeqFromMat( int seq_kind, const CvArr* mat,
                                 CvContour* contour_header,
                                 CvSeqBlock* block );


CVAPI(double) cvPointPolygonTest( const CvArr* contour,
                                  CvPoint2D32f pt, int measure_dist );




CVAPI(CvHistogram*)  cvCreateHist( int dims, int* sizes, int type,
                                   float** ranges CV_DEFAULT(NULL),
                                   int uniform CV_DEFAULT(1));


CVAPI(void)  cvSetHistBinRanges( CvHistogram* hist, float** ranges,
                                int uniform CV_DEFAULT(1));


CVAPI(CvHistogram*)  cvMakeHistHeaderForArray(
                            int  dims, int* sizes, CvHistogram* hist,
                            float* data, float** ranges CV_DEFAULT(NULL),
                            int uniform CV_DEFAULT(1));


CVAPI(void)  cvReleaseHist( CvHistogram** hist );


CVAPI(void)  cvClearHist( CvHistogram* hist );


CVAPI(void)  cvGetMinMaxHistValue( const CvHistogram* hist,
                                   float* min_value, float* max_value,
                                   int* min_idx CV_DEFAULT(NULL),
                                   int* max_idx CV_DEFAULT(NULL));



CVAPI(void)  cvNormalizeHist( CvHistogram* hist, double factor );



CVAPI(void)  cvThreshHist( CvHistogram* hist, double threshold );



CVAPI(double)  cvCompareHist( const CvHistogram* hist1,
                              const CvHistogram* hist2,
                              int method);


CVAPI(void)  cvCopyHist( const CvHistogram* src, CvHistogram** dst );



CVAPI(void)  cvCalcBayesianProb( CvHistogram** src, int number,
                                CvHistogram** dst);


CVAPI(void)  cvCalcArrHist( CvArr** arr, CvHistogram* hist,
                            int accumulate CV_DEFAULT(0),
                            const CvArr* mask CV_DEFAULT(NULL) );


CV_INLINE  void  cvCalcHist( IplImage** image, CvHistogram* hist,
                             int accumulate CV_DEFAULT(0),
                             const CvArr* mask CV_DEFAULT(NULL) )
{
    cvCalcArrHist( (CvArr**)image, hist, accumulate, mask );
}


CVAPI(void)  cvCalcArrBackProject( CvArr** image, CvArr* dst,
                                   const CvHistogram* hist );

#define  cvCalcBackProject(image, dst, hist) cvCalcArrBackProject((CvArr**)image, dst, hist)



CVAPI(void)  cvCalcArrBackProjectPatch( CvArr** image, CvArr* dst, CvSize range,
                                        CvHistogram* hist, int method,
                                        double factor );

#define  cvCalcBackProjectPatch( image, dst, range, hist, method, factor ) \
     cvCalcArrBackProjectPatch( (CvArr**)image, dst, range, hist, method, factor )



CVAPI(void)  cvCalcProbDensity( const CvHistogram* hist1, const CvHistogram* hist2,
                                CvHistogram* dst_hist, double scale CV_DEFAULT(255) );


CVAPI(void)  cvEqualizeHist( const CvArr* src, CvArr* dst );



CVAPI(void)  cvDistTransform( const CvArr* src, CvArr* dst,
                              int distance_type CV_DEFAULT(CV_DIST_L2),
                              int mask_size CV_DEFAULT(3),
                              const float* mask CV_DEFAULT(NULL),
                              CvArr* labels CV_DEFAULT(NULL),
                              int labelType CV_DEFAULT(CV_DIST_LABEL_CCOMP));



CVAPI(double)  cvThreshold( const CvArr*  src, CvArr*  dst,
                            double  threshold, double  max_value,
                            int threshold_type );


CVAPI(void)  cvAdaptiveThreshold( const CvArr* src, CvArr* dst, double max_value,
                                  int adaptive_method CV_DEFAULT(CV_ADAPTIVE_THRESH_MEAN_C),
                                  int threshold_type CV_DEFAULT(CV_THRESH_BINARY),
                                  int block_size CV_DEFAULT(3),
                                  double param1 CV_DEFAULT(5));


CVAPI(void)  cvFloodFill( CvArr* image, CvPoint seed_point,
                          CvScalar new_val, CvScalar lo_diff CV_DEFAULT(cvScalarAll(0)),
                          CvScalar up_diff CV_DEFAULT(cvScalarAll(0)),
                          CvConnectedComp* comp CV_DEFAULT(NULL),
                          int flags CV_DEFAULT(4),
                          CvArr* mask CV_DEFAULT(NULL));




CVAPI(void)  cvCanny( const CvArr* image, CvArr* edges, double threshold1,
                      double threshold2, int  aperture_size CV_DEFAULT(3) );


CVAPI(void) cvPreCornerDetect( const CvArr* image, CvArr* corners,
                               int aperture_size CV_DEFAULT(3) );


CVAPI(void)  cvCornerEigenValsAndVecs( const CvArr* image, CvArr* eigenvv,
                                       int block_size, int aperture_size CV_DEFAULT(3) );


CVAPI(void)  cvCornerMinEigenVal( const CvArr* image, CvArr* eigenval,
                                  int block_size, int aperture_size CV_DEFAULT(3) );


CVAPI(void)  cvCornerHarris( const CvArr* image, CvArr* harris_response,
                             int block_size, int aperture_size CV_DEFAULT(3),
                             double k CV_DEFAULT(0.04) );


CVAPI(void)  cvFindCornerSubPix( const CvArr* image, CvPoint2D32f* corners,
                                 int count, CvSize win, CvSize zero_zone,
                                 CvTermCriteria  criteria );


CVAPI(void)  cvGoodFeaturesToTrack( const CvArr* image, CvArr* eig_image,
                                    CvArr* temp_image, CvPoint2D32f* corners,
                                    int* corner_count, double  quality_level,
                                    double  min_distance,
                                    const CvArr* mask CV_DEFAULT(NULL),
                                    int block_size CV_DEFAULT(3),
                                    int use_harris CV_DEFAULT(0),
                                    double k CV_DEFAULT(0.04) );


CVAPI(CvSeq*)  cvHoughLines2( CvArr* image, void* line_storage, int method,
                              double rho, double theta, int threshold,
                              double param1 CV_DEFAULT(0), double param2 CV_DEFAULT(0),
                              double min_theta CV_DEFAULT(0), double max_theta CV_DEFAULT(CV_PI));


CVAPI(CvSeq*) cvHoughCircles( CvArr* image, void* circle_storage,
                              int method, double dp, double min_dist,
                              double param1 CV_DEFAULT(100),
                              double param2 CV_DEFAULT(100),
                              int min_radius CV_DEFAULT(0),
                              int max_radius CV_DEFAULT(0));


CVAPI(void)  cvFitLine( const CvArr* points, int dist_type, double param,
                        double reps, double aeps, float* line );





#define CV_FILLED -1

#define CV_AA 16


CVAPI(void)  cvLine( CvArr* img, CvPoint pt1, CvPoint pt2,
                     CvScalar color, int thickness CV_DEFAULT(1),
                     int line_type CV_DEFAULT(8), int shift CV_DEFAULT(0) );


CVAPI(void)  cvRectangle( CvArr* img, CvPoint pt1, CvPoint pt2,
                          CvScalar color, int thickness CV_DEFAULT(1),
                          int line_type CV_DEFAULT(8),
                          int shift CV_DEFAULT(0));


CVAPI(void)  cvRectangleR( CvArr* img, CvRect r,
                           CvScalar color, int thickness CV_DEFAULT(1),
                           int line_type CV_DEFAULT(8),
                           int shift CV_DEFAULT(0));



CVAPI(void)  cvCircle( CvArr* img, CvPoint center, int radius,
                       CvScalar color, int thickness CV_DEFAULT(1),
                       int line_type CV_DEFAULT(8), int shift CV_DEFAULT(0));


CVAPI(void)  cvEllipse( CvArr* img, CvPoint center, CvSize axes,
                        double angle, double start_angle, double end_angle,
                        CvScalar color, int thickness CV_DEFAULT(1),
                        int line_type CV_DEFAULT(8), int shift CV_DEFAULT(0));

CV_INLINE  void  cvEllipseBox( CvArr* img, CvBox2D box, CvScalar color,
                               int thickness CV_DEFAULT(1),
                               int line_type CV_DEFAULT(8), int shift CV_DEFAULT(0) )
{
    CvSize axes = cvSize(
        cvRound(box.size.width*0.5),
        cvRound(box.size.height*0.5)
    );

    cvEllipse( img, cvPointFrom32f( box.center ), axes, box.angle,
               0, 360, color, thickness, line_type, shift );
}


CVAPI(void)  cvFillConvexPoly( CvArr* img, const CvPoint* pts, int npts, CvScalar color,
                               int line_type CV_DEFAULT(8), int shift CV_DEFAULT(0));


CVAPI(void)  cvFillPoly( CvArr* img, CvPoint** pts, const int* npts,
                         int contours, CvScalar color,
                         int line_type CV_DEFAULT(8), int shift CV_DEFAULT(0) );


CVAPI(void)  cvPolyLine( CvArr* img, CvPoint** pts, const int* npts, int contours,
                         int is_closed, CvScalar color, int thickness CV_DEFAULT(1),
                         int line_type CV_DEFAULT(8), int shift CV_DEFAULT(0) );

#define cvDrawRect cvRectangle
#define cvDrawLine cvLine
#define cvDrawCircle cvCircle
#define cvDrawEllipse cvEllipse
#define cvDrawPolyLine cvPolyLine


CVAPI(int) cvClipLine( CvSize img_size, CvPoint* pt1, CvPoint* pt2 );


CVAPI(int)  cvInitLineIterator( const CvArr* image, CvPoint pt1, CvPoint pt2,
                                CvLineIterator* line_iterator,
                                int connectivity CV_DEFAULT(8),
                                int left_to_right CV_DEFAULT(0));

#define CV_NEXT_LINE_POINT( line_iterator )                     \
{                                                               \
    int _line_iterator_mask = (line_iterator).err < 0 ? -1 : 0; \
    (line_iterator).err += (line_iterator).minus_delta +        \
        ((line_iterator).plus_delta & _line_iterator_mask);     \
    (line_iterator).ptr += (line_iterator).minus_step +         \
        ((line_iterator).plus_step & _line_iterator_mask);      \
}


#define CV_FONT_HERSHEY_SIMPLEX         0
#define CV_FONT_HERSHEY_PLAIN           1
#define CV_FONT_HERSHEY_DUPLEX          2
#define CV_FONT_HERSHEY_COMPLEX         3
#define CV_FONT_HERSHEY_TRIPLEX         4
#define CV_FONT_HERSHEY_COMPLEX_SMALL   5
#define CV_FONT_HERSHEY_SCRIPT_SIMPLEX  6
#define CV_FONT_HERSHEY_SCRIPT_COMPLEX  7

#define CV_FONT_ITALIC                 16

#define CV_FONT_VECTOR0    CV_FONT_HERSHEY_SIMPLEX



typedef struct CvFont
{
  const char* nameFont;
  CvScalar color;
    int         font_face;
    const int*  ascii;
    const int*  greek;
    const int*  cyrillic;
    float       hscale, vscale;
    float       shear;
    int         thickness;
    float       dx;
    int         line_type;
}
CvFont;


CVAPI(void)  cvInitFont( CvFont* font, int font_face,
                         double hscale, double vscale,
                         double shear CV_DEFAULT(0),
                         int thickness CV_DEFAULT(1),
                         int line_type CV_DEFAULT(8));

CV_INLINE CvFont cvFont( double scale, int thickness CV_DEFAULT(1) )
{
    CvFont font;
    cvInitFont( &font, CV_FONT_HERSHEY_PLAIN, scale, scale, 0, thickness, CV_AA );
    return font;
}


CVAPI(void)  cvPutText( CvArr* img, const char* text, CvPoint org,
                        const CvFont* font, CvScalar color );


CVAPI(void)  cvGetTextSize( const char* text_string, const CvFont* font,
                            CvSize* text_size, int* baseline );


CVAPI(CvScalar)  cvColorToScalar( double packed_color, int arrtype );


CVAPI(int) cvEllipse2Poly( CvPoint center, CvSize axes,
                 int angle, int arc_start, int arc_end, CvPoint * pts, int delta );


CVAPI(void)  cvDrawContours( CvArr *img, CvSeq* contour,
                             CvScalar external_color, CvScalar hole_color,
                             int max_level, int thickness CV_DEFAULT(1),
                             int line_type CV_DEFAULT(8),
                             CvPoint offset CV_DEFAULT(cvPoint(0,0)));



#ifdef __cplusplus
}
#endif

#endif
