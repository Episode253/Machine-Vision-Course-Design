#ifndef LAB4_MAINWINDOW_H
#define LAB4_MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QSlider;
class LowVisionViewModel;

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    LowVisionViewModel *m_viewModel = nullptr;
    QLabel *m_imageLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    QSlider *m_blurSlider = nullptr;
    QSlider *m_fieldSlider = nullptr;
    QSlider *m_cataractSlider = nullptr;
    QSlider *m_glareSlider = nullptr;
};

#endif
