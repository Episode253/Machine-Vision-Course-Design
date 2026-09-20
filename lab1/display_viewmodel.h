#ifndef LAB1_DISPLAY_VIEWMODEL_H
#define LAB1_DISPLAY_VIEWMODEL_H

#include <QImage>
#include <QObject>
#include <QString>

#include "camera_worker.h"
#include "qt/latest_image_slot.h"

class QTimer;




class DisplayViewModel : public QObject {
    Q_OBJECT

public:
    enum class Mode { Idle, Photo, Video };
    Q_ENUM(Mode)

    explicit DisplayViewModel(QObject *parent = nullptr);
    ~DisplayViewModel() override;

    Mode mode() const { return m_mode; }


    bool openImage(const QString &path);


    bool startCamera();


    void stopCamera();

signals:
    void frameChanged(const QImage &image);
    void modeChanged(Mode mode);
    void errorOccurred(const QString &message);

private slots:

    void pullFrame();

private:
    void setMode(Mode mode);


    void stopCapture();

    CameraWorker m_worker;
    labqt::LatestImageSlot m_latest;
    QTimer *m_frameTimer = nullptr;
    Mode m_mode = Mode::Idle;
};

#endif
