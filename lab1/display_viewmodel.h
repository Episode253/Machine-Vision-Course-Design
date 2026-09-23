#ifndef LAB1_DISPLAY_VIEWMODEL_H
#define LAB1_DISPLAY_VIEWMODEL_H

#include <QImage>
#include <QObject>
#include <QString>

#include "camera_worker.h"
#include "qt/latest_image_slot.h"

class QTimer;




// lab1 的视图模型：把「打开图片」与「开摄像头」两种来源统一成 frameChanged 信号。
// 采集线程只写线程安全单槽 m_latest，界面更新一律由 GUI 线程的定时器取出后发出。
class DisplayViewModel : public QObject {
    Q_OBJECT

public:
    // 三种显示状态：界面按它决定按钮文案与是否清空画面。
    enum class Mode { Idle, Photo, Video };
    Q_ENUM(Mode)

    explicit DisplayViewModel(QObject *parent = nullptr);
    ~DisplayViewModel() override;

    Mode mode() const { return m_mode; }


    // 读取并显示指定图片，失败返回 false；仅由 GUI 线程调用。
    bool openImage(const QString &path);

    // 启动摄像头采集并切到视频模式，失败返回 false；仅由 GUI 线程调用。
    bool startCamera();

    // 停止采集并回到空闲模式；仅由 GUI 线程调用。
    void stopCamera();

signals:
    // 交给界面显示的一帧：图片走直连，摄像头帧经 GUI 线程定时器转发。
    void frameChanged(const QImage &image);
    // 模式变化，界面据此改按钮文案并决定是否清屏。
    void modeChanged(Mode mode);
    // 错误文案，界面显示在状态栏。
    void errorOccurred(const QString &message);

private slots:
    // 由 m_frameTimer 在 GUI 线程超时触发：从单槽取最新帧再转发。
    void pullFrame();

private:
    // 统一修改模式：值未变时不重复发信号。
    void setMode(Mode mode);

    // 停止采集线程、停止刷新定时器、清空单槽，三个入口共用的收尾动作。
    void stopCapture();

    // 采集线程的封装，其 stop()/析构内部都会 join。
    CameraWorker m_worker;
    // 线程安全的「最新帧」单槽：采集线程写、GUI 线程取，旧帧被覆盖丢弃。
    labqt::LatestImageSlot m_latest;
    // GUI 线程的 33ms 刷新定时器；由 QObject 父子树负责释放，这里只是非拥有指针。
    QTimer *m_frameTimer = nullptr;
    // 当前模式，只在 GUI 线程读写。
    Mode m_mode = Mode::Idle;
};

#endif
