

#ifndef OPENCV_IMGPROC_HPP
#define OPENCV_IMGPROC_HPP

#include "opencv2/core.hpp"



namespace cv
{






enum SpecialFilter {
    FILTER_SCHARR = -1
};


enum MorphTypes{
    MORPH_ERODE    = 0,
    MORPH_DILATE   = 1,
    MORPH_OPEN     = 2,

    MORPH_CLOSE    = 3,

    MORPH_GRADIENT = 4,

    MORPH_TOPHAT   = 5,

    MORPH_BLACKHAT = 6,

    MORPH_HITMISS  = 7

};


enum MorphShapes {
    MORPH_RECT    = 0,
    MORPH_CROSS   = 1,

    MORPH_ELLIPSE = 2

};







enum InterpolationFlags{

    INTER_NEAREST        = 0,

    INTER_LINEAR         = 1,

    INTER_CUBIC          = 2,

    INTER_AREA           = 3,

    INTER_LANCZOS4       = 4,

    INTER_LINEAR_EXACT = 5,

    INTER_MAX            = 7,

    WARP_FILL_OUTLIERS   = 8,

    WARP_INVERSE_MAP     = 16
};


enum WarpPolarMode
{
    WARP_POLAR_LINEAR = 0,
    WARP_POLAR_LOG = 256
};

enum InterpolationMasks {
       INTER_BITS      = 5,
       INTER_BITS2     = INTER_BITS * 2,
       INTER_TAB_SIZE  = 1 << INTER_BITS,
       INTER_TAB_SIZE2 = INTER_TAB_SIZE * INTER_TAB_SIZE
     };








enum DistanceTypes {
    DIST_USER    = -1,
    DIST_L1      = 1,
    DIST_L2      = 2,
    DIST_C       = 3,
    DIST_L12     = 4,
    DIST_FAIR    = 5,
    DIST_WELSCH  = 6,
    DIST_HUBER   = 7
};


enum DistanceTransformMasks {
    DIST_MASK_3       = 3,
    DIST_MASK_5       = 5,
    DIST_MASK_PRECISE = 0
};



enum ThresholdTypes {
    THRESH_BINARY     = 0,
    THRESH_BINARY_INV = 1,
    THRESH_TRUNC      = 2,
    THRESH_TOZERO     = 3,
    THRESH_TOZERO_INV = 4,
    THRESH_MASK       = 7,
    THRESH_OTSU       = 8,
    THRESH_TRIANGLE   = 16
};



enum AdaptiveThresholdTypes {

    ADAPTIVE_THRESH_MEAN_C     = 0,

    ADAPTIVE_THRESH_GAUSSIAN_C = 1
};


enum GrabCutClasses {
    GC_BGD    = 0,
    GC_FGD    = 1,
    GC_PR_BGD = 2,
    GC_PR_FGD = 3
};


enum GrabCutModes {

    GC_INIT_WITH_RECT  = 0,

    GC_INIT_WITH_MASK  = 1,

    GC_EVAL            = 2,

    GC_EVAL_FREEZE_MODEL = 3
};


enum DistanceTransformLabelTypes {

    DIST_LABEL_CCOMP = 0,

    DIST_LABEL_PIXEL = 1
};


enum FloodFillFlags {

    FLOODFILL_FIXED_RANGE = 1 << 16,

    FLOODFILL_MASK_ONLY   = 1 << 17
};







enum ConnectedComponentsTypes {
    CC_STAT_LEFT   = 0,

    CC_STAT_TOP    = 1,

    CC_STAT_WIDTH  = 2,
    CC_STAT_HEIGHT = 3,
    CC_STAT_AREA   = 4,
    CC_STAT_MAX    = 5
};


enum ConnectedComponentsAlgorithmsTypes {
    CCL_WU      = 0,
    CCL_DEFAULT = -1,
    CCL_GRANA   = 1
};


enum RetrievalModes {

    RETR_EXTERNAL  = 0,

    RETR_LIST      = 1,

    RETR_CCOMP     = 2,

    RETR_TREE      = 3,
    RETR_FLOODFILL = 4
};


enum ContourApproximationModes {

    CHAIN_APPROX_NONE      = 1,

    CHAIN_APPROX_SIMPLE    = 2,

    CHAIN_APPROX_TC89_L1   = 3,

    CHAIN_APPROX_TC89_KCOS = 4
};


enum ShapeMatchModes {
    CONTOURS_MATCH_I1  =1,
    CONTOURS_MATCH_I2  =2,
    CONTOURS_MATCH_I3  =3
};







enum HoughModes {


    HOUGH_STANDARD      = 0,

    HOUGH_PROBABILISTIC = 1,

    HOUGH_MULTI_SCALE   = 2,
    HOUGH_GRADIENT      = 3
};


enum LineSegmentDetectorModes {
    LSD_REFINE_NONE = 0,
    LSD_REFINE_STD  = 1,
    LSD_REFINE_ADV  = 2

};




enum HistCompMethods {

    HISTCMP_CORREL        = 0,

    HISTCMP_CHISQR        = 1,

    HISTCMP_INTERSECT     = 2,

    HISTCMP_BHATTACHARYYA = 3,
    HISTCMP_HELLINGER     = HISTCMP_BHATTACHARYYA,

    HISTCMP_CHISQR_ALT    = 4,

    HISTCMP_KL_DIV        = 5
};


enum ColorConversionCodes {
    COLOR_BGR2BGRA     = 0,
    COLOR_RGB2RGBA     = COLOR_BGR2BGRA,

    COLOR_BGRA2BGR     = 1,
    COLOR_RGBA2RGB     = COLOR_BGRA2BGR,

    COLOR_BGR2RGBA     = 2,
    COLOR_RGB2BGRA     = COLOR_BGR2RGBA,

    COLOR_RGBA2BGR     = 3,
    COLOR_BGRA2RGB     = COLOR_RGBA2BGR,

    COLOR_BGR2RGB      = 4,
    COLOR_RGB2BGR      = COLOR_BGR2RGB,

    COLOR_BGRA2RGBA    = 5,
    COLOR_RGBA2BGRA    = COLOR_BGRA2RGBA,

    COLOR_BGR2GRAY     = 6,
    COLOR_RGB2GRAY     = 7,
    COLOR_GRAY2BGR     = 8,
    COLOR_GRAY2RGB     = COLOR_GRAY2BGR,
    COLOR_GRAY2BGRA    = 9,
    COLOR_GRAY2RGBA    = COLOR_GRAY2BGRA,
    COLOR_BGRA2GRAY    = 10,
    COLOR_RGBA2GRAY    = 11,

    COLOR_BGR2BGR565   = 12,
    COLOR_RGB2BGR565   = 13,
    COLOR_BGR5652BGR   = 14,
    COLOR_BGR5652RGB   = 15,
    COLOR_BGRA2BGR565  = 16,
    COLOR_RGBA2BGR565  = 17,
    COLOR_BGR5652BGRA  = 18,
    COLOR_BGR5652RGBA  = 19,

