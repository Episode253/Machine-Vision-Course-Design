

#ifndef OPENCV_CORE_MAT_HPP
#define OPENCV_CORE_MAT_HPP

#ifndef __cplusplus
#  error mat.hpp header must be compiled as C++
#endif

#include "opencv2/core/matx.hpp"
#include "opencv2/core/types.hpp"

#include "opencv2/core/bufferpool.hpp"

#include <type_traits>

namespace cv
{




enum AccessFlag { ACCESS_READ=1<<24, ACCESS_WRITE=1<<25,
    ACCESS_RW=3<<24, ACCESS_MASK=ACCESS_RW, ACCESS_FAST=1<<26 };
CV_ENUM_FLAGS(AccessFlag)
__CV_ENUM_FLAGS_BITWISE_AND(AccessFlag, int, AccessFlag)

CV__DEBUG_NS_BEGIN

class CV_EXPORTS _OutputArray;




class CV_EXPORTS _InputArray
{
public:
    enum KindFlag {
        KIND_SHIFT = 16,
        FIXED_TYPE = 0x8000 << KIND_SHIFT,
        FIXED_SIZE = 0x4000 << KIND_SHIFT,
        KIND_MASK = 31 << KIND_SHIFT,

        NONE              = 0 << KIND_SHIFT,
        MAT               = 1 << KIND_SHIFT,
        MATX              = 2 << KIND_SHIFT,
        STD_VECTOR        = 3 << KIND_SHIFT,
        STD_VECTOR_VECTOR = 4 << KIND_SHIFT,
        STD_VECTOR_MAT    = 5 << KIND_SHIFT,
        EXPR              = 6 << KIND_SHIFT,
        OPENGL_BUFFER     = 7 << KIND_SHIFT,
        CUDA_HOST_MEM     = 8 << KIND_SHIFT,
        CUDA_GPU_MAT      = 9 << KIND_SHIFT,
        UMAT              =10 << KIND_SHIFT,
        STD_VECTOR_UMAT   =11 << KIND_SHIFT,
        STD_BOOL_VECTOR   =12 << KIND_SHIFT,
        STD_VECTOR_CUDA_GPU_MAT = 13 << KIND_SHIFT,
        STD_ARRAY         =14 << KIND_SHIFT,
        STD_ARRAY_MAT     =15 << KIND_SHIFT
    };

    _InputArray();
    _InputArray(int _flags, void* _obj);
    _InputArray(const Mat& m);
    _InputArray(const MatExpr& expr);
    _InputArray(const std::vector<Mat>& vec);
    template<typename _Tp> _InputArray(const Mat_<_Tp>& m);
    template<typename _Tp> _InputArray(const std::vector<_Tp>& vec);
    _InputArray(const std::vector<bool>& vec);
    template<typename _Tp> _InputArray(const std::vector<std::vector<_Tp> >& vec);
    _InputArray(const std::vector<std::vector<bool> >&) = delete;
    template<typename _Tp> _InputArray(const std::vector<Mat_<_Tp> >& vec);
    template<typename _Tp> _InputArray(const _Tp* vec, int n);
    template<typename _Tp, int m, int n> _InputArray(const Matx<_Tp, m, n>& matx);
    _InputArray(const double& val);
    _InputArray(const cuda::GpuMat& d_mat);
    _InputArray(const std::vector<cuda::GpuMat>& d_mat_array);
    _InputArray(const ogl::Buffer& buf);
    _InputArray(const cuda::HostMem& cuda_mem);
    template<typename _Tp> _InputArray(const cudev::GpuMat_<_Tp>& m);
    _InputArray(const UMat& um);
    _InputArray(const std::vector<UMat>& umv);

    template<typename _Tp, std::size_t _Nm> _InputArray(const std::array<_Tp, _Nm>& arr);
    template<std::size_t _Nm> _InputArray(const std::array<Mat, _Nm>& arr);

    template<typename _Tp> static _InputArray rawIn(const std::vector<_Tp>& vec);
    template<typename _Tp, std::size_t _Nm> static _InputArray rawIn(const std::array<_Tp, _Nm>& arr);

    Mat getMat(int idx=-1) const;
    Mat getMat_(int idx=-1) const;
    UMat getUMat(int idx=-1) const;
    void getMatVector(std::vector<Mat>& mv) const;
    void getUMatVector(std::vector<UMat>& umv) const;
    void getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) const;
    cuda::GpuMat getGpuMat() const;
    ogl::Buffer getOGlBuffer() const;

    int getFlags() const;
    void* getObj() const;
    Size getSz() const;

    _InputArray::KindFlag kind() const;
    int dims(int i=-1) const;
    int cols(int i=-1) const;
    int rows(int i=-1) const;
    Size size(int i=-1) const;
    int sizend(int* sz, int i=-1) const;
    bool sameSize(const _InputArray& arr) const;
    size_t total(int i=-1) const;
    int type(int i=-1) const;
    int depth(int i=-1) const;
    int channels(int i=-1) const;
    bool isContinuous(int i=-1) const;
    bool isSubmatrix(int i=-1) const;
    bool empty() const;
    void copyTo(const _OutputArray& arr) const;
    void copyTo(const _OutputArray& arr, const _InputArray & mask) const;
    size_t offset(int i=-1) const;
    size_t step(int i=-1) const;
    bool isMat() const;
    bool isUMat() const;
    bool isMatVector() const;
    bool isUMatVector() const;
    bool isMatx() const;
    bool isVector() const;
    bool isGpuMat() const;
    bool isGpuMatVector() const;
    ~_InputArray();

protected:
    int flags;
    void* obj;
    Size sz;

    void init(int _flags, const void* _obj);
    void init(int _flags, const void* _obj, Size _sz);
};
CV_ENUM_FLAGS(_InputArray::KindFlag)
__CV_ENUM_FLAGS_BITWISE_AND(_InputArray::KindFlag, int, _InputArray::KindFlag)


class CV_EXPORTS _OutputArray : public _InputArray
{
public:
    enum DepthMask
    {
        DEPTH_MASK_8U = 1 << CV_8U,
        DEPTH_MASK_8S = 1 << CV_8S,
        DEPTH_MASK_16U = 1 << CV_16U,
        DEPTH_MASK_16S = 1 << CV_16S,
        DEPTH_MASK_32S = 1 << CV_32S,
        DEPTH_MASK_32F = 1 << CV_32F,
        DEPTH_MASK_64F = 1 << CV_64F,
        DEPTH_MASK_16F = 1 << CV_16F,
        DEPTH_MASK_ALL = (DEPTH_MASK_64F<<1)-1,
        DEPTH_MASK_ALL_BUT_8S = DEPTH_MASK_ALL & ~DEPTH_MASK_8S,
        DEPTH_MASK_ALL_16F = (DEPTH_MASK_16F<<1)-1,
        DEPTH_MASK_FLT = DEPTH_MASK_32F + DEPTH_MASK_64F
    };

    _OutputArray();
    _OutputArray(int _flags, void* _obj);
    _OutputArray(Mat& m);
    _OutputArray(std::vector<Mat>& vec);
    _OutputArray(cuda::GpuMat& d_mat);
    _OutputArray(std::vector<cuda::GpuMat>& d_mat);
    _OutputArray(ogl::Buffer& buf);
    _OutputArray(cuda::HostMem& cuda_mem);
    template<typename _Tp> _OutputArray(cudev::GpuMat_<_Tp>& m);
    template<typename _Tp> _OutputArray(std::vector<_Tp>& vec);
    _OutputArray(std::vector<bool>& vec) = delete;
    template<typename _Tp> _OutputArray(std::vector<std::vector<_Tp> >& vec);
    _OutputArray(std::vector<std::vector<bool> >&) = delete;
    template<typename _Tp> _OutputArray(std::vector<Mat_<_Tp> >& vec);
    template<typename _Tp> _OutputArray(Mat_<_Tp>& m);
    template<typename _Tp> _OutputArray(_Tp* vec, int n);
    template<typename _Tp, int m, int n> _OutputArray(Matx<_Tp, m, n>& matx);
    _OutputArray(UMat& m);
    _OutputArray(std::vector<UMat>& vec);

