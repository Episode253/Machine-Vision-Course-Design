#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPixmap>
#include <QStatusBar>

#include "display_viewmodel.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
    , m_viewModel(new DisplayViewModel(this))
{
    // ui 没有 QObject 父对象关系，使用 unique_ptr 表达唯一所有权并自动析构。
    // m_viewModel 则交给 QObject 父子树（this）管理，因此这里的裸指针只是非拥有引用。
    ui->setupUi(this);

    connect(m_viewModel, &DisplayViewModel::frameChanged, this, [this](const QImage &image) {
        ui->label->setPixmap(QPixmap::fromImage(image).scaled(
            ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });

    connect(m_viewModel, &DisplayViewModel::modeChanged, this, [this](DisplayViewModel::Mode mode) {
        const bool previewing = mode == DisplayViewModel::Mode::Video;
        ui->pushButton->setText(previewing ? tr("关闭摄像头") : tr("开启摄像头"));


        if (mode == DisplayViewModel::Mode::Idle)
            ui->label->clear();
    });

    connect(m_viewModel, &DisplayViewModel::errorOccurred, this, [this](const QString &message) {
        statusBar()->showMessage(message, 5000);
    });
}

MainWindow::~MainWindow() = default;

void MainWindow::on_pushButton_2_clicked()
{
    const QString path = QFileDialog::getOpenFileName(
        this, tr("打开图片"), QString(),
        tr("Images (*.png *.jpg *.jpeg *.bmp);;All files (*)"));
    if (path.isEmpty())
        return;
    if (!m_viewModel->openImage(path))
        statusBar()->showMessage(tr("图片读取失败：%1").arg(path), 5000);
}

void MainWindow::on_pushButton_clicked()
{
    if (m_viewModel->mode() == DisplayViewModel::Mode::Video) {
        m_viewModel->stopCamera();
        return;
    }
    if (!m_viewModel->startCamera())
        statusBar()->showMessage(tr("摄像头不可用"), 5000);
}
