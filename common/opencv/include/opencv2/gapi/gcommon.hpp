






#ifndef OPENCV_GAPI_GCOMMON_HPP
#define OPENCV_GAPI_GCOMMON_HPP

#include <functional>
#include <vector>
#include <type_traits>

#include <opencv2/gapi/opencv_includes.hpp>

#include <opencv2/gapi/util/any.hpp>
#include <opencv2/gapi/own/exports.hpp>
#include <opencv2/gapi/own/assert.hpp>

namespace cv {

namespace detail
{


    template<typename T> struct CompileArgTag;
    template<typename T> struct CompileArgTag
    {
        static const char* tag() { return ""; };
    };


    struct KernelTag
    {};
    struct TransformTag
    {};
}







enum class GShape: int
{
    GMAT,
    GSCALAR,
    GARRAY,
};

struct GCompileArg;

namespace detail {
    template<typename T>
    using is_compile_arg = std::is_same<GCompileArg, typename std::decay<T>::type>;
}






struct GAPI_EXPORTS GCompileArg
{
public:
    std::string tag;


    template<typename T, typename std::enable_if<!detail::is_compile_arg<T>::value, int>::type = 0>
    explicit GCompileArg(T &&t)
        : tag(detail::CompileArgTag<typename std::decay<T>::type>::tag())
        , arg(t)
    {
    }

    template<typename T> T& get()
    {
        return util::any_cast<T>(arg);
    }

    template<typename T> const T& get() const
    {
        return util::any_cast<T>(arg);
    }

private:
    util::any arg;
};

using GCompileArgs = std::vector<GCompileArg>;


template<typename... Ts> GCompileArgs compile_args(Ts&&... args)
{
    return GCompileArgs{ GCompileArg(args)... };
}


struct graph_dump_path
{
    std::string m_dump_path;
};


namespace detail
{
    template<> struct CompileArgTag<cv::graph_dump_path>
    {
        static const char* tag() { return "gapi.graph_dump_path"; }
    };
}

}


namespace std
{
template<> struct hash<cv::GShape>
{
    size_t operator() (cv::GShape sh) const
    {
        return std::hash<int>()(static_cast<int>(sh));
    }
};
}


#endif
