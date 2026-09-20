

#ifndef OPENCV_CORE_OPENGL_HPP
#define OPENCV_CORE_OPENGL_HPP

#ifndef __cplusplus
#  error opengl.hpp header must be compiled as C++
#endif

#include "opencv2/core.hpp"
#include "ocl.hpp"

namespace cv { namespace ogl {







class CV_EXPORTS Buffer
{
public:

    enum Target
    {
        ARRAY_BUFFER         = 0x8892,
        ELEMENT_ARRAY_BUFFER = 0x8893,
        PIXEL_PACK_BUFFER    = 0x88EB,
        PIXEL_UNPACK_BUFFER  = 0x88EC
    };

    enum Access
    {
        READ_ONLY  = 0x88B8,
        WRITE_ONLY = 0x88B9,
        READ_WRITE = 0x88BA
    };


    Buffer();


    Buffer(int arows, int acols, int atype, unsigned int abufId, bool autoRelease = false);


    Buffer(Size asize, int atype, unsigned int abufId, bool autoRelease = false);


    Buffer(int arows, int acols, int atype, Target target = ARRAY_BUFFER, bool autoRelease = false);


    Buffer(Size asize, int atype, Target target = ARRAY_BUFFER, bool autoRelease = false);


    explicit Buffer(InputArray arr, Target target = ARRAY_BUFFER, bool autoRelease = false);


    void create(int arows, int acols, int atype, Target target = ARRAY_BUFFER, bool autoRelease = false);


    void create(Size asize, int atype, Target target = ARRAY_BUFFER, bool autoRelease = false);


    void release();


    void setAutoRelease(bool flag);


    void copyFrom(InputArray arr, Target target = ARRAY_BUFFER, bool autoRelease = false);


    void copyFrom(InputArray arr, cuda::Stream& stream, Target target = ARRAY_BUFFER, bool autoRelease = false);


    void copyTo(OutputArray arr) const;


    void copyTo(OutputArray arr, cuda::Stream& stream) const;


    Buffer clone(Target target = ARRAY_BUFFER, bool autoRelease = false) const;


    void bind(Target target) const;


    static void unbind(Target target);


    Mat mapHost(Access access);


    void unmapHost();


    cuda::GpuMat mapDevice();
    void unmapDevice();


    cuda::GpuMat mapDevice(cuda::Stream& stream);


    void unmapDevice(cuda::Stream& stream);

    int rows() const;
    int cols() const;
    Size size() const;
    bool empty() const;

    int type() const;
    int depth() const;
    int channels() const;
    int elemSize() const;
    int elemSize1() const;


    unsigned int bufId() const;

    class Impl;

private:
    Ptr<Impl> impl_;
    int rows_;
    int cols_;
    int type_;
};


class CV_EXPORTS Texture2D
{
public:

    enum Format
    {
        NONE            = 0,
        DEPTH_COMPONENT = 0x1902,
        RGB             = 0x1907,
        RGBA            = 0x1908
    };


    Texture2D();


    Texture2D(int arows, int acols, Format aformat, unsigned int atexId, bool autoRelease = false);


    Texture2D(Size asize, Format aformat, unsigned int atexId, bool autoRelease = false);


    Texture2D(int arows, int acols, Format aformat, bool autoRelease = false);


    Texture2D(Size asize, Format aformat, bool autoRelease = false);


    explicit Texture2D(InputArray arr, bool autoRelease = false);


    void create(int arows, int acols, Format aformat, bool autoRelease = false);

    void create(Size asize, Format aformat, bool autoRelease = false);


    void release();


    void setAutoRelease(bool flag);


    void copyFrom(InputArray arr, bool autoRelease = false);


    void copyTo(OutputArray arr, int ddepth = CV_32F, bool autoRelease = false) const;


    void bind() const;

    int rows() const;
    int cols() const;
    Size size() const;
    bool empty() const;

    Format format() const;


    unsigned int texId() const;

    class Impl;

private:
    Ptr<Impl> impl_;
    int rows_;
    int cols_;
    Format format_;
};


class CV_EXPORTS Arrays
{
public:

    Arrays();


    void setVertexArray(InputArray vertex);


    void resetVertexArray();


    void setColorArray(InputArray color);


    void resetColorArray();


    void setNormalArray(InputArray normal);


    void resetNormalArray();


    void setTexCoordArray(InputArray texCoord);


    void resetTexCoordArray();


    void release();


    void setAutoRelease(bool flag);


    void bind() const;


