#ifndef LAB2_PIPELINE_VIEWMODEL_H
#define LAB2_PIPELINE_VIEWMODEL_H

#include <memory>

#include <QImage>
#include <QObject>
#include <QString>

#include "core/stage_registry.h"
#include "qt/latest_image_slot.h"
#include "runtime/pipeline.h"

class QTimer;




// lab2 的视图模型：组装 CaptureSource 与 Pipeline，把管线输出的帧转成 QImage 交给界面。
// 成员函数都在 GUI 线程调用；工作线程只通过 LatestImageSlot 这个带锁单槽与它交换数据。
// 对界面只暴露 Mode 与四个信号，不暴露管线内部结构。
class PipelineViewModel : public QObject {
    Q_OBJECT

public:
    enum class Mode { Idle, Photo, Video };
    Q_ENUM(Mode)

    explicit PipelineViewModel(QObject *parent = nullptr);
    ~PipelineViewModel() override;

    Mode mode() const { return m_mode; }
    bool running() const;
    bool sobelEnabled() const { return m_sobelEnabled; }

public slots:
    bool startCamera();
    bool startImage(const QString &path);
    void stop();

    void setSobelEnabled(bool enabled);

signals:
    void frameChanged(const QImage &image);
    void modeChanged(Mode mode);
    void statsChanged(double fps, quint64 dropped);
    void errorOccurred(const QString &message);

private slots:
    void pullFrame();
    void refreshStats();

private:


    // 组装并启动管线；源只在目标变化时重建，Sobel 开关切换时复用已打开的源。
    bool startInternal(const QString &imagePath);


    // 停止并销毁管线（会 join 所有工作线程），但不动 m_source，以便复用。
    void stopPipeline();

    void releaseSource();
    void setMode(Mode mode);

    std::unique_ptr<labruntime::Pipeline> m_pipeline;



    // 管线持有 m_source 的裸指针（借用），不负责其生命周期；
    // 因此 stopPipeline 之后 m_source 依然有效，可以继续使用。
    std::unique_ptr<labcore::Source> m_source;
    labcore::StageRegistry m_registry;
    // 工作线程写、GUI 线程读的单槽缓冲，内部用 QMutex 保护；工作线程不得直接碰界面。
    labqt::LatestImageSlot m_latest;
    QTimer *m_frameTimer = nullptr;
    QTimer *m_statsTimer = nullptr;
    // m_currentImagePath 是用户最后选择的目标，m_sourceTarget 是当前源实际对应的目标，
    // 两者比较决定是否重建源。
    QString m_currentImagePath;
    QString m_sourceTarget;
    bool m_sourceOpen = false;
    Mode m_mode = Mode::Idle;
    bool m_sobelEnabled = false;
};

#endif
