

#ifndef OPENCV_FLANN_TIMER_H
#define OPENCV_FLANN_TIMER_H

#include <time.h>
#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"

namespace cvflann
{


class StartStopTimer
{
    int64 startTime;

public:

    double value;



    StartStopTimer()
    {
        reset();
    }


    void start()
    {
        startTime = cv::getTickCount();
    }


    void stop()
    {
        int64 stopTime = cv::getTickCount();
        value += ( (double)stopTime - startTime) / cv::getTickFrequency();
    }


    void reset()
    {
        value = 0;
    }

};

}

#endif
