






#ifndef OPENCV_GAPI_FLUID_KERNEL_HPP
#define OPENCV_GAPI_FLUID_KERNEL_HPP

#include <vector>
#include <functional>
#include <map>
#include <unordered_map>

#include <opencv2/gapi/opencv_includes.hpp>
#include <opencv2/gapi/gcommon.hpp>
#include <opencv2/gapi/gkernel.hpp>
#include <opencv2/gapi/garg.hpp>
#include <opencv2/gapi/own/types.hpp>

#include <opencv2/gapi/fluid/gfluidbuffer.hpp>


namespace cv {

namespace gapi
{
namespace fluid
{


    GAPI_EXPORTS cv::gapi::GBackend backend();

}
}


class GAPI_EXPORTS GFluidKernel
{
public:
    enum class Kind
    {
        Filter,
        Resize,
        NV12toRGB
    };


    using F = std::function<void(const cv::GArgs&, const std::vector<gapi::fluid::Buffer*> &)>;


    using IS = std::function<void(const cv::GMetaArgs &, const cv::GArgs&, gapi::fluid::Buffer &)>;


    using RS = std::function<void(gapi::fluid::Buffer &)>;


    using M = std::function<GMetaArgs(const GMetaArgs &, const GArgs &)>;


    using B = std::function<gapi::fluid::BorderOpt(const GMetaArgs&, const GArgs&)>;


    GFluidKernel() {}
    GFluidKernel(int w, Kind k, int l, bool scratch, const F& f, const IS &is, const RS &rs, const B& b)
        : m_window(w)
        , m_kind(k)
        , m_lpi(l)
        , m_scratch(scratch)
        , m_f(f)
        , m_is(is)
        , m_rs(rs)
        , m_b(b) {}

    int m_window = -1;
    Kind m_kind;
    const int  m_lpi     = -1;
    const bool m_scratch = false;

    const F    m_f;
    const IS   m_is;
    const RS   m_rs;
    const B    m_b;
};




struct GFluidOutputRois
{
    std::vector<cv::gapi::own::Rect> rois;
};

struct GFluidParallelOutputRois
{
    std::vector<GFluidOutputRois> parallel_rois;
};

struct GFluidParallelFor
{
    std::function<void(std::size_t, std::function<void(std::size_t)>)> parallel_for;
};

namespace detail
{
template<> struct CompileArgTag<GFluidOutputRois>
{
    static const char* tag() { return "gapi.fluid.outputRois"; }
};

template<> struct CompileArgTag<GFluidParallelFor>
{
    static const char* tag() { return "gapi.fluid.parallelFor"; }
};

template<> struct CompileArgTag<GFluidParallelOutputRois>
{
    static const char* tag() { return "gapi.fluid.parallelOutputRois"; }
};

}

namespace detail
{
template<class T> struct fluid_get_in;
template<> struct fluid_get_in<cv::GMat>
{
    static const cv::gapi::fluid::View& get(const cv::GArgs &in_args, int idx)
    {
        return in_args[idx].unsafe_get<cv::gapi::fluid::View>();
    }
};

template<> struct fluid_get_in<cv::GScalar>
{

#if !defined(GAPI_STANDALONE)
    static const cv::Scalar get(const cv::GArgs &in_args, int idx)
    {
        return cv::gapi::own::to_ocv(in_args[idx].unsafe_get<cv::gapi::own::Scalar>());
    }
#else
    static const cv::gapi::own::Scalar get(const cv::GArgs &in_args, int idx)
    {
        return in_args[idx].get<cv::gapi::own::Scalar>();
    }
#endif
};
template<class T> struct fluid_get_in
{
    static const T& get(const cv::GArgs &in_args, int idx)
    {
        return in_args[idx].unsafe_get<T>();
    }
};

template<bool, typename Impl, typename... Ins>
struct scratch_helper;

template<typename Impl, typename... Ins>
struct scratch_helper<true, Impl, Ins...>
{

    template<int... IIs>
    static void help_init_impl(const cv::GMetaArgs &metas,
                               const cv::GArgs     &in_args,
                               gapi::fluid::Buffer &scratch_buf,
                               detail::Seq<IIs...>)
    {
        Impl::initScratch(get_in_meta<Ins>(metas, in_args, IIs)..., scratch_buf);
    }

    static void help_init(const cv::GMetaArgs &metas,
                          const cv::GArgs     &in_args,
                          gapi::fluid::Buffer &b)
    {
        help_init_impl(metas, in_args, b, typename detail::MkSeq<sizeof...(Ins)>::type());
    }


