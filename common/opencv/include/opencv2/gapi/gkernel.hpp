






#ifndef OPENCV_GAPI_GKERNEL_HPP
#define OPENCV_GAPI_GKERNEL_HPP

#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include <opencv2/gapi/gcommon.hpp>
#include <opencv2/gapi/util/util.hpp>
#include <opencv2/gapi/gcall.hpp>
#include <opencv2/gapi/garg.hpp>
#include <opencv2/gapi/gmetaarg.hpp>
#include <opencv2/gapi/gtype_traits.hpp>
#include <opencv2/gapi/util/compiler_hints.hpp>
#include <opencv2/gapi/gtransform.hpp>

namespace cv {

using GShapes = std::vector<GShape>;




struct GAPI_EXPORTS GKernel
{
    using M = std::function<GMetaArgs(const GMetaArgs &, const GArgs &)>;

    const std::string name;
    const M           outMeta;
    const GShapes     outShapes;
};


struct GAPI_EXPORTS GKernelImpl
{
    util::any         opaque;
};

template<typename, typename> class GKernelTypeM;

namespace detail
{




    namespace
    {
        template<typename T> struct Yield;
        template<> struct Yield<cv::GMat>
        {
            static inline cv::GMat yield(cv::GCall &call, int i) { return call.yield(i); }
        };
        template<> struct Yield<cv::GMatP>
        {
            static inline cv::GMatP yield(cv::GCall &call, int i) { return call.yieldP(i); }
        };
        template<> struct Yield<cv::GScalar>
        {
            static inline cv::GScalar yield(cv::GCall &call, int i) { return call.yieldScalar(i); }
        };
        template<typename U> struct Yield<cv::GArray<U> >
        {
            static inline cv::GArray<U> yield(cv::GCall &call, int i) { return call.yieldArray<U>(i); }
        };
    }








    template<typename T> struct MetaType;
    template<> struct MetaType<cv::GMat>    { using type = GMatDesc; };
    template<> struct MetaType<cv::GMatP>   { using type = GMatDesc; };
    template<> struct MetaType<cv::GScalar> { using type = GScalarDesc; };
    template<typename U> struct MetaType<cv::GArray<U> > { using type = GArrayDesc; };
    template<typename T> struct MetaType    { using type = T; };


    template<typename T> using is_nongapi_type = std::is_same<T, typename MetaType<T>::type>;


    template<typename T>
    typename std::enable_if<!is_nongapi_type<T>::value, typename MetaType<T>::type>
    ::type get_in_meta(const GMetaArgs &in_meta, const GArgs &, int idx)
    {
        return util::get<typename MetaType<T>::type>(in_meta.at(idx));
    }

    template<typename T>
    typename std::enable_if<is_nongapi_type<T>::value, T>
    ::type get_in_meta(const GMetaArgs &, const GArgs &in_args, int idx)
    {
        return in_args.at(idx).template get<T>();
    }





    template<typename, typename, typename>
    struct MetaHelper;

    template<typename K, typename... Ins, typename... Outs>
    struct MetaHelper<K, std::tuple<Ins...>, std::tuple<Outs...> >
    {
        template<int... IIs, int... OIs>
        static GMetaArgs getOutMeta_impl(const GMetaArgs &in_meta,
                                         const GArgs &in_args,
                                         detail::Seq<IIs...>,
                                         detail::Seq<OIs...>)
        {

            using R   = std::tuple<typename MetaType<Outs>::type...>;
            const R r = K::outMeta( get_in_meta<Ins>(in_meta, in_args, IIs)... );
            return GMetaArgs{ GMetaArg(std::get<OIs>(r))... };
        }


        static GMetaArgs getOutMeta(const GMetaArgs &in_meta,
                                    const GArgs &in_args)
        {
            return getOutMeta_impl(in_meta,
                                   in_args,
                                   typename detail::MkSeq<sizeof...(Ins)>::type(),
                                   typename detail::MkSeq<sizeof...(Outs)>::type());
        }
    };



    template<typename K, typename... Ins, typename Out>
    struct MetaHelper<K, std::tuple<Ins...>, Out >
    {
        template<int... IIs>
        static GMetaArgs getOutMeta_impl(const GMetaArgs &in_meta,
                                         const GArgs &in_args,
                                         detail::Seq<IIs...>)
        {

            using R = typename MetaType<Out>::type;
            const R r = K::outMeta( get_in_meta<Ins>(in_meta, in_args, IIs)... );
            return GMetaArgs{ GMetaArg(r) };
        }


        static GMetaArgs getOutMeta(const GMetaArgs &in_meta,
                                    const GArgs &in_args)
        {
            return getOutMeta_impl(in_meta,
                                   in_args,
                                   typename detail::MkSeq<sizeof...(Ins)>::type());
        }
    };

}







template<typename K, typename... R, typename... Args>
class GKernelTypeM<K, std::function<std::tuple<R...>(Args...)> >:
        public detail::MetaHelper<K, std::tuple<Args...>, std::tuple<R...>>
{
    template<int... IIs>
    static std::tuple<R...> yield(cv::GCall &call, detail::Seq<IIs...>)
    {
        return std::make_tuple(detail::Yield<R>::yield(call, IIs)...);
    }

public:
    using InArgs  = std::tuple<Args...>;
    using OutArgs = std::tuple<R...>;

    static std::tuple<R...> on(Args... args)
    {
        cv::GCall call(GKernel{K::id(), &K::getOutMeta, {detail::GTypeTraits<R>::shape...}});
        call.pass(args...);
        return yield(call, typename detail::MkSeq<sizeof...(R)>::type());
    }
};

template<typename, typename> class GKernelType;

template<typename K, typename R, typename... Args>
class GKernelType<K, std::function<R(Args...)> >:
        public detail::MetaHelper<K, std::tuple<Args...>, R>
{
public:
    using InArgs  = std::tuple<Args...>;
    using OutArgs = std::tuple<R>;

    static R on(Args... args)
    {
        cv::GCall call(GKernel{K::id(), &K::getOutMeta, {detail::GTypeTraits<R>::shape}});
        call.pass(args...);
        return detail::Yield<R>::yield(call, 0);
    }
};

}