    COLOR_GRAY2BGR565  = 20,
    COLOR_BGR5652GRAY  = 21,

    COLOR_BGR2BGR555   = 22,
    COLOR_RGB2BGR555   = 23,
    COLOR_BGR5552BGR   = 24,
    COLOR_BGR5552RGB   = 25,
    COLOR_BGRA2BGR555  = 26,
    COLOR_RGBA2BGR555  = 27,
    COLOR_BGR5552BGRA  = 28,
    COLOR_BGR5552RGBA  = 29,

    COLOR_GRAY2BGR555  = 30,
    COLOR_BGR5552GRAY  = 31,

    COLOR_BGR2XYZ      = 32,
    COLOR_RGB2XYZ      = 33,
    COLOR_XYZ2BGR      = 34,
    COLOR_XYZ2RGB      = 35,

    COLOR_BGR2YCrCb    = 36,
    COLOR_RGB2YCrCb    = 37,
    COLOR_YCrCb2BGR    = 38,
    COLOR_YCrCb2RGB    = 39,

    COLOR_BGR2HSV      = 40,
    COLOR_RGB2HSV      = 41,

    COLOR_BGR2Lab      = 44,
    COLOR_RGB2Lab      = 45,

    COLOR_BGR2Luv      = 50,
    COLOR_RGB2Luv      = 51,
    COLOR_BGR2HLS      = 52,
    COLOR_RGB2HLS      = 53,

    COLOR_HSV2BGR      = 54,
    COLOR_HSV2RGB      = 55,

    COLOR_Lab2BGR      = 56,
    COLOR_Lab2RGB      = 57,
    COLOR_Luv2BGR      = 58,
    COLOR_Luv2RGB      = 59,
    COLOR_HLS2BGR      = 60,
    COLOR_HLS2RGB      = 61,

    COLOR_BGR2HSV_FULL = 66,
    COLOR_RGB2HSV_FULL = 67,
    COLOR_BGR2HLS_FULL = 68,
    COLOR_RGB2HLS_FULL = 69,

    COLOR_HSV2BGR_FULL = 70,
    COLOR_HSV2RGB_FULL = 71,
    COLOR_HLS2BGR_FULL = 72,
    COLOR_HLS2RGB_FULL = 73,

    COLOR_LBGR2Lab     = 74,
    COLOR_LRGB2Lab     = 75,
    COLOR_LBGR2Luv     = 76,
    COLOR_LRGB2Luv     = 77,

    COLOR_Lab2LBGR     = 78,
    COLOR_Lab2LRGB     = 79,
    COLOR_Luv2LBGR     = 80,
    COLOR_Luv2LRGB     = 81,

    COLOR_BGR2YUV      = 82,
    COLOR_RGB2YUV      = 83,
    COLOR_YUV2BGR      = 84,
    COLOR_YUV2RGB      = 85,


    COLOR_YUV2RGB_NV12  = 90,
    COLOR_YUV2BGR_NV12  = 91,
    COLOR_YUV2RGB_NV21  = 92,
    COLOR_YUV2BGR_NV21  = 93,
    COLOR_YUV420sp2RGB  = COLOR_YUV2RGB_NV21,
    COLOR_YUV420sp2BGR  = COLOR_YUV2BGR_NV21,

    COLOR_YUV2RGBA_NV12 = 94,
    COLOR_YUV2BGRA_NV12 = 95,
    COLOR_YUV2RGBA_NV21 = 96,
    COLOR_YUV2BGRA_NV21 = 97,
    COLOR_YUV420sp2RGBA = COLOR_YUV2RGBA_NV21,
    COLOR_YUV420sp2BGRA = COLOR_YUV2BGRA_NV21,

    COLOR_YUV2RGB_YV12  = 98,
    COLOR_YUV2BGR_YV12  = 99,
    COLOR_YUV2RGB_IYUV  = 100,
    COLOR_YUV2BGR_IYUV  = 101,
    COLOR_YUV2RGB_I420  = COLOR_YUV2RGB_IYUV,
    COLOR_YUV2BGR_I420  = COLOR_YUV2BGR_IYUV,
    COLOR_YUV420p2RGB   = COLOR_YUV2RGB_YV12,
    COLOR_YUV420p2BGR   = COLOR_YUV2BGR_YV12,

    COLOR_YUV2RGBA_YV12 = 102,
    COLOR_YUV2BGRA_YV12 = 103,
    COLOR_YUV2RGBA_IYUV = 104,
    COLOR_YUV2BGRA_IYUV = 105,
    COLOR_YUV2RGBA_I420 = COLOR_YUV2RGBA_IYUV,
    COLOR_YUV2BGRA_I420 = COLOR_YUV2BGRA_IYUV,
    COLOR_YUV420p2RGBA  = COLOR_YUV2RGBA_YV12,
    COLOR_YUV420p2BGRA  = COLOR_YUV2BGRA_YV12,

    COLOR_YUV2GRAY_420  = 106,
    COLOR_YUV2GRAY_NV21 = COLOR_YUV2GRAY_420,
    COLOR_YUV2GRAY_NV12 = COLOR_YUV2GRAY_420,
    COLOR_YUV2GRAY_YV12 = COLOR_YUV2GRAY_420,
    COLOR_YUV2GRAY_IYUV = COLOR_YUV2GRAY_420,
    COLOR_YUV2GRAY_I420 = COLOR_YUV2GRAY_420,
    COLOR_YUV420sp2GRAY = COLOR_YUV2GRAY_420,
    COLOR_YUV420p2GRAY  = COLOR_YUV2GRAY_420,


    COLOR_YUV2RGB_UYVY = 107,
    COLOR_YUV2BGR_UYVY = 108,


    COLOR_YUV2RGB_Y422 = COLOR_YUV2RGB_UYVY,
    COLOR_YUV2BGR_Y422 = COLOR_YUV2BGR_UYVY,
    COLOR_YUV2RGB_UYNV = COLOR_YUV2RGB_UYVY,
    COLOR_YUV2BGR_UYNV = COLOR_YUV2BGR_UYVY,

    COLOR_YUV2RGBA_UYVY = 111,
    COLOR_YUV2BGRA_UYVY = 112,


    COLOR_YUV2RGBA_Y422 = COLOR_YUV2RGBA_UYVY,
    COLOR_YUV2BGRA_Y422 = COLOR_YUV2BGRA_UYVY,
    COLOR_YUV2RGBA_UYNV = COLOR_YUV2RGBA_UYVY,
    COLOR_YUV2BGRA_UYNV = COLOR_YUV2BGRA_UYVY,

