#include "mainwindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "detector_worker.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_detector(std::make_unique<DetectorWorker>(this))
{
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);
    auto *buttons = new QHBoxLayout;
    auto *start = new QPushButton(tr("开始检测"), central);
    auto *stop = new QPushButton(tr("停止检测"), central);
    buttons->addWidget(start);
    buttons->addWidget(stop);
    buttons->addStretch();
    layout->addLayout(buttons);
    m_imageLabel = new QLabel(tr("等待 YuNet TensorRT 检测"), central);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setMinimumSize(800, 520);
    m_imageLabel->setStyleSheet(QStringLiteral("background:#20252b; color:#d4dae2;"));
    layout->addWidget(m_imageLabel, 1);
    m_statusLabel = new QLabel(tr("未启动"), central);
    layout->addWidget(m_statusLabel);
    setCentralWidget(central);
    setWindowTitle(tr("lab7 YuNet 人脸与眼睛 ROI 检测（TensorRT）"));
    resize(1000, 700);

    connect(start, &QPushButton::clicked, m_detector.get(), &DetectorWorker::start);
    connect(stop, &QPushButton::clicked, m_detector.get(), &DetectorWorker::stop);
    connect(m_detector.get(), &DetectorWorker::frameChanged, this, [this](const QImage &image) {
        m_imageLabel->setPixmap(QPixmap::fromImage(image).scaled(
            m_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });
    connect(m_detector.get(), &DetectorWorker::statusChanged,
            m_statusLabel, &QLabel::setText);
    connect(m_detector.get(), &DetectorWorker::errorOccurred, this, [this](const QString &message) {
        m_statusLabel->setText(tr("错误：%1").arg(message));
    });
}

MainWindow::~MainWindow() = default;
