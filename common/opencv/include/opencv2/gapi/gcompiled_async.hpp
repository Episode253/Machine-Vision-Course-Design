






#ifndef OPENCV_GAPI_GCOMPILED_ASYNC_HPP
#define OPENCV_GAPI_GCOMPILED_ASYNC_HPP

#include <future>
#include <exception>
#include <functional>
#include <opencv2/gapi/garg.hpp>
#include <opencv2/gapi/own/exports.hpp>

namespace cv {

    class GCompiled;

namespace gapi{
namespace wip {
    class GAsyncContext;








    GAPI_EXPORTS void                async(GCompiled& gcmpld, std::function<void(std::exception_ptr)>&& callback, GRunArgs &&ins, GRunArgsP &&outs);
    GAPI_EXPORTS void                async(GCompiled& gcmpld, std::function<void(std::exception_ptr)>&& callback, GRunArgs &&ins, GRunArgsP &&outs, GAsyncContext& ctx);

    GAPI_EXPORTS std::future<void>   async(GCompiled& gcmpld, GRunArgs &&ins, GRunArgsP &&outs);
    GAPI_EXPORTS std::future<void>   async(GCompiled& gcmpld, GRunArgs &&ins, GRunArgsP &&outs, GAsyncContext& ctx);
}
}
}

#endif
