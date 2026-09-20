






#ifndef OPENCV_GAPI_FLUID_BUFFER_HPP
#define OPENCV_GAPI_FLUID_BUFFER_HPP

#include <list>
#include <numeric>
#include <ostream>
#include <cstdint>

#include <opencv2/gapi/opencv_includes.hpp>
#include <opencv2/gapi/own/mat.hpp>
#include <opencv2/gapi/gmat.hpp>

#include <opencv2/gapi/util/optional.hpp>
#include <opencv2/gapi/own/scalar.hpp>
#include <opencv2/gapi/own/mat.hpp>

namespace cv {
namespace gapi {
namespace fluid {

struct Border
{
#if !defined(GAPI_STANDALONE)

    Border(int _type, cv::Scalar _val) : type(_type), value(to_own(_val)) {};
#endif
    Border(int _type, cv::gapi::own::Scalar _val) : type(_type), value(_val) {};
    int type;
    cv::gapi::own::Scalar value;
};

using BorderOpt = util::optional<Border>;

bool operator == (const Border& b1, const Border& b2);

class GAPI_EXPORTS Buffer;

class GAPI_EXPORTS View
{
public:
    struct Cache
    {
        std::vector<const uint8_t*> m_linePtrs;
        GMatDesc m_desc;
        int m_border_size = 0;

        inline const uint8_t* linePtr(int index) const
        {


            GAPI_DbgAssert(index >= -m_border_size
                        && index <  -m_border_size + static_cast<int>(m_linePtrs.size()));
            return m_linePtrs[index + m_border_size];
        }
    };

    View() = default;

    const inline uint8_t* InLineB(int index) const
    {
        return m_cache->linePtr(index);
    }

    template<typename T> const inline T* InLine(int i) const
    {
        const uint8_t* ptr = this->InLineB(i);
        return reinterpret_cast<const T*>(ptr);
    }

    inline operator bool() const { return m_priv != nullptr; }
    bool ready() const;
    inline int length() const { return m_cache->m_desc.size.width; }
    int y() const;

    inline const GMatDesc& meta() const { return m_cache->m_desc; }

    class GAPI_EXPORTS Priv;
    Priv& priv();
    const Priv& priv() const;

    View(Priv* p);

private:
    std::shared_ptr<Priv> m_priv;
    const Cache* m_cache;
};

class GAPI_EXPORTS Buffer
{
public:
    struct Cache
    {
        std::vector<uint8_t*> m_linePtrs;
        GMatDesc m_desc;
    };



    Buffer();

    Buffer(const cv::GMatDesc &desc);


    Buffer(const cv::GMatDesc &desc,
           int max_line_consumption, int border_size,
           int skew,
           int wlpi,
           BorderOpt border);

    Buffer(const cv::gapi::own::Mat &data, bool is_input);

    inline uint8_t* OutLineB(int index = 0)
    {
        return m_cache->m_linePtrs[index];
    }

    template<typename T> inline T* OutLine(int index = 0)
    {
        uint8_t* ptr = this->OutLineB(index);
        return reinterpret_cast<T*>(ptr);
    }

    int y() const;

    int linesReady() const;
    void debug(std::ostream &os) const;
    inline int length() const { return m_cache->m_desc.size.width; }
    int lpi() const;

    inline const GMatDesc& meta() const { return m_cache->m_desc; }

    View mkView(int borderSize, bool ownStorage);

    class GAPI_EXPORTS Priv;
    Priv& priv();
    const Priv& priv() const;

private:
    std::shared_ptr<Priv> m_priv;
    const Cache* m_cache;
};

}
}
}

#endif
