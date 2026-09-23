#include "mainwindow.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

#include "low_vision_viewmodel.h"

namespace {

// 匿名命名空间内的辅助函数，仅本文件可见：四个效果行共用同一套滑条配置。
QSlider *makeSlider(QWidget *parent)
{
    auto *slider = new QSlider(Qt::Horizontal, parent);
    // 量程与默认值和 lab3 一致：0~100、默认 0（0 表示该效果不生效）。
    slider->setRange(0, 100);
    slider->setValue(0);
    return slider;
}

}

// 构造（GUI 线程）：界面与 lab3 同构，差别是摄像头按钮做成开关、多一个 Sobel 复选框、效果行用循环生成。
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_viewModel(new LowVisionViewModel(this))
{
    auto *central = new QWidget(this);
    auto *root = new QVBoxLayout(central);
    auto *toolbar = new QHBoxLayout;
    auto *cameraButton = new QPushButton(tr("打开/关闭摄像头"), central);
    auto *imageButton = new QPushButton(tr("加载图片"), central);
    auto *stopButton = new QPushButton(tr("停止处理"), central);
    // lab3 的 ViewModel 里已有 setSobel，但 lab3 界面上没有任何控件调用它；只有 lab4 这个复选框打开了 Sobel 分支。
    auto *sobelCheck = new QCheckBox(tr("Sobel边缘检测"), central);
    toolbar->addWidget(cameraButton);
    toolbar->addWidget(imageButton);
    toolbar->addWidget(stopButton);
    toolbar->addWidget(sobelCheck);
    toolbar->addStretch();
    root->addLayout(toolbar);

    m_imageLabel = new QLabel(central);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    // 画面标签 720×450、窗口 980×800，都比 lab3 的 640×420 / 900×760 大一档。
    m_imageLabel->setMinimumSize(720, 450);
    m_imageLabel->setStyleSheet(QStringLiteral("background:#20252b; color:#cfd6df;"));
    m_imageLabel->setText(tr("请选择图片或打开摄像头"));
    root->addWidget(m_imageLabel, 1);

    auto *effects = new QGridLayout;
    const QStringList names{tr("视力模糊"), tr("视野缺失"), tr("白内障"), tr("眩光")};
    // 循环生成四行效果，取代 lab3 手写的四份：names 是显示文本，sliderSlots 把循环下标映射到成员指针。
    const QList<QSlider **> sliderSlots{&m_blurSlider, &m_fieldSlider,
                                        &m_cataractSlider, &m_glareSlider};
    for (int row = 0; row < names.size(); ++row) {
        auto *check = new QCheckBox(names[row], central);
        auto *slider = makeSlider(central);
        // 通过二级指针写回对应成员，界面上仍能按名字取到这四个滑条。
        *sliderSlots[row] = slider;
        effects->addWidget(check, row, 0);
        effects->addWidget(slider, row, 1);
        // 与 lab3 相同的「取消勾选归零」语义：row 按值捕获，用来分派到对应的 setXxx。
        connect(check, &QCheckBox::toggled, this, [this, row, slider](bool enabled) {
            const int value = enabled ? slider->value() : 0;
            switch (row) {
            case 0: m_viewModel->setBlur(value); break;
            case 1: m_viewModel->setFieldLoss(value); break;
            case 2: m_viewModel->setCataract(value); break;
            case 3: m_viewModel->setGlare(value); break;
            }
        });
        // 滑条：未勾选时直接返回，避免该项未启用时拖动滑条意外下发参数（与 lab3 的 if 判断同义）。
        connect(slider, &QSlider::valueChanged, this, [this, row, check](int value) {
            if (!check->isChecked())
                return;
            switch (row) {
            case 0: m_viewModel->setBlur(value); break;
            case 1: m_viewModel->setFieldLoss(value); break;
            case 2: m_viewModel->setCataract(value); break;
            case 3: m_viewModel->setGlare(value); break;
            }
        });
    }
    root->addLayout(effects);
    m_statusLabel = new QLabel(tr("就绪"), central);
    root->addWidget(m_statusLabel);
    setCentralWidget(central);
    setWindowTitle(tr("综合视觉模拟程序"));
    resize(980, 800);

    // 摄像头按钮做成开关：按 running() 决定停还是开；lab3 是两个独立按钮（打开摄像头 / 停止）。
    connect(cameraButton, &QPushButton::clicked, this, [this] {
        if (m_viewModel->running())
            m_viewModel->stop();
        else
            m_viewModel->startCamera();
    });
    connect(imageButton, &QPushButton::clicked, this, [this] {
        const QString path = QFileDialog::getOpenFileName(
            this, tr("打开图片"), QString(),
            tr("Images (*.png *.jpg *.jpeg *.bmp);;All files (*)"));
        if (!path.isEmpty())
            m_viewModel->openImage(path);
    });
    // 停止与 Sobel 都直接连到 ViewModel 的槽（同线程直连，不需要 lambda）。
    connect(stopButton, &QPushButton::clicked, m_viewModel, &LowVisionViewModel::stop);
    connect(sobelCheck, &QCheckBox::toggled, m_viewModel, &LowVisionViewModel::setSobel);
    // 显示：frameChanged 由 ViewModel 的 GUI 侧定时器发出，按标签尺寸等比缩放后贴图。
    connect(m_viewModel, &LowVisionViewModel::frameChanged, this, [this](const QImage &image) {
        m_imageLabel->setPixmap(QPixmap::fromImage(image).scaled(
            m_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });
    connect(m_viewModel, &LowVisionViewModel::stateChanged,
            m_statusLabel, &QLabel::setText);
    connect(m_viewModel, &LowVisionViewModel::errorOccurred, this, [this](const QString &message) {
        m_statusLabel->setText(tr("错误：%1").arg(message));
    });
}

// 析构默认实现：ViewModel 由构造时建立的 QObject 父子关系释放。
MainWindow::~MainWindow() = default;
