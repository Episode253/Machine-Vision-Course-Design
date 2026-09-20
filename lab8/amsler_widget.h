#ifndef LAB8_AMSLER_WIDGET_H
#define LAB8_AMSLER_WIDGET_H

#include <QRectF>
#include <QWidget>
#include "amsler_test.h"
class QTimer;
class QPainter;
class QPaintEvent;
class AmslerWidget : public QWidget {
    Q_OBJECT
public:
    explicit AmslerWidget(QWidget *parent = nullptr);
public slots:
    void start();
    void answerNormal();
    void answerDistorted();
    void setFixationValid(bool valid);
protected:
    void paintEvent(QPaintEvent *event) override;
signals:
    void started();
    void finished(int sampledRois, int distortedRois);
    void stateChanged(const QString &message);
private slots:
    void onStimulusTimeout();
private:
    QRectF boardRect() const;
    QRectF cellRect(const lab8::Roi &roi) const;
    void drawGrid(QPainter &painter) const;
    void drawStimulus(QPainter &painter, const lab8::Stimulus &stimulus) const;
    lab8::AmslerTest m_test;
    QTimer *m_flashTimer = nullptr;
};
#endif
