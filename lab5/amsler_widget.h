#ifndef LAB5_AMSLER_WIDGET_H
#define LAB5_AMSLER_WIDGET_H

#include <QRectF>
#include <QWidget>

#include "amsler_test.h"

class QTimer;

// Amsler 测试的显示控件：白底网格 + 当前刺激 + 正中央的红色注视点。
// 它同时充当刺激计时器（m_flashTimer）：到点后把状态机从“显示刺激”推进到“等待作答”。
// 状态机本体在 m_test 中，控件与状态机都只在 GUI 线程访问。
class AmslerWidget : public QWidget {
    Q_OBJECT

public:
    explicit AmslerWidget(QWidget *parent = nullptr);
    ~AmslerWidget() override;

    // 绘制入口，由 Qt 事件循环在 GUI 线程调用：按阶段画完整网格或当前刺激，并始终画中心注视点。
    void paintEvent(QPaintEvent *event) override;

public slots:
    // 开始一轮测试，由主窗口「开始测试」按钮在 GUI 线程触发。
    void start();
    // 作答「正常」，由「正常」按钮触发；状态机不在等待作答阶段时会忽略这次回答。
    void answerNormal();
    // 作答「弯曲」，由「弯曲」按钮触发；同样只在等待作答阶段生效。
    void answerDistorted();

signals:
    // 一轮测试真正开始后发出，主窗口据此把结果标签复位为“测试中…”。
    void started();
    // 测试结束时发出，携带已采样区域数与其中被判变形的区域数。
    void finished(int sampledRois, int distortedRois);

private slots:
    // 刺激显示计时结束的槽（GUI 线程）：推进状态机并请求重绘。
    void onStimulusTimeout();

private:
    // 棋盘区域：控件短边减去 20px 边距后的正方形并居中；网格与刺激都画在这块区域内。
    QRectF boardRect() const;
    // 网格坐标 → 棋盘内的像素矩形，行列等分；不读 logicalWidth/logicalHeight。
    QRectF cellRect(const lab5::Roi &roi) const;
    // 画完整网格：每格黑框 1px，左上角标从 1 开始的行优先编号。
    void drawGrid(QPainter &painter) const;
    // 画当前刺激：4px 黑框圈出所在格，再按线型画对角线。
    void drawStimulus(QPainter &painter, const lab5::Stimulus &stimulus) const;

    // 状态机本体；不是 QObject，只在 GUI 线程访问，所以内部不加锁。
    lab5::AmslerTest m_test;
    // 单次触发定时器，控制一次刺激的显示时长；以 this 为父对象，由 QObject 树回收。
    QTimer *m_flashTimer = nullptr;
};

#endif
