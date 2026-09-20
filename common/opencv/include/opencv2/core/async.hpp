



#ifndef OPENCV_CORE_ASYNC_HPP
#define OPENCV_CORE_ASYNC_HPP

#include <opencv2/core/mat.hpp>

#ifdef CV_CXX11

#include <chrono>
#endif

namespace cv {





class CV_EXPORTS_W AsyncArray
{
public:
    ~AsyncArray() CV_NOEXCEPT;
    CV_WRAP AsyncArray() CV_NOEXCEPT;
    AsyncArray(const AsyncArray& o) CV_NOEXCEPT;
    AsyncArray& operator=(const AsyncArray& o) CV_NOEXCEPT;
    CV_WRAP void release() CV_NOEXCEPT;


    CV_WRAP void get(OutputArray dst) const;


    bool get(OutputArray dst, int64 timeoutNs) const;

    CV_WRAP inline
    bool get(OutputArray dst, double timeoutNs) const { return get(dst, (int64)timeoutNs); }

    bool wait_for(int64 timeoutNs) const;

    CV_WRAP inline
    bool wait_for(double timeoutNs) const { return wait_for((int64)timeoutNs); }

    CV_WRAP bool valid() const CV_NOEXCEPT;

#ifdef CV_CXX11
    inline AsyncArray(AsyncArray&& o) { p = o.p; o.p = NULL; }
    inline AsyncArray& operator=(AsyncArray&& o) CV_NOEXCEPT { std::swap(p, o.p); return *this; }

    template<typename _Rep, typename _Period>
    inline bool get(OutputArray dst, const std::chrono::duration<_Rep, _Period>& timeout)
    {
        return get(dst, (int64)(std::chrono::nanoseconds(timeout).count()));
    }

    template<typename _Rep, typename _Period>
    inline bool wait_for(const std::chrono::duration<_Rep, _Period>& timeout)
    {
        return wait_for((int64)(std::chrono::nanoseconds(timeout).count()));
    }

#if 0
    std::future<Mat> getFutureMat() const;
    std::future<UMat> getFutureUMat() const;
#endif
#endif



    struct Impl; friend struct Impl;
    inline void* _getImpl() const CV_NOEXCEPT { return p; }
protected:
    Impl* p;
};



}
#endif
