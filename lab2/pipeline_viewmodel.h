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


    bool startInternal(const QString &imagePath);


    void stopPipeline();

    void releaseSource();
    void setMode(Mode mode);

    std::unique_ptr<labruntime::Pipeline> m_pipeline;



    std::unique_ptr<labcore::Source> m_source;
    labcore::StageRegistry m_registry;
    labqt::LatestImageSlot m_latest;
    QTimer *m_frameTimer = nullptr;
    QTimer *m_statsTimer = nullptr;
    QString m_currentImagePath;
    QString m_sourceTarget;
    bool m_sourceOpen = false;
    Mode m_mode = Mode::Idle;
    bool m_sobelEnabled = false;
};

#endif
