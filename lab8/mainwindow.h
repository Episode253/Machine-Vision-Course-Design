#ifndef LAB8_MAINWINDOW_H
#define LAB8_MAINWINDOW_H

#include <memory>

#include <QMainWindow>

class QLabel;
class AmslerWidget;
class FixationMonitor;

// 主窗口：左栏是 Amsler 测试控件与作答按钮，右栏是注视检测的启停按钮、摄像头画面和状态栏。
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    std::unique_ptr<AmslerWidget> m_amsler;
    std::unique_ptr<FixationMonitor> m_monitor;
    QLabel *m_cameraLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
};

#endif