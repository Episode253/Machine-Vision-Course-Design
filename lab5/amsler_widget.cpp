#include "amsler_widget.h"

#include <algorithm>

#include <QFont>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

#include "qt/logging.h"

namespace {
constexpr int kFlashMs = 200;
}

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

void AmslerWidget::start()
{
    m_test.start();
    if (!m_test.running()) {
        qCWarning(labUi) << "AmslerTest::start() is still an exercise placeholder";
        return;
    }
    m_flashTimer->start();
    update();
    emit started();
}

void AmslerWidget::answerNormal()
{
    if (!m_test.running())
        return;
    m_test.answer(lab5::Answer::Normal);
    if (m_test.finished()) {
        const lab5::TestResult result = m_test.result();
        emit finished(result.sampledRois, result.distortedRois);
    } else {
        m_flashTimer->start();
    }
    update();
}

void AmslerWidget::answerDistorted()
{
    if (!m_test.running())
        return;
    m_test.answer(lab5::Answer::Distorted);
    if (m_test.finished()) {
        const lab5::TestResult result = m_test.result();
        emit finished(result.sampledRois, result.distortedRois);
    } else {
        m_flashTimer->start();
    }
    update();
}

void AmslerWidget::onStimulusTimeout()
{
    // 定时器只改变测试状态，重绘统一交给事件循环，避免在槽内直接调用 paintEvent。
    m_test.stimulusTimeout();
    update();
}

QRectF AmslerWidget::boardRect() const
{
    const double side = std::min(width(), height()) - 20.0;
    return QRectF((width() - side) / 2.0, (height() - side) / 2.0, side, side);
}

QRectF AmslerWidget::cellRect(const lab5::Roi &roi) const
{
    const QRectF board = boardRect();
    const lab5::GridConfig &config = m_test.config();
    const double cellWidth = board.width() / config.cols;
    const double cellHeight = board.height() / config.rows;
    return QRectF(board.left() + roi.col * cellWidth, board.top() + roi.row * cellHeight,
                  cellWidth, cellHeight);
}

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

void AmslerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);

    if (m_test.phase() == lab5::Phase::Idle || m_test.finished()) {
        drawGrid(painter);
    } else if (const std::optional<lab5::Stimulus> stimulus = m_test.currentStimulus()) {
        drawStimulus(painter, *stimulus);
    }

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


    painter.setPen(QPen(Qt::red, 10.0, Qt::SolidLine, Qt::RoundCap));
    painter.drawPoint(width() / 2, height() / 2);
}
