#ifndef LAB7_MAINWINDOW_H
#define LAB7_MAINWINDOW_H

#include <memory>
#include <QMainWindow>

class DetectorWorker;
class QLabel;

// 主窗口不声明自己的信号槽（连接全用 lambda 写在 .cpp 里），所以没有 Q_OBJECT 宏，不生成 moc_mainwindow.cpp。
class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    // 检测器由本窗口持有并传 this 作父对象，因此它连同 QTimer 都活在主线程。
    std::unique_ptr<DetectorWorker> m_detector;
    QLabel *m_imageLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
};

#endif
