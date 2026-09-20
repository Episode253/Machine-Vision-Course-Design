#ifndef LAB3_MAINWINDOW_H
#define LAB3_MAINWINDOW_H

#include <memory>

#include <QMainWindow>

class QCheckBox;
class QLabel;
class QSlider;
class LowVisionViewModel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    std::unique_ptr<LowVisionViewModel> m_viewModel;
    QLabel *m_imageLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    QCheckBox *m_blurCheck = nullptr;
    QCheckBox *m_fieldCheck = nullptr;
    QCheckBox *m_cataractCheck = nullptr;
    QCheckBox *m_glareCheck = nullptr;
    QSlider *m_blurSlider = nullptr;
    QSlider *m_fieldSlider = nullptr;
    QSlider *m_cataractSlider = nullptr;
    QSlider *m_glareSlider = nullptr;
};

#endif
