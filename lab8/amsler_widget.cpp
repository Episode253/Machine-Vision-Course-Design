#include "amsler_widget.h"

#include <algorithm>
#include <QPainter>
#include <QTimer>

// 构造：定下最小尺寸，并建一个 250ms 单次触发的闪烁定时器——每格刺激只显示这么久，
// 超时后由 onStimulusTimeout 把状态机推进到可作答阶段。
AmslerWidget::AmslerWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(500, 500);
    m_flashTimer = new QTimer(this);
    m_flashTimer->setSingleShot(true);
    m_flashTimer->setInterval(250);
    connect(m_flashTimer, &QTimer::timeout, this, &AmslerWidget::onStimulusTimeout);
}

// 「开始 Amsler 测试」按钮 → GUI 线程。先让状态机按当前注视状态选起始阶段，并停掉上一轮的闪烁计时；
// 注视无效时只提示、不启动计时也不发 started；否则启动闪烁计时并通知测试已开始。
void AmslerWidget::start()
{
    m_test.start();
    m_flashTimer->stop();
    if (!m_test.fixationValid()) {
        emit stateChanged(tr("请注视中心红点，等待注视有效"));
        update();
        return;
    }
    m_flashTimer->start();
    emit started();
    update();
}

// FixationMonitor::fixationChanged 的槽，经队列连接在 GUI 线程执行（因此状态机与绘制无需加锁）。
// 转给状态机后同步定时器：注视无效就停表并提示试次作废，恢复且正在测试时重新起表（重放当前刺激）。
void AmslerWidget::setFixationValid(bool valid)
{
    m_test.setFixationValid(valid);
    if (!valid) {
        m_flashTimer->stop();
        emit stateChanged(tr("注视无效，当前试次作废，等待恢复"));
    } else if (m_test.running()) {
        m_flashTimer->start();
        emit stateChanged(tr("注视有效，测试继续"));
    }
    update();
}

// 「正常」按钮 → GUI 线程。非测试中时直接忽略（按钮在空闲或等待注视时也可能被按下）；
// 作答后若测试已结束就发 finished，否则重启闪烁计时去显示下一个刺激。
// 计时在这里是无条件重启的：若上一次刺激的 250ms 还没走完就按键，本次作答会被状态机忽略
// （此时阶段仍是 ShowingStimulus，不是 WaitingAnswer），而闪烁计时从 250ms 重新开始。
void AmslerWidget::answerNormal()
{
    if (!m_test.running()) return;
    m_test.answer(lab8::Answer::Normal);
    if (m_test.finished()) { const auto result = m_test.result(); emit finished(result.sampledRois, result.distortedRois); }
    else m_flashTimer->start();
    update();
}

// 「弯曲」按钮 → GUI 线程，流程与 answerNormal 相同，只是作答类型不同。
void AmslerWidget::answerDistorted()
{
    if (!m_test.running()) return;
    m_test.answer(lab8::Answer::Distorted);
    if (m_test.finished()) { const auto result = m_test.result(); emit finished(result.sampledRois, result.distortedRois); }
    else m_flashTimer->start();
    update();
}

// 闪烁计时到期：把当前刺激转为"等待作答"（此后作答才会被状态机接受），并重绘。
void AmslerWidget::onStimulusTimeout() { m_test.stimulusTimeout(); update(); }

// 棋盘区域：取控件宽高的较小值再留 24px 边距，居中成正方形——8×8 网格要求每格都是正方形。
QRectF AmslerWidget::boardRect() const
{
    const double side = std::min(width(), height()) - 24.0;
    return QRectF((width() - side) / 2.0, (height() - side) / 2.0, side, side);
}

// 单个格子的矩形：把棋盘按 rows/cols 等分，用区域自身的 row/col 定位（不是访问顺序中的位置）。
QRectF AmslerWidget::cellRect(const lab8::Roi &roi) const
{
    const QRectF board = boardRect();
    const auto &config = m_test.config();
    return QRectF(board.left() + roi.col * board.width() / config.cols,
                  board.top() + roi.row * board.height() / config.rows,
                  board.width() / config.cols, board.height() / config.rows);
}

// 画完整网格：黑色 1px 方框，每格左上角写行优先、从 1 开始的编号。只在 Idle/Finished 阶段调用。
void AmslerWidget::drawGrid(QPainter &painter) const
{
    const auto &config = m_test.config();
    painter.setPen(QPen(Qt::black, 1));
    for (int row = 0; row < config.rows; ++row) {
        for (int col = 0; col < config.cols; ++col) {
            const QRectF cell = cellRect(m_test.roiAt(row, col));
            painter.drawRect(cell);
            painter.drawText(cell.adjusted(3, 3, -3, -3), Qt::AlignTop | Qt::AlignLeft,
                             QString::number(row * config.cols + col + 1));
        }
    }
}

// 画当前刺激格：黑色 4px 粗框；Backslash/Cross 画左上→右下对角线，Slash/Cross 画左下→右上，
// 所以 Cross 是两条线交叉成 X。
void AmslerWidget::drawStimulus(QPainter &painter, const lab8::Stimulus &stimulus) const
{
    const QRectF cell = cellRect(stimulus.roi);
    painter.setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap));
    painter.drawRect(cell);
    if (stimulus.pattern == lab8::LinePattern::Backslash || stimulus.pattern == lab8::LinePattern::Cross)
        painter.drawLine(cell.topLeft(), cell.bottomRight());
    if (stimulus.pattern == lab8::LinePattern::Slash || stimulus.pattern == lab8::LinePattern::Cross)
        painter.drawLine(cell.bottomLeft(), cell.topRight());
}

// 绘制，GUI 线程。白底；Idle/Finished 时画完整网格供整体查看，测试中则完全不画网格、只画当前格——
// 这是刻意的：把网格画出来等于告诉受试者当前格在整张表上的方位，会给视野缺损的定位提供线索，
// 受试者只需注视中心红点、判断眼前这一格是否弯曲。之后始终在控件中心画 10px 宽的红点作为注视目标，
// 注视无效时加一行深红提示，测试结束后在底部报出变形区域数。
void AmslerWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);
    // 测试期间走 else 分支，只会画当前刺激格，绝不画其余格子。
    if (m_test.phase() == lab8::Phase::Idle || m_test.finished())
        drawGrid(painter);
    else if (const auto stimulus = m_test.currentStimulus())
        drawStimulus(painter, *stimulus);
    // 红点在任何阶段都画：它是受试者的注视目标，也是注视判定的参照。
    painter.setPen(QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
    painter.drawPoint(width() / 2, height() / 2);
    if (!m_test.fixationValid()) {
        painter.setPen(Qt::darkRed);
        painter.drawText(rect().adjusted(8, 8, -8, -8), Qt::AlignTop | Qt::AlignHCenter,
                         tr("注视无效，测试暂停"));
    }
    if (m_test.finished()) {
        painter.setPen(Qt::darkGreen);
        painter.drawText(rect(), Qt::AlignBottom | Qt::AlignHCenter,
                         tr("测试完成：%1 个区域中 %2 个出现变形")
                             .arg(m_test.result().sampledRois)
                             .arg(m_test.result().distortedRois));
    }
}
