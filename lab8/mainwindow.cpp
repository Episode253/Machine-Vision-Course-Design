#include "mainwindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "amsler_widget.h"
#include "fixation_monitor.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_amsler(new AmslerWidget), m_monitor(new FixationMonitor(this))
{
    auto *central = new QWidget(this);
    auto *layout = new QHBoxLayout(central);
    auto *left = new QVBoxLayout;
    auto *right = new QVBoxLayout;
    auto *startMonitor = new QPushButton(tr("启动 lab7 注视检测"), central);
    auto *stopMonitor = new QPushButton(tr("停止注视检测"), central);
    auto *startTest = new QPushButton(tr("开始 Amsler 测试"), central);
    auto *normal = new QPushButton(tr("正常"), central);
    auto *distorted = new QPushButton(tr("弯曲"), central);
    m_cameraLabel = new QLabel(tr("等待 YuNet TensorRT"), central);
    m_cameraLabel->setMinimumSize(480, 360);
    m_cameraLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel = new QLabel(tr("未启动"), central);
    m_statusLabel->setWordWrap(true);
    left->addWidget(m_amsler.get(), 1);
    left->addWidget(startTest); left->addWidget(normal); left->addWidget(distorted);
    right->addWidget(startMonitor); right->addWidget(stopMonitor); right->addWidget(m_cameraLabel, 1); right->addWidget(m_statusLabel);
    layout->addLayout(left, 3); layout->addLayout(right, 2);
    setCentralWidget(central); setWindowTitle(tr("lab8 综合注视门控与视物变形检测")); resize(1400, 850);

    connect(startMonitor, &QPushButton::clicked, m_monitor.get(), &FixationMonitor::start);
    connect(stopMonitor, &QPushButton::clicked, m_monitor.get(), &FixationMonitor::stop);
    connect(startTest, &QPushButton::clicked, m_amsler.get(), &AmslerWidget::start);
    connect(normal, &QPushButton::clicked, m_amsler.get(), &AmslerWidget::answerNormal);
    connect(distorted, &QPushButton::clicked, m_amsler.get(), &AmslerWidget::answerDistorted);
    connect(m_monitor.get(), &FixationMonitor::fixationChanged, m_amsler.get(), &AmslerWidget::setFixationValid);
    connect(m_monitor.get(), &FixationMonitor::frameChanged, this, [this](const QImage &image) {
        m_cameraLabel->setPixmap(QPixmap::fromImage(image).scaled(m_cameraLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });
    connect(m_monitor.get(), &FixationMonitor::errorOccurred, m_statusLabel, &QLabel::setText);
    connect(m_monitor.get(), &FixationMonitor::fixationChanged, this, [this](bool valid) {
        m_statusLabel->setText(valid ? tr("注视有效，Amsler 测试可继续") : tr("注视无效，当前试次已暂停"));
    });
    connect(m_amsler.get(), &AmslerWidget::stateChanged, m_statusLabel, &QLabel::setText);
    connect(m_amsler.get(), &AmslerWidget::finished, this, [this](int sampled, int distortedCount) {
        m_statusLabel->setText(tr("测试完成：%1 个区域，%2 个变形").arg(sampled).arg(distortedCount));
    });
}
MainWindow::~MainWindow() = default;