    _OutputArray(const Mat& m);
    _OutputArray(const std::vector<Mat>& vec);
    _OutputArray(const cuda::GpuMat& d_mat);
    _OutputArray(const std::vector<cuda::GpuMat>& d_mat);
    _OutputArray(const ogl::Buffer& buf);
    _OutputArray(const cuda::HostMem& cuda_mem);
    template<typename _Tp> _OutputArray(const cudev::GpuMat_<_Tp>& m);
    template<typename _Tp> _OutputArray(const std::vector<_Tp>& vec);
    template<typename _Tp> _OutputArray(const std::vector<std::vector<_Tp> >& vec);
    template<typename _Tp> _OutputArray(const std::vector<Mat_<_Tp> >& vec);
    template<typename _Tp> _OutputArray(const Mat_<_Tp>& m);
    template<typename _Tp> _OutputArray(const _Tp* vec, int n);
    template<typename _Tp, int m, int n> _OutputArray(const Matx<_Tp, m, n>& matx);
    _OutputArray(const UMat& m);
    _OutputArray(const std::vector<UMat>& vec);

    template<typename _Tp, std::size_t _Nm> _OutputArray(std::array<_Tp, _Nm>& arr);
    template<typename _Tp, std::size_t _Nm> _OutputArray(const std::array<_Tp, _Nm>& arr);
    template<std::size_t _Nm> _OutputArray(std::array<Mat, _Nm>& arr);
    template<std::size_t _Nm> _OutputArray(const std::array<Mat, _Nm>& arr);

    template<typename _Tp> static _OutputArray rawOut(std::vector<_Tp>& vec);
    template<typename _Tp, std::size_t _Nm> static _OutputArray rawOut(std::array<_Tp, _Nm>& arr);

    bool fixedSize() const;
    bool fixedType() const;
    bool needed() const;
    Mat& getMatRef(int i=-1) const;
    UMat& getUMatRef(int i=-1) const;
    cuda::GpuMat& getGpuMatRef() const;
    std::vector<cuda::GpuMat>& getGpuMatVecRef() const;
    ogl::Buffer& getOGlBufferRef() const;
    cuda::HostMem& getHostMemRef() const;
    void create(Size sz, int type, int i=-1, bool allowTransposed=false, _OutputArray::DepthMask fixedDepthMask=static_cast<_OutputArray::DepthMask>(0)) const;
    void create(int rows, int cols, int type, int i=-1, bool allowTransposed=false, _OutputArray::DepthMask fixedDepthMask=static_cast<_OutputArray::DepthMask>(0)) const;
    void create(int dims, const int* size, int type, int i=-1, bool allowTransposed=false, _OutputArray::DepthMask fixedDepthMask=static_cast<_OutputArray::DepthMask>(0)) const;
    void createSameSize(const _InputArray& arr, int mtype) const;
    void release() const;
    void clear() const;
    void setTo(const _InputArray& value, const _InputArray & mask = _InputArray()) const;

    void assign(const UMat& u) const;
    void assign(const Mat& m) const;

    void assign(const std::vector<UMat>& v) const;
    void assign(const std::vector<Mat>& v) const;

    void move(UMat& u) const;
    void move(Mat& m) const;
};


class CV_EXPORTS _InputOutputArray : public _OutputArray
{
public:
    _InputOutputArray();
    _InputOutputArray(int _flags, void* _obj);
    _InputOutputArray(Mat& m);
    _InputOutputArray(std::vector<Mat>& vec);
    _InputOutputArray(cuda::GpuMat& d_mat);
    _InputOutputArray(ogl::Buffer& buf);
    _InputOutputArray(cuda::HostMem& cuda_mem);
    template<typename _Tp> _InputOutputArray(cudev::GpuMat_<_Tp>& m);
    template<typename _Tp> _InputOutputArray(std::vector<_Tp>& vec);
    _InputOutputArray(std::vector<bool>& vec) = delete;
    template<typename _Tp> _InputOutputArray(std::vector<std::vector<_Tp> >& vec);
    template<typename _Tp> _InputOutputArray(std::vector<Mat_<_Tp> >& vec);
    template<typename _Tp> _InputOutputArray(Mat_<_Tp>& m);
    template<typename _Tp> _InputOutputArray(_Tp* vec, int n);
    template<typename _Tp, int m, int n> _InputOutputArray(Matx<_Tp, m, n>& matx);
    _InputOutputArray(UMat& m);
    _InputOutputArray(std::vector<UMat>& vec);

    _InputOutputArray(const Mat& m);
    _InputOutputArray(const std::vector<Mat>& vec);
    _InputOutputArray(const cuda::GpuMat& d_mat);
    _InputOutputArray(const std::vector<cuda::GpuMat>& d_mat);
    _InputOutputArray(const ogl::Buffer& buf);
    _InputOutputArray(const cuda::HostMem& cuda_mem);
    template<typename _Tp> _InputOutputArray(const cudev::GpuMat_<_Tp>& m);
    template<typename _Tp> _InputOutputArray(const std::vector<_Tp>& vec);
    template<typename _Tp> _InputOutputArray(const std::vector<std::vector<_Tp> >& vec);
    template<typename _Tp> _InputOutputArray(const std::vector<Mat_<_Tp> >& vec);
    template<typename _Tp> _InputOutputArray(const Mat_<_Tp>& m);
    template<typename _Tp> _InputOutputArray(const _Tp* vec, int n);
    template<typename _Tp, int m, int n> _InputOutputArray(const Matx<_Tp, m, n>& matx);
    _InputOutputArray(const UMat& m);
    _InputOutputArray(const std::vector<UMat>& vec);

    template<typename _Tp, std::size_t _Nm> _InputOutputArray(std::array<_Tp, _Nm>& arr);
    template<typename _Tp, std::size_t _Nm> _InputOutputArray(const std::array<_Tp, _Nm>& arr);
    template<std::size_t _Nm> _InputOutputArray(std::array<Mat, _Nm>& arr);
    template<std::size_t _Nm> _InputOutputArray(const std::array<Mat, _Nm>& arr);

    template<typename _Tp> static _InputOutputArray rawInOut(std::vector<_Tp>& vec);
    template<typename _Tp, std::size_t _Nm> _InputOutputArray rawInOut(std::array<_Tp, _Nm>& arr);

};


template<typename _Tp> static inline _InputArray rawIn(_Tp& v);

template<typename _Tp> static inline _OutputArray rawOut(_Tp& v);

template<typename _Tp> static inline _InputOutputArray rawInOut(_Tp& v);

CV__DEBUG_NS_END

typedef const _InputArray& InputArray;
typedef InputArray InputArrayOfArrays;
typedef const _OutputArray& OutputArray;
typedef OutputArray OutputArrayOfArrays;
typedef const _InputOutputArray& InputOutputArray;
typedef InputOutputArray InputOutputArrayOfArrays;

CV_EXPORTS InputOutputArray noArray();




enum UMatUsageFlags
{
    USAGE_DEFAULT = 0,


    USAGE_ALLOCATE_HOST_MEMORY = 1 << 0,
    USAGE_ALLOCATE_DEVICE_MEMORY = 1 << 1,
    USAGE_ALLOCATE_SHARED_MEMORY = 1 << 2,

    __UMAT_USAGE_FLAGS_32BIT = 0x7fffffff
};

struct CV_EXPORTS UMatData;


class CV_EXPORTS MatAllocator
{
public:
    MatAllocator() {}
    virtual ~MatAllocator() {}





    virtual UMatData* allocate(int dims, const int* sizes, int type,
                               void* data, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) const = 0;
    virtual bool allocate(UMatData* data, AccessFlag accessflags, UMatUsageFlags usageFlags) const = 0;
    virtual void deallocate(UMatData* data) const = 0;
    virtual void map(UMatData* data, AccessFlag accessflags) const;
    virtual void unmap(UMatData* data) const;
    virtual void download(UMatData* data, void* dst, int dims, const size_t sz[],
                          const size_t srcofs[], const size_t srcstep[],
                          const size_t dststep[]) const;
    virtual void upload(UMatData* data, const void* src, int dims, const size_t sz[],
                        const size_t dstofs[], const size_t dststep[],
                        const size_t srcstep[]) const;
    virtual void copy(UMatData* srcdata, UMatData* dstdata, int dims, const size_t sz[],
                      const size_t srcofs[], const size_t srcstep[],
                      const size_t dstofs[], const size_t dststep[], bool sync) const;


