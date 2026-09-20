

#ifndef OPENCV_CORE_CUDA_HPP
#define OPENCV_CORE_CUDA_HPP

#ifndef __cplusplus
#  error cuda.hpp header must be compiled as C++
#endif

#include "opencv2/core.hpp"
#include "opencv2/core/cuda_types.hpp"



namespace cv { namespace cuda {









class CV_EXPORTS_W GpuMat
{
public:
    class CV_EXPORTS_W Allocator
    {
    public:
        virtual ~Allocator() {}


        virtual bool allocate(GpuMat* mat, int rows, int cols, size_t elemSize) = 0;
        virtual void free(GpuMat* mat) = 0;
    };


    CV_WRAP static GpuMat::Allocator* defaultAllocator();
    CV_WRAP static void setDefaultAllocator(GpuMat::Allocator* allocator);


    CV_WRAP explicit GpuMat(GpuMat::Allocator* allocator = GpuMat::defaultAllocator());


    CV_WRAP GpuMat(int rows, int cols, int type, GpuMat::Allocator* allocator = GpuMat::defaultAllocator());
    CV_WRAP GpuMat(Size size, int type, GpuMat::Allocator* allocator = GpuMat::defaultAllocator());


    CV_WRAP GpuMat(int rows, int cols, int type, Scalar s, GpuMat::Allocator* allocator = GpuMat::defaultAllocator());
    CV_WRAP GpuMat(Size size, int type, Scalar s, GpuMat::Allocator* allocator = GpuMat::defaultAllocator());


    CV_WRAP GpuMat(const GpuMat& m);


    GpuMat(int rows, int cols, int type, void* data, size_t step = Mat::AUTO_STEP);
    GpuMat(Size size, int type, void* data, size_t step = Mat::AUTO_STEP);


    CV_WRAP GpuMat(const GpuMat& m, Range rowRange, Range colRange);
    CV_WRAP GpuMat(const GpuMat& m, Rect roi);


    CV_WRAP explicit GpuMat(InputArray arr, GpuMat::Allocator* allocator = GpuMat::defaultAllocator());


    ~GpuMat();


    GpuMat& operator =(const GpuMat& m);


    CV_WRAP void create(int rows, int cols, int type);
    CV_WRAP void create(Size size, int type);


    void release();


    CV_WRAP void swap(GpuMat& mat);


    CV_WRAP void upload(InputArray arr);


    CV_WRAP void upload(InputArray arr, Stream& stream);


    CV_WRAP void download(OutputArray dst) const;


    CV_WRAP void download(OutputArray dst, Stream& stream) const;


    CV_WRAP GpuMat clone() const;


    CV_WRAP void copyTo(OutputArray dst) const;


    CV_WRAP void copyTo(OutputArray dst, Stream& stream) const;


    CV_WRAP void copyTo(OutputArray dst, InputArray mask) const;


    CV_WRAP void copyTo(OutputArray dst, InputArray mask, Stream& stream) const;


    CV_WRAP GpuMat& setTo(Scalar s);


    CV_WRAP GpuMat& setTo(Scalar s, Stream& stream);


    CV_WRAP GpuMat& setTo(Scalar s, InputArray mask);


    CV_WRAP GpuMat& setTo(Scalar s, InputArray mask, Stream& stream);


    CV_WRAP void convertTo(OutputArray dst, int rtype) const;


    CV_WRAP void convertTo(OutputArray dst, int rtype, Stream& stream) const;


    CV_WRAP void convertTo(OutputArray dst, int rtype, double alpha, double beta = 0.0) const;


    CV_WRAP void convertTo(OutputArray dst, int rtype, double alpha, Stream& stream) const;


    CV_WRAP void convertTo(OutputArray dst, int rtype, double alpha, double beta, Stream& stream) const;

    CV_WRAP void assignTo(GpuMat& m, int type = -1) const;


    uchar* ptr(int y = 0);
    const uchar* ptr(int y = 0) const;


    template<typename _Tp> _Tp* ptr(int y = 0);
    template<typename _Tp> const _Tp* ptr(int y = 0) const;