    static void help_reset(gapi::fluid::Buffer &b)
    {
        Impl::resetScratch(b);
    }
};

template<typename Impl, typename... Ins>
struct scratch_helper<false, Impl, Ins...>
{
    static void help_init(const cv::GMetaArgs &,
                          const cv::GArgs     &,
                          gapi::fluid::Buffer &)
    {
        GAPI_Assert(false);
    }
    static void help_reset(gapi::fluid::Buffer &)
    {
        GAPI_Assert(false);
    }
};

template<typename T> struct is_gmat_type
{
    static const constexpr bool value = std::is_same<cv::GMat, T>::value;
};

template<bool CallCustomGetBorder, typename Impl, typename... Ins>
struct get_border_helper;

template<typename Impl, typename... Ins>
struct get_border_helper<true, Impl, Ins...>
{
    template<int... IIs>
    static gapi::fluid::BorderOpt get_border_impl(const GMetaArgs &metas,
                                                  const cv::GArgs &in_args,
                                                  cv::detail::Seq<IIs...>)
    {
        return util::make_optional(Impl::getBorder(cv::detail::get_in_meta<Ins>(metas, in_args, IIs)...));
    }

    static gapi::fluid::BorderOpt help(const GMetaArgs &metas,
                                       const cv::GArgs &in_args)
    {
        return get_border_impl(metas, in_args, typename detail::MkSeq<sizeof...(Ins)>::type());
    }
};

template<typename Impl, typename... Ins>
struct get_border_helper<false, Impl, Ins...>
{
    static gapi::fluid::BorderOpt help(const cv::GMetaArgs &,
                                       const cv::GArgs     &)
    {
        return {};
    }
};

template<typename, typename, typename, bool UseScratch>
struct FluidCallHelper;

template<typename Impl, typename... Ins, typename... Outs, bool UseScratch>
struct FluidCallHelper<Impl, std::tuple<Ins...>, std::tuple<Outs...>, UseScratch>
{
    static_assert(all_satisfy<is_gmat_type, Outs...>::value, "return type must be GMat");


    template<int... IIs, int... OIs>
    static void call_impl(const cv::GArgs &in_args,
                          const std::vector<gapi::fluid::Buffer*> &out_bufs,
                          detail::Seq<IIs...>,
                          detail::Seq<OIs...>)
    {
        Impl::run(fluid_get_in<Ins>::get(in_args, IIs)..., *out_bufs[OIs]...);
    }

    static void call(const cv::GArgs &in_args,
                     const std::vector<gapi::fluid::Buffer*> &out_bufs)
    {
        constexpr int numOuts = (sizeof...(Outs)) + (UseScratch ? 1 : 0);
        call_impl(in_args, out_bufs,
                  typename detail::MkSeq<sizeof...(Ins)>::type(),
                  typename detail::MkSeq<numOuts>::type());
    }


    static void init_scratch(const GMetaArgs &metas,
                             const cv::GArgs &in_args,
                             gapi::fluid::Buffer &b)
    {
        scratch_helper<UseScratch, Impl, Ins...>::help_init(metas, in_args, b);
    }


    static void reset_scratch(gapi::fluid::Buffer &scratch_buf)
    {
        scratch_helper<UseScratch, Impl, Ins...>::help_reset(scratch_buf);
    }

    static gapi::fluid::BorderOpt getBorder(const GMetaArgs &metas, const cv::GArgs &in_args)
    {


        constexpr bool callCustomGetBorder = (Impl::Window != 1);
        return get_border_helper<callCustomGetBorder, Impl, Ins...>::help(metas, in_args);
    }
};
}


template<class Impl, class K, bool UseScratch>
class GFluidKernelImpl : public cv::detail::KernelTag
{
    static const int LPI = 1;
    static const auto Kind = GFluidKernel::Kind::Filter;
    using P = detail::FluidCallHelper<Impl, typename K::InArgs, typename K::OutArgs, UseScratch>;

public:
    using API = K;

    static GFluidKernel kernel()
    {


        return GFluidKernel(Impl::Window, Impl::Kind, Impl::LPI,
                            UseScratch,
                            &P::call, &P::init_scratch, &P::reset_scratch, &P::getBorder);
    }

    static cv::gapi::GBackend backend() { return cv::gapi::fluid::backend(); }
};

#define GAPI_FLUID_KERNEL(Name, API, Scratch) struct Name: public cv::GFluidKernelImpl<Name, API, Scratch>

}

#endif
