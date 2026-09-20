



#ifndef OPENCV_CORE_BINDINGS_UTILS_HPP
#define OPENCV_CORE_BINDINGS_UTILS_HPP

#include <opencv2/core/async.hpp>
#include <opencv2/core/detail/async_promise.hpp>

namespace cv { namespace utils {



CV_EXPORTS_W String dumpInputArray(InputArray argument);

CV_EXPORTS_W String dumpInputArrayOfArrays(InputArrayOfArrays argument);

CV_EXPORTS_W String dumpInputOutputArray(InputOutputArray argument);

CV_EXPORTS_W String dumpInputOutputArrayOfArrays(InputOutputArrayOfArrays argument);

CV_WRAP static inline
AsyncArray testAsyncArray(InputArray argument)
{
    AsyncPromise p;
    p.setValue(argument);
    return p.getArrayResult();
}

CV_WRAP static inline
AsyncArray testAsyncException()
{
    AsyncPromise p;
    try
    {
        CV_Error(Error::StsOk, "Test: Generated async error");
    }
    catch (const cv::Exception& e)
    {
        p.setException(e);
    }
    return p.getArrayResult();
}


}}

#endif
