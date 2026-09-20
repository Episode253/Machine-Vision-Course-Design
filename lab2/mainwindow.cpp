#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPixmap>
#include <QStatusBar>

#include "pipeline_viewmodel.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
    , m_viewModel(new PipelineViewModel(this))
{
    ui->setupUi(this);
    ui->label->clear();

    connect(m_viewModel, &PipelineViewModel::frameChanged, this, [this](const QImage &image) {
        ui->label->setPixmap(QPixmap::fromImage(image).scaled(
            ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });

    connect(m_viewModel, &PipelineViewModel::modeChanged, this, [this](PipelineViewModel::Mode mode) {
        const bool previewing = mode == PipelineViewModel::Mode::Video;
        ui->pushButton->setText(previewing ? tr("关闭摄像") : tr("打开摄像"));


        if (mode == PipelineViewModel::Mode::Idle)
            ui->label->clear();
    });

    connect(m_viewModel, &PipelineViewModel::statsChanged, this,
            [this](double fps, quint64 dropped) {
                statusBar()->showMessage(
                    tr("fps: %1    dropped: %2").arg(fps, 0, 'f', 1).arg(dropped));
            });

    connect(m_viewModel, &PipelineViewModel::errorOccurred, this, [this](const QString &message) {
        statusBar()->showMessage(message, 5000);
    });
}

MainWindow::~MainWindow() = default;

void MainWindow::on_pushButton_clicked()
{
    if (m_viewModel->mode() == PipelineViewModel::Mode::Video) {
        m_viewModel->stop();
        return;
    }
    if (!m_viewModel->startCamera())
        statusBar()->showMessage(tr("摄像头不可用"), 5000);
}

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

void MainWindow::on_checkBox_clicked()
{
    m_viewModel->setSobelEnabled(ui->checkBox->isChecked());
}
