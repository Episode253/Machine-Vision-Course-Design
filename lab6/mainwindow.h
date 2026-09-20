#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CalibrationViewModel;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void updateRoi();

    void setPreviewActive(bool active);

    std::unique_ptr<Ui::MainWindow> ui;
    CalibrationViewModel *m_viewModel = nullptr;
};

#endif
