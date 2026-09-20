#include "latest_image_slot.h"

#include <QMutexLocker>

namespace labqt {

void LatestImageSlot::set(const QImage &image)
{
    // QMutexLocker 是 Qt 的作用域锁；无论从此处如何返回都会自动解锁。
    QMutexLocker locker(&m_mutex);
    m_image = image;
    m_hasNew = !image.isNull();
}

std::optional<QImage> LatestImageSlot::takeLatest()
{
    QMutexLocker locker(&m_mutex);
    if (!m_hasNew)
        return std::nullopt;
    // 只保留最新一帧：界面较慢时主动丢弃旧帧，以换取更低的预览延迟。
    m_hasNew = false;
    return m_image;
}

void LatestImageSlot::clear()
{
    QMutexLocker locker(&m_mutex);
    m_image = QImage();
    m_hasNew = false;
}

}
