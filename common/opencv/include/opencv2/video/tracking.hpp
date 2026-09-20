

#ifndef OPENCV_TRACKING_HPP
#define OPENCV_TRACKING_HPP

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

namespace cv
{




enum { OPTFLOW_USE_INITIAL_FLOW     = 4,
       OPTFLOW_LK_GET_MIN_EIGENVALS = 8,
       OPTFLOW_FARNEBACK_GAUSSIAN   = 256
     };


CV_EXPORTS_W RotatedRect CamShift( InputArray probImage, CV_IN_OUT Rect& window,
                                   TermCriteria criteria );



CV_EXPORTS_W int meanShift( InputArray probImage, CV_IN_OUT Rect& window, TermCriteria criteria );


CV_EXPORTS_W int buildOpticalFlowPyramid( InputArray img, OutputArrayOfArrays pyramid,
                                          Size winSize, int maxLevel, bool withDerivatives = true,
                                          int pyrBorder = BORDER_REFLECT_101,
                                          int derivBorder = BORDER_CONSTANT,
                                          bool tryReuseInputImage = true );




CV_EXPORTS_W void calcOpticalFlowPyrLK( InputArray prevImg, InputArray nextImg,
                                        InputArray prevPts, InputOutputArray nextPts,
                                        OutputArray status, OutputArray err,
                                        Size winSize = Size(21,21), int maxLevel = 3,
                                        TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01),
                                        int flags = 0, double minEigThreshold = 1e-4 );


CV_EXPORTS_W void calcOpticalFlowFarneback( InputArray prev, InputArray next, InputOutputArray flow,
                                            double pyr_scale, int levels, int winsize,
                                            int iterations, int poly_n, double poly_sigma,
                                            int flags );


CV_DEPRECATED CV_EXPORTS Mat estimateRigidTransform( InputArray src, InputArray dst, bool fullAffine );

enum
{
    MOTION_TRANSLATION = 0,
    MOTION_EUCLIDEAN   = 1,
    MOTION_AFFINE      = 2,
    MOTION_HOMOGRAPHY  = 3
};



CV_EXPORTS_W double computeECC(InputArray templateImage, InputArray inputImage, InputArray inputMask = noArray());




CV_EXPORTS_W double findTransformECC( InputArray templateImage, InputArray inputImage,
                                      InputOutputArray warpMatrix, int motionType,
                                      TermCriteria criteria,
                                      InputArray inputMask, int gaussFiltSize);


CV_EXPORTS
double findTransformECC(InputArray templateImage, InputArray inputImage,
    InputOutputArray warpMatrix, int motionType = MOTION_AFFINE,
    TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 50, 0.001),
    InputArray inputMask = noArray());




class CV_EXPORTS_W KalmanFilter
{
public:
    CV_WRAP KalmanFilter();

    CV_WRAP KalmanFilter( int dynamParams, int measureParams, int controlParams = 0, int type = CV_32F );


    void init( int dynamParams, int measureParams, int controlParams = 0, int type = CV_32F );


    CV_WRAP const Mat& predict( const Mat& control = Mat() );


    CV_WRAP const Mat& correct( const Mat& measurement );

    CV_PROP_RW Mat statePre;
    CV_PROP_RW Mat statePost;
    CV_PROP_RW Mat transitionMatrix;
    CV_PROP_RW Mat controlMatrix;
    CV_PROP_RW Mat measurementMatrix;
    CV_PROP_RW Mat processNoiseCov;
    CV_PROP_RW Mat measurementNoiseCov;
    CV_PROP_RW Mat errorCovPre;
    CV_PROP_RW Mat gain;
    CV_PROP_RW Mat errorCovPost;


    Mat temp1;
    Mat temp2;
    Mat temp3;
    Mat temp4;
    Mat temp5;
};



CV_EXPORTS_W Mat readOpticalFlow( const String& path );

CV_EXPORTS_W bool writeOpticalFlow( const String& path, InputArray flow );


class CV_EXPORTS_W DenseOpticalFlow : public Algorithm
{
public:

    CV_WRAP virtual void calc( InputArray I0, InputArray I1, InputOutputArray flow ) = 0;

    CV_WRAP virtual void collectGarbage() = 0;
};


class CV_EXPORTS_W SparseOpticalFlow : public Algorithm
{
public:

    CV_WRAP virtual void calc(InputArray prevImg, InputArray nextImg,
                      InputArray prevPts, InputOutputArray nextPts,
                      OutputArray status,
                      OutputArray err = cv::noArray()) = 0;
};



class CV_EXPORTS_W FarnebackOpticalFlow : public DenseOpticalFlow
{
public:
    CV_WRAP virtual int getNumLevels() const = 0;
    CV_WRAP virtual void setNumLevels(int numLevels) = 0;

    CV_WRAP virtual double getPyrScale() const = 0;
    CV_WRAP virtual void setPyrScale(double pyrScale) = 0;

    CV_WRAP virtual bool getFastPyramids() const = 0;
    CV_WRAP virtual void setFastPyramids(bool fastPyramids) = 0;