    virtual BufferPoolController* getBufferPoolController(const char* id = NULL) const;
};





template<typename _Tp> class MatCommaInitializer_
{
public:

    MatCommaInitializer_(Mat_<_Tp>* _m);

    template<typename T2> MatCommaInitializer_<_Tp>& operator , (T2 v);

    operator Mat_<_Tp>() const;
protected:
    MatIterator_<_Tp> it;
};








struct CV_EXPORTS UMatData
{
    enum MemoryFlag { COPY_ON_MAP=1, HOST_COPY_OBSOLETE=2,
        DEVICE_COPY_OBSOLETE=4, TEMP_UMAT=8, TEMP_COPIED_UMAT=24,
        USER_ALLOCATED=32, DEVICE_MEM_MAPPED=64,
        ASYNC_CLEANUP=128
    };
    UMatData(const MatAllocator* allocator);
    ~UMatData();


    void lock();
    void unlock();

    bool hostCopyObsolete() const;
    bool deviceCopyObsolete() const;
    bool deviceMemMapped() const;
    bool copyOnMap() const;
    bool tempUMat() const;
    bool tempCopiedUMat() const;
    void markHostCopyObsolete(bool flag);
    void markDeviceCopyObsolete(bool flag);
    void markDeviceMemMapped(bool flag);

    const MatAllocator* prevAllocator;
    const MatAllocator* currAllocator;
    int urefcount;
    int refcount;
    uchar* data;
    uchar* origdata;
    size_t size;

    UMatData::MemoryFlag flags;
    void* handle;
    void* userdata;
    int allocatorFlags_;
    int mapcount;
    UMatData* originalUMatData;
};
CV_ENUM_FLAGS(UMatData::MemoryFlag)


struct CV_EXPORTS MatSize
{
    explicit MatSize(int* _p);
    int dims() const;
    Size operator()() const;
    const int& operator[](int i) const;
    int& operator[](int i);
    operator const int*() const;
    bool operator == (const MatSize& sz) const;
    bool operator != (const MatSize& sz) const;

    int* p;
};

struct CV_EXPORTS MatStep
{
    MatStep();
    explicit MatStep(size_t s);
    const size_t& operator[](int i) const;
    size_t& operator[](int i);
    operator size_t() const;
    MatStep& operator = (size_t s);

    size_t* p;
    size_t buf[2];
protected:
    MatStep& operator = (const MatStep&);
};




class CV_EXPORTS Mat
{
public:

    Mat();


    Mat(int rows, int cols, int type);


    Mat(Size size, int type);


    Mat(int rows, int cols, int type, const Scalar& s);


    Mat(Size size, int type, const Scalar& s);


    Mat(int ndims, const int* sizes, int type);


    Mat(const std::vector<int>& sizes, int type);


    Mat(int ndims, const int* sizes, int type, const Scalar& s);


    Mat(const std::vector<int>& sizes, int type, const Scalar& s);



    Mat(const Mat& m);


    Mat(int rows, int cols, int type, void* data, size_t step=AUTO_STEP);


    Mat(Size size, int type, void* data, size_t step=AUTO_STEP);


    Mat(int ndims, const int* sizes, int type, void* data, const size_t* steps=0);


    Mat(const std::vector<int>& sizes, int type, void* data, const size_t* steps=0);


    Mat(const Mat& m, const Range& rowRange, const Range& colRange=Range::all());


    Mat(const Mat& m, const Rect& roi);


    Mat(const Mat& m, const Range* ranges);


    Mat(const Mat& m, const std::vector<Range>& ranges);


    template<typename _Tp> explicit Mat(const std::vector<_Tp>& vec, bool copyData=false);


    template<typename _Tp, typename = typename std::enable_if<std::is_arithmetic<_Tp>::value>::type>
    explicit Mat(const std::initializer_list<_Tp> list);


    template<typename _Tp> explicit Mat(const std::initializer_list<int> sizes, const std::initializer_list<_Tp> list);


    template<typename _Tp, size_t _Nm> explicit Mat(const std::array<_Tp, _Nm>& arr, bool copyData=false);


    template<typename _Tp, int n> explicit Mat(const Vec<_Tp, n>& vec, bool copyData=true);


    template<typename _Tp, int m, int n> explicit Mat(const Matx<_Tp, m, n>& mtx, bool copyData=true);


    template<typename _Tp> explicit Mat(const Point_<_Tp>& pt, bool copyData=true);


    template<typename _Tp> explicit Mat(const Point3_<_Tp>& pt, bool copyData=true);


    template<typename _Tp> explicit Mat(const MatCommaInitializer_<_Tp>& commaInitializer);


    explicit Mat(const cuda::GpuMat& m);


    ~Mat();


    Mat& operator = (const Mat& m);


    Mat& operator = (const MatExpr& expr);


