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
    connect(ui->thresholdSlider, &QSlider::valueChanged, this, [this](int value) {
        ui->thresholdValue->setText(QString::number(value));
        m_viewModel->setThreshold(value);
    });


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
    connect(m_viewModel, &CalibrationViewModel::calibrationFinished, this,
            [this](const QString &centreText) {
                ui->centreValueLabel->setText(tr("校准中心：%1").arg(centreText));
                ui->verdictLabel->setText(tr("已锁定中心，实时显示偏差"));
                ui->measureButton->setEnabled(true);
            });
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

    updateRoi();
    m_viewModel->setThreshold(ui->thresholdSlider->value());
    setPreviewActive(false);
    ui->measureButton->setEnabled(false);
}

MainWindow::~MainWindow() = default;

void MainWindow::setPreviewActive(bool active)
{
    ui->startCalibrationButton->setEnabled(!active);
    ui->confirmCentreButton->setEnabled(active);
    ui->stopPreviewButton->setEnabled(active);

    if (!active)
        ui->binaryLabel->clear();
}

void MainWindow::updateRoi()
{
    m_viewModel->setRoi(ui->roiX1Slider->value(), ui->roiY1Slider->value(),
                        ui->roiX2Slider->value(), ui->roiY2Slider->value());
}