    CV_WRAP virtual int getWinSize() const = 0;
    CV_WRAP virtual void setWinSize(int winSize) = 0;

    CV_WRAP virtual int getNumIters() const = 0;
    CV_WRAP virtual void setNumIters(int numIters) = 0;

    CV_WRAP virtual int getPolyN() const = 0;
    CV_WRAP virtual void setPolyN(int polyN) = 0;

    CV_WRAP virtual double getPolySigma() const = 0;
    CV_WRAP virtual void setPolySigma(double polySigma) = 0;

    CV_WRAP virtual int getFlags() const = 0;
    CV_WRAP virtual void setFlags(int flags) = 0;

    CV_WRAP static Ptr<FarnebackOpticalFlow> create(
            int numLevels = 5,
            double pyrScale = 0.5,
            bool fastPyramids = false,
            int winSize = 13,
            int numIters = 10,
            int polyN = 5,
            double polySigma = 1.1,
            int flags = 0);
};


class CV_EXPORTS_W VariationalRefinement : public DenseOpticalFlow
{
public:

    CV_WRAP virtual void calcUV(InputArray I0, InputArray I1, InputOutputArray flow_u, InputOutputArray flow_v) = 0;


    CV_WRAP virtual int getFixedPointIterations() const = 0;

    CV_WRAP virtual void setFixedPointIterations(int val) = 0;


    CV_WRAP virtual int getSorIterations() const = 0;

    CV_WRAP virtual void setSorIterations(int val) = 0;


    CV_WRAP virtual float getOmega() const = 0;

    CV_WRAP virtual void setOmega(float val) = 0;


    CV_WRAP virtual float getAlpha() const = 0;

    CV_WRAP virtual void setAlpha(float val) = 0;


    CV_WRAP virtual float getDelta() const = 0;

    CV_WRAP virtual void setDelta(float val) = 0;


    CV_WRAP virtual float getGamma() const = 0;

    CV_WRAP virtual void setGamma(float val) = 0;


    CV_WRAP static Ptr<VariationalRefinement> create();
};


class CV_EXPORTS_W DISOpticalFlow : public DenseOpticalFlow
{
public:
    enum
    {
        PRESET_ULTRAFAST = 0,
        PRESET_FAST = 1,
        PRESET_MEDIUM = 2
    };


    CV_WRAP virtual int getFinestScale() const = 0;

    CV_WRAP virtual void setFinestScale(int val) = 0;


    CV_WRAP virtual int getPatchSize() const = 0;

    CV_WRAP virtual void setPatchSize(int val) = 0;


    CV_WRAP virtual int getPatchStride() const = 0;

    CV_WRAP virtual void setPatchStride(int val) = 0;


    CV_WRAP virtual int getGradientDescentIterations() const = 0;

    CV_WRAP virtual void setGradientDescentIterations(int val) = 0;


    CV_WRAP virtual int getVariationalRefinementIterations() const = 0;

    CV_WRAP virtual void setVariationalRefinementIterations(int val) = 0;


    CV_WRAP virtual float getVariationalRefinementAlpha() const = 0;

    CV_WRAP virtual void setVariationalRefinementAlpha(float val) = 0;


    CV_WRAP virtual float getVariationalRefinementDelta() const = 0;

    CV_WRAP virtual void setVariationalRefinementDelta(float val) = 0;


    CV_WRAP virtual float getVariationalRefinementGamma() const = 0;

    CV_WRAP virtual void setVariationalRefinementGamma(float val) = 0;



    CV_WRAP virtual bool getUseMeanNormalization() const = 0;

    CV_WRAP virtual void setUseMeanNormalization(bool val) = 0;


    CV_WRAP virtual bool getUseSpatialPropagation() const = 0;

    CV_WRAP virtual void setUseSpatialPropagation(bool val) = 0;


    CV_WRAP static Ptr<DISOpticalFlow> create(int preset = DISOpticalFlow::PRESET_FAST);
};


class CV_EXPORTS_W SparsePyrLKOpticalFlow : public SparseOpticalFlow
{
public:
    CV_WRAP virtual Size getWinSize() const = 0;
    CV_WRAP virtual void setWinSize(Size winSize) = 0;

    CV_WRAP virtual int getMaxLevel() const = 0;
    CV_WRAP virtual void setMaxLevel(int maxLevel) = 0;

    CV_WRAP virtual TermCriteria getTermCriteria() const = 0;
    CV_WRAP virtual void setTermCriteria(TermCriteria& crit) = 0;

    CV_WRAP virtual int getFlags() const = 0;
    CV_WRAP virtual void setFlags(int flags) = 0;

    CV_WRAP virtual double getMinEigThreshold() const = 0;
    CV_WRAP virtual void setMinEigThreshold(double minEigThreshold) = 0;

    CV_WRAP static Ptr<SparsePyrLKOpticalFlow> create(
            Size winSize = Size(21, 21),
            int maxLevel = 3, TermCriteria crit =
            TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01),
            int flags = 0,
            double minEigThreshold = 1e-4);
};



}

#endif
