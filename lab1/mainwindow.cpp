#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPixmap>
#include <QStatusBar>

#include "display_viewmodel.h"



// GUI 线程构造：搭好界面、创建 ViewModel，并把三条信号接到界面。
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
    , m_viewModel(new DisplayViewModel(this))
{
    // ui 没有 QObject 父对象关系，使用 unique_ptr 表达唯一所有权并自动析构。
    // m_viewModel 则交给 QObject 父子树（this）管理，因此这里的裸指针只是非拥有引用。
    ui->setupUi(this);

    // 每次换帧都按 label 当前尺寸缩放：保持长宽比，平滑缩放以减少锯齿。
    connect(m_viewModel, &DisplayViewModel::frameChanged, this, [this](const QImage &image) {
        ui->label->setPixmap(QPixmap::fromImage(image).scaled(
            ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });

    connect(m_viewModel, &DisplayViewModel::modeChanged, this, [this](DisplayViewModel::Mode mode) {
        const bool previewing = mode == DisplayViewModel::Mode::Video;
        ui->pushButton->setText(previewing ? tr("关闭摄像头") : tr("开启摄像头"));

        // 回到空闲（例如看完图片再关摄像头）时清空画面，不留上一张图或最后一帧。
        if (mode == DisplayViewModel::Mode::Idle)
            ui->label->clear();
    });

    connect(m_viewModel, &DisplayViewModel::errorOccurred, this, [this](const QString &message) {
        // 错误只在状态栏停留 5 秒，不打断用户操作。
        statusBar()->showMessage(message, 5000);
    });
}

// ui 由 unique_ptr 释放、m_viewModel 由 QObject 父子树释放，没有额外资源要回收。
MainWindow::~MainWindow() = default;

// 「加载图片」按钮（pushButton_2）的槽：名字符合 on_<控件>_<信号> 规则，由 .ui 自动连接。
void MainWindow::on_pushButton_2_clicked()
{
    const QString path = QFileDialog::getOpenFileName(
        this, tr("打开图片"), QString(),
        tr("Images (*.png *.jpg *.jpeg *.bmp);;All files (*)"));
    // 用户取消对话框时路径为空，直接返回。
    if (path.isEmpty())
        return;
    if (!m_viewModel->openImage(path))
        statusBar()->showMessage(tr("图片读取失败：%1").arg(path), 5000);
}

// 「打开摄像头」按钮（pushButton）的槽：同一个按钮复用，按当前模式决定这次是开还是关。
void MainWindow::on_pushButton_clicked()
{
    if (m_viewModel->mode() == DisplayViewModel::Mode::Video) {
        m_viewModel->stopCamera();
        return;
    }
    // 返回 false 表示摄像头不可用（设备打不开或采集线程起不来）。
    if (!m_viewModel->startCamera())
        statusBar()->showMessage(tr("摄像头不可用"), 5000);
}
