#include "display_viewmodel.h"

#include <chrono>
#include <optional>

#include <opencv2/imgcodecs.hpp>

#include <QTimer>

#include "qt/logging.h"
#include "qt/qt_interop.h"

DisplayViewModel::DisplayViewModel(QObject *parent)
    : QObject(parent)
{
    m_frameTimer = new QTimer(this);
    m_frameTimer->setInterval(33);
    connect(m_frameTimer, &QTimer::timeout, this, &DisplayViewModel::pullFrame);
}

DisplayViewModel::~DisplayViewModel()
{
    m_worker.stop();
}

bool DisplayViewModel::openImage(const QString &path)
{
    // TODO(lab1): 读取所选图片；失败要报错并返回 false。
    // TODO(lab1): 显示图片前先停止摄像头采集（图片与摄像头互斥）。
    // TODO(lab1): 把这一帧交给界面并切换到图片模式。
    (void)path;
    return false;
}

bool DisplayViewModel::startCamera()
{
    // TODO(lab1): 启动采集线程，把最新帧写入线程安全槽（失败要报错返回 false）。
    // TODO(lab1): 启动刷新定时器并切换到视频模式。
    return false;
}

void DisplayViewModel::stopCamera()
{
    // TODO(lab1): 停止采集线程与刷新定时器。
    // TODO(lab1): 把模式切回空闲。
}

void DisplayViewModel::stopCapture()
{
    m_worker.stop();
    m_frameTimer->stop();
    m_latest.clear();
}

void DisplayViewModel::pullFrame()
{
    if (const std::optional<QImage> image = m_latest.takeLatest())
        emit frameChanged(*image);
}

void DisplayViewModel::setMode(Mode mode)
{
    if (m_mode == mode)
        return;
    m_mode = mode;
    emit modeChanged(m_mode);
}
