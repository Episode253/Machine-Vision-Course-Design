#include "amsler_widget.h"

#include <algorithm>
#include <QPainter>
#include <QTimer>

AmslerWidget::AmslerWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(500, 500);
    m_flashTimer = new QTimer(this);
    m_flashTimer->setSingleShot(true);
    m_flashTimer->setInterval(250);
    connect(m_flashTimer, &QTimer::timeout, this, &AmslerWidget::onStimulusTimeout);
}

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

void AmslerWidget::answerNormal()
{
    if (!m_test.running()) return;
    m_test.answer(lab8::Answer::Normal);
    if (m_test.finished()) { const auto result = m_test.result(); emit finished(result.sampledRois, result.distortedRois); }
    else m_flashTimer->start();
    update();
}

void AmslerWidget::answerDistorted()
{
    if (!m_test.running()) return;
    m_test.answer(lab8::Answer::Distorted);
    if (m_test.finished()) { const auto result = m_test.result(); emit finished(result.sampledRois, result.distortedRois); }
    else m_flashTimer->start();
    update();
}

void AmslerWidget::onStimulusTimeout() { m_test.stimulusTimeout(); update(); }

QRectF AmslerWidget::boardRect() const
{
    const double side = std::min(width(), height()) - 24.0;
    return QRectF((width() - side) / 2.0, (height() - side) / 2.0, side, side);
}

QRectF AmslerWidget::cellRect(const lab8::Roi &roi) const
{
    const QRectF board = boardRect();
    const auto &config = m_test.config();
    return QRectF(board.left() + roi.col * board.width() / config.cols,
                  board.top() + roi.row * board.height() / config.rows,
                  board.width() / config.cols, board.height() / config.rows);
}

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

void AmslerWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);
    if (m_test.phase() == lab8::Phase::Idle || m_test.finished())
        drawGrid(painter);
    else if (const auto stimulus = m_test.currentStimulus())
        drawStimulus(painter, *stimulus);
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
