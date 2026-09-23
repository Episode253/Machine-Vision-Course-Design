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
    // 构造时把 this 传作 ViewModel 的 QObject 父对象，此处父对象与 unique_ptr 共同持有该对象。
    std::unique_ptr<LowVisionViewModel> m_viewModel;
    QLabel *m_imageLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    // 下面四组「复选框 + 滑条」一一对应四种效果：勾选框决定是否把滑条值下发（连接见 .cpp）。
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
