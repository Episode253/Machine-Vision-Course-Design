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

    connect(openCamera, &QPushButton::clicked, this, [this] { m_viewModel->startCamera(); });
    connect(openImage, &QPushButton::clicked, this, [this] {
        const QString path = QFileDialog::getOpenFileName(this, tr("打开图片"), QString(),
            tr("Images (*.png *.jpg *.jpeg *.bmp);;All files (*)"));
        if (!path.isEmpty()) m_viewModel->openImage(path);
    });
    connect(stop, &QPushButton::clicked, m_viewModel.get(), &LowVisionViewModel::stop);
    connect(m_viewModel.get(), &LowVisionViewModel::frameChanged, this, [this](const QImage &image) {
        m_imageLabel->setPixmap(QPixmap::fromImage(image).scaled(
            m_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });
    connect(m_viewModel.get(), &LowVisionViewModel::stateChanged, m_statusLabel, &QLabel::setText);
    connect(m_viewModel.get(), &LowVisionViewModel::errorOccurred, this, [this](const QString &message) {
        m_statusLabel->setText(tr("错误：%1").arg(message));
    });
    connect(m_blurCheck, &QCheckBox::toggled, this, [this](bool on) { m_viewModel->setBlur(on ? m_blurSlider->value() : 0); });
    connect(m_fieldCheck, &QCheckBox::toggled, this, [this](bool on) { m_viewModel->setFieldLoss(on ? m_fieldSlider->value() : 0); });
    connect(m_cataractCheck, &QCheckBox::toggled, this, [this](bool on) { m_viewModel->setCataract(on ? m_cataractSlider->value() : 0); });
    connect(m_glareCheck, &QCheckBox::toggled, this, [this](bool on) { m_viewModel->setGlare(on ? m_glareSlider->value() : 0); });
    connect(m_blurSlider, &QSlider::valueChanged, this, [this](int value) { if (m_blurCheck->isChecked()) m_viewModel->setBlur(value); });
    connect(m_fieldSlider, &QSlider::valueChanged, this, [this](int value) { if (m_fieldCheck->isChecked()) m_viewModel->setFieldLoss(value); });
    connect(m_cataractSlider, &QSlider::valueChanged, this, [this](int value) { if (m_cataractCheck->isChecked()) m_viewModel->setCataract(value); });
    connect(m_glareSlider, &QSlider::valueChanged, this, [this](int value) { if (m_glareCheck->isChecked()) m_viewModel->setGlare(value); });
}

MainWindow::~MainWindow() = default;
