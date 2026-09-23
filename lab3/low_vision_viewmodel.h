#ifndef LAB3_LOW_VISION_VIEWMODEL_H
#define LAB3_LOW_VISION_VIEWMODEL_H

#include <atomic>
#include <mutex>
#include <string>
#include <thread>

#include <QImage>
#include <QObject>
#include <QString>

#include "runtime/frame_source.h"

class QTimer;

// lab3 的主 ViewModel：GUI 线程调用槽起停和改参数，工作线程取帧并跑四种效果；
// 帧只经 m_latest 单槽交给 GUI 线程的 QTimer，再由定时器 emit，两端都不跨线程碰 QWidget。
class LowVisionViewModel : public QObject {
    Q_OBJECT
public:
    explicit LowVisionViewModel(QObject *parent = nullptr);
    ~LowVisionViewModel() override;

    bool running() const { return m_running.load(); }

public slots:
    // 以下槽都由 GUI 线程的按钮/滑条信号触发，内部只置标志或加锁改参数，不做图像处理。
    bool startCamera();
    bool openImage(const QString &path);
    void stop();
    void setBlur(int value);
    void setFieldLoss(int value);
    void setCataract(int value);
    void setGlare(int value);
    void setSobel(bool enabled);

signals:
    // frameChanged 由 GUI 线程的刷新定时器发出（不是工作线程），槽里可以直接操作界面。
    void frameChanged(const QImage &image);
    void stateChanged(const QString &state);
    void errorOccurred(const QString &message);

private:
    // 四个效果的强度快照：GUI 线程写、工作线程每帧整份读，读写都经 m_paramMutex。
    struct Params { int blur = 0; int fieldLoss = 0; int cataract = 0; int glare = 0; };
    bool start(bool imageMode, const QString &path);
    // 工作线程入口：打开源 → 循环取帧 → process() → publish()，直到 m_stopRequested 置位。
    void workerLoop(bool imageMode, QString path);
    // 固定的处理链 blur → fieldLoss → cataract → glare，末尾按需叠 Sobel；在工作线程执行。
    cv::Mat process(const cv::Mat &source, const Params &params) const;
    // 把一帧写入单槽 m_latest（工作线程调用，内部加锁）。
    void publish(const QImage &image);
    // 成员指针转发：四个 setXxx 共用「加锁 + 夹取」这一段逻辑。
    void setParam(int Params::*member, int value);

    FrameSource m_source;
    // GUI 线程的取帧定时器，由 QObject 父子关系随本对象回收。
    QTimer *m_timer = nullptr;
    // 工作线程；start() 赋值前必先 stop() 并 join，因此赋值时旧线程已不可 join。
    std::thread m_worker;
    // 运行状态与停止请求：跨线程只靠这两个原子量传递，工作线程不读 QObject 状态。
    std::atomic_bool m_running{false};
    std::atomic_bool m_stopRequested{false};
    // 保护 m_params：GUI 线程写、工作线程每帧整份拷贝，算法执行期间不持锁。
    mutable std::mutex m_paramMutex;
    Params m_params;
    // Sobel 开关单独用原子量（布尔量，不必走 m_paramMutex）；lab3 界面上没有控件调用 setSobel，
    // 这个分支实际只有 lab4 的「Sobel边缘检测」复选框打开。
    std::atomic_bool m_sobelEnabled{false};
    // 单槽「最新帧」：工作线程写、GUI 定时器取走，两端都持 m_imageMutex；旧帧被静默覆盖。
    mutable std::mutex m_imageMutex;
    QImage m_latest;
};

#endif
