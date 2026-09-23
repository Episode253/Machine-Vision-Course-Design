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

// 构造（GUI 线程）：搭建界面并连接信号槽。所有 lambda 都在 GUI 线程执行，因此可以直接操作控件。
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_viewModel(std::make_unique<LowVisionViewModel>(this))
{
    auto *central = new QWidget(this);
    auto *root = new QVBoxLayout(central);
    auto *toolbar = new QHBoxLayout;
    auto *openCamera = new QPushButton(tr("打开摄像头"), central);
    auto *openImage = new QPushButton(tr("打开图片"), central);
    auto *stop = new QPushButton(tr("停止"), central);
    toolbar->addWidget(openCamera);
    toolbar->addWidget(openImage);
    toolbar->addWidget(stop);
    toolbar->addStretch();
    root->addLayout(toolbar);

    m_imageLabel = new QLabel(central);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    // 画面标签的初始尺寸；实际显示时仍按标签当前大小等比缩放。
    m_imageLabel->setMinimumSize(640, 420);
    m_imageLabel->setStyleSheet(QStringLiteral("background:#20252b; color:#cfd6df;"));
    m_imageLabel->setText(tr("请选择图片或打开摄像头"));
    root->addWidget(m_imageLabel, 1);

    auto *effects = new QGridLayout;
    m_blurCheck = new QCheckBox(tr("视力模糊"), central);
    m_fieldCheck = new QCheckBox(tr("视野缺失"), central);
    m_cataractCheck = new QCheckBox(tr("白内障"), central);
    m_glareCheck = new QCheckBox(tr("眩光"), central);
    m_blurSlider = new QSlider(Qt::Horizontal, central);
    m_fieldSlider = new QSlider(Qt::Horizontal, central);
    m_cataractSlider = new QSlider(Qt::Horizontal, central);
    m_glareSlider = new QSlider(Qt::Horizontal, central);
    const QList<QSlider *> sliders{m_blurSlider, m_fieldSlider, m_cataractSlider, m_glareSlider};
    // 四组滑条量程统一 0~100、默认 0；与 ViewModel 里 setParam 的夹取范围一致。
    for (QSlider *slider : sliders) {
        slider->setRange(0, 100);
        slider->setValue(0);
    }
    effects->addWidget(m_blurCheck, 0, 0); effects->addWidget(m_blurSlider, 0, 1);
    effects->addWidget(m_fieldCheck, 1, 0); effects->addWidget(m_fieldSlider, 1, 1);
    effects->addWidget(m_cataractCheck, 2, 0); effects->addWidget(m_cataractSlider, 2, 1);
    effects->addWidget(m_glareCheck, 3, 0); effects->addWidget(m_glareSlider, 3, 1);
    root->addLayout(effects);
    m_statusLabel = new QLabel(tr("就绪"), central);
    root->addWidget(m_statusLabel);
    setCentralWidget(central);
    setWindowTitle(tr("低视力成像模拟"));
    resize(900, 760);

    // 打开摄像头／打开图片：startCamera() 与 openImage() 的失败会经 errorOccurred 报到状态栏，故此处不看返回值。
    connect(openCamera, &QPushButton::clicked, this, [this] { m_viewModel->startCamera(); });
    connect(openImage, &QPushButton::clicked, this, [this] {
        const QString path = QFileDialog::getOpenFileName(this, tr("打开图片"), QString(),
            tr("Images (*.png *.jpg *.jpeg *.bmp);;All files (*)"));
        if (!path.isEmpty()) m_viewModel->openImage(path);
    });
    // 直接连到 ViewModel 的槽：同线程直连，不需要 lambda。
    connect(stop, &QPushButton::clicked, m_viewModel.get(), &LowVisionViewModel::stop);
    // 显示：stateChanged 只改状态栏文本，画面按标签当前尺寸等比缩放后贴图。
    // frameChanged 由 ViewModel 的 GUI 侧定时器发出，QImage 已在工作线程里深拷贝，这里可安全使用。
    connect(m_viewModel.get(), &LowVisionViewModel::frameChanged, this, [this](const QImage &image) {
        m_imageLabel->setPixmap(QPixmap::fromImage(image).scaled(
            m_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });
    connect(m_viewModel.get(), &LowVisionViewModel::stateChanged, m_statusLabel, &QLabel::setText);
    connect(m_viewModel.get(), &LowVisionViewModel::errorOccurred, this, [this](const QString &message) {
        m_statusLabel->setText(tr("错误：%1").arg(message));
    });
    // 勾选框：勾上则下发滑条当前值，取消勾选则下发 0。
    // 这就是四种效果「取消勾选」的实现路径——参数归零，算法侧自然成为空操作（severity<=0 返回原图）。
    connect(m_blurCheck, &QCheckBox::toggled, this, [this](bool on) { m_viewModel->setBlur(on ? m_blurSlider->value() : 0); });
    connect(m_fieldCheck, &QCheckBox::toggled, this, [this](bool on) { m_viewModel->setFieldLoss(on ? m_fieldSlider->value() : 0); });
    connect(m_cataractCheck, &QCheckBox::toggled, this, [this](bool on) { m_viewModel->setCataract(on ? m_cataractSlider->value() : 0); });
    connect(m_glareCheck, &QCheckBox::toggled, this, [this](bool on) { m_viewModel->setGlare(on ? m_glareSlider->value() : 0); });
    // 滑条：仅在该项已勾选时下发，未勾选时拖动不改参数（滑条自身仍记住位置，勾回来立刻生效）。
    connect(m_blurSlider, &QSlider::valueChanged, this, [this](int value) { if (m_blurCheck->isChecked()) m_viewModel->setBlur(value); });
    connect(m_fieldSlider, &QSlider::valueChanged, this, [this](int value) { if (m_fieldCheck->isChecked()) m_viewModel->setFieldLoss(value); });
    connect(m_cataractSlider, &QSlider::valueChanged, this, [this](int value) { if (m_cataractCheck->isChecked()) m_viewModel->setCataract(value); });
    connect(m_glareSlider, &QSlider::valueChanged, this, [this](int value) { if (m_glareCheck->isChecked()) m_viewModel->setGlare(value); });
}

// 析构用默认实现：成员按声明逆序销毁，m_viewModel 排在最后一个释放，其析构会 stop() 并 join 工作线程。
MainWindow::~MainWindow() = default;
