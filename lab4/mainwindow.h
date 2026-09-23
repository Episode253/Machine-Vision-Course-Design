#ifndef LAB4_MAINWINDOW_H
#define LAB4_MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QSlider;
class LowVisionViewModel;

// lab4 的窗口：算法与 ViewModel 都来自 lab3（由 lab4.pro 直接把 lab3 的源文件编进来），这里只负责界面与连接。
// 本类不声明自己的信号和槽（连接一律用 lambda），因此不写 Q_OBJECT，也就不会生成 moc_mainwindow.cpp；lab3 的 MainWindow 有 Q_OBJECT。
class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    // 裸指针 + 构造时传 this 作 QObject 父对象：由父子树负责释放，不使用 unique_ptr（lab3 那边用的是 unique_ptr）。
    LowVisionViewModel *m_viewModel = nullptr;
    QLabel *m_imageLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    // 四个效果滑条；对应的复选框在构造函数里局部创建、只被 lambda 捕获，因此不必做成成员。
    QSlider *m_blurSlider = nullptr;
    QSlider *m_fieldSlider = nullptr;
    QSlider *m_cataractSlider = nullptr;
    QSlider *m_glareSlider = nullptr;
};

#endif
