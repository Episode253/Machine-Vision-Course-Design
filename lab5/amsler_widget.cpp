#include "amsler_widget.h"

#include <algorithm>

#include <QFont>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

#include "qt/logging.h"

namespace {
// 单次刺激的显示时长（毫秒）。到点由 onStimulusTimeout() 把阶段推进到等待作答，
// 因此这个值同时是“受试者最早可以作答的时刻”。
constexpr int kFlashMs = 200;
}

// 构造：设最小尺寸并建好刺激定时器（GUI 线程，随控件存活）。
AmslerWidget::AmslerWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(400, 400);
    // QTimer 绑定 this 后由 QObject 树回收；保存裸指针只用于访问，不表示手动 delete。
    m_flashTimer = new QTimer(this);
    m_flashTimer->setSingleShot(true);
    m_flashTimer->setInterval(kFlashMs);
    connect(m_flashTimer, &QTimer::timeout, this, &AmslerWidget::onStimulusTimeout);
}

AmslerWidget::~AmslerWidget() = default;

// 开始一轮测试（GUI 线程，由主窗口「开始测试」按钮触发）：复位状态机并启动第一个刺激的计时。
void AmslerWidget::start()
{
    m_test.start();
    // 起测后阶段仍不是运行态（例如区域数为 0）时不启动定时器，只保留这条警告。
    if (!m_test.running()) {
        qCWarning(labUi) << "AmslerTest::start() is still an exercise placeholder";
        return;
    }
    // 定时器到点即显示结束，状态机转入等待作答。
    m_flashTimer->start();
    update();
    emit started();
}

// 作答「正常」（GUI 线程，由「正常」按钮触发）：转交状态机后，结束则上报结果，否则重启计时显示下一条刺激。
void AmslerWidget::answerNormal()
{
    // 未开始或已结束时的按键一律忽略。
    if (!m_test.running())
        return;
    m_test.answer(lab5::Answer::Normal);
    if (m_test.finished()) {
        const lab5::TestResult result = m_test.result();
        emit finished(result.sampledRois, result.distortedRois);
    } else {
        // 只要没结束就重启定时器。注意状态机拒收（阶段不符）时这里同样会重启，
        // 所以在刺激显示期间抢答会让这段计时从头开始。
        m_flashTimer->start();
    }
    update();
}

// 作答「弯曲」（GUI 线程，由「弯曲」按钮触发）：与 answerNormal() 只有回答种类不同。
// 答「弯曲」时状态机会立刻结束当前区域，所以这里通常马上进入下一格或结束整轮测试。
void AmslerWidget::answerDistorted()
{
    // 未开始或已结束时的按键一律忽略。
    if (!m_test.running())
        return;
    m_test.answer(lab5::Answer::Distorted);
    if (m_test.finished()) {
        const lab5::TestResult result = m_test.result();
        emit finished(result.sampledRois, result.distortedRois);
    } else {
        // 抢答会重启计时，原因同 answerNormal()。
        m_flashTimer->start();
    }
    update();
}

// 刺激显示到时（m_flashTimer 单次超时，GUI 线程）：把状态机推进到等待作答。
void AmslerWidget::onStimulusTimeout()
{
    // 定时器只改变测试状态，重绘统一交给事件循环，避免在槽内直接调用 paintEvent。
    m_test.stimulusTimeout();
    update();
}

// 计算棋盘区域：取控件短边减 20px 得到正方形并居中，使网格在任意窗口比例下都不变形。
QRectF AmslerWidget::boardRect() const
{
    const double side = std::min(width(), height()) - 20.0;
    return QRectF((width() - side) / 2.0, (height() - side) / 2.0, side, side);
}

// 把网格坐标 (row, col) 换算成棋盘内的像素矩形：每格宽高按棋盘尺寸等分。
QRectF AmslerWidget::cellRect(const lab5::Roi &roi) const
{
    const QRectF board = boardRect();
    const lab5::GridConfig &config = m_test.config();
    const double cellWidth = board.width() / config.cols;
    const double cellHeight = board.height() / config.rows;
    return QRectF(board.left() + roi.col * cellWidth, board.top() + roi.row * cellHeight,
                  cellWidth, cellHeight);
}

// 画完整网格（Idle 与 Finished 阶段）：黑框 1px，并在每格左上角标注编号。
// 编号是行优先且从 1 开始，用于把结果里的编号对应回具体格子。
void AmslerWidget::drawGrid(QPainter &painter) const
{
    const lab5::GridConfig &config = m_test.config();
    QFont font = painter.font();
    font.setPixelSize(14);
    painter.setFont(font);
    painter.setPen(QPen(Qt::black, 1.0));

    for (int row = 0; row < config.rows; ++row) {
        for (int col = 0; col < config.cols; ++col) {
            const QRectF cell = cellRect(m_test.roiAt(row, col));
            painter.drawRect(cell);
            painter.drawText(cell.adjusted(2, 2, -2, -2), Qt::AlignTop | Qt::AlignLeft,
                             QString::number(row * config.cols + col + 1));
        }
    }
}

// 画当前刺激：先用 4px 黑框强调所在格，再按线型画对角线（\ 左上→右下，/ 左下→右上，X 两条都画）。
// 测试中只画这一格、不画网格，避免受试者从网格编号推出当前方位。
void AmslerWidget::drawStimulus(QPainter &painter, const lab5::Stimulus &stimulus) const
{
    const QRectF cell = cellRect(stimulus.roi);
    painter.setPen(QPen(Qt::black, 4.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawRect(cell);
    switch (stimulus.pattern) {
    case lab5::LinePattern::Backslash:
        painter.drawLine(cell.topLeft(), cell.bottomRight());
        break;
    case lab5::LinePattern::Slash:
        painter.drawLine(cell.bottomLeft(), cell.topRight());
        break;
    case lab5::LinePattern::Cross:
        painter.drawLine(cell.topLeft(), cell.bottomRight());
        painter.drawLine(cell.bottomLeft(), cell.topRight());
        break;
    }
}

// 绘制入口（GUI 线程，由 Qt 事件循环调用），全部按 m_test 的当前阶段决定画什么：
// Idle 与 Finished 画完整网格，测试中只画当前刺激；结果文字与中心注视点按阶段叠加。
void AmslerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);

    // 只有 Idle 与 Finished 展示完整网格；测试中只画当前格。
    // 等待作答时 currentStimulus() 为空，此时除中心注视点外什么都不画。
    if (m_test.phase() == lab5::Phase::Idle || m_test.finished()) {
        drawGrid(painter);
    } else if (const std::optional<lab5::Stimulus> stimulus = m_test.currentStimulus()) {
        drawStimulus(painter, *stimulus);
    }

    // 结束后在底部叠加一行结果文字，数字同样取自 result()。
    if (m_test.finished()) {
        QFont font = painter.font();
        font.setPixelSize(20);
        painter.setFont(font);
        painter.setPen(Qt::darkGreen);
        painter.drawText(rect(), Qt::AlignBottom | Qt::AlignHCenter,
                         tr("测试完成：%1 个区域中 %2 个出现变形")
                             .arg(m_test.result().sampledRois)
                             .arg(m_test.result().distortedRois));
    }

    // 中心红点与阶段无关，始终绘制：受试者全程注视它，才能判断视野中哪一格出现变形。
    painter.setPen(QPen(Qt::red, 10.0, Qt::SolidLine, Qt::RoundCap));
    painter.drawPoint(width() / 2, height() / 2);
}