    COLOR_YUV2RGB_YUY2 = 115,
    COLOR_YUV2BGR_YUY2 = 116,
    COLOR_YUV2RGB_YVYU = 117,
    COLOR_YUV2BGR_YVYU = 118,
    COLOR_YUV2RGB_YUYV = COLOR_YUV2RGB_YUY2,
    COLOR_YUV2BGR_YUYV = COLOR_YUV2BGR_YUY2,
    COLOR_YUV2RGB_YUNV = COLOR_YUV2RGB_YUY2,
    COLOR_YUV2BGR_YUNV = COLOR_YUV2BGR_YUY2,

    COLOR_YUV2RGBA_YUY2 = 119,
    COLOR_YUV2BGRA_YUY2 = 120,
    COLOR_YUV2RGBA_YVYU = 121,
    COLOR_YUV2BGRA_YVYU = 122,
    COLOR_YUV2RGBA_YUYV = COLOR_YUV2RGBA_YUY2,
    COLOR_YUV2BGRA_YUYV = COLOR_YUV2BGRA_YUY2,
    COLOR_YUV2RGBA_YUNV = COLOR_YUV2RGBA_YUY2,
    COLOR_YUV2BGRA_YUNV = COLOR_YUV2BGRA_YUY2,

    COLOR_YUV2GRAY_UYVY = 123,
    COLOR_YUV2GRAY_YUY2 = 124,

    COLOR_YUV2GRAY_Y422 = COLOR_YUV2GRAY_UYVY,
    COLOR_YUV2GRAY_UYNV = COLOR_YUV2GRAY_UYVY,
    COLOR_YUV2GRAY_YVYU = COLOR_YUV2GRAY_YUY2,
    COLOR_YUV2GRAY_YUYV = COLOR_YUV2GRAY_YUY2,
    COLOR_YUV2GRAY_YUNV = COLOR_YUV2GRAY_YUY2,


    COLOR_RGBA2mRGBA    = 125,
    COLOR_mRGBA2RGBA    = 126,


    COLOR_RGB2YUV_I420  = 127,
    COLOR_BGR2YUV_I420  = 128,
    COLOR_RGB2YUV_IYUV  = COLOR_RGB2YUV_I420,
    COLOR_BGR2YUV_IYUV  = COLOR_BGR2YUV_I420,

    COLOR_RGBA2YUV_I420 = 129,
    COLOR_BGRA2YUV_I420 = 130,
    COLOR_RGBA2YUV_IYUV = COLOR_RGBA2YUV_I420,
    COLOR_BGRA2YUV_IYUV = COLOR_BGRA2YUV_I420,
    COLOR_RGB2YUV_YV12  = 131,
    COLOR_BGR2YUV_YV12  = 132,
    COLOR_RGBA2YUV_YV12 = 133,
    COLOR_BGRA2YUV_YV12 = 134,


    COLOR_BayerBG2BGR = 46,
    COLOR_BayerGB2BGR = 47,
    COLOR_BayerRG2BGR = 48,
    COLOR_BayerGR2BGR = 49,

    COLOR_BayerBG2RGB = COLOR_BayerRG2BGR,
    COLOR_BayerGB2RGB = COLOR_BayerGR2BGR,
    COLOR_BayerRG2RGB = COLOR_BayerBG2BGR,
    COLOR_BayerGR2RGB = COLOR_BayerGB2BGR,

    COLOR_BayerBG2GRAY = 86,
    COLOR_BayerGB2GRAY = 87,
    COLOR_BayerRG2GRAY = 88,
    COLOR_BayerGR2GRAY = 89,


    COLOR_BayerBG2BGR_VNG = 62,
    COLOR_BayerGB2BGR_VNG = 63,
    COLOR_BayerRG2BGR_VNG = 64,
    COLOR_BayerGR2BGR_VNG = 65,

    COLOR_BayerBG2RGB_VNG = COLOR_BayerRG2BGR_VNG,
    COLOR_BayerGB2RGB_VNG = COLOR_BayerGR2BGR_VNG,
    COLOR_BayerRG2RGB_VNG = COLOR_BayerBG2BGR_VNG,
    COLOR_BayerGR2RGB_VNG = COLOR_BayerGB2BGR_VNG,


    COLOR_BayerBG2BGR_EA  = 135,
    COLOR_BayerGB2BGR_EA  = 136,
    COLOR_BayerRG2BGR_EA  = 137,
    COLOR_BayerGR2BGR_EA  = 138,

    COLOR_BayerBG2RGB_EA  = COLOR_BayerRG2BGR_EA,
    COLOR_BayerGB2RGB_EA  = COLOR_BayerGR2BGR_EA,
    COLOR_BayerRG2RGB_EA  = COLOR_BayerBG2BGR_EA,
    COLOR_BayerGR2RGB_EA  = COLOR_BayerGB2BGR_EA,


    COLOR_BayerBG2BGRA = 139,
    COLOR_BayerGB2BGRA = 140,
    COLOR_BayerRG2BGRA = 141,
    COLOR_BayerGR2BGRA = 142,

    COLOR_BayerBG2RGBA = COLOR_BayerRG2BGRA,
    COLOR_BayerGB2RGBA = COLOR_BayerGR2BGRA,
    COLOR_BayerRG2RGBA = COLOR_BayerBG2BGRA,
    COLOR_BayerGR2RGBA = COLOR_BayerGB2BGRA,

    COLOR_COLORCVT_MAX  = 143
};





enum RectanglesIntersectTypes {
    INTERSECT_NONE = 0,
    INTERSECT_PARTIAL  = 1,
    INTERSECT_FULL  = 2
};


enum LineTypes {
    FILLED  = -1,
    LINE_4  = 4,
    LINE_8  = 8,
    LINE_AA = 16
};


enum HersheyFonts {
    FONT_HERSHEY_SIMPLEX        = 0,
    FONT_HERSHEY_PLAIN          = 1,
    FONT_HERSHEY_DUPLEX         = 2,
    FONT_HERSHEY_COMPLEX        = 3,
    FONT_HERSHEY_TRIPLEX        = 4,
    FONT_HERSHEY_COMPLEX_SMALL  = 5,
    FONT_HERSHEY_SCRIPT_SIMPLEX = 6,
    FONT_HERSHEY_SCRIPT_COMPLEX = 7,
    FONT_ITALIC                 = 16
};


enum MarkerTypes
{
    MARKER_CROSS = 0,
    MARKER_TILTED_CROSS = 1,
    MARKER_STAR = 2,
    MARKER_DIAMOND = 3,
    MARKER_SQUARE = 4,
    MARKER_TRIANGLE_UP = 5,
    MARKER_TRIANGLE_DOWN = 6
};


class CV_EXPORTS_W GeneralizedHough : public Algorithm
{
public:

    CV_WRAP virtual void setTemplate(InputArray templ, Point templCenter = Point(-1, -1)) = 0;
    CV_WRAP virtual void setTemplate(InputArray edges, InputArray dx, InputArray dy, Point templCenter = Point(-1, -1)) = 0;