    UMat getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags = USAGE_DEFAULT) const;


    Mat row(int y) const;


    Mat col(int x) const;


    Mat rowRange(int startrow, int endrow) const;


    Mat rowRange(const Range& r) const;


    Mat colRange(int startcol, int endcol) const;


    Mat colRange(const Range& r) const;


    Mat diag(int d=0) const;


    static Mat diag(const Mat& d);


    Mat clone() const CV_NODISCARD;


    void copyTo( OutputArray m ) const;


    void copyTo( OutputArray m, InputArray mask ) const;


    void convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;


    void assignTo( Mat& m, int type=-1 ) const;


    Mat& operator = (const Scalar& s);


    Mat& setTo(InputArray value, InputArray mask=noArray());


    Mat reshape(int cn, int rows=0) const;


    Mat reshape(int cn, int newndims, const int* newsz) const;


    Mat reshape(int cn, const std::vector<int>& newshape) const;


    MatExpr t() const;


    MatExpr inv(int method=DECOMP_LU) const;


    MatExpr mul(InputArray m, double scale=1) const;


    Mat cross(InputArray m) const;


    double dot(InputArray m) const;


    static MatExpr zeros(int rows, int cols, int type);


    static MatExpr zeros(Size size, int type);


    static MatExpr zeros(int ndims, const int* sz, int type);


    static MatExpr ones(int rows, int cols, int type);


    static MatExpr ones(Size size, int type);


    static MatExpr ones(int ndims, const int* sz, int type);


    static MatExpr eye(int rows, int cols, int type);


    static MatExpr eye(Size size, int type);


    void create(int rows, int cols, int type);


    void create(Size size, int type);


    void create(int ndims, const int* sizes, int type);


    void create(const std::vector<int>& sizes, int type);


    void addref();


    void release();


    void deallocate();

    void copySize(const Mat& m);


    void reserve(size_t sz);


    void reserveBuffer(size_t sz);


    void resize(size_t sz);


    void resize(size_t sz, const Scalar& s);


    void push_back_(const void* elem);


    template<typename _Tp> void push_back(const _Tp& elem);


    template<typename _Tp> void push_back(const Mat_<_Tp>& elem);


    template<typename _Tp> void push_back(const std::vector<_Tp>& elem);


    void push_back(const Mat& m);


    void pop_back(size_t nelems=1);


    void locateROI( Size& wholeSize, Point& ofs ) const;


    Mat& adjustROI( int dtop, int dbottom, int dleft, int dright );


    Mat operator()( Range rowRange, Range colRange ) const;


    Mat operator()( const Rect& roi ) const;


    Mat operator()( const Range* ranges ) const;


    Mat operator()(const std::vector<Range>& ranges) const;

    template<typename _Tp> operator std::vector<_Tp>() const;
    template<typename _Tp, int n> operator Vec<_Tp, n>() const;
    template<typename _Tp, int m, int n> operator Matx<_Tp, m, n>() const;

    template<typename _Tp, std::size_t _Nm> operator std::array<_Tp, _Nm>() const;


    bool isContinuous() const;


    bool isSubmatrix() const;


    size_t elemSize() const;


    size_t elemSize1() const;


    int type() const;


    int depth() const;


    int channels() const;


    size_t step1(int i=0) const;


    bool empty() const;


    size_t total() const;


    size_t total(int startDim, int endDim=INT_MAX) const;


    int checkVector(int elemChannels, int depth=-1, bool requireContinuous=true) const;


    uchar* ptr(int i0=0);

    const uchar* ptr(int i0=0) const;


    uchar* ptr(int row, int col);

    const uchar* ptr(int row, int col) const;


    uchar* ptr(int i0, int i1, int i2);

    const uchar* ptr(int i0, int i1, int i2) const;


    uchar* ptr(const int* idx);

    const uchar* ptr(const int* idx) const;

    template<int n> uchar* ptr(const Vec<int, n>& idx);

    template<int n> const uchar* ptr(const Vec<int, n>& idx) const;


    template<typename _Tp> _Tp* ptr(int i0=0);

    template<typename _Tp> const _Tp* ptr(int i0=0) const;

    template<typename _Tp> _Tp* ptr(int row, int col);

    template<typename _Tp> const _Tp* ptr(int row, int col) const;

    template<typename _Tp> _Tp* ptr(int i0, int i1, int i2);

    template<typename _Tp> const _Tp* ptr(int i0, int i1, int i2) const;

    template<typename _Tp> _Tp* ptr(const int* idx);

    template<typename _Tp> const _Tp* ptr(const int* idx) const;

    template<typename _Tp, int n> _Tp* ptr(const Vec<int, n>& idx);

    template<typename _Tp, int n> const _Tp* ptr(const Vec<int, n>& idx) const;


    template<typename _Tp> _Tp& at(int i0=0);

    template<typename _Tp> const _Tp& at(int i0=0) const;

    template<typename _Tp> _Tp& at(int row, int col);

    template<typename _Tp> const _Tp& at(int row, int col) const;


    template<typename _Tp> _Tp& at(int i0, int i1, int i2);

    template<typename _Tp> const _Tp& at(int i0, int i1, int i2) const;


    template<typename _Tp> _Tp& at(const int* idx);

    template<typename _Tp> const _Tp& at(const int* idx) const;


    template<typename _Tp, int n> _Tp& at(const Vec<int, n>& idx);

    template<typename _Tp, int n> const _Tp& at(const Vec<int, n>& idx) const;


    template<typename _Tp> _Tp& at(Point pt);

    template<typename _Tp> const _Tp& at(Point pt) const;


    template<typename _Tp> MatIterator_<_Tp> begin();
    template<typename _Tp> MatConstIterator_<_Tp> begin() const;


    template<typename _Tp> MatIterator_<_Tp> end();
    template<typename _Tp> MatConstIterator_<_Tp> end() const;


    template<typename _Tp, typename Functor> void forEach(const Functor& operation);

    template<typename _Tp, typename Functor> void forEach(const Functor& operation) const;

    Mat(Mat&& m);
    Mat& operator = (Mat&& m);

    enum { MAGIC_VAL  = 0x42FF0000, AUTO_STEP = 0, CONTINUOUS_FLAG = CV_MAT_CONT_FLAG, SUBMATRIX_FLAG = CV_SUBMAT_FLAG };
    enum { MAGIC_MASK = 0xFFFF0000, TYPE_MASK = 0x00000FFF, DEPTH_MASK = 7 };


    int flags;

    int dims;

    int rows, cols;

    uchar* data;


    const uchar* datastart;
    const uchar* dataend;
    const uchar* datalimit;


    MatAllocator* allocator;

    static MatAllocator* getStdAllocator();
    static MatAllocator* getDefaultAllocator();
    static void setDefaultAllocator(MatAllocator* allocator);


    void updateContinuityFlag();


    UMatData* u;

    MatSize size;
    MatStep step;

protected:
    template<typename _Tp, typename Functor> void forEach_impl(const Functor& operation);
};





template<typename _Tp> class Mat_ : public Mat
{
public:
    typedef _Tp value_type;
    typedef typename DataType<_Tp>::channel_type channel_type;
    typedef MatIterator_<_Tp> iterator;
    typedef MatConstIterator_<_Tp> const_iterator;


    Mat_();

    Mat_(int _rows, int _cols);

    Mat_(int _rows, int _cols, const _Tp& value);

    explicit Mat_(Size _size);

    Mat_(Size _size, const _Tp& value);

    Mat_(int _ndims, const int* _sizes);

    Mat_(int _ndims, const int* _sizes, const _Tp& value);

    Mat_(const Mat& m);

    Mat_(const Mat_& m);

    Mat_(int _rows, int _cols, _Tp* _data, size_t _step=AUTO_STEP);

    Mat_(int _ndims, const int* _sizes, _Tp* _data, const size_t* _steps=0);

    Mat_(const Mat_& m, const Range& rowRange, const Range& colRange=Range::all());

    Mat_(const Mat_& m, const Rect& roi);

    Mat_(const Mat_& m, const Range* ranges);

    Mat_(const Mat_& m, const std::vector<Range>& ranges);

    explicit Mat_(const MatExpr& e);

    explicit Mat_(const std::vector<_Tp>& vec, bool copyData=false);
    template<int n> explicit Mat_(const Vec<typename DataType<_Tp>::channel_type, n>& vec, bool copyData=true);
    template<int m, int n> explicit Mat_(const Matx<typename DataType<_Tp>::channel_type, m, n>& mtx, bool copyData=true);
    explicit Mat_(const Point_<typename DataType<_Tp>::channel_type>& pt, bool copyData=true);
    explicit Mat_(const Point3_<typename DataType<_Tp>::channel_type>& pt, bool copyData=true);
    explicit Mat_(const MatCommaInitializer_<_Tp>& commaInitializer);

    Mat_(std::initializer_list<_Tp> values);
    explicit Mat_(const std::initializer_list<int> sizes, const std::initializer_list<_Tp> values);

    template <std::size_t _Nm> explicit Mat_(const std::array<_Tp, _Nm>& arr, bool copyData=false);

    Mat_& operator = (const Mat& m);
    Mat_& operator = (const Mat_& m);

    Mat_& operator = (const _Tp& s);

    Mat_& operator = (const MatExpr& e);


    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;



    template<typename Functor> void forEach(const Functor& operation);
    template<typename Functor> void forEach(const Functor& operation) const;


    void create(int _rows, int _cols);

    void create(Size _size);

    void create(int _ndims, const int* _sizes);

    void release();

    Mat_ cross(const Mat_& m) const;

    template<typename T2> operator Mat_<T2>() const;

    Mat_ row(int y) const;
    Mat_ col(int x) const;
    Mat_ diag(int d=0) const;
    Mat_ clone() const CV_NODISCARD;


    size_t elemSize() const;
    size_t elemSize1() const;
    int type() const;
    int depth() const;
    int channels() const;
    size_t step1(int i=0) const;

    size_t stepT(int i=0) const;


    static MatExpr zeros(int rows, int cols);
    static MatExpr zeros(Size size);
    static MatExpr zeros(int _ndims, const int* _sizes);
    static MatExpr ones(int rows, int cols);
    static MatExpr ones(Size size);
    static MatExpr ones(int _ndims, const int* _sizes);
    static MatExpr eye(int rows, int cols);
    static MatExpr eye(Size size);


    Mat_& adjustROI( int dtop, int dbottom, int dleft, int dright );
    Mat_ operator()( const Range& rowRange, const Range& colRange ) const;
    Mat_ operator()( const Rect& roi ) const;
    Mat_ operator()( const Range* ranges ) const;
    Mat_ operator()(const std::vector<Range>& ranges) const;


    _Tp* operator [](int y);
    const _Tp* operator [](int y) const;


    _Tp& operator ()(const int* idx);

    const _Tp& operator ()(const int* idx) const;


    template<int n> _Tp& operator ()(const Vec<int, n>& idx);

    template<int n> const _Tp& operator ()(const Vec<int, n>& idx) const;


    _Tp& operator ()(int idx0);

    const _Tp& operator ()(int idx0) const;

    _Tp& operator ()(int row, int col);

    const _Tp& operator ()(int row, int col) const;

    _Tp& operator ()(int idx0, int idx1, int idx2);

    const _Tp& operator ()(int idx0, int idx1, int idx2) const;

    _Tp& operator ()(Point pt);
    const _Tp& operator ()(Point pt) const;


    operator std::vector<_Tp>() const;


    template<std::size_t _Nm> operator std::array<_Tp, _Nm>() const;


    template<int n> operator Vec<typename DataType<_Tp>::channel_type, n>() const;

    template<int m, int n> operator Matx<typename DataType<_Tp>::channel_type, m, n>() const;

    Mat_(Mat_&& m);
    Mat_& operator = (Mat_&& m);

    Mat_(Mat&& m);
    Mat_& operator = (Mat&& m);

    Mat_(MatExpr&& e);
};