    int size() const;
    bool empty() const;

private:
    int size_;
    Buffer vertex_;
    Buffer color_;
    Buffer normal_;
    Buffer texCoord_;
};




enum RenderModes {
    POINTS         = 0x0000,
    LINES          = 0x0001,
    LINE_LOOP      = 0x0002,
    LINE_STRIP     = 0x0003,
    TRIANGLES      = 0x0004,
    TRIANGLE_STRIP = 0x0005,
    TRIANGLE_FAN   = 0x0006,
    QUADS          = 0x0007,
    QUAD_STRIP     = 0x0008,
    POLYGON        = 0x0009
};


CV_EXPORTS void render(const Texture2D& tex,
    Rect_<double> wndRect = Rect_<double>(0.0, 0.0, 1.0, 1.0),
    Rect_<double> texRect = Rect_<double>(0.0, 0.0, 1.0, 1.0));


CV_EXPORTS void render(const Arrays& arr, int mode = POINTS, Scalar color = Scalar::all(255));


CV_EXPORTS void render(const Arrays& arr, InputArray indices, int mode = POINTS, Scalar color = Scalar::all(255));



namespace ocl {
using namespace cv::ocl;



CV_EXPORTS Context& initializeContextFromGL();

}


CV_EXPORTS void convertToGLTexture2D(InputArray src, Texture2D& texture);


CV_EXPORTS void convertFromGLTexture2D(const Texture2D& texture, OutputArray dst);


CV_EXPORTS UMat mapGLBuffer(const Buffer& buffer, AccessFlag accessFlags = ACCESS_READ | ACCESS_WRITE);


CV_EXPORTS void unmapGLBuffer(UMat& u);


}}

namespace cv { namespace cuda {


CV_EXPORTS void setGlDevice(int device = 0);

}}







inline
cv::ogl::Buffer::Buffer(int arows, int acols, int atype, Target target, bool autoRelease) : rows_(0), cols_(0), type_(0)
{
    create(arows, acols, atype, target, autoRelease);
}

inline
cv::ogl::Buffer::Buffer(Size asize, int atype, Target target, bool autoRelease) : rows_(0), cols_(0), type_(0)
{
    create(asize, atype, target, autoRelease);
}

inline
void cv::ogl::Buffer::create(Size asize, int atype, Target target, bool autoRelease)
{
    create(asize.height, asize.width, atype, target, autoRelease);
}

inline
int cv::ogl::Buffer::rows() const
{
    return rows_;
}

inline
int cv::ogl::Buffer::cols() const
{
    return cols_;
}

inline
cv::Size cv::ogl::Buffer::size() const
{
    return Size(cols_, rows_);
}

inline
bool cv::ogl::Buffer::empty() const
{
    return rows_ == 0 || cols_ == 0;
}

inline
int cv::ogl::Buffer::type() const
{
    return type_;
}

inline
int cv::ogl::Buffer::depth() const
{
    return CV_MAT_DEPTH(type_);
}

inline
int cv::ogl::Buffer::channels() const
{
    return CV_MAT_CN(type_);
}

inline
int cv::ogl::Buffer::elemSize() const
{
    return CV_ELEM_SIZE(type_);
}

inline
int cv::ogl::Buffer::elemSize1() const
{
    return CV_ELEM_SIZE1(type_);
}



inline
cv::ogl::Texture2D::Texture2D(int arows, int acols, Format aformat, bool autoRelease) : rows_(0), cols_(0), format_(NONE)
{
    create(arows, acols, aformat, autoRelease);
}

inline
cv::ogl::Texture2D::Texture2D(Size asize, Format aformat, bool autoRelease) : rows_(0), cols_(0), format_(NONE)
{
    create(asize, aformat, autoRelease);
}

inline
void cv::ogl::Texture2D::create(Size asize, Format aformat, bool autoRelease)
{
    create(asize.height, asize.width, aformat, autoRelease);
}

inline
int cv::ogl::Texture2D::rows() const
{
    return rows_;
}

inline
int cv::ogl::Texture2D::cols() const
{
    return cols_;
}

inline
cv::Size cv::ogl::Texture2D::size() const
{
    return Size(cols_, rows_);
}

inline
bool cv::ogl::Texture2D::empty() const
{
    return rows_ == 0 || cols_ == 0;
}

inline
cv::ogl::Texture2D::Format cv::ogl::Texture2D::format() const
{
    return format_;
}



inline
cv::ogl::Arrays::Arrays() : size_(0)
{
}

inline
int cv::ogl::Arrays::size() const
{
    return size_;
}

inline
bool cv::ogl::Arrays::empty() const
{
    return size_ == 0;
}



#endif
