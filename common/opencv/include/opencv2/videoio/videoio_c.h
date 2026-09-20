

#ifndef OPENCV_VIDEOIO_H
#define OPENCV_VIDEOIO_H

#include "opencv2/core/core_c.h"

#include "opencv2/videoio/legacy/constants_c.h"

#ifdef __cplusplus
extern "C" {
#endif






typedef struct CvCapture CvCapture;


CVAPI(CvCapture*) cvCreateFileCapture( const char* filename );


CVAPI(CvCapture*) cvCreateFileCaptureWithPreference( const char* filename , int apiPreference);


CVAPI(CvCapture*) cvCreateCameraCapture( int index );


CVAPI(int) cvGrabFrame( CvCapture* capture );


CVAPI(IplImage*) cvRetrieveFrame( CvCapture* capture, int streamIdx CV_DEFAULT(0) );


CVAPI(IplImage*) cvQueryFrame( CvCapture* capture );


CVAPI(void) cvReleaseCapture( CvCapture** capture );


CVAPI(double) cvGetCaptureProperty( CvCapture* capture, int property_id );

CVAPI(int)    cvSetCaptureProperty( CvCapture* capture, int property_id, double value );


CVAPI(int)    cvGetCaptureDomain( CvCapture* capture);


typedef struct CvVideoWriter CvVideoWriter;


CVAPI(CvVideoWriter*) cvCreateVideoWriter( const char* filename, int fourcc,
                                           double fps, CvSize frame_size,
                                           int is_color CV_DEFAULT(1));


CVAPI(int) cvWriteFrame( CvVideoWriter* writer, const IplImage* image );


CVAPI(void) cvReleaseVideoWriter( CvVideoWriter** writer );




#define cvCaptureFromCAM cvCreateCameraCapture
#define cvCaptureFromFile cvCreateFileCapture
#define cvCaptureFromAVI cvCaptureFromFile
#define cvCreateAVIWriter cvCreateVideoWriter
#define cvWriteToAVI cvWriteFrame




#ifdef __cplusplus
}
#endif

#endif