typedef Mat_<uchar> Mat1b;
typedef Mat_<Vec2b> Mat2b;
typedef Mat_<Vec3b> Mat3b;
typedef Mat_<Vec4b> Mat4b;

typedef Mat_<short> Mat1s;
typedef Mat_<Vec2s> Mat2s;
typedef Mat_<Vec3s> Mat3s;
typedef Mat_<Vec4s> Mat4s;

typedef Mat_<ushort> Mat1w;
typedef Mat_<Vec2w> Mat2w;
typedef Mat_<Vec3w> Mat3w;
typedef Mat_<Vec4w> Mat4w;

typedef Mat_<int>   Mat1i;
typedef Mat_<Vec2i> Mat2i;
typedef Mat_<Vec3i> Mat3i;
typedef Mat_<Vec4i> Mat4i;

typedef Mat_<float> Mat1f;
typedef Mat_<Vec2f> Mat2f;
typedef Mat_<Vec3f> Mat3f;
typedef Mat_<Vec4f> Mat4f;

typedef Mat_<double> Mat1d;
typedef Mat_<Vec2d> Mat2d;
typedef Mat_<Vec3d> Mat3d;
typedef Mat_<Vec4d> Mat4d;


class CV_EXPORTS UMat
{
public:

    UMat(UMatUsageFlags usageFlags = USAGE_DEFAULT);


    UMat(int rows, int cols, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);
    UMat(Size size, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    UMat(int rows, int cols, int type, const Scalar& s, UMatUsageFlags usageFlags = USAGE_DEFAULT);
    UMat(Size size, int type, const Scalar& s, UMatUsageFlags usageFlags = USAGE_DEFAULT);


    UMat(int ndims, const int* sizes, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);
    UMat(int ndims, const int* sizes, int type, const Scalar& s, UMatUsageFlags usageFlags = USAGE_DEFAULT);


    UMat(const UMat& m);


    UMat(const UMat& m, const Range& rowRange, const Range& colRange=Range::all());
    UMat(const UMat& m, const Rect& roi);
    UMat(const UMat& m, const Range* ranges);
    UMat(const UMat& m, const std::vector<Range>& ranges);

    template<typename _Tp> explicit UMat(const std::vector<_Tp>& vec, bool copyData=false);


    template<typename _Tp, int n> explicit UMat(const Vec<_Tp, n>& vec, bool copyData=true);

    template<typename _Tp, int m, int n> explicit UMat(const Matx<_Tp, m, n>& mtx, bool copyData=true);

    template<typename _Tp> explicit UMat(const Point_<_Tp>& pt, bool copyData=true);

    template<typename _Tp> explicit UMat(const Point3_<_Tp>& pt, bool copyData=true);

    template<typename _Tp> explicit UMat(const MatCommaInitializer_<_Tp>& commaInitializer);


    ~UMat();

    UMat& operator = (const UMat& m);

    Mat getMat(AccessFlag flags) const;


    UMat row(int y) const;

    UMat col(int x) const;

    UMat rowRange(int startrow, int endrow) const;
    UMat rowRange(const Range& r) const;

    UMat colRange(int startcol, int endcol) const;
    UMat colRange(const Range& r) const;




    UMat diag(int d=0) const;

    static UMat diag(const UMat& d);


    UMat clone() const CV_NODISCARD;


    void copyTo( OutputArray m ) const;

    void copyTo( OutputArray m, InputArray mask ) const;

    void convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;

    void assignTo( UMat& m, int type=-1 ) const;


    UMat& operator = (const Scalar& s);

    UMat& setTo(InputArray value, InputArray mask=noArray());


    UMat reshape(int cn, int rows=0) const;
    UMat reshape(int cn, int newndims, const int* newsz) const;


    UMat t() const;

    UMat inv(int method=DECOMP_LU) const;

    UMat mul(InputArray m, double scale=1) const;


    double dot(InputArray m) const;


    static UMat zeros(int rows, int cols, int type);
    static UMat zeros(Size size, int type);
    static UMat zeros(int ndims, const int* sz, int type);
    static UMat ones(int rows, int cols, int type);
    static UMat ones(Size size, int type);
    static UMat ones(int ndims, const int* sz, int type);
    static UMat eye(int rows, int cols, int type);
    static UMat eye(Size size, int type);



    void create(int rows, int cols, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);
    void create(Size size, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);
    void create(int ndims, const int* sizes, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);
    void create(const std::vector<int>& sizes, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);


    void addref();


    void release();


    void deallocate();

    void copySize(const UMat& m);


    void locateROI( Size& wholeSize, Point& ofs ) const;

    UMat& adjustROI( int dtop, int dbottom, int dleft, int dright );


    UMat operator()( Range rowRange, Range colRange ) const;
    UMat operator()( const Rect& roi ) const;
    UMat operator()( const Range* ranges ) const;
    UMat operator()(const std::vector<Range>& ranges) const;




    bool isContinuous() const;


    bool isSubmatrix() const;



    size_t elemSize() const;

    size_t elemSize1() const;

    int type() const;

    int depth() const;

    int channels() const;

    size_t step1(int i=0) const;

    bool empty() const;

    size_t total() const;


    int checkVector(int elemChannels, int depth=-1, bool requireContinuous=true) const;

    UMat(UMat&& m);
    UMat& operator = (UMat&& m);


    void* handle(AccessFlag accessFlags) const;
    void ndoffset(size_t* ofs) const;

    enum { MAGIC_VAL  = 0x42FF0000, AUTO_STEP = 0, CONTINUOUS_FLAG = CV_MAT_CONT_FLAG, SUBMATRIX_FLAG = CV_SUBMAT_FLAG };
    enum { MAGIC_MASK = 0xFFFF0000, TYPE_MASK = 0x00000FFF, DEPTH_MASK = 7 };


    int flags;

    int dims;

    int rows, cols;


    MatAllocator* allocator;
    UMatUsageFlags usageFlags;

    static MatAllocator* getStdAllocator();


    void updateContinuityFlag();


    UMatData* u;


    size_t offset;

    MatSize size;
    MatStep step;

protected:
};





class CV_EXPORTS SparseMat
{
public:
    typedef SparseMatIterator iterator;
    typedef SparseMatConstIterator const_iterator;

    enum { MAGIC_VAL=0x42FD0000, MAX_DIM=32, HASH_SCALE=0x5bd1e995, HASH_BIT=0x80000000 };


    struct CV_EXPORTS Hdr
    {
        Hdr(int _dims, const int* _sizes, int _type);
        void clear();
        int refcount;
        int dims;
        int valueOffset;
        size_t nodeSize;
        size_t nodeCount;
        size_t freeList;
        std::vector<uchar> pool;
        std::vector<size_t> hashtab;
        int size[MAX_DIM];
    };


    struct CV_EXPORTS Node
    {

        size_t hashval;

        size_t next;

        int idx[MAX_DIM];
    };


    SparseMat();


    SparseMat(int dims, const int* _sizes, int _type);


    SparseMat(const SparseMat& m);


    explicit SparseMat(const Mat& m);


    ~SparseMat();


    SparseMat& operator = (const SparseMat& m);

    SparseMat& operator = (const Mat& m);


    SparseMat clone() const CV_NODISCARD;


    void copyTo( SparseMat& m ) const;

    void copyTo( Mat& m ) const;

    void convertTo( SparseMat& m, int rtype, double alpha=1 ) const;


    void convertTo( Mat& m, int rtype, double alpha=1, double beta=0 ) const;


    void assignTo( SparseMat& m, int type=-1 ) const;



    void create(int dims, const int* _sizes, int _type);

    void clear();

    void addref();

    void release();




    size_t elemSize() const;

