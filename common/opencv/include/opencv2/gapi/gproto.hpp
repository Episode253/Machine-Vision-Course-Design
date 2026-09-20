






#ifndef OPENCV_GAPI_GPROTO_HPP
#define OPENCV_GAPI_GPROTO_HPP

#include <type_traits>
#include <vector>
#include <ostream>

#include <opencv2/gapi/util/variant.hpp>

#include <opencv2/gapi/gmat.hpp>
#include <opencv2/gapi/gscalar.hpp>
#include <opencv2/gapi/garray.hpp>
#include <opencv2/gapi/garg.hpp>
#include <opencv2/gapi/gmetaarg.hpp>

namespace cv {











using GProtoArg = util::variant
    < GMat
    , GMatP
    , GScalar
    , detail::GArrayU
    >;

using GProtoArgs = std::vector<GProtoArg>;

namespace detail
{
template<typename... Ts> inline GProtoArgs packArgs(Ts... args)
{
    return GProtoArgs{ GProtoArg(wrap_gapi_helper<Ts>::wrap(args))... };
}

}

template<class Tag>
struct GIOProtoArgs
{
public:
    explicit GIOProtoArgs(const GProtoArgs& args) : m_args(args) {}
    explicit GIOProtoArgs(GProtoArgs &&args)      : m_args(std::move(args)) {}

    GProtoArgs m_args;
};

struct In_Tag{};
struct Out_Tag{};

using GProtoInputArgs  = GIOProtoArgs<In_Tag>;
using GProtoOutputArgs = GIOProtoArgs<Out_Tag>;


template<typename... Ts> inline GProtoInputArgs GIn(Ts&&... ts)
{
    return GProtoInputArgs(detail::packArgs(std::forward<Ts>(ts)...));
}

template<typename... Ts> inline GProtoOutputArgs GOut(Ts&&... ts)
{
    return GProtoOutputArgs(detail::packArgs(std::forward<Ts>(ts)...));
}

namespace detail
{


    template<typename... Ts, int... Indexes>
    static GProtoOutputArgs getGOut_impl(const std::tuple<Ts...>& ts, detail::Seq<Indexes...>)
    {
        return GProtoOutputArgs{ detail::packArgs(std::get<Indexes>(ts)...)};
    }
}

template<typename... Ts> inline GProtoOutputArgs GOut(const std::tuple<Ts...>& ts)
{

    return detail::getGOut_impl(ts, typename detail::MkSeq<sizeof...(Ts)>::type());
}


template<typename... Ts> inline GProtoOutputArgs GOut(std::tuple<Ts...>&& ts)
{

    return detail::getGOut_impl(ts, typename detail::MkSeq<sizeof...(Ts)>::type());
}




GRunArg value_of(const GOrigin &origin);



GMetaArg  GAPI_EXPORTS descr_of(const GRunArg  &arg );
GMetaArgs GAPI_EXPORTS descr_of(const GRunArgs &args);



GMetaArg GAPI_EXPORTS descr_of(const GRunArgP& argp);


bool GAPI_EXPORTS can_describe(const GMetaArg&  meta,  const GRunArg&  arg);
bool GAPI_EXPORTS can_describe(const GMetaArgs& metas, const GRunArgs& args);




bool GAPI_EXPORTS can_describe(const GMetaArg&  meta,  const GRunArgP& argp);

}

#endif
