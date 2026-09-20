

#ifndef OPENCV_BACKGROUND_SEGM_HPP
#define OPENCV_BACKGROUND_SEGM_HPP

#include "opencv2/core.hpp"

namespace cv
{





class CV_EXPORTS_W BackgroundSubtractor : public Algorithm
{
public:

    CV_WRAP virtual void apply(InputArray image, OutputArray fgmask, double learningRate=-1) = 0;


    CV_WRAP virtual void getBackgroundImage(OutputArray backgroundImage) const = 0;
};



class CV_EXPORTS_W BackgroundSubtractorMOG2 : public BackgroundSubtractor
{
public:

    CV_WRAP virtual int getHistory() const = 0;

    CV_WRAP virtual void setHistory(int history) = 0;


    CV_WRAP virtual int getNMixtures() const = 0;

    CV_WRAP virtual void setNMixtures(int nmixtures) = 0;


    CV_WRAP virtual double getBackgroundRatio() const = 0;

    CV_WRAP virtual void setBackgroundRatio(double ratio) = 0;


    CV_WRAP virtual double getVarThreshold() const = 0;

    CV_WRAP virtual void setVarThreshold(double varThreshold) = 0;


    CV_WRAP virtual double getVarThresholdGen() const = 0;

    CV_WRAP virtual void setVarThresholdGen(double varThresholdGen) = 0;


    CV_WRAP virtual double getVarInit() const = 0;

    CV_WRAP virtual void setVarInit(double varInit) = 0;

    CV_WRAP virtual double getVarMin() const = 0;
    CV_WRAP virtual void setVarMin(double varMin) = 0;

    CV_WRAP virtual double getVarMax() const = 0;
    CV_WRAP virtual void setVarMax(double varMax) = 0;


    CV_WRAP virtual double getComplexityReductionThreshold() const = 0;

    CV_WRAP virtual void setComplexityReductionThreshold(double ct) = 0;


    CV_WRAP virtual bool getDetectShadows() const = 0;

    CV_WRAP virtual void setDetectShadows(bool detectShadows) = 0;


    CV_WRAP virtual int getShadowValue() const = 0;

    CV_WRAP virtual void setShadowValue(int value) = 0;


    CV_WRAP virtual double getShadowThreshold() const = 0;

    CV_WRAP virtual void setShadowThreshold(double threshold) = 0;


    CV_WRAP virtual void apply(InputArray image, OutputArray fgmask, double learningRate=-1) CV_OVERRIDE = 0;
};


CV_EXPORTS_W Ptr<BackgroundSubtractorMOG2>
    createBackgroundSubtractorMOG2(int history=500, double varThreshold=16,
                                   bool detectShadows=true);


class CV_EXPORTS_W BackgroundSubtractorKNN : public BackgroundSubtractor
{
public:

    CV_WRAP virtual int getHistory() const = 0;

    CV_WRAP virtual void setHistory(int history) = 0;


    CV_WRAP virtual int getNSamples() const = 0;

    CV_WRAP virtual void setNSamples(int _nN) = 0;


    CV_WRAP virtual double getDist2Threshold() const = 0;

    CV_WRAP virtual void setDist2Threshold(double _dist2Threshold) = 0;


    CV_WRAP virtual int getkNNSamples() const = 0;

    CV_WRAP virtual void setkNNSamples(int _nkNN) = 0;


    CV_WRAP virtual bool getDetectShadows() const = 0;

    CV_WRAP virtual void setDetectShadows(bool detectShadows) = 0;


    CV_WRAP virtual int getShadowValue() const = 0;

    CV_WRAP virtual void setShadowValue(int value) = 0;


    CV_WRAP virtual double getShadowThreshold() const = 0;

    CV_WRAP virtual void setShadowThreshold(double threshold) = 0;
};


CV_EXPORTS_W Ptr<BackgroundSubtractorKNN>
    createBackgroundSubtractorKNN(int history=500, double dist2Threshold=400.0,
                                   bool detectShadows=true);



}

#endif
