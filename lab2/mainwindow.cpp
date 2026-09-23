#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPixmap>
#include <QStatusBar>

#include "pipeline_viewmodel.h"



// 构造：建立界面、清空图像标签，并把 ViewModel 的信号接到界面更新上。
// ui->setupUi 内部会调用 connectSlotsByName，自动连上 on_xxx_yyy 形式的槽。
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
    , m_viewModel(new PipelineViewModel(this))
{
    ui->setupUi(this);
    ui->label->clear();

    // 帧已由 ViewModel 在管线线程转成 QImage，这里只做缩放显示（GUI 线程）。
    connect(m_viewModel, &PipelineViewModel::frameChanged, this, [this](const QImage &image) {
        ui->label->setPixmap(QPixmap::fromImage(image).scaled(
            ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });

    connect(m_viewModel, &PipelineViewModel::modeChanged, this, [this](PipelineViewModel::Mode mode) {
        // 只有视频模式的按钮文案是「关闭摄像」，其余情况点一下都尝试打开摄像头。
        const bool previewing = mode == PipelineViewModel::Mode::Video;
        ui->pushButton->setText(previewing ? tr("关闭摄像") : tr("打开摄像"));
        // 回到空闲态说明采集已停止，清掉上一帧以免误导。
        if (mode == PipelineViewModel::Mode::Idle)
            ui->label->clear();
    });

    // 1s 一次的统计：fps 保留一位小数，丢帧数由管线各级队列溢出累加而来。
    connect(m_viewModel, &PipelineViewModel::statsChanged, this,
            [this](double fps, quint64 dropped) {
                statusBar()->showMessage(
                    tr("fps: %1    dropped: %2").arg(fps, 0, 'f', 1).arg(dropped));
            });

    // 错误只提示 5 秒，不打断当前操作。
    connect(m_viewModel, &PipelineViewModel::errorOccurred, this, [this](const QString &message) {
        statusBar()->showMessage(message, 5000);
    });
}

// 界面对象由 unique_ptr 管理，析构体保持默认即可。
MainWindow::~MainWindow() = default;

// 「打开/关闭摄像」按钮：视频模式下是关闭，其余情况一律尝试打开摄像头，
// 因此从图片模式点一下会切到摄像头。
void MainWindow::on_pushButton_clicked()
{
    if (m_viewModel->mode() == PipelineViewModel::Mode::Video) {
        m_viewModel->stop();
        return;
    }
    if (!m_viewModel->startCamera())
        statusBar()->showMessage(tr("摄像头不可用"), 5000);
}

// 「打开图片」按钮：取消选择时直接返回，失败由 ViewModel 负责收尾，这里只提示路径。
void MainWindow::on_pushButton_2_clicked()
{
    const QString path = QFileDialog::getOpenFileName(
        this, tr("打开图片"), QString(),
        tr("Images (*.png *.jpg *.jpeg *.bmp);;All files (*)"));
    if (path.isEmpty())
        return;
    if (!m_viewModel->startImage(path))
        statusBar()->showMessage(tr("图片读取失败：%1").arg(path), 5000);
}

// Sobel 复选框：把勾选状态同步给 ViewModel，由它决定是否重建管线。
void MainWindow::on_checkBox_clicked()
{
    m_viewModel->setSobelEnabled(ui->checkBox->isChecked());
}