    CV_WRAP virtual void detect(InputArray image, OutputArray positions, OutputArray votes = noArray()) = 0;
    CV_WRAP virtual void detect(InputArray edges, InputArray dx, InputArray dy, OutputArray positions, OutputArray votes = noArray()) = 0;


    CV_WRAP virtual void setCannyLowThresh(int cannyLowThresh) = 0;
    CV_WRAP virtual int getCannyLowThresh() const = 0;


    CV_WRAP virtual void setCannyHighThresh(int cannyHighThresh) = 0;
    CV_WRAP virtual int getCannyHighThresh() const = 0;


    CV_WRAP virtual void setMinDist(double minDist) = 0;
    CV_WRAP virtual double getMinDist() const = 0;


    CV_WRAP virtual void setDp(double dp) = 0;
    CV_WRAP virtual double getDp() const = 0;


    CV_WRAP virtual void setMaxBufferSize(int maxBufferSize) = 0;
    CV_WRAP virtual int getMaxBufferSize() const = 0;
};


class CV_EXPORTS_W GeneralizedHoughBallard : public GeneralizedHough
{
public:

    CV_WRAP virtual void setLevels(int levels) = 0;
    CV_WRAP virtual int getLevels() const = 0;


    CV_WRAP virtual void setVotesThreshold(int votesThreshold) = 0;
    CV_WRAP virtual int getVotesThreshold() const = 0;
};


class CV_EXPORTS_W GeneralizedHoughGuil : public GeneralizedHough
{
public:

    CV_WRAP virtual void setXi(double xi) = 0;
    CV_WRAP virtual double getXi() const = 0;


    CV_WRAP virtual void setLevels(int levels) = 0;
    CV_WRAP virtual int getLevels() const = 0;


    CV_WRAP virtual void setAngleEpsilon(double angleEpsilon) = 0;
    CV_WRAP virtual double getAngleEpsilon() const = 0;


    CV_WRAP virtual void setMinAngle(double minAngle) = 0;
    CV_WRAP virtual double getMinAngle() const = 0;


    CV_WRAP virtual void setMaxAngle(double maxAngle) = 0;
    CV_WRAP virtual double getMaxAngle() const = 0;


    CV_WRAP virtual void setAngleStep(double angleStep) = 0;
    CV_WRAP virtual double getAngleStep() const = 0;


    CV_WRAP virtual void setAngleThresh(int angleThresh) = 0;
    CV_WRAP virtual int getAngleThresh() const = 0;


    CV_WRAP virtual void setMinScale(double minScale) = 0;
    CV_WRAP virtual double getMinScale() const = 0;


    CV_WRAP virtual void setMaxScale(double maxScale) = 0;
    CV_WRAP virtual double getMaxScale() const = 0;


    CV_WRAP virtual void setScaleStep(double scaleStep) = 0;
    CV_WRAP virtual double getScaleStep() const = 0;


    CV_WRAP virtual void setScaleThresh(int scaleThresh) = 0;
    CV_WRAP virtual int getScaleThresh() const = 0;


    CV_WRAP virtual void setPosThresh(int posThresh) = 0;
    CV_WRAP virtual int getPosThresh() const = 0;
};







class CV_EXPORTS_W CLAHE : public Algorithm
{
public:

    CV_WRAP virtual void apply(InputArray src, OutputArray dst) = 0;


    CV_WRAP virtual void setClipLimit(double clipLimit) = 0;


    CV_WRAP virtual double getClipLimit() const = 0;


    CV_WRAP virtual void setTilesGridSize(Size tileGridSize) = 0;


    CV_WRAP virtual Size getTilesGridSize() const = 0;

    CV_WRAP virtual void collectGarbage() = 0;
};






class CV_EXPORTS_W Subdiv2D
{
public:

    enum { PTLOC_ERROR        = -2,
           PTLOC_OUTSIDE_RECT = -1,
           PTLOC_INSIDE       = 0,
           PTLOC_VERTEX       = 1,
           PTLOC_ON_EDGE      = 2
         };


    enum { NEXT_AROUND_ORG   = 0x00,
           NEXT_AROUND_DST   = 0x22,
           PREV_AROUND_ORG   = 0x11,
           PREV_AROUND_DST   = 0x33,
           NEXT_AROUND_LEFT  = 0x13,
           NEXT_AROUND_RIGHT = 0x31,
           PREV_AROUND_LEFT  = 0x20,
           PREV_AROUND_RIGHT = 0x02
         };


    CV_WRAP Subdiv2D();


    CV_WRAP Subdiv2D(Rect rect);


    CV_WRAP void initDelaunay(Rect rect);


    CV_WRAP int insert(Point2f pt);


    CV_WRAP void insert(const std::vector<Point2f>& ptvec);


    CV_WRAP int locate(Point2f pt, CV_OUT int& edge, CV_OUT int& vertex);


    CV_WRAP int findNearest(Point2f pt, CV_OUT Point2f* nearestPt = 0);


    CV_WRAP void getEdgeList(CV_OUT std::vector<Vec4f>& edgeList) const;


    CV_WRAP void getLeadingEdgeList(CV_OUT std::vector<int>& leadingEdgeList) const;


    CV_WRAP void getTriangleList(CV_OUT std::vector<Vec6f>& triangleList) const;


    CV_WRAP void getVoronoiFacetList(const std::vector<int>& idx, CV_OUT std::vector<std::vector<Point2f> >& facetList,
                                     CV_OUT std::vector<Point2f>& facetCenters);


    CV_WRAP Point2f getVertex(int vertex, CV_OUT int* firstEdge = 0) const;


    CV_WRAP int getEdge( int edge, int nextEdgeType ) const;


    CV_WRAP int nextEdge(int edge) const;


    CV_WRAP int rotateEdge(int edge, int rotate) const;
    CV_WRAP int symEdge(int edge) const;


    CV_WRAP int edgeOrg(int edge, CV_OUT Point2f* orgpt = 0) const;


    CV_WRAP int edgeDst(int edge, CV_OUT Point2f* dstpt = 0) const;

protected:
    int newEdge();
    void deleteEdge(int edge);
    int newPoint(Point2f pt, bool isvirtual, int firstEdge = 0);
    void deletePoint(int vtx);
    void setEdgePoints( int edge, int orgPt, int dstPt );
    void splice( int edgeA, int edgeB );
    int connectEdges( int edgeA, int edgeB );
    void swapEdges( int edge );
    int isRightOf(Point2f pt, int edge) const;
    void calcVoronoi();
    void clearVoronoi();
    void checkSubdiv() const;

    struct CV_EXPORTS Vertex
    {
        Vertex();
        Vertex(Point2f pt, bool _isvirtual, int _firstEdge=0);
        bool isvirtual() const;
        bool isfree() const;

        int firstEdge;
        int type;
        Point2f pt;
    };

    struct CV_EXPORTS QuadEdge
    {
        QuadEdge();
        QuadEdge(int edgeidx);
        bool isfree() const;

        int next[4];
        int pt[4];
    };


