#ifndef LAB2_CAPTURE_SOURCE_H
#define LAB2_CAPTURE_SOURCE_H

#include <atomic>
#include <cstdint>

#include <QString>

#include "core/stage.hpp"
#include "runtime/frame_source.h"



class CaptureSource : public labcore::Source {
public:
    CaptureSource() = default;
    explicit CaptureSource(QString imagePath);

    std::string_view name() const override;
    labcore::PixelFormat outputFormat() const override { return labcore::PixelFormat::BGR8; }

    bool open(std::string *error) override;
    void close() override;




    labcore::FramePtr next() override;

private:
    QString m_imagePath;
    FrameSource m_source;
    std::atomic_bool m_stopped{false};
};

#endif