    template <typename _Tp> operator PtrStepSz<_Tp>() const;
    template <typename _Tp> operator PtrStep<_Tp>() const;


    CV_WRAP GpuMat row(int y) const;


    CV_WRAP GpuMat col(int x) const;


    CV_WRAP GpuMat rowRange(int startrow, int endrow) const;
    CV_WRAP GpuMat rowRange(Range r) const;


    CV_WRAP GpuMat colRange(int startcol, int endcol) const;
    CV_WRAP GpuMat colRange(Range r) const;


    GpuMat operator ()(Range rowRange, Range colRange) const;
    GpuMat operator ()(Rect roi) const;



    CV_WRAP GpuMat reshape(int cn, int rows = 0) const;


    CV_WRAP void locateROI(Size& wholeSize, Point& ofs) const;


    CV_WRAP GpuMat& adjustROI(int dtop, int dbottom, int dleft, int dright);



    CV_WRAP bool isContinuous() const;


    CV_WRAP size_t elemSize() const;


    CV_WRAP size_t elemSize1() const;


    CV_WRAP int type() const;


    CV_WRAP int depth() const;


    CV_WRAP int channels() const;


    CV_WRAP size_t step1() const;


    CV_WRAP Size size() const;


    CV_WRAP bool empty() const;


    CV_WRAP void updateContinuityFlag();


    int flags;


    int rows, cols;


    CV_PROP size_t step;


    uchar* data;



    int* refcount;


    uchar* datastart;
    const uchar* dataend;


    Allocator* allocator;
};


CV_EXPORTS_W void createContinuous(int rows, int cols, int type, OutputArray arr);


CV_EXPORTS_W void ensureSizeIsEnough(int rows, int cols, int type, OutputArray arr);


class CV_EXPORTS_W BufferPool
{
public:


    explicit BufferPool(Stream& stream);


    CV_WRAP GpuMat getBuffer(int rows, int cols, int type);


    CV_WRAP GpuMat getBuffer(Size size, int type) { return getBuffer(size.height, size.width, type); }


    CV_WRAP Ptr<GpuMat::Allocator> getAllocator() const { return allocator_; }

private:
    Ptr<GpuMat::Allocator> allocator_;
};


CV_EXPORTS_W void setBufferPoolUsage(bool on);
CV_EXPORTS_W void setBufferPoolConfig(int deviceId, size_t stackSize, int stackCount);






class CV_EXPORTS_W HostMem
{
public:
    enum AllocType { PAGE_LOCKED = 1, SHARED = 2, WRITE_COMBINED = 4 };

    static MatAllocator* getAllocator(HostMem::AllocType alloc_type = HostMem::AllocType::PAGE_LOCKED);

    CV_WRAP explicit HostMem(HostMem::AllocType alloc_type = HostMem::AllocType::PAGE_LOCKED);

    HostMem(const HostMem& m);

    CV_WRAP HostMem(int rows, int cols, int type, HostMem::AllocType alloc_type = HostMem::AllocType::PAGE_LOCKED);
    CV_WRAP HostMem(Size size, int type, HostMem::AllocType alloc_type = HostMem::AllocType::PAGE_LOCKED);


    CV_WRAP explicit HostMem(InputArray arr, HostMem::AllocType alloc_type = HostMem::AllocType::PAGE_LOCKED);

    ~HostMem();

    HostMem& operator =(const HostMem& m);


    CV_WRAP void swap(HostMem& b);


    CV_WRAP HostMem clone() const;


    CV_WRAP void create(int rows, int cols, int type);
    void create(Size size, int type);



    CV_WRAP HostMem reshape(int cn, int rows = 0) const;


    void release();


    CV_WRAP Mat createMatHeader() const;


    GpuMat createGpuMatHeader() const;


    CV_WRAP bool isContinuous() const;
    CV_WRAP size_t elemSize() const;
    CV_WRAP size_t elemSize1() const;
    CV_WRAP int type() const;
    CV_WRAP int depth() const;
    CV_WRAP int channels() const;
    CV_WRAP size_t step1() const;
    CV_WRAP Size size() const;
    CV_WRAP bool empty() const;