    std::vector<Vertex> vtx;

    std::vector<QuadEdge> qedges;
    int freeQEdge;
    int freePoint;
    bool validGeometry;

    int recentEdge;

    Point2f topLeft;

    Point2f bottomRight;
};







class CV_EXPORTS_W LineSegmentDetector : public Algorithm
{
public:


    CV_WRAP virtual void detect(InputArray _image, OutputArray _lines,
                        OutputArray width = noArray(), OutputArray prec = noArray(),
                        OutputArray nfa = noArray()) = 0;


    CV_WRAP virtual void drawSegments(InputOutputArray _image, InputArray lines) = 0;


    CV_WRAP virtual int compareSegments(const Size& size, InputArray lines1, InputArray lines2, InputOutputArray _image = noArray()) = 0;

    virtual ~LineSegmentDetector() { }
};


CV_EXPORTS_W Ptr<LineSegmentDetector> createLineSegmentDetector(
    int _refine = LSD_REFINE_STD, double _scale = 0.8,
    double _sigma_scale = 0.6, double _quant = 2.0, double _ang_th = 22.5,
    double _log_eps = 0, double _density_th = 0.7, int _n_bins = 1024);







CV_EXPORTS_W Mat getGaussianKernel( int ksize, double sigma, int ktype = CV_64F );


CV_EXPORTS_W void getDerivKernels( OutputArray kx, OutputArray ky,
                                   int dx, int dy, int ksize,
                                   bool normalize = false, int ktype = CV_32F );


CV_EXPORTS_W Mat getGaborKernel( Size ksize, double sigma, double theta, double lambd,
                                 double gamma, double psi = CV_PI*0.5, int ktype = CV_64F );


static inline Scalar morphologyDefaultBorderValue() { return Scalar::all(DBL_MAX); }


CV_EXPORTS_W Mat getStructuringElement(int shape, Size ksize, Point anchor = Point(-1,-1));




CV_EXPORTS_W void medianBlur( InputArray src, OutputArray dst, int ksize );


CV_EXPORTS_W void GaussianBlur( InputArray src, OutputArray dst, Size ksize,
                                double sigmaX, double sigmaY = 0,
                                int borderType = BORDER_DEFAULT );


CV_EXPORTS_W void bilateralFilter( InputArray src, OutputArray dst, int d,
                                   double sigmaColor, double sigmaSpace,
                                   int borderType = BORDER_DEFAULT );


CV_EXPORTS_W void boxFilter( InputArray src, OutputArray dst, int ddepth,
                             Size ksize, Point anchor = Point(-1,-1),
                             bool normalize = true,
                             int borderType = BORDER_DEFAULT );


CV_EXPORTS_W void sqrBoxFilter( InputArray src, OutputArray dst, int ddepth,
                                Size ksize, Point anchor = Point(-1, -1),
                                bool normalize = true,
                                int borderType = BORDER_DEFAULT );


CV_EXPORTS_W void blur( InputArray src, OutputArray dst,
                        Size ksize, Point anchor = Point(-1,-1),
                        int borderType = BORDER_DEFAULT );


CV_EXPORTS_W void filter2D( InputArray src, OutputArray dst, int ddepth,
                            InputArray kernel, Point anchor = Point(-1,-1),
                            double delta = 0, int borderType = BORDER_DEFAULT );


CV_EXPORTS_W void sepFilter2D( InputArray src, OutputArray dst, int ddepth,
                               InputArray kernelX, InputArray kernelY,
                               Point anchor = Point(-1,-1),
                               double delta = 0, int borderType = BORDER_DEFAULT );




CV_EXPORTS_W void Sobel( InputArray src, OutputArray dst, int ddepth,
                         int dx, int dy, int ksize = 3,
                         double scale = 1, double delta = 0,
                         int borderType = BORDER_DEFAULT );



CV_EXPORTS_W void spatialGradient( InputArray src, OutputArray dx,
                                   OutputArray dy, int ksize = 3,
                                   int borderType = BORDER_DEFAULT );


CV_EXPORTS_W void Scharr( InputArray src, OutputArray dst, int ddepth,
                          int dx, int dy, double scale = 1, double delta = 0,
                          int borderType = BORDER_DEFAULT );




CV_EXPORTS_W void Laplacian( InputArray src, OutputArray dst, int ddepth,
                             int ksize = 1, double scale = 1, double delta = 0,
                             int borderType = BORDER_DEFAULT );









CV_EXPORTS_W void Canny( InputArray image, OutputArray edges,
                         double threshold1, double threshold2,
                         int apertureSize = 3, bool L2gradient = false );


CV_EXPORTS_W void Canny( InputArray dx, InputArray dy,
                         OutputArray edges,
                         double threshold1, double threshold2,
                         bool L2gradient = false );


CV_EXPORTS_W void cornerMinEigenVal( InputArray src, OutputArray dst,
                                     int blockSize, int ksize = 3,
                                     int borderType = BORDER_DEFAULT );


CV_EXPORTS_W void cornerHarris( InputArray src, OutputArray dst, int blockSize,
                                int ksize, double k,
                                int borderType = BORDER_DEFAULT );


CV_EXPORTS_W void cornerEigenValsAndVecs( InputArray src, OutputArray dst,
                                          int blockSize, int ksize,
                                          int borderType = BORDER_DEFAULT );


CV_EXPORTS_W void preCornerDetect( InputArray src, OutputArray dst, int ksize,
                                   int borderType = BORDER_DEFAULT );


CV_EXPORTS_W void cornerSubPix( InputArray image, InputOutputArray corners,
                                Size winSize, Size zeroZone,
                                TermCriteria criteria );



CV_EXPORTS_W void goodFeaturesToTrack( InputArray image, OutputArray corners,
                                     int maxCorners, double qualityLevel, double minDistance,
                                     InputArray mask = noArray(), int blockSize = 3,
                                     bool useHarrisDetector = false, double k = 0.04 );

CV_EXPORTS_W void goodFeaturesToTrack( InputArray image, OutputArray corners,
                                     int maxCorners, double qualityLevel, double minDistance,
                                     InputArray mask, int blockSize,
                                     int gradientSize, bool useHarrisDetector = false,
                                     double k = 0.04 );



CV_EXPORTS_W void HoughLines( InputArray image, OutputArray lines,
                              double rho, double theta, int threshold,
                              double srn = 0, double stn = 0,
                              double min_theta = 0, double max_theta = CV_PI );


CV_EXPORTS_W void HoughLinesP( InputArray image, OutputArray lines,
                               double rho, double theta, int threshold,
                               double minLineLength = 0, double maxLineGap = 0 );


CV_EXPORTS_W void HoughLinesPointSet( InputArray _point, OutputArray _lines, int lines_max, int threshold,
                                      double min_rho, double max_rho, double rho_step,
                                      double min_theta, double max_theta, double theta_step );




CV_EXPORTS_W void HoughCircles( InputArray image, OutputArray circles,
                               int method, double dp, double minDist,
                               double param1 = 100, double param2 = 100,
                               int minRadius = 0, int maxRadius = 0 );









CV_EXPORTS_W void erode( InputArray src, OutputArray dst, InputArray kernel,
                         Point anchor = Point(-1,-1), int iterations = 1,
                         int borderType = BORDER_CONSTANT,
                         const Scalar& borderValue = morphologyDefaultBorderValue() );




CV_EXPORTS_W void dilate( InputArray src, OutputArray dst, InputArray kernel,
                          Point anchor = Point(-1,-1), int iterations = 1,
                          int borderType = BORDER_CONSTANT,
                          const Scalar& borderValue = morphologyDefaultBorderValue() );


CV_EXPORTS_W void morphologyEx( InputArray src, OutputArray dst,
                                int op, InputArray kernel,
                                Point anchor = Point(-1,-1), int iterations = 1,
                                int borderType = BORDER_CONSTANT,
                                const Scalar& borderValue = morphologyDefaultBorderValue() );







CV_EXPORTS_W void resize( InputArray src, OutputArray dst,
                          Size dsize, double fx = 0, double fy = 0,
                          int interpolation = INTER_LINEAR );


CV_EXPORTS_W void warpAffine( InputArray src, OutputArray dst,
                              InputArray M, Size dsize,
                              int flags = INTER_LINEAR,
                              int borderMode = BORDER_CONSTANT,
                              const Scalar& borderValue = Scalar());




CV_EXPORTS_W void warpPerspective( InputArray src, OutputArray dst,
                                   InputArray M, Size dsize,
                                   int flags = INTER_LINEAR,
                                   int borderMode = BORDER_CONSTANT,
                                   const Scalar& borderValue = Scalar());


CV_EXPORTS_W void remap( InputArray src, OutputArray dst,
                         InputArray map1, InputArray map2,
                         int interpolation, int borderMode = BORDER_CONSTANT,
                         const Scalar& borderValue = Scalar());


CV_EXPORTS_W void convertMaps( InputArray map1, InputArray map2,
                               OutputArray dstmap1, OutputArray dstmap2,
                               int dstmap1type, bool nninterpolation = false );


CV_EXPORTS_W Mat getRotationMatrix2D( Point2f center, double angle, double scale );


CV_EXPORTS Mat getAffineTransform( const Point2f src[], const Point2f dst[] );


CV_EXPORTS_W void invertAffineTransform( InputArray M, OutputArray iM );


CV_EXPORTS_W Mat getPerspectiveTransform(InputArray src, InputArray dst, int solveMethod = DECOMP_LU);


CV_EXPORTS Mat getPerspectiveTransform(const Point2f src[], const Point2f dst[], int solveMethod = DECOMP_LU);


CV_EXPORTS_W Mat getAffineTransform( InputArray src, InputArray dst );


CV_EXPORTS_W void getRectSubPix( InputArray image, Size patchSize,
                                 Point2f center, OutputArray patch, int patchType = -1 );




CV_EXPORTS_W void logPolar( InputArray src, OutputArray dst,
                            Point2f center, double M, int flags );


CV_EXPORTS_W void linearPolar( InputArray src, OutputArray dst,
                               Point2f center, double maxRadius, int flags );



CV_EXPORTS_W void warpPolar(InputArray src, OutputArray dst, Size dsize,
                            Point2f center, double maxRadius, int flags);








CV_EXPORTS_W void integral( InputArray src, OutputArray sum, int sdepth = -1 );


CV_EXPORTS_AS(integral2) void integral( InputArray src, OutputArray sum,
                                        OutputArray sqsum, int sdepth = -1, int sqdepth = -1 );


CV_EXPORTS_AS(integral3) void integral( InputArray src, OutputArray sum,
                                        OutputArray sqsum, OutputArray tilted,
                                        int sdepth = -1, int sqdepth = -1 );







CV_EXPORTS_W void accumulate( InputArray src, InputOutputArray dst,
                              InputArray mask = noArray() );


CV_EXPORTS_W void accumulateSquare( InputArray src, InputOutputArray dst,
                                    InputArray mask = noArray() );


CV_EXPORTS_W void accumulateProduct( InputArray src1, InputArray src2,
                                     InputOutputArray dst, InputArray mask=noArray() );


CV_EXPORTS_W void accumulateWeighted( InputArray src, InputOutputArray dst,
                                      double alpha, InputArray mask = noArray() );


CV_EXPORTS_W Point2d phaseCorrelate(InputArray src1, InputArray src2,
                                    InputArray window = noArray(), CV_OUT double* response = 0);


CV_EXPORTS_W void createHanningWindow(OutputArray dst, Size winSize, int type);







CV_EXPORTS_W double threshold( InputArray src, OutputArray dst,
                               double thresh, double maxval, int type );



CV_EXPORTS_W void adaptiveThreshold( InputArray src, OutputArray dst,
                                     double maxValue, int adaptiveMethod,
                                     int thresholdType, int blockSize, double C );









CV_EXPORTS_W void pyrDown( InputArray src, OutputArray dst,
                           const Size& dstsize = Size(), int borderType = BORDER_DEFAULT );


CV_EXPORTS_W void pyrUp( InputArray src, OutputArray dst,
                         const Size& dstsize = Size(), int borderType = BORDER_DEFAULT );


CV_EXPORTS void buildPyramid( InputArray src, OutputArrayOfArrays dst,
                              int maxlevel, int borderType = BORDER_DEFAULT );









CV_EXPORTS void calcHist( const Mat* images, int nimages,
                          const int* channels, InputArray mask,
                          OutputArray hist, int dims, const int* histSize,
                          const float** ranges, bool uniform = true, bool accumulate = false );


CV_EXPORTS void calcHist( const Mat* images, int nimages,
                          const int* channels, InputArray mask,
                          SparseMat& hist, int dims,
                          const int* histSize, const float** ranges,
                          bool uniform = true, bool accumulate = false );


CV_EXPORTS_W void calcHist( InputArrayOfArrays images,
                            const std::vector<int>& channels,
                            InputArray mask, OutputArray hist,
                            const std::vector<int>& histSize,
                            const std::vector<float>& ranges,
                            bool accumulate = false );


CV_EXPORTS void calcBackProject( const Mat* images, int nimages,
                                 const int* channels, InputArray hist,
                                 OutputArray backProject, const float** ranges,
                                 double scale = 1, bool uniform = true );


CV_EXPORTS void calcBackProject( const Mat* images, int nimages,
                                 const int* channels, const SparseMat& hist,
                                 OutputArray backProject, const float** ranges,
                                 double scale = 1, bool uniform = true );


CV_EXPORTS_W void calcBackProject( InputArrayOfArrays images, const std::vector<int>& channels,
                                   InputArray hist, OutputArray dst,
                                   const std::vector<float>& ranges,
                                   double scale );


CV_EXPORTS_W double compareHist( InputArray H1, InputArray H2, int method );


CV_EXPORTS double compareHist( const SparseMat& H1, const SparseMat& H2, int method );


CV_EXPORTS_W void equalizeHist( InputArray src, OutputArray dst );


CV_EXPORTS_W Ptr<CLAHE> createCLAHE(double clipLimit = 40.0, Size tileGridSize = Size(8, 8));


CV_EXPORTS float EMD( InputArray signature1, InputArray signature2,
                      int distType, InputArray cost=noArray(),
                      float* lowerBound = 0, OutputArray flow = noArray() );

CV_EXPORTS_AS(EMD) float wrapperEMD( InputArray signature1, InputArray signature2,
                      int distType, InputArray cost=noArray(),
                      CV_IN_OUT Ptr<float> lowerBound = Ptr<float>(), OutputArray flow = noArray() );






CV_EXPORTS_W void watershed( InputArray image, InputOutputArray markers );





CV_EXPORTS_W void pyrMeanShiftFiltering( InputArray src, OutputArray dst,
                                         double sp, double sr, int maxLevel = 1,
                                         TermCriteria termcrit=TermCriteria(TermCriteria::MAX_ITER+TermCriteria::EPS,5,1) );









CV_EXPORTS_W void grabCut( InputArray img, InputOutputArray mask, Rect rect,
                           InputOutputArray bgdModel, InputOutputArray fgdModel,
                           int iterCount, int mode = GC_EVAL );




CV_EXPORTS_AS(distanceTransformWithLabels) void distanceTransform( InputArray src, OutputArray dst,
                                     OutputArray labels, int distanceType, int maskSize,
                                     int labelType = DIST_LABEL_CCOMP );


CV_EXPORTS_W void distanceTransform( InputArray src, OutputArray dst,
                                     int distanceType, int maskSize, int dstType=CV_32F);




CV_EXPORTS int floodFill( InputOutputArray image,
                          Point seedPoint, Scalar newVal, CV_OUT Rect* rect = 0,
                          Scalar loDiff = Scalar(), Scalar upDiff = Scalar(),
                          int flags = 4 );


CV_EXPORTS_W int floodFill( InputOutputArray image, InputOutputArray mask,
                            Point seedPoint, Scalar newVal, CV_OUT Rect* rect=0,
                            Scalar loDiff = Scalar(), Scalar upDiff = Scalar(),
                            int flags = 4 );








CV_EXPORTS void blendLinear(InputArray src1, InputArray src2, InputArray weights1, InputArray weights2, OutputArray dst);







CV_EXPORTS_W void cvtColor( InputArray src, OutputArray dst, int code, int dstCn = 0 );


CV_EXPORTS_W void cvtColorTwoPlane( InputArray src1, InputArray src2, OutputArray dst, int code );


CV_EXPORTS_W void demosaicing(InputArray src, OutputArray dst, int code, int dstCn = 0);







CV_EXPORTS_W Moments moments( InputArray array, bool binaryImage = false );


CV_EXPORTS void HuMoments( const Moments& moments, double hu[7] );


CV_EXPORTS_W void HuMoments( const Moments& m, OutputArray hu );







enum TemplateMatchModes {
    TM_SQDIFF        = 0,
    TM_SQDIFF_NORMED = 1,
    TM_CCORR         = 2,
    TM_CCORR_NORMED  = 3,
    TM_CCOEFF        = 4,