    size_t elemSize1() const;


    int type() const;

    int depth() const;

    int channels() const;


    const int* size() const;

    int size(int i) const;

    int dims() const;

    size_t nzcount() const;


    size_t hash(int i0) const;

    size_t hash(int i0, int i1) const;

    size_t hash(int i0, int i1, int i2) const;

    size_t hash(const int* idx) const;




    uchar* ptr(int i0, bool createMissing, size_t* hashval=0);

    uchar* ptr(int i0, int i1, bool createMissing, size_t* hashval=0);

    uchar* ptr(int i0, int i1, int i2, bool createMissing, size_t* hashval=0);

    uchar* ptr(const int* idx, bool createMissing, size_t* hashval=0);





    template<typename _Tp> _Tp& ref(int i0, size_t* hashval=0);

    template<typename _Tp> _Tp& ref(int i0, int i1, size_t* hashval=0);

    template<typename _Tp> _Tp& ref(int i0, int i1, int i2, size_t* hashval=0);

    template<typename _Tp> _Tp& ref(const int* idx, size_t* hashval=0);





    template<typename _Tp> _Tp value(int i0, size_t* hashval=0) const;

    template<typename _Tp> _Tp value(int i0, int i1, size_t* hashval=0) const;

    template<typename _Tp> _Tp value(int i0, int i1, int i2, size_t* hashval=0) const;

    template<typename _Tp> _Tp value(const int* idx, size_t* hashval=0) const;





    template<typename _Tp> const _Tp* find(int i0, size_t* hashval=0) const;

    template<typename _Tp> const _Tp* find(int i0, int i1, size_t* hashval=0) const;

    template<typename _Tp> const _Tp* find(int i0, int i1, int i2, size_t* hashval=0) const;

    template<typename _Tp> const _Tp* find(const int* idx, size_t* hashval=0) const;



    void erase(int i0, int i1, size_t* hashval=0);

    void erase(int i0, int i1, int i2, size_t* hashval=0);

    void erase(const int* idx, size_t* hashval=0);




    SparseMatIterator begin();

    template<typename _Tp> SparseMatIterator_<_Tp> begin();

    SparseMatConstIterator begin() const;

    template<typename _Tp> SparseMatConstIterator_<_Tp> begin() const;



    SparseMatIterator end();

    SparseMatConstIterator end() const;

    template<typename _Tp> SparseMatIterator_<_Tp> end();

    template<typename _Tp> SparseMatConstIterator_<_Tp> end() const;


    template<typename _Tp> _Tp& value(Node* n);

    template<typename _Tp> const _Tp& value(const Node* n) const;


    Node* node(size_t nidx);
    const Node* node(size_t nidx) const;

    uchar* newNode(const int* idx, size_t hashval);
    void removeNode(size_t hidx, size_t nidx, size_t previdx);
    void resizeHashTab(size_t newsize);

    int flags;
    Hdr* hdr;
};






template<typename _Tp> class SparseMat_ : public SparseMat
{
public:
    typedef SparseMatIterator_<_Tp> iterator;
    typedef SparseMatConstIterator_<_Tp> const_iterator;


    SparseMat_();

    SparseMat_(int dims, const int* _sizes);

    SparseMat_(const SparseMat& m);

    SparseMat_(const SparseMat_& m);

    SparseMat_(const Mat& m);



    SparseMat_& operator = (const SparseMat& m);

    SparseMat_& operator = (const SparseMat_& m);

    SparseMat_& operator = (const Mat& m);


    SparseMat_ clone() const CV_NODISCARD;

    void create(int dims, const int* _sizes);




    int type() const;

    int depth() const;

    int channels() const;


    _Tp& ref(int i0, size_t* hashval=0);

    _Tp& ref(int i0, int i1, size_t* hashval=0);

    _Tp& ref(int i0, int i1, int i2, size_t* hashval=0);

    _Tp& ref(const int* idx, size_t* hashval=0);


    _Tp operator()(int i0, size_t* hashval=0) const;

    _Tp operator()(int i0, int i1, size_t* hashval=0) const;

    _Tp operator()(int i0, int i1, int i2, size_t* hashval=0) const;

    _Tp operator()(const int* idx, size_t* hashval=0) const;


    SparseMatIterator_<_Tp> begin();

    SparseMatConstIterator_<_Tp> begin() const;

    SparseMatIterator_<_Tp> end();

    SparseMatConstIterator_<_Tp> end() const;
};





class CV_EXPORTS MatConstIterator
{
public:
    typedef uchar* value_type;
    typedef ptrdiff_t difference_type;
    typedef const uchar** pointer;
    typedef uchar* reference;

    typedef std::random_access_iterator_tag iterator_category;


    MatConstIterator();

    MatConstIterator(const Mat* _m);

    MatConstIterator(const Mat* _m, int _row, int _col=0);

    MatConstIterator(const Mat* _m, Point _pt);

    MatConstIterator(const Mat* _m, const int* _idx);

    MatConstIterator(const MatConstIterator& it);


    MatConstIterator& operator = (const MatConstIterator& it);

    const uchar* operator *() const;

    const uchar* operator [](ptrdiff_t i) const;


    MatConstIterator& operator += (ptrdiff_t ofs);

    MatConstIterator& operator -= (ptrdiff_t ofs);

    MatConstIterator& operator --();

    MatConstIterator operator --(int);

    MatConstIterator& operator ++();

    MatConstIterator operator ++(int);

    Point pos() const;

    void pos(int* _idx) const;

    ptrdiff_t lpos() const;
    void seek(ptrdiff_t ofs, bool relative = false);
    void seek(const int* _idx, bool relative = false);

    const Mat* m;
    size_t elemSize;
    const uchar* ptr;
    const uchar* sliceStart;
    const uchar* sliceEnd;
};






template<typename _Tp>
class MatConstIterator_ : public MatConstIterator
{
public:
    typedef _Tp value_type;
    typedef ptrdiff_t difference_type;
    typedef const _Tp* pointer;
    typedef const _Tp& reference;

    typedef std::random_access_iterator_tag iterator_category;


    MatConstIterator_();

    MatConstIterator_(const Mat_<_Tp>* _m);

    MatConstIterator_(const Mat_<_Tp>* _m, int _row, int _col=0);

    MatConstIterator_(const Mat_<_Tp>* _m, Point _pt);

    MatConstIterator_(const Mat_<_Tp>* _m, const int* _idx);

    MatConstIterator_(const MatConstIterator_& it);


    MatConstIterator_& operator = (const MatConstIterator_& it);

    const _Tp& operator *() const;

    const _Tp& operator [](ptrdiff_t i) const;


    MatConstIterator_& operator += (ptrdiff_t ofs);

    MatConstIterator_& operator -= (ptrdiff_t ofs);

    MatConstIterator_& operator --();

    MatConstIterator_ operator --(int);

    MatConstIterator_& operator ++();

    MatConstIterator_ operator ++(int);

    Point pos() const;
};






template<typename _Tp>
class MatIterator_ : public MatConstIterator_<_Tp>
{
public:
    typedef _Tp* pointer;
    typedef _Tp& reference;

    typedef std::random_access_iterator_tag iterator_category;


    MatIterator_();

    MatIterator_(Mat_<_Tp>* _m);

    MatIterator_(Mat_<_Tp>* _m, int _row, int _col=0);

    MatIterator_(Mat_<_Tp>* _m, Point _pt);

    MatIterator_(Mat_<_Tp>* _m, const int* _idx);

    MatIterator_(const MatIterator_& it);

    MatIterator_& operator = (const MatIterator_<_Tp>& it );


    _Tp& operator *() const;

    _Tp& operator [](ptrdiff_t i) const;


    MatIterator_& operator += (ptrdiff_t ofs);

    MatIterator_& operator -= (ptrdiff_t ofs);

    MatIterator_& operator --();

    MatIterator_ operator --(int);

    MatIterator_& operator ++();

    MatIterator_ operator ++(int);
};






class CV_EXPORTS SparseMatConstIterator
{
public:

    SparseMatConstIterator();

    SparseMatConstIterator(const SparseMat* _m);

    SparseMatConstIterator(const SparseMatConstIterator& it);


