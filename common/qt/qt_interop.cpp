#include "qt_interop.h"

#include <opencv2/imgproc.hpp>

namespace labqt {

QImage toQImage(const cv::Mat &source)
{
    if (source.empty())
        return QImage();

    cv::Mat rgb;
    switch (source.channels()) {
    case 1:
        cv::cvtColor(source, rgb, cv::COLOR_GRAY2RGB);
        break;
    case 3:
        cv::cvtColor(source, rgb, cv::COLOR_BGR2RGB);
        break;
    case 4:
        cv::cvtColor(source, rgb, cv::COLOR_BGRA2RGB);
        break;
    default:
        return QImage();
    }

    const QImage view(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step),
                      QImage::Format_RGB888);
    return view.copy();
}

}
