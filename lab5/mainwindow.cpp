#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>

#include "amsler_widget.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    connect(ui->pushButtonStart, &QPushButton::clicked, ui->amslerWidget, &AmslerWidget::start);
    connect(ui->pushButtonNormal, &QPushButton::clicked, ui->amslerWidget,
            &AmslerWidget::answerNormal);
    connect(ui->pushButtonDistorted, &QPushButton::clicked, ui->amslerWidget,
            &AmslerWidget::answerDistorted);

    // A new session starts "尚未开始" over: drop the previous round's result.
    connect(ui->amslerWidget, &AmslerWidget::started, this,
            [this] { ui->labelResult->setText(tr("测试中…")); });

    connect(ui->amslerWidget, &AmslerWidget::finished, this,
            [this](int sampledRois, int distortedRois) {
                ui->labelResult->setText(
                    tr("测试完成：%1 个区域中 %2 个出现变形").arg(sampledRois).arg(distortedRois));
                statusBar()->showMessage(tr("测试完成"), 5000);
            });
}

MainWindow::~MainWindow() = default;