    SparseMatConstIterator& operator = (const SparseMatConstIterator& it);


    template<typename _Tp> const _Tp& value() const;

    const SparseMat::Node* node() const;


    SparseMatConstIterator& operator --();

    SparseMatConstIterator operator --(int);

    SparseMatConstIterator& operator ++();

    SparseMatConstIterator operator ++(int);


    void seekEnd();

    const SparseMat* m;
    size_t hashidx;
    uchar* ptr;
};






class CV_EXPORTS SparseMatIterator : public SparseMatConstIterator
{
public:

    SparseMatIterator();

    SparseMatIterator(SparseMat* _m);

    SparseMatIterator(SparseMat* _m, const int* idx);

    SparseMatIterator(const SparseMatIterator& it);


    SparseMatIterator& operator = (const SparseMatIterator& it);

    template<typename _Tp> _Tp& value() const;

    SparseMat::Node* node() const;


    SparseMatIterator& operator ++();

    SparseMatIterator operator ++(int);
};






template<typename _Tp> class SparseMatConstIterator_ : public SparseMatConstIterator
{
public:

    typedef std::forward_iterator_tag iterator_category;


    SparseMatConstIterator_();

    SparseMatConstIterator_(const SparseMat_<_Tp>* _m);
    SparseMatConstIterator_(const SparseMat* _m);

    SparseMatConstIterator_(const SparseMatConstIterator_& it);


    SparseMatConstIterator_& operator = (const SparseMatConstIterator_& it);

    const _Tp& operator *() const;


    SparseMatConstIterator_& operator ++();

    SparseMatConstIterator_ operator ++(int);
};






template<typename _Tp> class SparseMatIterator_ : public SparseMatConstIterator_<_Tp>
{
public:

    typedef std::forward_iterator_tag iterator_category;


    SparseMatIterator_();

    SparseMatIterator_(SparseMat_<_Tp>* _m);
    SparseMatIterator_(SparseMat* _m);

    SparseMatIterator_(const SparseMatIterator_& it);


    SparseMatIterator_& operator = (const SparseMatIterator_& it);

    _Tp& operator *() const;


    SparseMatIterator_& operator ++();

    SparseMatIterator_ operator ++(int);
};






class CV_EXPORTS NAryMatIterator
{
public:

    NAryMatIterator();

    NAryMatIterator(const Mat** arrays, uchar** ptrs, int narrays=-1);

    NAryMatIterator(const Mat** arrays, Mat* planes, int narrays=-1);

    void init(const Mat** arrays, Mat* planes, uchar** ptrs, int narrays=-1);


    NAryMatIterator& operator ++();

    NAryMatIterator operator ++(int);


    const Mat** arrays;

    Mat* planes;

    uchar** ptrs;

    int narrays;

    size_t nplanes;

    size_t size;
protected:
    int iterdepth;
    size_t idx;
};





class CV_EXPORTS MatOp
{
public:
    MatOp();
    virtual ~MatOp();

    virtual bool elementWise(const MatExpr& expr) const;
    virtual void assign(const MatExpr& expr, Mat& m, int type=-1) const = 0;
    virtual void roi(const MatExpr& expr, const Range& rowRange,
                     const Range& colRange, MatExpr& res) const;
    virtual void diag(const MatExpr& expr, int d, MatExpr& res) const;
    virtual void augAssignAdd(const MatExpr& expr, Mat& m) const;
    virtual void augAssignSubtract(const MatExpr& expr, Mat& m) const;
    virtual void augAssignMultiply(const MatExpr& expr, Mat& m) const;
    virtual void augAssignDivide(const MatExpr& expr, Mat& m) const;
    virtual void augAssignAnd(const MatExpr& expr, Mat& m) const;
    virtual void augAssignOr(const MatExpr& expr, Mat& m) const;
    virtual void augAssignXor(const MatExpr& expr, Mat& m) const;

    virtual void add(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res) const;
    virtual void add(const MatExpr& expr1, const Scalar& s, MatExpr& res) const;

    virtual void subtract(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res) const;
    virtual void subtract(const Scalar& s, const MatExpr& expr, MatExpr& res) const;

    virtual void multiply(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res, double scale=1) const;
    virtual void multiply(const MatExpr& expr1, double s, MatExpr& res) const;

    virtual void divide(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res, double scale=1) const;
    virtual void divide(double s, const MatExpr& expr, MatExpr& res) const;

    virtual void abs(const MatExpr& expr, MatExpr& res) const;

    virtual void transpose(const MatExpr& expr, MatExpr& res) const;
    virtual void matmul(const MatExpr& expr1, const MatExpr& expr2, MatExpr& res) const;
    virtual void invert(const MatExpr& expr, int method, MatExpr& res) const;

    virtual Size size(const MatExpr& expr) const;
    virtual int type(const MatExpr& expr) const;
};


class CV_EXPORTS MatExpr
{
public:
    MatExpr();
    explicit MatExpr(const Mat& m);

    MatExpr(const MatOp* _op, int _flags, const Mat& _a = Mat(), const Mat& _b = Mat(),
            const Mat& _c = Mat(), double _alpha = 1, double _beta = 1, const Scalar& _s = Scalar());

    operator Mat() const;
    template<typename _Tp> operator Mat_<_Tp>() const;

    Size size() const;
    int type() const;

    MatExpr row(int y) const;
    MatExpr col(int x) const;
    MatExpr diag(int d = 0) const;
    MatExpr operator()( const Range& rowRange, const Range& colRange ) const;
    MatExpr operator()( const Rect& roi ) const;

    MatExpr t() const;
    MatExpr inv(int method = DECOMP_LU) const;
    MatExpr mul(const MatExpr& e, double scale=1) const;
    MatExpr mul(const Mat& m, double scale=1) const;

    Mat cross(const Mat& m) const;
    double dot(const Mat& m) const;

    const MatOp* op;
    int flags;

