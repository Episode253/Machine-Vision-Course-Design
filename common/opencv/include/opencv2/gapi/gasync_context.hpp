





#ifndef OPENCV_GAPI_GASYNC_CONTEXT_HPP
#define OPENCV_GAPI_GASYNC_CONTEXT_HPP

#if !defined(GAPI_STANDALONE)
#  include <opencv2/core/cvdef.h>
#else
#  include <opencv2/gapi/own/cvdefs.hpp>
#endif

#include <opencv2/gapi/own/exports.hpp>

namespace cv {
namespace gapi{
namespace wip {

class GAPI_EXPORTS GAsyncContext{
    std::atomic<bool> cancelation_requested = {false};
public:

    bool cancel();
    bool isCanceled() const;
};

class GAPI_EXPORTS GAsyncCanceled : public std::exception {
public:
    virtual const char* what() const noexcept CV_OVERRIDE;
};
}
}
}

#endif
