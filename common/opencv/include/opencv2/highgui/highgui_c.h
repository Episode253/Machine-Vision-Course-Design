

#ifndef OPENCV_HIGHGUI_H
#define OPENCV_HIGHGUI_H

#include "opencv2/core/core_c.h"
#include "opencv2/imgproc/imgproc_c.h"

#ifdef __cplusplus
extern "C" {
#endif







enum {  CV_FONT_LIGHT           = 25,
        CV_FONT_NORMAL          = 50,
        CV_FONT_DEMIBOLD        = 63,
        CV_FONT_BOLD            = 75,
        CV_FONT_BLACK           = 87
};

enum {  CV_STYLE_NORMAL         = 0,
        CV_STYLE_ITALIC         = 1,
        CV_STYLE_OBLIQUE        = 2
};




CVAPI(CvFont) cvFontQt(const char* nameFont, int pointSize CV_DEFAULT(-1), CvScalar color CV_DEFAULT(cvScalarAll(0)), int weight CV_DEFAULT(CV_FONT_NORMAL),  int style CV_DEFAULT(CV_STYLE_NORMAL), int spacing CV_DEFAULT(0));

CVAPI(void) cvAddText(const CvArr* img, const char* text, CvPoint org, CvFont *arg2);

CVAPI(void) cvDisplayOverlay(const char* name, const char* text, int delayms CV_DEFAULT(0));
CVAPI(void) cvDisplayStatusBar(const char* name, const char* text, int delayms CV_DEFAULT(0));

CVAPI(void) cvSaveWindowParameters(const char* name);
CVAPI(void) cvLoadWindowParameters(const char* name);
CVAPI(int) cvStartLoop(int (*pt2Func)(int argc, char *argv[]), int argc, char* argv[]);
CVAPI(void) cvStopLoop( void );

typedef void (CV_CDECL *CvButtonCallback)(int state, void* userdata);
enum {CV_PUSH_BUTTON = 0, CV_CHECKBOX = 1, CV_RADIOBOX = 2};
CVAPI(int) cvCreateButton( const char* button_name CV_DEFAULT(NULL),CvButtonCallback on_change CV_DEFAULT(NULL), void* userdata CV_DEFAULT(NULL) , int button_type CV_DEFAULT(CV_PUSH_BUTTON), int initial_button_state CV_DEFAULT(0));




CVAPI(int) cvInitSystem( int argc, char** argv );

CVAPI(int) cvStartWindowThread( void );


enum
{

    CV_WND_PROP_FULLSCREEN = 0,
    CV_WND_PROP_AUTOSIZE   = 1,
    CV_WND_PROP_ASPECTRATIO= 2,
    CV_WND_PROP_OPENGL     = 3,
    CV_WND_PROP_VISIBLE    = 4,


    CV_WINDOW_NORMAL       = 0x00000000,
    CV_WINDOW_AUTOSIZE     = 0x00000001,
    CV_WINDOW_OPENGL       = 0x00001000,


    CV_GUI_EXPANDED         = 0x00000000,
    CV_GUI_NORMAL           = 0x00000010,


    CV_WINDOW_FULLSCREEN   = 1,
    CV_WINDOW_FREERATIO    = 0x00000100,
    CV_WINDOW_KEEPRATIO    = 0x00000000
};


CVAPI(int) cvNamedWindow( const char* name, int flags CV_DEFAULT(CV_WINDOW_AUTOSIZE) );


CVAPI(void) cvSetWindowProperty(const char* name, int prop_id, double prop_value);
CVAPI(double) cvGetWindowProperty(const char* name, int prop_id);

#ifdef __cplusplus

CVAPI(cv::Rect)cvGetWindowImageRect(const char* name);
#endif


CVAPI(void) cvShowImage( const char* name, const CvArr* image );


CVAPI(void) cvResizeWindow( const char* name, int width, int height );
CVAPI(void) cvMoveWindow( const char* name, int x, int y );



CVAPI(void) cvDestroyWindow( const char* name );

CVAPI(void) cvDestroyAllWindows(void);


CVAPI(void*) cvGetWindowHandle( const char* name );


CVAPI(const char*) cvGetWindowName( void* window_handle );


typedef void (CV_CDECL *CvTrackbarCallback)(int pos);


CVAPI(int) cvCreateTrackbar( const char* trackbar_name, const char* window_name,
                             int* value, int count, CvTrackbarCallback on_change CV_DEFAULT(NULL));

typedef void (CV_CDECL *CvTrackbarCallback2)(int pos, void* userdata);

CVAPI(int) cvCreateTrackbar2( const char* trackbar_name, const char* window_name,
                              int* value, int count, CvTrackbarCallback2 on_change,
                              void* userdata CV_DEFAULT(0));


CVAPI(int) cvGetTrackbarPos( const char* trackbar_name, const char* window_name );
CVAPI(void) cvSetTrackbarPos( const char* trackbar_name, const char* window_name, int pos );
CVAPI(void) cvSetTrackbarMax(const char* trackbar_name, const char* window_name, int maxval);
CVAPI(void) cvSetTrackbarMin(const char* trackbar_name, const char* window_name, int minval);

enum
{
    CV_EVENT_MOUSEMOVE      =0,
    CV_EVENT_LBUTTONDOWN    =1,
    CV_EVENT_RBUTTONDOWN    =2,
    CV_EVENT_MBUTTONDOWN    =3,
    CV_EVENT_LBUTTONUP      =4,
    CV_EVENT_RBUTTONUP      =5,
    CV_EVENT_MBUTTONUP      =6,
    CV_EVENT_LBUTTONDBLCLK  =7,
    CV_EVENT_RBUTTONDBLCLK  =8,
    CV_EVENT_MBUTTONDBLCLK  =9,
    CV_EVENT_MOUSEWHEEL     =10,
    CV_EVENT_MOUSEHWHEEL    =11
};

enum
{
    CV_EVENT_FLAG_LBUTTON   =1,
    CV_EVENT_FLAG_RBUTTON   =2,
    CV_EVENT_FLAG_MBUTTON   =4,
    CV_EVENT_FLAG_CTRLKEY   =8,
    CV_EVENT_FLAG_SHIFTKEY  =16,
    CV_EVENT_FLAG_ALTKEY    =32
};


#define CV_GET_WHEEL_DELTA(flags) ((short)((flags >> 16) & 0xffff))

typedef void (CV_CDECL *CvMouseCallback )(int event, int x, int y, int flags, void* param);


CVAPI(void) cvSetMouseCallback( const char* window_name, CvMouseCallback on_mouse,
                                void* param CV_DEFAULT(NULL));


CVAPI(int) cvWaitKey(int delay CV_DEFAULT(0));



typedef void (CV_CDECL *CvOpenGlDrawCallback)(void* userdata);
CVAPI(void) cvSetOpenGlDrawCallback(const char* window_name, CvOpenGlDrawCallback callback, void* userdata CV_DEFAULT(NULL));

CVAPI(void) cvSetOpenGlContext(const char* window_name);
CVAPI(void) cvUpdateWindow(const char* window_name);




#define cvAddSearchPath(path)
#define cvvInitSystem cvInitSystem
#define cvvNamedWindow cvNamedWindow
#define cvvShowImage cvShowImage
#define cvvResizeWindow cvResizeWindow
#define cvvDestroyWindow cvDestroyWindow
#define cvvCreateTrackbar cvCreateTrackbar
#define cvvAddSearchPath cvAddSearchPath
#define cvvWaitKey(name) cvWaitKey(0)
#define cvvWaitKeyEx(name,delay) cvWaitKey(delay)
#define HG_AUTOSIZE CV_WINDOW_AUTOSIZE
#define set_preprocess_func cvSetPreprocessFuncWin32
#define set_postprocess_func cvSetPostprocessFuncWin32

#if defined _WIN32

CVAPI(void) cvSetPreprocessFuncWin32_(const void* callback);
CVAPI(void) cvSetPostprocessFuncWin32_(const void* callback);
#define cvSetPreprocessFuncWin32(callback) cvSetPreprocessFuncWin32_((const void*)(callback))
#define cvSetPostprocessFuncWin32(callback) cvSetPostprocessFuncWin32_((const void*)(callback))

#endif



#ifdef __cplusplus
}
#endif

#endif
