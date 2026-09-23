#include "mainwindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "detector_worker.h"

// 构造主窗口（只在主线程）：装配按钮、画面标签与状态栏，并把 DetectorWorker 的信号接到界面更新。
// m_detector 由 unique_ptr 持有，同时把 this 传作 QObject 父对象，因此它和窗口同属主线程。
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
    // 画面标签占满剩余空间（伸缩 1），帧只缩放不改比例地贴上来。
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

    // 两个槽都是直连：DetectorWorker 亲和主线程，所以 start()/stop() 就在这里同步执行，
    // stop() 里的 join 会让界面最多卡一个检测周期。
    connect(start, &QPushButton::clicked, m_detector.get(), &DetectorWorker::start);
    connect(stop, &QPushButton::clicked, m_detector.get(), &DetectorWorker::stop);
    // frameChanged 由主线程定时器（33ms）发出，这里在 GUI 线程收到后再缩放贴到标签上。
    connect(m_detector.get(), &DetectorWorker::frameChanged, this, [this](const QImage &image) {
        m_imageLabel->setPixmap(QPixmap::fromImage(image).scaled(
            m_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });
    connect(m_detector.get(), &DetectorWorker::statusChanged,
            m_statusLabel, &QLabel::setText);
    // 错误既可能来自主线程的 start()，也可能来自工作线程的 loop()，后者经队列连接投递到这里。
    connect(m_detector.get(), &DetectorWorker::errorOccurred, this, [this](const QString &message) {
        m_statusLabel->setText(tr("错误：%1").arg(message));
    });
}

// 默认析构：成员 m_detector 先析构，其析构函数里会 stop() 并 join 工作线程。
MainWindow::~MainWindow() = default;
