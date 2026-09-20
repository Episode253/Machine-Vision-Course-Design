#ifndef LAB5_AMSLER_WIDGET_H
#define LAB5_AMSLER_WIDGET_H

#include <QRectF>
#include <QWidget>

#include "amsler_test.h"

class QTimer;



class AmslerWidget : public QWidget {
    Q_OBJECT

public:
    explicit AmslerWidget(QWidget *parent = nullptr);
    ~AmslerWidget() override;

    void paintEvent(QPaintEvent *event) override;

public slots:
    void start();
    void answerNormal();
    void answerDistorted();

signals:


    void started();
    void finished(int sampledRois, int distortedRois);

private slots:
    void onStimulusTimeout();

private:
    QRectF boardRect() const;
    QRectF cellRect(const lab5::Roi &roi) const;
    void drawGrid(QPainter &painter) const;
    void drawStimulus(QPainter &painter, const lab5::Stimulus &stimulus) const;

    lab5::AmslerTest m_test;
    QTimer *m_flashTimer = nullptr;
};

#endif
