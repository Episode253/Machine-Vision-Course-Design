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

QSlider *makeSlider(QWidget *parent)
{
    auto *slider = new QSlider(Qt::Horizontal, parent);
    slider->setRange(0, 100);
    slider->setValue(0);
    return slider;
}

}

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
    auto *sobelCheck = new QCheckBox(tr("Sobel边缘检测"), central);
    toolbar->addWidget(cameraButton);
    toolbar->addWidget(imageButton);
    toolbar->addWidget(stopButton);
    toolbar->addWidget(sobelCheck);
    toolbar->addStretch();
    root->addLayout(toolbar);

    m_imageLabel = new QLabel(central);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setMinimumSize(720, 450);
    m_imageLabel->setStyleSheet(QStringLiteral("background:#20252b; color:#cfd6df;"));
    m_imageLabel->setText(tr("请选择图片或打开摄像头"));
    root->addWidget(m_imageLabel, 1);

    auto *effects = new QGridLayout;
    const QStringList names{tr("视力模糊"), tr("视野缺失"), tr("白内障"), tr("眩光")};
    const QList<QSlider **> sliderSlots{&m_blurSlider, &m_fieldSlider,
                                        &m_cataractSlider, &m_glareSlider};
    for (int row = 0; row < names.size(); ++row) {
        auto *check = new QCheckBox(names[row], central);
        auto *slider = makeSlider(central);
        *sliderSlots[row] = slider;
        effects->addWidget(check, row, 0);
        effects->addWidget(slider, row, 1);
        connect(check, &QCheckBox::toggled, this, [this, row, slider](bool enabled) {
            const int value = enabled ? slider->value() : 0;
            switch (row) {
            case 0: m_viewModel->setBlur(value); break;
            case 1: m_viewModel->setFieldLoss(value); break;
            case 2: m_viewModel->setCataract(value); break;
            case 3: m_viewModel->setGlare(value); break;
            }
        });
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
    connect(stopButton, &QPushButton::clicked, m_viewModel, &LowVisionViewModel::stop);
    connect(sobelCheck, &QCheckBox::toggled, m_viewModel, &LowVisionViewModel::setSobel);
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

MainWindow::~MainWindow() = default;
