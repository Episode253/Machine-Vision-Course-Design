#ifndef LAB8_MAINWINDOW_H
#define LAB8_MAINWINDOW_H

#include <memory>

#include <QMainWindow>

class QLabel;
class AmslerWidget;
class FixationMonitor;

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