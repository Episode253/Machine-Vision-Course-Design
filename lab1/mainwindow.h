#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DisplayViewModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    DisplayViewModel *m_viewModel = nullptr;
};

#endif
