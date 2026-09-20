






#ifndef OPENCV_GAPI_GCPUKERNEL_HPP
#define OPENCV_GAPI_GCPUKERNEL_HPP

#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

#include <opencv2/core/mat.hpp>
#include <opencv2/gapi/gcommon.hpp>
#include <opencv2/gapi/gkernel.hpp>
#include <opencv2/gapi/garg.hpp>
#include <opencv2/gapi/own/convert.hpp>
#include <opencv2/gapi/util/compiler_hints.hpp>
#include <opencv2/gapi/util/util.hpp>


namespace cv {

namespace gimpl
{

    class GCPUExecutable;
}

namespace gapi
{
namespace cpu
{



    GAPI_EXPORTS cv::gapi::GBackend backend();

}
}



class GAPI_EXPORTS GCPUContext
{
public:

    template<typename T>
    const T& inArg(int input) { return m_args.at(input).get<T>(); }


    const cv::gapi::own::Mat&   inMat(int input);
    cv::gapi::own::Mat&         outMatR(int output);

    const cv::gapi::own::Scalar& inVal(int input);
    cv::gapi::own::Scalar& outValR(int output);
    template<typename T> std::vector<T>& outVecR(int output)
    {
        return outVecRef(output).wref<T>();
    }

protected:
    detail::VectorRef& outVecRef(int output);

    std::vector<GArg> m_args;




    std::unordered_map<std::size_t, GRunArgP> m_results;

    friend class gimpl::GCPUExecutable;
};

class GAPI_EXPORTS GCPUKernel
{
public:

    using F = std::function<void(GCPUContext &)>;

    GCPUKernel();
    explicit GCPUKernel(const F& f);

    void apply(GCPUContext &ctx);

protected:
    F m_f;
};



namespace detail
{
template<class T> struct get_in;
template<> struct get_in<cv::GMat>
{
    static cv::Mat    get(GCPUContext &ctx, int idx) { return to_ocv(ctx.inMat(idx)); }
};
template<> struct get_in<cv::GMatP>
{
    static cv::Mat    get(GCPUContext &ctx, int idx) { return get_in<cv::GMat>::get(ctx, idx); }
};
template<> struct get_in<cv::GScalar>
{
    static cv::Scalar get(GCPUContext &ctx, int idx) { return to_ocv(ctx.inVal(idx)); }
};
template<typename U> struct get_in<cv::GArray<U> >
{
    static const std::vector<U>& get(GCPUContext &ctx, int idx) { return ctx.inArg<VectorRef>(idx).rref<U>(); }
};
template<class T> struct get_in
{
    static T get(GCPUContext &ctx, int idx) { return ctx.inArg<T>(idx); }
};

struct tracked_cv_mat{
    tracked_cv_mat(cv::gapi::own::Mat& m) : r{to_ocv(m)}, original_data{m.data} {}
    cv::Mat r;
    uchar* original_data;

    operator cv::Mat& (){ return r;}
    void validate() const{
        if (r.data != original_data)
        {
            util::throw_error
                (std::logic_error
                 ("OpenCV kernel output parameter was reallocated. \n"
                  "Incorrect meta data was provided ?"));
        }
    }
};

struct scalar_wrapper
{
    scalar_wrapper(cv::gapi::own::Scalar& s) : m_s{cv::gapi::own::to_ocv(s)}, m_org_s(s) {};
    operator cv::Scalar& () { return m_s; }
    void writeBack() const  { m_org_s = to_own(m_s); }

    cv::Scalar m_s;
    cv::gapi::own::Scalar& m_org_s;
};

template<typename... Outputs>
void postprocess(Outputs&... outs)
{
    struct
    {
        void operator()(tracked_cv_mat* bm) { bm->validate();  }
        void operator()(scalar_wrapper* sw) { sw->writeBack(); }
        void operator()(...)                {                  }

    } validate;

    int dummy[] = { 0, (validate(&outs), 0)... };
    cv::util::suppress_unused_warning(dummy);
}

template<class T> struct get_out;
template<> struct get_out<cv::GMat>
{
    static tracked_cv_mat get(GCPUContext &ctx, int idx)
    {
        auto& r = ctx.outMatR(idx);
        return {r};
    }
};
template<> struct get_out<cv::GMatP>
{
    static tracked_cv_mat get(GCPUContext &ctx, int idx)
    {
        return get_out<cv::GMat>::get(ctx, idx);
    }
};
template<> struct get_out<cv::GScalar>
{
    static scalar_wrapper get(GCPUContext &ctx, int idx)
    {
        auto& s = ctx.outValR(idx);
        return {s};
    }
};
template<typename U> struct get_out<cv::GArray<U>>
{
    static std::vector<U>& get(GCPUContext &ctx, int idx)
    {
        return ctx.outVecR<U>(idx);
    }
};

template<typename, typename, typename>
struct OCVCallHelper;


template<typename Impl, typename... Ins, typename... Outs>
struct OCVCallHelper<Impl, std::tuple<Ins...>, std::tuple<Outs...> >
{
    template<typename... Inputs>
    struct call_and_postprocess
    {
        template<typename... Outputs>
        static void call(Inputs&&... ins, Outputs&&... outs)
        {


            Impl::run(std::forward<Inputs>(ins)..., outs...);

            postprocess(outs...);
        }
    };

    template<int... IIs, int... OIs>
    static void call_impl(GCPUContext &ctx, detail::Seq<IIs...>, detail::Seq<OIs...>)
    {






        call_and_postprocess<decltype(get_in<Ins>::get(ctx, IIs))...>::call(get_in<Ins>::get(ctx, IIs)..., get_out<Outs>::get(ctx, OIs)...);
    }

    static void call(GCPUContext &ctx)
    {
        call_impl(ctx,
                  typename detail::MkSeq<sizeof...(Ins)>::type(),
                  typename detail::MkSeq<sizeof...(Outs)>::type());
    }
};

}

template<class Impl, class K>
class GCPUKernelImpl: public cv::detail::OCVCallHelper<Impl, typename K::InArgs, typename K::OutArgs>,
                      public cv::detail::KernelTag
{
    using P = detail::OCVCallHelper<Impl, typename K::InArgs, typename K::OutArgs>;

public:
    using API = K;

    static cv::gapi::GBackend backend()  { return cv::gapi::cpu::backend(); }
    static cv::GCPUKernel     kernel()   { return GCPUKernel(&P::call);     }
};

#define GAPI_OCV_KERNEL(Name, API) struct Name: public cv::GCPUKernelImpl<Name, API>

}

#endif
