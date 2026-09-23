#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CalibrationViewModel;



// 主窗口：只做控件连接与画面显示，全部逻辑转发给 CalibrationViewModel（两者都在 GUI 线程）。
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    // 读四个 ROI 滑块的值并整成一个矩形交给 ViewModel。
    void updateRoi();

    // 按预览状态启停按钮：预览中才允许“确认中心 / 停止预览”。
    void setPreviewActive(bool active);

    std::unique_ptr<Ui::MainWindow> ui;
    // 以 this 为父对象，随窗口一起销毁，无需手动 delete。
    CalibrationViewModel *m_viewModel = nullptr;
};

#endif
