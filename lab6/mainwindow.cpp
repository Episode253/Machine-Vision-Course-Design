#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSlider>
#include <QPixmap>
#include <QStatusBar>

#include "calibration_viewmodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
    , m_viewModel(new CalibrationViewModel(this))
{
    ui->setupUi(this);


    // 按钮 → ViewModel 槽：界面不做逻辑，只转调，顺带清掉上一轮的显示内容。
    // “开始校准”先把两块画面和上次的结论清掉，再启动预览。
    connect(ui->startCalibrationButton, &QPushButton::clicked, this, [this] {
        ui->previewLabel->clear();
        ui->binaryLabel->clear();
        ui->verdictLabel->setText(tr("校准中：注视中心红点，然后点击“确认中心”"));
        m_viewModel->startCalibration();
    });
    connect(ui->confirmCentreButton, &QPushButton::clicked, this,
            [this] { m_viewModel->confirmCentre(); });
    connect(ui->stopPreviewButton, &QPushButton::clicked, this,
            [this] { m_viewModel->stopCalibration(); });
    connect(ui->measureButton, &QPushButton::clicked, this, [this] { m_viewModel->measure(); });

    // 四个 ROI 滑块共用一种处理方式：更新数值标签，再把四个值整体重算一次 ROI
    //（不记录“哪个角在被拖动”，反正矩形归一化后与拖动顺序无关）。
    connect(ui->roiX1Slider, &QSlider::valueChanged, this, [this](int value) {
        ui->roiX1Value->setText(QString::number(value));
        updateRoi();
    });
    connect(ui->roiY1Slider, &QSlider::valueChanged, this, [this](int value) {
        ui->roiY1Value->setText(QString::number(value));
        updateRoi();
    });
    connect(ui->roiX2Slider, &QSlider::valueChanged, this, [this](int value) {
        ui->roiX2Value->setText(QString::number(value));
        updateRoi();
    });
    connect(ui->roiY2Slider, &QSlider::valueChanged, this, [this](int value) {
        ui->roiY2Value->setText(QString::number(value));
        updateRoi();
    });
    // 阈值滑块直接下发：下一次检测就会用新阈值，不必重启预览。
    connect(ui->thresholdSlider, &QSlider::valueChanged, this, [this](int value) {
        ui->thresholdValue->setText(QString::number(value));
        m_viewModel->setThreshold(value);
    });

    // ViewModel 信号 → 界面：QImage 转 QPixmap 并按标签大小等比缩放，
    // 彩色预览用平滑插值、二值图用快速插值（二值图放大会糊，但看得清轮廓就够）。
    connect(m_viewModel, &CalibrationViewModel::previewFrame, this, [this](const QImage &image) {
        ui->previewLabel->setPixmap(QPixmap::fromImage(image).scaled(
            ui->previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });
    connect(m_viewModel, &CalibrationViewModel::detectionFrame, this, [this](const QImage &image) {
        ui->binaryLabel->setPixmap(QPixmap::fromImage(image).scaled(
            ui->binaryLabel->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
    });
    connect(m_viewModel, &CalibrationViewModel::previewActiveChanged, this,
            &MainWindow::setPreviewActive);
    // 锁定中心成功后才启用“测量”按钮——测量以锁定中心为基准。
    connect(m_viewModel, &CalibrationViewModel::calibrationFinished, this,
            [this](const QString &centreText) {
                ui->centreValueLabel->setText(tr("校准中心：%1").arg(centreText));
                ui->verdictLabel->setText(tr("已锁定中心，实时显示偏差"));
                ui->measureButton->setEnabled(true);
            });
    // 实时偏差：未检出就提示，检出则显示 Δx/Δy 与方位（水平 4px 内算中心）。
    connect(m_viewModel, &CalibrationViewModel::deviationChanged, this,
            [this](bool found, int dx, int dy) {
                if (!found) {
                    ui->verdictLabel->setText(tr("未检测到瞳孔"));
                    return;
                }
                const QString where = qAbs(dx) < 4 ? tr("中心")
                                                   : (dx < 0 ? tr("偏左") : tr("偏右"));
                ui->verdictLabel->setText(tr("偏差 Δx=%1 Δy=%2 — %3")
                                              .arg(dx)
                                              .arg(dy)
                                              .arg(where));
            });
    // 一次测量的最终结论：同时刷新中心、实测位置与结论文案（valid 已折进结论文案里）。
    connect(m_viewModel, &CalibrationViewModel::measurementFinished, this,
            [this](const QString &centreText, const QString &actualText,
                   const QString &verdictText, bool) {
                ui->centreValueLabel->setText(tr("校准中心：%1").arg(centreText));
                ui->actualValueLabel->setText(tr("当前瞳孔：%1").arg(actualText));
                ui->verdictLabel->setText(verdictText);
            });
    connect(m_viewModel, &CalibrationViewModel::errorOccurred, this,
            [this](const QString &message) {
                ui->verdictLabel->setText(message);
                statusBar()->showMessage(message, 5000);
            });

    // 启动时把界面初值同步给 ViewModel（默认 ROI 0,0–50,50、阈值 55）。
    updateRoi();
    m_viewModel->setThreshold(ui->thresholdSlider->value());
    setPreviewActive(false);
    // 未校准前不允许测量。
    ui->measureButton->setEnabled(false);
}

// 默认析构：m_viewModel 以 this 为父对象，由 QObject 的父子关系释放。
MainWindow::~MainWindow() = default;

// 预览状态切换：三个按钮的可用性互斥，退出预览时清掉二值图（它只在预览/测量时有意义）。
void MainWindow::setPreviewActive(bool active)
{
    ui->startCalibrationButton->setEnabled(!active);
    ui->confirmCentreButton->setEnabled(active);
    ui->stopPreviewButton->setEnabled(active);

    if (!active)
        ui->binaryLabel->clear();
}

// 读四个滑块转交 ViewModel；滑块是整帧绝对像素坐标（X 上限 1280、Y 上限 960）。
void MainWindow::updateRoi()
{
    m_viewModel->setRoi(ui->roiX1Slider->value(), ui->roiY1Slider->value(),
                        ui->roiX2Slider->value(), ui->roiY2Slider->value());
}
