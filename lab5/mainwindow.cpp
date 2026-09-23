#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>

#include "amsler_widget.h"

// 构造（GUI 线程）：setupUi 之后把三个按钮连到 AmslerWidget 的槽，
// 并把测试的 started / finished 信号接到结果标签与状态栏。
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    // 三个按钮与控件同属 GUI 线程，信号槽是直连，按钮点击会同步进入 AmslerWidget 的槽。
    connect(ui->pushButtonStart, &QPushButton::clicked, ui->amslerWidget, &AmslerWidget::start);
    connect(ui->pushButtonNormal, &QPushButton::clicked, ui->amslerWidget,
            &AmslerWidget::answerNormal);
    connect(ui->pushButtonDistorted, &QPushButton::clicked, ui->amslerWidget,
            &AmslerWidget::answerDistorted);

    // A new session starts "尚未开始" over: drop the previous round's result.
    connect(ui->amslerWidget, &AmslerWidget::started, this,
            [this] { ui->labelResult->setText(tr("测试中…")); });

    // 结束信号带回两个数字：结果标签显示整句文案，状态栏再提示 5 秒。
    connect(ui->amslerWidget, &AmslerWidget::finished, this,
            [this](int sampledRois, int distortedRois) {
                ui->labelResult->setText(
                    tr("测试完成：%1 个区域中 %2 个出现变形").arg(sampledRois).arg(distortedRois));
                statusBar()->showMessage(tr("测试完成"), 5000);
            });
}

MainWindow::~MainWindow() = default;
