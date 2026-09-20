

#ifndef OPENCV_CORE_CVSTD_HPP
#define OPENCV_CORE_CVSTD_HPP

#ifndef __cplusplus
#  error cvstd.hpp header must be compiled as C++
#endif

#include "opencv2/core/cvdef.h"
#include <cstddef>
#include <cstring>
#include <cctype>

#include <string>


#  include <algorithm>
#  include <utility>
#  include <cstdlib>
#  include <cmath>

namespace cv
{
    static inline uchar abs(uchar a) { return a; }
    static inline ushort abs(ushort a) { return a; }
    static inline unsigned abs(unsigned a) { return a; }
    static inline uint64 abs(uint64 a) { return a; }

    using std::min;
    using std::max;
    using std::abs;
    using std::swap;
    using std::sqrt;
    using std::exp;
    using std::pow;
    using std::log;
}

#include "cvstd_wrapper.hpp"

namespace cv {







CV_EXPORTS void* fastMalloc(size_t bufSize);


CV_EXPORTS void fastFree(void* ptr);


template<typename _Tp> class Allocator
{
public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    template<typename U> class rebind { typedef Allocator<U> other; };

    explicit Allocator() {}
    ~Allocator() {}
    explicit Allocator(Allocator const&) {}
    template<typename U>
    explicit Allocator(Allocator<U> const&) {}


    pointer address(reference r) { return &r; }
    const_pointer address(const_reference r) { return &r; }

    pointer allocate(size_type count, const void* =0) { return reinterpret_cast<pointer>(fastMalloc(count * sizeof (_Tp))); }
    void deallocate(pointer p, size_type) { fastFree(p); }

    void construct(pointer p, const _Tp& v) { new(static_cast<void*>(p)) _Tp(v); }
    void destroy(pointer p) { p->~_Tp(); }

    size_type max_size() const { return cv::max(static_cast<_Tp>(-1)/sizeof(_Tp), 1); }
};










class CV_EXPORTS FileNode;

typedef std::string String;

#ifndef OPENCV_DISABLE_STRING_LOWER_UPPER_CONVERSIONS


namespace details {

static inline char char_tolower(char ch)
{
    return (char)std::tolower((int)ch);
}

static inline char char_toupper(char ch)
{
    return (char)std::toupper((int)ch);
}
}


static inline std::string toLowerCase(const std::string& str)
{
    std::string result(str);
    std::transform(result.begin(), result.end(), result.begin(), details::char_tolower);
    return result;
}

static inline std::string toUpperCase(const std::string& str)
{
    std::string result(str);
    std::transform(result.begin(), result.end(), result.begin(), details::char_toupper);
    return result;
}

#endif


}

#endif