    int flags;
    int rows, cols;
    CV_PROP size_t step;

    uchar* data;
    int* refcount;

    uchar* datastart;
    const uchar* dataend;

    AllocType alloc_type;
};


CV_EXPORTS_W void registerPageLocked(Mat& m);


CV_EXPORTS_W void unregisterPageLocked(Mat& m);






class CV_EXPORTS_W Stream
{
    typedef void (Stream::*bool_type)() const;
    void this_type_does_not_support_comparisons() const {}

public:
    typedef void (*StreamCallback)(int status, void* userData);


    CV_WRAP Stream();


    CV_WRAP Stream(const Ptr<GpuMat::Allocator>& allocator);


    CV_WRAP bool queryIfComplete() const;


    CV_WRAP void waitForCompletion();


    CV_WRAP void waitEvent(const Event& event);


    void enqueueHostCallback(StreamCallback callback, void* userData);


    CV_WRAP static Stream& Null();


    operator bool_type() const;

    class Impl;

private:
    Ptr<Impl> impl_;
    Stream(const Ptr<Impl>& impl);

    friend struct StreamAccessor;
    friend class BufferPool;
    friend class DefaultDeviceInitializer;
};

class CV_EXPORTS_W Event
{
public:
    enum CreateFlags
    {
        DEFAULT        = 0x00,
        BLOCKING_SYNC  = 0x01,
        DISABLE_TIMING = 0x02,
        INTERPROCESS   = 0x04
    };

    CV_WRAP explicit Event(Event::CreateFlags flags = Event::CreateFlags::DEFAULT);


    CV_WRAP void record(Stream& stream = Stream::Null());


    CV_WRAP bool queryIfComplete() const;


    CV_WRAP void waitForCompletion();


    CV_WRAP static float elapsedTime(const Event& start, const Event& end);

    class Impl;

private:
    Ptr<Impl> impl_;
    Event(const Ptr<Impl>& impl);

    friend struct EventAccessor;
};











CV_EXPORTS_W int getCudaEnabledDeviceCount();


CV_EXPORTS_W void setDevice(int device);


CV_EXPORTS_W int getDevice();


CV_EXPORTS_W void resetDevice();


enum FeatureSet
{
    FEATURE_SET_COMPUTE_10 = 10,
    FEATURE_SET_COMPUTE_11 = 11,
    FEATURE_SET_COMPUTE_12 = 12,
    FEATURE_SET_COMPUTE_13 = 13,
    FEATURE_SET_COMPUTE_20 = 20,
    FEATURE_SET_COMPUTE_21 = 21,
    FEATURE_SET_COMPUTE_30 = 30,
    FEATURE_SET_COMPUTE_32 = 32,
    FEATURE_SET_COMPUTE_35 = 35,
    FEATURE_SET_COMPUTE_50 = 50,

    GLOBAL_ATOMICS = FEATURE_SET_COMPUTE_11,
    SHARED_ATOMICS = FEATURE_SET_COMPUTE_12,
    NATIVE_DOUBLE = FEATURE_SET_COMPUTE_13,
    WARP_SHUFFLE_FUNCTIONS = FEATURE_SET_COMPUTE_30,
    DYNAMIC_PARALLELISM = FEATURE_SET_COMPUTE_35
};


CV_EXPORTS bool deviceSupports(FeatureSet feature_set);


class CV_EXPORTS_W TargetArchs
{
public:

    static bool builtWith(FeatureSet feature_set);


    CV_WRAP static bool has(int major, int minor);
    CV_WRAP static bool hasPtx(int major, int minor);
    CV_WRAP static bool hasBin(int major, int minor);

    CV_WRAP static bool hasEqualOrLessPtx(int major, int minor);
    CV_WRAP static bool hasEqualOrGreater(int major, int minor);
    CV_WRAP static bool hasEqualOrGreaterPtx(int major, int minor);
    CV_WRAP static bool hasEqualOrGreaterBin(int major, int minor);
};


class CV_EXPORTS_W DeviceInfo
{
public:

    CV_WRAP DeviceInfo();


    CV_WRAP DeviceInfo(int device_id);


    CV_WRAP int deviceID() const;


    const char* name() const;


    CV_WRAP size_t totalGlobalMem() const;


    CV_WRAP size_t sharedMemPerBlock() const;


    CV_WRAP int regsPerBlock() const;


    CV_WRAP int warpSize() const;


    CV_WRAP size_t memPitch() const;


    CV_WRAP int maxThreadsPerBlock() const;


    CV_WRAP Vec3i maxThreadsDim() const;


    CV_WRAP Vec3i maxGridSize() const;


    CV_WRAP int clockRate() const;


    CV_WRAP size_t totalConstMem() const;


    CV_WRAP int majorVersion() const;


    CV_WRAP int minorVersion() const;


    CV_WRAP size_t textureAlignment() const;


    CV_WRAP size_t texturePitchAlignment() const;


    CV_WRAP int multiProcessorCount() const;


    CV_WRAP bool kernelExecTimeoutEnabled() const;


    CV_WRAP bool integrated() const;


    CV_WRAP bool canMapHostMemory() const;

    enum ComputeMode
    {
        ComputeModeDefault,
        ComputeModeExclusive,
        ComputeModeProhibited,
        ComputeModeExclusiveProcess
    };


    CV_WRAP DeviceInfo::ComputeMode computeMode() const;


    CV_WRAP int maxTexture1D() const;


    CV_WRAP int maxTexture1DMipmap() const;


    CV_WRAP int maxTexture1DLinear() const;


    CV_WRAP Vec2i maxTexture2D() const;


    CV_WRAP Vec2i maxTexture2DMipmap() const;


    CV_WRAP Vec3i maxTexture2DLinear() const;


    CV_WRAP Vec2i maxTexture2DGather() const;


    CV_WRAP Vec3i maxTexture3D() const;


    CV_WRAP int maxTextureCubemap() const;


    CV_WRAP Vec2i maxTexture1DLayered() const;


    CV_WRAP Vec3i maxTexture2DLayered() const;


    CV_WRAP Vec2i maxTextureCubemapLayered() const;


    CV_WRAP int maxSurface1D() const;


    CV_WRAP Vec2i maxSurface2D() const;


    CV_WRAP Vec3i maxSurface3D() const;


    CV_WRAP Vec2i maxSurface1DLayered() const;


    CV_WRAP Vec3i maxSurface2DLayered() const;


    CV_WRAP int maxSurfaceCubemap() const;


    CV_WRAP Vec2i maxSurfaceCubemapLayered() const;


    CV_WRAP size_t surfaceAlignment() const;


    CV_WRAP bool concurrentKernels() const;


    CV_WRAP bool ECCEnabled() const;


    CV_WRAP int pciBusID() const;


    CV_WRAP int pciDeviceID() const;


    CV_WRAP int pciDomainID() const;


    CV_WRAP bool tccDriver() const;


    CV_WRAP int asyncEngineCount() const;


    CV_WRAP bool unifiedAddressing() const;


    CV_WRAP int memoryClockRate() const;


    CV_WRAP int memoryBusWidth() const;


    CV_WRAP int l2CacheSize() const;


    CV_WRAP int maxThreadsPerMultiProcessor() const;


    CV_WRAP void queryMemory(size_t& totalMemory, size_t& freeMemory) const;
    CV_WRAP size_t freeMemory() const;
    CV_WRAP size_t totalMemory() const;


    bool supports(FeatureSet feature_set) const;


    CV_WRAP bool isCompatible() const;

private:
    int device_id_;
};

CV_EXPORTS_W void printCudaDeviceInfo(int device);
CV_EXPORTS_W void printShortCudaDeviceInfo(int device);


CV_EXPORTS void convertFp16(InputArray _src, OutputArray _dst, Stream& stream = Stream::Null());



}}


#include "opencv2/core/cuda.inl.hpp"

#endif
