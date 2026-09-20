






#ifndef OPENCV_GAPI_GARRAY_HPP
#define OPENCV_GAPI_GARRAY_HPP

#include <functional>
#include <ostream>
#include <vector>
#include <memory>

#include <opencv2/gapi/own/exports.hpp>
#include <opencv2/gapi/opencv_includes.hpp>

#include <opencv2/gapi/util/variant.hpp>
#include <opencv2/gapi/util/throw.hpp>
#include <opencv2/gapi/own/assert.hpp>

namespace cv
{


class GNode;
struct GOrigin;

template<typename T> class GArray;


struct GArrayDesc
{


    bool operator== (const GArrayDesc&) const { return true; }
};
template<typename U> GArrayDesc descr_of(const std::vector<U> &) { return {};}
static inline GArrayDesc empty_array_desc() {return {}; }


std::ostream& operator<<(std::ostream& os, const cv::GArrayDesc &desc);

namespace detail
{





    class VectorRef;
    using ConstructVec = std::function<void(VectorRef&)>;


    struct TypeHintBase{virtual ~TypeHintBase() = default;};


    template <typename T>
    struct TypeHint final : public TypeHintBase{};




    class GAPI_EXPORTS GArrayU
    {
    public:
        GArrayU(const GNode &n, std::size_t out);

        template <typename T>
        bool holds() const;

        GOrigin& priv();
        const GOrigin& priv() const;

    protected:
        GArrayU();
        template<class> friend class cv::GArray;

        void setConstructFcn(ConstructVec &&cv);

        template <typename T>
        void specifyType();

        std::shared_ptr<GOrigin> m_priv;
        std::shared_ptr<TypeHintBase> m_hint;
    };

    template <typename T>
    bool GArrayU::holds() const{
        GAPI_Assert(m_hint != nullptr);
        using U = typename std::decay<T>::type;
        return dynamic_cast<TypeHint<U>*>(m_hint.get()) != nullptr;
    };

    template <typename T>
    void GArrayU::specifyType(){
        m_hint.reset(new TypeHint<typename std::decay<T>::type>);
    };





    class BasicVectorRef
    {
    public:
        std::size_t    m_elemSize = 0ul;
        cv::GArrayDesc m_desc;
        virtual ~BasicVectorRef() {}
    };

    template<typename T> class VectorRefT: public BasicVectorRef
    {
        using empty_t  = util::monostate;
        using ro_ext_t = const std::vector<T> *;
        using rw_ext_t =       std::vector<T> *;
        using rw_own_t =       std::vector<T>  ;
        util::variant<empty_t, ro_ext_t, rw_ext_t, rw_own_t> m_ref;

        inline bool isEmpty() const { return util::holds_alternative<empty_t>(m_ref);  }
        inline bool isROExt() const { return util::holds_alternative<ro_ext_t>(m_ref); }
        inline bool isRWExt() const { return util::holds_alternative<rw_ext_t>(m_ref); }
        inline bool isRWOwn() const { return util::holds_alternative<rw_own_t>(m_ref); }

        void init(const std::vector<T>* vec = nullptr)
        {
            m_elemSize = sizeof(T);
            if (vec) m_desc = cv::descr_of(*vec);
        }

    public:
        VectorRefT() { init(); }
        virtual ~VectorRefT() {}

        explicit VectorRefT(const std::vector<T>& vec) : m_ref(&vec)      { init(&vec); }
        explicit VectorRefT(std::vector<T>& vec)  : m_ref(&vec)           { init(&vec); }
        explicit VectorRefT(std::vector<T>&& vec) : m_ref(std::move(vec)) { init(&vec); }







        void reset()
        {
            if (isEmpty())
            {
                std::vector<T> empty_vector;
                m_desc = cv::descr_of(empty_vector);
                m_ref  = std::move(empty_vector);
                GAPI_Assert(isRWOwn());
            }
            else if (isRWOwn())
            {
                util::get<rw_own_t>(m_ref).clear();
            }
            else GAPI_Assert(false);
        }




        std::vector<T>& wref()
        {
            GAPI_Assert(isRWExt() || isRWOwn());
            if (isRWExt()) return *util::get<rw_ext_t>(m_ref);
            if (isRWOwn()) return  util::get<rw_own_t>(m_ref);
            util::throw_error(std::logic_error("Impossible happened"));
        }




        const std::vector<T>& rref() const
        {














            if (isROExt()) return *util::get<ro_ext_t>(m_ref);
            if (isRWExt()) return *util::get<rw_ext_t>(m_ref);
            if (isRWOwn()) return  util::get<rw_own_t>(m_ref);
            util::throw_error(std::logic_error("Impossible happened"));
        }
    };








    class VectorRef
    {
        std::shared_ptr<BasicVectorRef> m_ref;

        template<typename T> inline void check() const
        {
            GAPI_DbgAssert(dynamic_cast<VectorRefT<T>*>(m_ref.get()) != nullptr);
            GAPI_Assert(sizeof(T) == m_ref->m_elemSize);
        }

    public:
        VectorRef() = default;
        template<typename T> explicit VectorRef(const std::vector<T>& vec) : m_ref(new VectorRefT<T>(vec)) {}
        template<typename T> explicit VectorRef(std::vector<T>& vec)       : m_ref(new VectorRefT<T>(vec)) {}
        template<typename T> explicit VectorRef(std::vector<T>&& vec)      : m_ref(new VectorRefT<T>(vec)) {}

        template<typename T> void reset()
        {
            if (!m_ref) m_ref.reset(new VectorRefT<T>());

            check<T>();
            static_cast<VectorRefT<T>&>(*m_ref).reset();
        }

        template<typename T> std::vector<T>& wref()
        {
            check<T>();
            return static_cast<VectorRefT<T>&>(*m_ref).wref();
        }

        template<typename T> const std::vector<T>& rref() const
        {
            check<T>();
            return static_cast<VectorRefT<T>&>(*m_ref).rref();
        }

        cv::GArrayDesc descr_of() const
        {
            return m_ref->m_desc;
        }
    };
}



template<typename T> class GArray
{
public:
    GArray() { putDetails(); }
    explicit GArray(detail::GArrayU &&ref)
        : m_ref(ref) { putDetails(); }

    detail::GArrayU strip() const { return m_ref; }

private:
    static void VCTor(detail::VectorRef& vref) { vref.reset<T>(); }
    void putDetails() {
        m_ref.setConstructFcn(&VCTor);
        m_ref.specifyType<T>();
    }

    detail::GArrayU m_ref;
};



}

#endif
