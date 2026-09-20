#ifndef VISUAL_LAB_QT_INTEROP_H
#define VISUAL_LAB_QT_INTEROP_H

#include <QImage>

#include <opencv2/core.hpp>

namespace labqt {





QImage toQImage(const cv::Mat &source);

inline bool isSupported(const cv::Mat &source)
{
    return !source.empty() && (source.channels() == 1 || source.channels() == 3
                               || source.channels() == 4);
}

}

#endif
