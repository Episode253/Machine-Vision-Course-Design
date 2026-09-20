






#ifndef OPENCV_GAPI_GCOMPUTATION_HPP
#define OPENCV_GAPI_GCOMPUTATION_HPP

#include <functional>

#include <opencv2/gapi/util/util.hpp>
#include <opencv2/gapi/gcommon.hpp>
#include <opencv2/gapi/gproto.hpp>
#include <opencv2/gapi/garg.hpp>
#include <opencv2/gapi/gcompiled.hpp>

namespace cv {

namespace detail
{


    template<typename...>
    struct last_type;

    template<typename T>
    struct last_type<T> { using type = T;};

    template<typename T, typename... Ts>
    struct last_type<T, Ts...> { using type = typename last_type<Ts...>::type; };

    template<typename... Ts>
    using last_type_t = typename last_type<Ts...>::type;
}



class GAPI_EXPORTS GComputation
{
public:
    class Priv;
    typedef std::function<GComputation()> Generator;




    GComputation(const Generator& gen);



    GComputation(GProtoInputArgs &&ins,
                 GProtoOutputArgs &&outs);



    GComputation(GMat in, GMat out);


    GComputation(GMat in, GScalar out);


    GComputation(GMat in1, GMat in2, GMat out);


    GComputation(GMat in1, GMat in2, GScalar out);




    GComputation(const std::vector<GMat> &ins,
                 const std::vector<GMat> &outs);




    void apply(GRunArgs &&ins, GRunArgsP &&outs, GCompileArgs &&args = {});


    void apply(const std::vector<cv::gapi::own::Mat>& ins,
               const std::vector<cv::gapi::own::Mat>& outs,
               GCompileArgs &&args = {});


#if !defined(GAPI_STANDALONE)

    void apply(cv::Mat in, cv::Mat &out, GCompileArgs &&args = {});


    void apply(cv::Mat in, cv::Scalar &out, GCompileArgs &&args = {});


    void apply(cv::Mat in1, cv::Mat in2, cv::Mat &out, GCompileArgs &&args = {});


    void apply(cv::Mat in1, cv::Mat in2, cv::Scalar &out, GCompileArgs &&args = {});


    void apply(const std::vector<cv::Mat>& ins,
                     std::vector<cv::Mat>& outs,
               GCompileArgs &&args = {});
#endif



    GCompiled compile(GMetaArgs &&in_metas, GCompileArgs &&args = {});




    template<typename... Ts>
    auto compile(const Ts&... metas) ->
        typename std::enable_if<detail::are_meta_descrs<Ts...>::value, GCompiled>::type
    {
        return compile(GMetaArgs{GMetaArg(metas)...}, GCompileArgs());
    }













    template<typename... Ts>
    auto compile(const Ts&... meta_and_compile_args) ->
        typename std::enable_if<detail::are_meta_descrs_but_last<Ts...>::value
                                && std::is_same<GCompileArgs, detail::last_type_t<Ts...> >::value,
                                GCompiled>::type
    {

        return compile(std::make_tuple(meta_and_compile_args...),
                       typename detail::MkSeq<sizeof...(Ts)-1>::type());
    }



    Priv& priv();

    const Priv& priv() const;

protected:



    template<typename... Ts, int... IIs>
    GCompiled compile(const std::tuple<Ts...> &meta_and_compile_args, detail::Seq<IIs...>)
    {
        GMetaArgs meta_args = {GMetaArg(std::get<IIs>(meta_and_compile_args))...};
        GCompileArgs comp_args = std::get<sizeof...(Ts)-1>(meta_and_compile_args);
        return compile(std::move(meta_args), std::move(comp_args));
    }

    std::shared_ptr<Priv> m_priv;
};


namespace gapi
{



    void GAPI_EXPORTS island(const std::string &name,
                             GProtoInputArgs  &&ins,
                             GProtoOutputArgs &&outs);
}

}
#endif
