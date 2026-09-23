#include "mainwindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "amsler_widget.h"
#include "fixation_monitor.h"

// 组装界面并连线，全部在 GUI 线程执行：左栏（伸缩 3）放 AmslerWidget 与三个测试按钮，
// 右栏（伸缩 2）放注视检测的启停按钮、摄像头画面和状态栏。
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

    // 启停按钮直连到 FixationMonitor 的槽（同线程直连）；stop() 内部会 join 工作线程，
    // 因此点「停止注视检测」时界面最多卡住一个检测周期。
    connect(startMonitor, &QPushButton::clicked, m_monitor.get(), &FixationMonitor::start);
    connect(stopMonitor, &QPushButton::clicked, m_monitor.get(), &FixationMonitor::stop);
    connect(startTest, &QPushButton::clicked, m_amsler.get(), &AmslerWidget::start);
    connect(normal, &QPushButton::clicked, m_amsler.get(), &AmslerWidget::answerNormal);
    connect(distorted, &QPushButton::clicked, m_amsler.get(), &AmslerWidget::answerDistorted);
    // 注视状态 → Amsler 状态机：两边同在 GUI 线程，直连即可，所以状态机与绘制都不需要加锁。
    connect(m_monitor.get(), &FixationMonitor::fixationChanged, m_amsler.get(), &AmslerWidget::setFixationValid);
    // 画面由工作线程 emit、QImage 已深拷贝，靠队列连接投递到 GUI 线程后才转成 QPixmap。
    connect(m_monitor.get(), &FixationMonitor::frameChanged, this, [this](const QImage &image) {
        m_cameraLabel->setPixmap(QPixmap::fromImage(image).scaled(m_cameraLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });
    connect(m_monitor.get(), &FixationMonitor::errorOccurred, m_statusLabel, &QLabel::setText);
    // 第二个 fixationChanged 连接只管刷新状态栏文案，不影响状态机。
    connect(m_monitor.get(), &FixationMonitor::fixationChanged, this, [this](bool valid) {
        m_statusLabel->setText(valid ? tr("注视有效，Amsler 测试可继续") : tr("注视无效，当前试次已暂停"));
    });
    connect(m_amsler.get(), &AmslerWidget::stateChanged, m_statusLabel, &QLabel::setText);
    connect(m_amsler.get(), &AmslerWidget::finished, this, [this](int sampled, int distortedCount) {
        m_statusLabel->setText(tr("测试完成：%1 个区域，%2 个变形").arg(sampled).arg(distortedCount));
    });
}
// 默认析构：成员按声明逆序销毁，m_monitor 先于 m_amsler；FixationMonitor 的析构会 stop() 并 join 工作线程。
MainWindow::~MainWindow() = default;
