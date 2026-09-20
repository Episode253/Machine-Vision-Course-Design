






#ifndef OPENCV_GAPI_GCALL_HPP
#define OPENCV_GAPI_GCALL_HPP

#include <opencv2/gapi/garg.hpp>
#include <opencv2/gapi/gmat.hpp>
#include <opencv2/gapi/gscalar.hpp>
#include <opencv2/gapi/garray.hpp>

namespace cv {

struct GKernel;





class GAPI_EXPORTS GCall final
{
public:
    class Priv;

    explicit GCall(const GKernel &k);
    ~GCall();

    template<typename... Ts>
    GCall& pass(Ts&&... args)
    {
        setArgs({cv::GArg(std::move(args))...});
        return *this;
    }


    GMat    yield      (int output = 0);
    GMatP   yieldP     (int output = 0);
    GScalar yieldScalar(int output = 0);

    template<class T> GArray<T> yieldArray(int output = 0)
    {
        return GArray<T>(yieldArray(output));
    }


    Priv& priv();
    const Priv& priv() const;

protected:
    std::shared_ptr<Priv> m_priv;

    void setArgs(std::vector<GArg> &&args);


    detail::GArrayU yieldArray(int output = 0);
};

}

#endif
