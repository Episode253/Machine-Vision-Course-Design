



#ifndef OPENCV_CORE_ASYNC_PROMISE_HPP
#define OPENCV_CORE_ASYNC_PROMISE_HPP

#include "../async.hpp"

#include "exception_ptr.hpp"

namespace cv {





class CV_EXPORTS AsyncPromise
{
public:
    ~AsyncPromise() CV_NOEXCEPT;
    AsyncPromise() CV_NOEXCEPT;
    explicit AsyncPromise(const AsyncPromise& o) CV_NOEXCEPT;
    AsyncPromise& operator=(const AsyncPromise& o) CV_NOEXCEPT;
    void release() CV_NOEXCEPT;


    AsyncArray getArrayResult();


    void setValue(InputArray value);



#if CV__EXCEPTION_PTR

    void setException(std::exception_ptr exception);
#endif


    void setException(const cv::Exception& exception);

#ifdef CV_CXX11
    explicit AsyncPromise(AsyncPromise&& o) { p = o.p; o.p = NULL; }
    AsyncPromise& operator=(AsyncPromise&& o) CV_NOEXCEPT { std::swap(p, o.p); return *this; }
#endif



    typedef struct AsyncArray::Impl Impl; friend struct AsyncArray::Impl;
    inline void* _getImpl() const CV_NOEXCEPT { return p; }
protected:
    Impl* p;
};



}
#endif