    Mat a, b, c;
    double alpha, beta;
    Scalar s;
};





CV_EXPORTS MatExpr operator + (const Mat& a, const Mat& b);
CV_EXPORTS MatExpr operator + (const Mat& a, const Scalar& s);
CV_EXPORTS MatExpr operator + (const Scalar& s, const Mat& a);
CV_EXPORTS MatExpr operator + (const MatExpr& e, const Mat& m);
CV_EXPORTS MatExpr operator + (const Mat& m, const MatExpr& e);
CV_EXPORTS MatExpr operator + (const MatExpr& e, const Scalar& s);
CV_EXPORTS MatExpr operator + (const Scalar& s, const MatExpr& e);
CV_EXPORTS MatExpr operator + (const MatExpr& e1, const MatExpr& e2);
template<typename _Tp, int m, int n> static inline
MatExpr operator + (const Mat& a, const Matx<_Tp, m, n>& b) { return a + Mat(b); }
template<typename _Tp, int m, int n> static inline
MatExpr operator + (const Matx<_Tp, m, n>& a, const Mat& b) { return Mat(a) + b; }

CV_EXPORTS MatExpr operator - (const Mat& a, const Mat& b);
CV_EXPORTS MatExpr operator - (const Mat& a, const Scalar& s);
CV_EXPORTS MatExpr operator - (const Scalar& s, const Mat& a);
CV_EXPORTS MatExpr operator - (const MatExpr& e, const Mat& m);
CV_EXPORTS MatExpr operator - (const Mat& m, const MatExpr& e);
CV_EXPORTS MatExpr operator - (const MatExpr& e, const Scalar& s);
CV_EXPORTS MatExpr operator - (const Scalar& s, const MatExpr& e);
CV_EXPORTS MatExpr operator - (const MatExpr& e1, const MatExpr& e2);
template<typename _Tp, int m, int n> static inline
MatExpr operator - (const Mat& a, const Matx<_Tp, m, n>& b) { return a - Mat(b); }
template<typename _Tp, int m, int n> static inline
MatExpr operator - (const Matx<_Tp, m, n>& a, const Mat& b) { return Mat(a) - b; }

CV_EXPORTS MatExpr operator - (const Mat& m);
CV_EXPORTS MatExpr operator - (const MatExpr& e);

CV_EXPORTS MatExpr operator * (const Mat& a, const Mat& b);
CV_EXPORTS MatExpr operator * (const Mat& a, double s);
CV_EXPORTS MatExpr operator * (double s, const Mat& a);
CV_EXPORTS MatExpr operator * (const MatExpr& e, const Mat& m);
CV_EXPORTS MatExpr operator * (const Mat& m, const MatExpr& e);
CV_EXPORTS MatExpr operator * (const MatExpr& e, double s);
CV_EXPORTS MatExpr operator * (double s, const MatExpr& e);
CV_EXPORTS MatExpr operator * (const MatExpr& e1, const MatExpr& e2);
template<typename _Tp, int m, int n> static inline
MatExpr operator * (const Mat& a, const Matx<_Tp, m, n>& b) { return a * Mat(b); }
template<typename _Tp, int m, int n> static inline
MatExpr operator * (const Matx<_Tp, m, n>& a, const Mat& b) { return Mat(a) * b; }

CV_EXPORTS MatExpr operator / (const Mat& a, const Mat& b);
CV_EXPORTS MatExpr operator / (const Mat& a, double s);
CV_EXPORTS MatExpr operator / (double s, const Mat& a);
CV_EXPORTS MatExpr operator / (const MatExpr& e, const Mat& m);
CV_EXPORTS MatExpr operator / (const Mat& m, const MatExpr& e);
CV_EXPORTS MatExpr operator / (const MatExpr& e, double s);
CV_EXPORTS MatExpr operator / (double s, const MatExpr& e);
CV_EXPORTS MatExpr operator / (const MatExpr& e1, const MatExpr& e2);
template<typename _Tp, int m, int n> static inline
MatExpr operator / (const Mat& a, const Matx<_Tp, m, n>& b) { return a / Mat(b); }
template<typename _Tp, int m, int n> static inline
MatExpr operator / (const Matx<_Tp, m, n>& a, const Mat& b) { return Mat(a) / b; }

CV_EXPORTS MatExpr operator < (const Mat& a, const Mat& b);
CV_EXPORTS MatExpr operator < (const Mat& a, double s);
CV_EXPORTS MatExpr operator < (double s, const Mat& a);
template<typename _Tp, int m, int n> static inline
MatExpr operator < (const Mat& a, const Matx<_Tp, m, n>& b) { return a < Mat(b); }
template<typename _Tp, int m, int n> static inline
MatExpr operator < (const Matx<_Tp, m, n>& a, const Mat& b) { return Mat(a) < b; }

CV_EXPORTS MatExpr operator <= (const Mat& a, const Mat& b);
CV_EXPORTS MatExpr operator <= (const Mat& a, double s);
CV_EXPORTS MatExpr operator <= (double s, const Mat& a);
template<typename _Tp, int m, int n> static inline
MatExpr operator <= (const Mat& a, const Matx<_Tp, m, n>& b) { return a <= Mat(b); }
template<typename _Tp, int m, int n> static inline
MatExpr operator <= (const Matx<_Tp, m, n>& a, const Mat& b) { return Mat(a) <= b; }

CV_EXPORTS MatExpr operator == (const Mat& a, const Mat& b);
CV_EXPORTS MatExpr operator == (const Mat& a, double s);
CV_EXPORTS MatExpr operator == (double s, const Mat& a);
template<typename _Tp, int m, int n> static inline
MatExpr operator == (const Mat& a, const Matx<_Tp, m, n>& b) { return a == Mat(b); }
template<typename _Tp, int m, int n> static inline
MatExpr operator == (const Matx<_Tp, m, n>& a, const Mat& b) { return Mat(a) == b; }

CV_EXPORTS MatExpr operator != (const Mat& a, const Mat& b);
CV_EXPORTS MatExpr operator != (const Mat& a, double s);
CV_EXPORTS MatExpr operator != (double s, const Mat& a);
template<typename _Tp, int m, int n> static inline
MatExpr operator != (const Mat& a, const Matx<_Tp, m, n>& b) { return a != Mat(b); }
template<typename _Tp, int m, int n> static inline
MatExpr operator != (const Matx<_Tp, m, n>& a, const Mat& b) { return Mat(a) != b; }

CV_EXPORTS MatExpr operator >= (const Mat& a, const Mat& b);
CV_EXPORTS MatExpr operator >= (const Mat& a, double s);
CV_EXPORTS MatExpr operator >= (double s, const Mat& a);
template<typename _Tp, int m, int n> static inline
MatExpr operator >= (const Mat& a, const Matx<_Tp, m, n>& b) { return a >= Mat(b); }
template<typename _Tp, int m, int n> static inline
MatExpr operator >= (const Matx<_Tp, m, n>& a, const Mat& b) { return Mat(a) >= b; }

CV_EXPORTS MatExpr operator > (const Mat& a, const Mat& b);
CV_EXPORTS MatExpr operator > (const Mat& a, double s);
CV_EXPORTS MatExpr operator > (double s, const Mat& a);
template<typename _Tp, int m, int n> static inline
MatExpr operator > (const Mat& a, const Matx<_Tp, m, n>& b) { return a > Mat(b); }
template<typename _Tp, int m, int n> static inline
MatExpr operator > (const Matx<_Tp, m, n>& a, const Mat& b) { return Mat(a) > b; }

CV_EXPORTS MatExpr operator & (const Mat& a, const Mat& b);
CV_EXPORTS MatExpr operator & (const Mat& a, const Scalar& s);
CV_EXPORTS MatExpr operator & (const Scalar& s, const Mat& a);
template<typename _Tp, int m, int n> static inline
MatExpr operator & (const Mat& a, const Matx<_Tp, m, n>& b) { return a & Mat(b); }
template<typename _Tp, int m, int n> static inline
MatExpr operator & (const Matx<_Tp, m, n>& a, const Mat& b) { return Mat(a) & b; }

CV_EXPORTS MatExpr operator | (const Mat& a, const Mat& b);
CV_EXPORTS MatExpr operator | (const Mat& a, const Scalar& s);
CV_EXPORTS MatExpr operator | (const Scalar& s, const Mat& a);
template<typename _Tp, int m, int n> static inline
MatExpr operator | (const Mat& a, const Matx<_Tp, m, n>& b) { return a | Mat(b); }
template<typename _Tp, int m, int n> static inline
MatExpr operator | (const Matx<_Tp, m, n>& a, const Mat& b) { return Mat(a) | b; }

CV_EXPORTS MatExpr operator ^ (const Mat& a, const Mat& b);
CV_EXPORTS MatExpr operator ^ (const Mat& a, const Scalar& s);
CV_EXPORTS MatExpr operator ^ (const Scalar& s, const Mat& a);
template<typename _Tp, int m, int n> static inline
MatExpr operator ^ (const Mat& a, const Matx<_Tp, m, n>& b) { return a ^ Mat(b); }
template<typename _Tp, int m, int n> static inline
MatExpr operator ^ (const Matx<_Tp, m, n>& a, const Mat& b) { return Mat(a) ^ b; }

CV_EXPORTS MatExpr operator ~(const Mat& m);

CV_EXPORTS MatExpr min(const Mat& a, const Mat& b);
CV_EXPORTS MatExpr min(const Mat& a, double s);
CV_EXPORTS MatExpr min(double s, const Mat& a);
template<typename _Tp, int m, int n> static inline
MatExpr min (const Mat& a, const Matx<_Tp, m, n>& b) { return min(a, Mat(b)); }
template<typename _Tp, int m, int n> static inline
MatExpr min (const Matx<_Tp, m, n>& a, const Mat& b) { return min(Mat(a), b); }

CV_EXPORTS MatExpr max(const Mat& a, const Mat& b);
CV_EXPORTS MatExpr max(const Mat& a, double s);
CV_EXPORTS MatExpr max(double s, const Mat& a);
template<typename _Tp, int m, int n> static inline
MatExpr max (const Mat& a, const Matx<_Tp, m, n>& b) { return max(a, Mat(b)); }
template<typename _Tp, int m, int n> static inline
MatExpr max (const Matx<_Tp, m, n>& a, const Mat& b) { return max(Mat(a), b); }


CV_EXPORTS MatExpr abs(const Mat& m);

CV_EXPORTS MatExpr abs(const MatExpr& e);


}

#include "opencv2/core/mat.inl.hpp"

#endif
