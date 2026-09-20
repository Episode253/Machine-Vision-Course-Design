#ifndef VISUAL_LAB_CORE_STAGE_H
#define VISUAL_LAB_CORE_STAGE_H

#include <functional>
#include <memory>
#include <string>
#include <string_view>

#include "frame.hpp"

namespace labcore {



struct StageParams {
    virtual ~StageParams() = default;
};



class Source {
public:
    virtual ~Source() = default;
    virtual std::string_view name() const = 0;
    virtual PixelFormat outputFormat() const = 0;
    virtual bool open(std::string *error) = 0;
    virtual void close() = 0;
    virtual FramePtr next() = 0;
};




class Stage {
public:


    enum class Execution { OwnThread, Inline };

    virtual ~Stage() = default;
    virtual std::string_view name() const = 0;
    virtual PixelFormat inputFormat() const = 0;
    virtual PixelFormat outputFormat() const = 0;
    virtual FramePtr process(const FramePtr &input) = 0;
    virtual Execution execution() const { return Execution::OwnThread; }
    virtual void setParams(std::shared_ptr<const StageParams> params) { (void)params; }
    virtual void onStop() {}
};



using OutputFn = std::function<void(FramePtr)>;

}

#endif
