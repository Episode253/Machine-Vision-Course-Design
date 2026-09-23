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
    // 四个槽都由 GUI 线程调用：前三个来自 MainWindow 的按钮，setFixationValid 来自注视监视器的队列投递信号。
    void start();
    void answerNormal();
    void answerDistorted();
    void setFixationValid(bool valid);
protected:
    // 测试中只画当前格、不画网格（见 .cpp 的说明）。
    void paintEvent(QPaintEvent *event) override;
signals:
    // 只有真正开始显示刺激时才发（注视无效的那次启动不发）。
    void started();
    // 走完全部区域时发，参数即 TestResult 的两项。
    void finished(int sampledRois, int distortedRois);
    // 给状态栏的提示文案。
    void stateChanged(const QString &message);
private slots:
    void onStimulusTimeout();
private:
    // 以下几何换算与绘制辅助只在 GUI 线程内使用。
    QRectF boardRect() const;
    QRectF cellRect(const lab8::Roi &roi) const;
    void drawGrid(QPainter &painter) const;
    void drawStimulus(QPainter &painter, const lab8::Stimulus &stimulus) const;
    // 状态机与绘制同在 GUI 线程，因此零同步。
    lab8::AmslerTest m_test;
    // 每格刺激的 250ms 单次触发计时器，测试全程只有它驱动状态机向前推进。
    QTimer *m_flashTimer = nullptr;
};
#endif