    TM_CCOEFF_NORMED = 5
};




CV_EXPORTS_W void matchTemplate( InputArray image, InputArray templ,
                                 OutputArray result, int method, InputArray mask = noArray() );









CV_EXPORTS_AS(connectedComponentsWithAlgorithm) int connectedComponents(InputArray image, OutputArray labels,
                                                                        int connectivity, int ltype, int ccltype);



CV_EXPORTS_W int connectedComponents(InputArray image, OutputArray labels,
                                     int connectivity = 8, int ltype = CV_32S);



CV_EXPORTS_AS(connectedComponentsWithStatsWithAlgorithm) int connectedComponentsWithStats(InputArray image, OutputArray labels,
                                                                                          OutputArray stats, OutputArray centroids,
                                                                                          int connectivity, int ltype, int ccltype);


CV_EXPORTS_W int connectedComponentsWithStats(InputArray image, OutputArray labels,
                                              OutputArray stats, OutputArray centroids,
                                              int connectivity = 8, int ltype = CV_32S);



CV_EXPORTS_W void findContours( InputArray image, OutputArrayOfArrays contours,
                              OutputArray hierarchy, int mode,
                              int method, Point offset = Point());


CV_EXPORTS void findContours( InputArray image, OutputArrayOfArrays contours,
                              int mode, int method, Point offset = Point());






CV_EXPORTS_W void approxPolyDP( InputArray curve,
                                OutputArray approxCurve,
                                double epsilon, bool closed );


CV_EXPORTS_W double arcLength( InputArray curve, bool closed );


CV_EXPORTS_W Rect boundingRect( InputArray array );


CV_EXPORTS_W double contourArea( InputArray contour, bool oriented = false );


CV_EXPORTS_W RotatedRect minAreaRect( InputArray points );


CV_EXPORTS_W void boxPoints(RotatedRect box, OutputArray points);


CV_EXPORTS_W void minEnclosingCircle( InputArray points,
                                      CV_OUT Point2f& center, CV_OUT float& radius );




CV_EXPORTS_W double minEnclosingTriangle( InputArray points, CV_OUT OutputArray triangle );


CV_EXPORTS_W double matchShapes( InputArray contour1, InputArray contour2,
                                 int method, double parameter );




CV_EXPORTS_W void convexHull( InputArray points, OutputArray hull,
                              bool clockwise = false, bool returnPoints = true );


CV_EXPORTS_W void convexityDefects( InputArray contour, InputArray convexhull, OutputArray convexityDefects );


CV_EXPORTS_W bool isContourConvex( InputArray contour );


CV_EXPORTS_W float intersectConvexConvex( InputArray _p1, InputArray _p2,
                                          OutputArray _p12, bool handleNested = true );




CV_EXPORTS_W RotatedRect fitEllipse( InputArray points );


CV_EXPORTS_W RotatedRect fitEllipseAMS( InputArray points );



CV_EXPORTS_W RotatedRect fitEllipseDirect( InputArray points );


CV_EXPORTS_W void fitLine( InputArray points, OutputArray line, int distType,
                           double param, double reps, double aeps );


CV_EXPORTS_W double pointPolygonTest( InputArray contour, Point2f pt, bool measureDist );


CV_EXPORTS_W int rotatedRectangleIntersection( const RotatedRect& rect1, const RotatedRect& rect2, OutputArray intersectingRegion  );


CV_EXPORTS_W Ptr<GeneralizedHoughBallard> createGeneralizedHoughBallard();


CV_EXPORTS_W Ptr<GeneralizedHoughGuil> createGeneralizedHoughGuil();







enum ColormapTypes
{
    COLORMAP_AUTUMN = 0,
    COLORMAP_BONE = 1,
    COLORMAP_JET = 2,
    COLORMAP_WINTER = 3,
    COLORMAP_RAINBOW = 4,
    COLORMAP_OCEAN = 5,
    COLORMAP_SUMMER = 6,
    COLORMAP_SPRING = 7,
    COLORMAP_COOL = 8,
    COLORMAP_HSV = 9,
    COLORMAP_PINK = 10,
    COLORMAP_HOT = 11,
    COLORMAP_PARULA = 12,
    COLORMAP_MAGMA = 13,
    COLORMAP_INFERNO = 14,
    COLORMAP_PLASMA = 15,
    COLORMAP_VIRIDIS = 16,
    COLORMAP_CIVIDIS = 17,
    COLORMAP_TWILIGHT = 18,
    COLORMAP_TWILIGHT_SHIFTED = 19
};




CV_EXPORTS_W void applyColorMap(InputArray src, OutputArray dst, int colormap);


CV_EXPORTS_W void applyColorMap(InputArray src, OutputArray dst, InputArray userColor);








#define CV_RGB(r, g, b)  cv::Scalar((b), (g), (r), 0)


CV_EXPORTS_W void line(InputOutputArray img, Point pt1, Point pt2, const Scalar& color,
                     int thickness = 1, int lineType = LINE_8, int shift = 0);


CV_EXPORTS_W void arrowedLine(InputOutputArray img, Point pt1, Point pt2, const Scalar& color,
                     int thickness=1, int line_type=8, int shift=0, double tipLength=0.1);


CV_EXPORTS_W void rectangle(InputOutputArray img, Point pt1, Point pt2,
                          const Scalar& color, int thickness = 1,
                          int lineType = LINE_8, int shift = 0);


CV_EXPORTS_W void rectangle(InputOutputArray img, Rect rec,
                          const Scalar& color, int thickness = 1,
                          int lineType = LINE_8, int shift = 0);




CV_EXPORTS_W void circle(InputOutputArray img, Point center, int radius,
                       const Scalar& color, int thickness = 1,
                       int lineType = LINE_8, int shift = 0);


CV_EXPORTS_W void ellipse(InputOutputArray img, Point center, Size axes,
                        double angle, double startAngle, double endAngle,
                        const Scalar& color, int thickness = 1,
                        int lineType = LINE_8, int shift = 0);


CV_EXPORTS_W void ellipse(InputOutputArray img, const RotatedRect& box, const Scalar& color,
                        int thickness = 1, int lineType = LINE_8);






CV_EXPORTS_W void drawMarker(InputOutputArray img, Point position, const Scalar& color,
                             int markerType = MARKER_CROSS, int markerSize=20, int thickness=1,
                             int line_type=8);






CV_EXPORTS void fillConvexPoly(InputOutputArray img, const Point* pts, int npts,
                               const Scalar& color, int lineType = LINE_8,
                               int shift = 0);


CV_EXPORTS_W void fillConvexPoly(InputOutputArray img, InputArray points,
                                 const Scalar& color, int lineType = LINE_8,
                                 int shift = 0);


CV_EXPORTS void fillPoly(InputOutputArray img, const Point** pts,
                         const int* npts, int ncontours,
                         const Scalar& color, int lineType = LINE_8, int shift = 0,
                         Point offset = Point() );




CV_EXPORTS_W void fillPoly(InputOutputArray img, InputArrayOfArrays pts,
                           const Scalar& color, int lineType = LINE_8, int shift = 0,
                           Point offset = Point() );


CV_EXPORTS void polylines(InputOutputArray img, const Point* const* pts, const int* npts,
                          int ncontours, bool isClosed, const Scalar& color,
                          int thickness = 1, int lineType = LINE_8, int shift = 0 );


CV_EXPORTS_W void polylines(InputOutputArray img, InputArrayOfArrays pts,
                            bool isClosed, const Scalar& color,
                            int thickness = 1, int lineType = LINE_8, int shift = 0 );






CV_EXPORTS_W void drawContours( InputOutputArray image, InputArrayOfArrays contours,
                              int contourIdx, const Scalar& color,
                              int thickness = 1, int lineType = LINE_8,
                              InputArray hierarchy = noArray(),
                              int maxLevel = INT_MAX, Point offset = Point() );


CV_EXPORTS bool clipLine(Size imgSize, CV_IN_OUT Point& pt1, CV_IN_OUT Point& pt2);


CV_EXPORTS bool clipLine(Size2l imgSize, CV_IN_OUT Point2l& pt1, CV_IN_OUT Point2l& pt2);


CV_EXPORTS_W bool clipLine(Rect imgRect, CV_OUT CV_IN_OUT Point& pt1, CV_OUT CV_IN_OUT Point& pt2);


CV_EXPORTS_W void ellipse2Poly( Point center, Size axes, int angle,
                                int arcStart, int arcEnd, int delta,
                                CV_OUT std::vector<Point>& pts );


CV_EXPORTS void ellipse2Poly(Point2d center, Size2d axes, int angle,
                             int arcStart, int arcEnd, int delta,
                             CV_OUT std::vector<Point2d>& pts);


CV_EXPORTS_W void putText( InputOutputArray img, const String& text, Point org,
                         int fontFace, double fontScale, Scalar color,
                         int thickness = 1, int lineType = LINE_8,
                         bool bottomLeftOrigin = false );


CV_EXPORTS_W Size getTextSize(const String& text, int fontFace,
                            double fontScale, int thickness,
                            CV_OUT int* baseLine);



CV_EXPORTS_W double getFontScaleFromHeight(const int fontFace,
                                           const int pixelHeight,
                                           const int thickness = 1);


class CV_EXPORTS LineIterator
{
public:

    LineIterator( const Mat& img, Point pt1, Point pt2,
                  int connectivity = 8, bool leftToRight = false );

    uchar* operator *();

    LineIterator& operator ++();

    LineIterator operator ++(int);

    Point pos() const;

    uchar* ptr;
    const uchar* ptr0;
    int step, elemSize;
    int err, count;
    int minusDelta, plusDelta;
    int minusStep, plusStep;
};





inline
uchar* LineIterator::operator *()
{
    return ptr;
}

inline
LineIterator& LineIterator::operator ++()
{
    int mask = err < 0 ? -1 : 0;
    err += minusDelta + (plusDelta & mask);
    ptr += minusStep + (plusStep & mask);
    return *this;
}

inline
LineIterator LineIterator::operator ++(int)
{
    LineIterator it = *this;
    ++(*this);
    return it;
}

inline
Point LineIterator::pos() const
{
    Point p;
    p.y = (int)((ptr - ptr0)/step);
    p.x = (int)(((ptr - ptr0) - p.y*step)/elemSize);
    return p;
}







}

#endif
