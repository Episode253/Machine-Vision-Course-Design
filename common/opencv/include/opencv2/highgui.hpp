

#ifndef OPENCV_HIGHGUI_HPP
#define OPENCV_HIGHGUI_HPP

#include "opencv2/core.hpp"
#ifdef HAVE_OPENCV_IMGCODECS
#include "opencv2/imgcodecs.hpp"
#endif
#ifdef HAVE_OPENCV_VIDEOIO
#include "opencv2/videoio.hpp"
#endif




namespace cv
{





enum WindowFlags {
       WINDOW_NORMAL     = 0x00000000,
       WINDOW_AUTOSIZE   = 0x00000001,
       WINDOW_OPENGL     = 0x00001000,

       WINDOW_FULLSCREEN = 1,
       WINDOW_FREERATIO  = 0x00000100,
       WINDOW_KEEPRATIO  = 0x00000000,
       WINDOW_GUI_EXPANDED=0x00000000,
       WINDOW_GUI_NORMAL = 0x00000010,
    };


enum WindowPropertyFlags {
       WND_PROP_FULLSCREEN   = 0,
       WND_PROP_AUTOSIZE     = 1,
       WND_PROP_ASPECT_RATIO = 2,
       WND_PROP_OPENGL       = 3,
       WND_PROP_VISIBLE      = 4
     };


enum MouseEventTypes {
       EVENT_MOUSEMOVE      = 0,
       EVENT_LBUTTONDOWN    = 1,
       EVENT_RBUTTONDOWN    = 2,
       EVENT_MBUTTONDOWN    = 3,
       EVENT_LBUTTONUP      = 4,
       EVENT_RBUTTONUP      = 5,
       EVENT_MBUTTONUP      = 6,
       EVENT_LBUTTONDBLCLK  = 7,
       EVENT_RBUTTONDBLCLK  = 8,
       EVENT_MBUTTONDBLCLK  = 9,
       EVENT_MOUSEWHEEL     = 10,
       EVENT_MOUSEHWHEEL    = 11
     };


enum MouseEventFlags {
       EVENT_FLAG_LBUTTON   = 1,
       EVENT_FLAG_RBUTTON   = 2,
       EVENT_FLAG_MBUTTON   = 4,
       EVENT_FLAG_CTRLKEY   = 8,
       EVENT_FLAG_SHIFTKEY  = 16,
       EVENT_FLAG_ALTKEY    = 32
     };


enum QtFontWeights {
        QT_FONT_LIGHT           = 25,
        QT_FONT_NORMAL          = 50,
        QT_FONT_DEMIBOLD        = 63,
        QT_FONT_BOLD            = 75,
        QT_FONT_BLACK           = 87
     };


enum QtFontStyles {
        QT_STYLE_NORMAL         = 0,
        QT_STYLE_ITALIC         = 1,
        QT_STYLE_OBLIQUE        = 2
     };


enum QtButtonTypes {
       QT_PUSH_BUTTON   = 0,
       QT_CHECKBOX      = 1,
       QT_RADIOBOX      = 2,
       QT_NEW_BUTTONBAR = 1024
     };


typedef void (*MouseCallback)(int event, int x, int y, int flags, void* userdata);


typedef void (*TrackbarCallback)(int pos, void* userdata);


typedef void (*OpenGlDrawCallback)(void* userdata);


typedef void (*ButtonCallback)(int state, void* userdata);


CV_EXPORTS_W void namedWindow(const String& winname, int flags = WINDOW_AUTOSIZE);


CV_EXPORTS_W void destroyWindow(const String& winname);


CV_EXPORTS_W void destroyAllWindows();

CV_EXPORTS_W int startWindowThread();


CV_EXPORTS_W int waitKeyEx(int delay = 0);


CV_EXPORTS_W int waitKey(int delay = 0);


CV_EXPORTS_W void imshow(const String& winname, InputArray mat);


CV_EXPORTS_W void resizeWindow(const String& winname, int width, int height);


CV_EXPORTS_W void resizeWindow(const String& winname, const cv::Size& size);


CV_EXPORTS_W void moveWindow(const String& winname, int x, int y);


CV_EXPORTS_W void setWindowProperty(const String& winname, int prop_id, double prop_value);


CV_EXPORTS_W void setWindowTitle(const String& winname, const String& title);


CV_EXPORTS_W double getWindowProperty(const String& winname, int prop_id);


CV_EXPORTS_W Rect getWindowImageRect(const String& winname);



CV_EXPORTS void setMouseCallback(const String& winname, MouseCallback onMouse, void* userdata = 0);


CV_EXPORTS int getMouseWheelDelta(int flags);


CV_EXPORTS_W Rect selectROI(const String& windowName, InputArray img, bool showCrosshair = true, bool fromCenter = false);


CV_EXPORTS_W Rect selectROI(InputArray img, bool showCrosshair = true, bool fromCenter = false);


CV_EXPORTS_W void selectROIs(const String& windowName, InputArray img,
                             CV_OUT std::vector<Rect>& boundingBoxes, bool showCrosshair = true, bool fromCenter = false);


CV_EXPORTS int createTrackbar(const String& trackbarname, const String& winname,
                              int* value, int count,
                              TrackbarCallback onChange = 0,
                              void* userdata = 0);


CV_EXPORTS_W int getTrackbarPos(const String& trackbarname, const String& winname);


CV_EXPORTS_W void setTrackbarPos(const String& trackbarname, const String& winname, int pos);


CV_EXPORTS_W void setTrackbarMax(const String& trackbarname, const String& winname, int maxval);


CV_EXPORTS_W void setTrackbarMin(const String& trackbarname, const String& winname, int minval);





CV_EXPORTS void imshow(const String& winname, const ogl::Texture2D& tex);


CV_EXPORTS void setOpenGlDrawCallback(const String& winname, OpenGlDrawCallback onOpenGlDraw, void* userdata = 0);


CV_EXPORTS void setOpenGlContext(const String& winname);


CV_EXPORTS void updateWindow(const String& winname);







struct QtFont
{
    const char* nameFont;
    Scalar      color;
    int         font_face;
    const int*  ascii;
    const int*  greek;
    const int*  cyrillic;
    float       hscale, vscale;
    float       shear;
    int         thickness;
    float       dx;
    int         line_type;
};


CV_EXPORTS QtFont fontQt(const String& nameFont, int pointSize = -1,
                         Scalar color = Scalar::all(0), int weight = QT_FONT_NORMAL,
                         int style = QT_STYLE_NORMAL, int spacing = 0);


CV_EXPORTS void addText( const Mat& img, const String& text, Point org, const QtFont& font);


CV_EXPORTS_W void addText(const Mat& img, const String& text, Point org, const String& nameFont, int pointSize = -1, Scalar color = Scalar::all(0),
        int weight = QT_FONT_NORMAL, int style = QT_STYLE_NORMAL, int spacing = 0);


CV_EXPORTS_W void displayOverlay(const String& winname, const String& text, int delayms = 0);


CV_EXPORTS_W void displayStatusBar(const String& winname, const String& text, int delayms = 0);


CV_EXPORTS void saveWindowParameters(const String& windowName);


CV_EXPORTS void loadWindowParameters(const String& windowName);

CV_EXPORTS  int startLoop(int (*pt2Func)(int argc, char *argv[]), int argc, char* argv[]);

CV_EXPORTS  void stopLoop();


CV_EXPORTS int createButton( const String& bar_name, ButtonCallback on_change,
                             void* userdata = 0, int type = QT_PUSH_BUTTON,
                             bool initial_button_state = false);





}

#endif
