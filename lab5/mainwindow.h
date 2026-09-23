#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// 主窗口：把界面上的「开始测试 / 正常 / 弯曲」三个按钮接到 AmslerWidget 的槽，
// 再把 AmslerWidget 的 started/finished 信号映射到结果标签与状态栏；控件均由 .ui 生成。
// 界面逻辑全部在 GUI 线程，不涉及其他线程。
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // 构造（GUI 线程）：setupUi 之后装配信号槽；析构为默认实现，界面对象由 unique_ptr 释放。
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    // .ui 生成的界面对象，随窗口析构自动释放。
    std::unique_ptr<Ui::MainWindow> ui;
};

#endif
