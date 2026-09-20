

#ifndef OPENCV_DNN_DNN_SHAPE_UTILS_HPP
#define OPENCV_DNN_DNN_SHAPE_UTILS_HPP

#include <opencv2/dnn/dnn.hpp>
#include <opencv2/core/types_c.h>
#include <iostream>
#include <ostream>
#include <sstream>

namespace cv {
namespace dnn {
CV__DNN_INLINE_NS_BEGIN



struct _Range : public cv::Range
{
    _Range(const Range &r) : cv::Range(r) {}
    _Range(int start_, int size_ = 1) : cv::Range(start_, start_ + size_) {}
};

static inline Mat slice(const Mat &m, const _Range &r0)
{
    Range ranges[CV_MAX_DIM];
    for (int i = 1; i < m.dims; i++)
        ranges[i] = Range::all();
    ranges[0] = r0;
    return m(&ranges[0]);
}

static inline Mat slice(const Mat &m, const _Range &r0, const _Range &r1)
{
    CV_Assert(m.dims >= 2);
    Range ranges[CV_MAX_DIM];
    for (int i = 2; i < m.dims; i++)
        ranges[i] = Range::all();
    ranges[0] = r0;
    ranges[1] = r1;
    return m(&ranges[0]);
}

static inline Mat slice(const Mat &m, const _Range &r0, const _Range &r1, const _Range &r2)
{
    CV_Assert(m.dims >= 3);
    Range ranges[CV_MAX_DIM];
    for (int i = 3; i < m.dims; i++)
        ranges[i] = Range::all();
    ranges[0] = r0;
    ranges[1] = r1;
    ranges[2] = r2;
    return m(&ranges[0]);
}

static inline Mat slice(const Mat &m, const _Range &r0, const _Range &r1, const _Range &r2, const _Range &r3)
{
    CV_Assert(m.dims >= 4);
    Range ranges[CV_MAX_DIM];
    for (int i = 4; i < m.dims; i++)
        ranges[i] = Range::all();
    ranges[0] = r0;
    ranges[1] = r1;
    ranges[2] = r2;
    ranges[3] = r3;
    return m(&ranges[0]);
}

static inline Mat getPlane(const Mat &m, int n, int cn)
{
    CV_Assert(m.dims > 2);
    int sz[CV_MAX_DIM];
    for(int i = 2; i < m.dims; i++)
    {
        sz[i-2] = m.size.p[i];
    }
    return Mat(m.dims - 2, sz, m.type(), (void*)m.ptr<float>(n, cn));
}

static inline MatShape shape(const int* dims, const int n)
{
    MatShape shape;
    shape.assign(dims, dims + n);
    return shape;
}

static inline MatShape shape(const Mat& mat)
{
    return shape(mat.size.p, mat.dims);
}

static inline MatShape shape(const MatSize& sz)
{
    return shape(sz.p, sz.dims());
}

static inline MatShape shape(const UMat& mat)
{
    return shape(mat.size.p, mat.dims);
}

namespace {inline bool is_neg(int i) { return i < 0; }}

static inline MatShape shape(int a0, int a1=-1, int a2=-1, int a3=-1)
{
    int dims[] = {a0, a1, a2, a3};
    MatShape s = shape(dims, 4);
    s.erase(std::remove_if(s.begin(), s.end(), is_neg), s.end());
    return s;
}

static inline int total(const MatShape& shape, int start = -1, int end = -1)
{
    if (start == -1) start = 0;
    if (end == -1) end = (int)shape.size();

    if (shape.empty())
        return 0;

    int elems = 1;
    CV_Assert(start <= (int)shape.size() && end <= (int)shape.size() &&
              start <= end);
    for(int i = start; i < end; i++)
    {
        elems *= shape[i];
    }
    return elems;
}

static inline MatShape concat(const MatShape& a, const MatShape& b)
{
    MatShape c = a;
    c.insert(c.end(), b.begin(), b.end());

    return c;
}

static inline std::string toString(const MatShape& shape, const String& name = "")
{
    std::ostringstream ss;
    if (!name.empty())
        ss << name << ' ';
    ss << '[';
    for(size_t i = 0, n = shape.size(); i < n; ++i)
        ss << ' ' << shape[i];
    ss << " ]";
    return ss.str();
}
static inline void print(const MatShape& shape, const String& name = "")
{
    std::cout << toString(shape, name) << std::endl;
}
static inline std::ostream& operator<<(std::ostream &out, const MatShape& shape)
{
    out << toString(shape);
    return out;
}

inline int clamp(int ax, int dims)
{
    return ax < 0 ? ax + dims : ax;
}

inline int clamp(int ax, const MatShape& shape)
{
    return clamp(ax, (int)shape.size());
}

inline Range clamp(const Range& r, int axisSize)
{
    Range clamped(std::max(r.start, 0),
                  r.end > 0 ? std::min(r.end, axisSize) : axisSize + r.end + 1);
    CV_Assert_N(clamped.start < clamped.end, clamped.end <= axisSize);
    return clamped;
}

CV__DNN_INLINE_NS_END
}
}
#endif
