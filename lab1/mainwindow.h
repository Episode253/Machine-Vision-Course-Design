#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DisplayViewModel;

// 主窗口：只负责把控件与 DisplayViewModel 连起来，自身不保存图像数据。
// 所有槽与 lambda 都在 GUI 线程执行。
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // 「加载图片」按钮（pushButton_2）：名字符合 on_<控件>_<信号> 规则，由 .ui 自动连接。
    void on_pushButton_2_clicked();

    // 「打开摄像头」按钮（pushButton），开/关复用同一个。
    void on_pushButton_clicked();

private:
    // 界面对象没有 QObject 父对象关系，用 unique_ptr 表达唯一所有权。
    std::unique_ptr<Ui::MainWindow> ui;
    // 非拥有指针：实际由 QObject 父子树（this）释放。
    DisplayViewModel *m_viewModel = nullptr;
};

#endif
