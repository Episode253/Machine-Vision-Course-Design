#ifndef LAB7_MAINWINDOW_H
#define LAB7_MAINWINDOW_H

#include <memory>
#include <QMainWindow>

class DetectorWorker;
class QLabel;

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    std::unique_ptr<DetectorWorker> m_detector;
    QLabel *m_imageLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
};

#endif
