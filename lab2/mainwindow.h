#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class PipelineViewModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // 槽名必须与 .ui 文件里的控件名 pushButton / pushButton_2 / checkBox 严格对应，
    // 由 setupUi 的 connectSlotsByName 自动连接，代码里没有显式的 connect 语句。
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_clicked();

private:
    // ViewModel 以 this 为 QObject 父对象，生命周期交给 Qt，这里不额外持有所有权。
    std::unique_ptr<Ui::MainWindow> ui;
    PipelineViewModel *m_viewModel = nullptr;
};

#endif