#define G_ID_HELPER_CLASS(Class)  Class##IdHelper

#define G_ID_HELPER_BODY(Class, Id)                                         \
    namespace detail                                                        \
    {                                                                       \
        struct G_ID_HELPER_CLASS(Class)                                     \
        {                                                                   \
            static constexpr const char * id() {return Id;};                \
        };                                                                  \
    }

#define G_TYPED_KERNEL(Class, API, Id)                                      \
    G_ID_HELPER_BODY(Class, Id)                                             \
    struct Class final: public cv::GKernelType<Class, std::function API >,  \
                        public detail::G_ID_HELPER_CLASS(Class)


#define G_TYPED_KERNEL_M(Class, API, Id)                                    \
    G_ID_HELPER_BODY(Class, Id)                                             \
    struct Class final: public cv::GKernelTypeM<Class, std::function API >, \
                        public detail::G_ID_HELPER_CLASS(Class)


namespace cv
{
namespace gapi
{


    class GAPI_EXPORTS GBackend
    {
    public:
        class Priv;


        GBackend();
        explicit GBackend(std::shared_ptr<Priv> &&p);

        Priv& priv();
        const Priv& priv() const;
        std::size_t hash() const;

        bool operator== (const GBackend &rhs) const;

    private:
        std::shared_ptr<Priv> m_priv;
    };

    inline bool operator != (const GBackend &lhs, const GBackend &rhs)
    {
        return !(lhs == rhs);
    }
}
}

namespace std
{
    template<> struct hash<cv::gapi::GBackend>
    {
        std::size_t operator() (const cv::gapi::GBackend &b) const
        {
            return b.hash();
        }
    };
}


namespace cv {
namespace gapi {




    class GAPI_EXPORTS GKernelPackage
    {


        using M = std::unordered_map<std::string, std::pair<GBackend, GKernelImpl>>;


        M m_id_kernels;


        std::vector<GTransform> m_transformations;

    protected:



        bool includesAPI(const std::string &id) const;



        void removeAPI(const std::string &id);



        template <typename KImpl>
        typename std::enable_if<(std::is_base_of<detail::KernelTag, KImpl>::value), void>::type
        includeHelper()
        {
            auto backend     = KImpl::backend();
            auto kernel_id   = KImpl::API::id();
            auto kernel_impl = GKernelImpl{KImpl::kernel()};
            removeAPI(kernel_id);

            m_id_kernels[kernel_id] = std::make_pair(backend, kernel_impl);
        }



        template <typename TImpl>
        typename std::enable_if<(std::is_base_of<detail::TransformTag, TImpl>::value), void>::type
        includeHelper()
        {
            m_transformations.emplace_back(TImpl::transformation());
        }

    public:

        std::size_t size() const;


        const std::vector<GTransform>& get_transformations() const;


        template<typename KImpl>
        bool includes() const
        {
            static_assert(std::is_base_of<detail::KernelTag, KImpl>::value,
                          "includes() can be applied to kernels only");

            auto kernel_it = m_id_kernels.find(KImpl::API::id());
            return kernel_it != m_id_kernels.end() &&
                   kernel_it->second.first == KImpl::backend();
        }


        void remove(const GBackend& backend);


        template<typename KAPI>
        void remove()
        {
            removeAPI(KAPI::id());
        }




        template<typename KAPI>
        bool includesAPI() const
        {
            return includesAPI(KAPI::id());
        }


        template<typename KAPI>
        GBackend lookup() const
        {
            return lookup(KAPI::id()).first;
        }


        std::pair<cv::gapi::GBackend, cv::GKernelImpl>
        lookup(const std::string &id) const;



        template<typename KImpl>
        void include()
        {
            includeHelper<KImpl>();
        }


        std::vector<GBackend> backends() const;




        friend GAPI_EXPORTS GKernelPackage combine(const GKernelPackage  &lhs,
                                                   const GKernelPackage  &rhs);
    };


    template<typename... KK> GKernelPackage kernels()
    {


        static_assert(detail::all_unique<typename KK::API...>::value, "Kernels API must be unique");

        GKernelPackage pkg;







        int unused[] = { 0, (pkg.include<KK>(), 0)... };
        cv::util::suppress_unused_warning(unused);
        return pkg;
    };



    GAPI_EXPORTS GKernelPackage combine(const GKernelPackage  &lhs,
                                        const GKernelPackage  &rhs);

    struct GAPI_EXPORTS use_only
    {
        GKernelPackage pkg;
    };

}

namespace detail
{
    template<> struct CompileArgTag<cv::gapi::GKernelPackage>
    {
        static const char* tag() { return "gapi.kernel_package"; }
    };

    template<> struct CompileArgTag<cv::gapi::use_only>
    {
        static const char* tag() { return "gapi.use_only"; }
    };
}
}

#endif
