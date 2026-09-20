





#ifndef OPENCV_GAPI_GCOMPUTATION_ASYNC_HPP
#define OPENCV_GAPI_GCOMPUTATION_ASYNC_HPP


#include <future>
#include <exception>
#include <functional>
#include <opencv2/gapi/garg.hpp>
#include <opencv2/gapi/gcommon.hpp>
#include <opencv2/gapi/own/exports.hpp>


namespace cv {

    class GComputation;
namespace gapi {
namespace wip  {
    class GAsyncContext;








    GAPI_EXPORTS void                async_apply(GComputation& gcomp, std::function<void(std::exception_ptr)>&& callback, GRunArgs &&ins, GRunArgsP &&outs, GCompileArgs &&args = {});
    GAPI_EXPORTS void                async_apply(GComputation& gcomp, std::function<void(std::exception_ptr)>&& callback, GRunArgs &&ins, GRunArgsP &&outs, GCompileArgs &&args, GAsyncContext& ctx);

    GAPI_EXPORTS std::future<void>   async_apply(GComputation& gcomp, GRunArgs &&ins, GRunArgsP &&outs, GCompileArgs &&args = {});
    GAPI_EXPORTS std::future<void>   async_apply(GComputation& gcomp, GRunArgs &&ins, GRunArgsP &&outs, GCompileArgs &&args,  GAsyncContext& ctx);
}
}
}


#endif
