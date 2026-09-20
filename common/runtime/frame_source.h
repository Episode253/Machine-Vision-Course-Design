#ifndef VISUAL_LAB_FRAME_SOURCE_H
#define VISUAL_LAB_FRAME_SOURCE_H

#include <cstdint>
#include <optional>

#include <QString>
#include <opencv2/opencv.hpp>

#include "core/frame.hpp"





class FrameSource
{
public:
    bool open(int cameraIndex = 0);

    bool openImage(const QString &imagePath);


    std::optional<labcore::Frame> next();
    bool isOpened() const;
    void release();
    QString description() const;
    QString lastError() const { return m_lastError; }


    bool isTestImage() const { return m_usingTestFrame; }

private:
    cv::VideoCapture m_camera;
    cv::Mat m_testFrame;
    bool m_usingTestFrame = false;
    int m_cameraIndex = 0;
    std::uint64_t m_seq = 0;
    QString m_testImagePath;
    QString m_lastError;
};

#endif
