#ifndef VISUAL_LAB_QT_LATEST_IMAGE_SLOT_H
#define VISUAL_LAB_QT_LATEST_IMAGE_SLOT_H

#include <optional>

#include <QImage>
#include <QMutex>

namespace labqt {




class LatestImageSlot {
public:
    void set(const QImage &image);

    std::optional<QImage> takeLatest();
    void clear();

private:
    mutable QMutex m_mutex;
    QImage m_image;
    bool m_hasNew = false;
};

}

#endif
