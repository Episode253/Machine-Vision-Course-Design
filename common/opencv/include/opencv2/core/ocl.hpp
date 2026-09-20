

#ifndef OPENCV_OPENCL_HPP
#define OPENCV_OPENCL_HPP

#include "opencv2/core.hpp"

namespace cv { namespace ocl {




CV_EXPORTS_W bool haveOpenCL();
CV_EXPORTS_W bool useOpenCL();
CV_EXPORTS_W bool haveAmdBlas();
CV_EXPORTS_W bool haveAmdFft();
CV_EXPORTS_W void setUseOpenCL(bool flag);
CV_EXPORTS_W void finish();

CV_EXPORTS bool haveSVM();

class CV_EXPORTS Context;
class CV_EXPORTS_W_SIMPLE Device;
class CV_EXPORTS Kernel;
class CV_EXPORTS Program;
class CV_EXPORTS ProgramSource;
class CV_EXPORTS Queue;
class CV_EXPORTS PlatformInfo;
class CV_EXPORTS Image2D;

class CV_EXPORTS_W_SIMPLE Device
{
public:
    CV_WRAP Device();
    explicit Device(void* d);
    Device(const Device& d);
    Device& operator = (const Device& d);
    CV_WRAP ~Device();

    void set(void* d);

    enum
    {
        TYPE_DEFAULT     = (1 << 0),
        TYPE_CPU         = (1 << 1),
        TYPE_GPU         = (1 << 2),
        TYPE_ACCELERATOR = (1 << 3),
        TYPE_DGPU        = TYPE_GPU + (1 << 16),
        TYPE_IGPU        = TYPE_GPU + (1 << 17),
        TYPE_ALL         = 0xFFFFFFFF
    };

    CV_WRAP String name() const;
    CV_WRAP String extensions() const;
    CV_WRAP bool isExtensionSupported(const String& extensionName) const;
    CV_WRAP String version() const;
    CV_WRAP String vendorName() const;
    CV_WRAP String OpenCL_C_Version() const;
    CV_WRAP String OpenCLVersion() const;
    CV_WRAP int deviceVersionMajor() const;
    CV_WRAP int deviceVersionMinor() const;
    CV_WRAP String driverVersion() const;
    void* ptr() const;

    CV_WRAP int type() const;

    CV_WRAP int addressBits() const;
    CV_WRAP bool available() const;
    CV_WRAP bool compilerAvailable() const;
    CV_WRAP bool linkerAvailable() const;

    enum
    {
        FP_DENORM=(1 << 0),
        FP_INF_NAN=(1 << 1),
        FP_ROUND_TO_NEAREST=(1 << 2),
        FP_ROUND_TO_ZERO=(1 << 3),
        FP_ROUND_TO_INF=(1 << 4),
        FP_FMA=(1 << 5),
        FP_SOFT_FLOAT=(1 << 6),
        FP_CORRECTLY_ROUNDED_DIVIDE_SQRT=(1 << 7)
    };
    CV_WRAP int doubleFPConfig() const;
    CV_WRAP int singleFPConfig() const;
    CV_WRAP int halfFPConfig() const;

    CV_WRAP bool endianLittle() const;
    CV_WRAP bool errorCorrectionSupport() const;

    enum
    {
        EXEC_KERNEL=(1 << 0),
        EXEC_NATIVE_KERNEL=(1 << 1)
    };
    CV_WRAP int executionCapabilities() const;

    CV_WRAP size_t globalMemCacheSize() const;

    enum
    {
        NO_CACHE=0,
        READ_ONLY_CACHE=1,
        READ_WRITE_CACHE=2
    };
    CV_WRAP int globalMemCacheType() const;
    CV_WRAP int globalMemCacheLineSize() const;
    CV_WRAP size_t globalMemSize() const;

    CV_WRAP size_t localMemSize() const;
    enum
    {
        NO_LOCAL_MEM=0,
        LOCAL_IS_LOCAL=1,
        LOCAL_IS_GLOBAL=2
    };
    CV_WRAP int localMemType() const;
    CV_WRAP bool hostUnifiedMemory() const;

    CV_WRAP bool imageSupport() const;

    CV_WRAP bool imageFromBufferSupport() const;
    uint imagePitchAlignment() const;
    uint imageBaseAddressAlignment() const;


    CV_WRAP bool intelSubgroupsSupport() const;

    CV_WRAP size_t image2DMaxWidth() const;
    CV_WRAP size_t image2DMaxHeight() const;

    CV_WRAP size_t image3DMaxWidth() const;
    CV_WRAP size_t image3DMaxHeight() const;
    CV_WRAP size_t image3DMaxDepth() const;

    CV_WRAP size_t imageMaxBufferSize() const;
    CV_WRAP size_t imageMaxArraySize() const;

    enum
    {
        UNKNOWN_VENDOR=0,
        VENDOR_AMD=1,
        VENDOR_INTEL=2,
        VENDOR_NVIDIA=3
    };
    CV_WRAP int vendorID() const;




    CV_WRAP inline bool isAMD() const { return vendorID() == VENDOR_AMD; }
    CV_WRAP inline bool isIntel() const { return vendorID() == VENDOR_INTEL; }
    CV_WRAP inline bool isNVidia() const { return vendorID() == VENDOR_NVIDIA; }

    CV_WRAP int maxClockFrequency() const;
    CV_WRAP int maxComputeUnits() const;
    CV_WRAP int maxConstantArgs() const;
    CV_WRAP size_t maxConstantBufferSize() const;

    CV_WRAP size_t maxMemAllocSize() const;
    CV_WRAP size_t maxParameterSize() const;

    CV_WRAP int maxReadImageArgs() const;
    CV_WRAP int maxWriteImageArgs() const;
    CV_WRAP int maxSamplers() const;

    CV_WRAP size_t maxWorkGroupSize() const;
    CV_WRAP int maxWorkItemDims() const;
    void maxWorkItemSizes(size_t*) const;

    CV_WRAP int memBaseAddrAlign() const;

    CV_WRAP int nativeVectorWidthChar() const;
    CV_WRAP int nativeVectorWidthShort() const;
    CV_WRAP int nativeVectorWidthInt() const;
    CV_WRAP int nativeVectorWidthLong() const;
    CV_WRAP int nativeVectorWidthFloat() const;
    CV_WRAP int nativeVectorWidthDouble() const;
    CV_WRAP int nativeVectorWidthHalf() const;

    CV_WRAP int preferredVectorWidthChar() const;
    CV_WRAP int preferredVectorWidthShort() const;
    CV_WRAP int preferredVectorWidthInt() const;
    CV_WRAP int preferredVectorWidthLong() const;
    CV_WRAP int preferredVectorWidthFloat() const;
    CV_WRAP int preferredVectorWidthDouble() const;
    CV_WRAP int preferredVectorWidthHalf() const;

    CV_WRAP size_t printfBufferSize() const;
    CV_WRAP size_t profilingTimerResolution() const;

    CV_WRAP static const Device& getDefault();

protected:
    struct Impl;
    Impl* p;
};


class CV_EXPORTS Context
{
public:
    Context();
    explicit Context(int dtype);
    ~Context();
    Context(const Context& c);
    Context& operator = (const Context& c);

    bool create();
    bool create(int dtype);
    size_t ndevices() const;
    const Device& device(size_t idx) const;
    Program getProg(const ProgramSource& prog,
                    const String& buildopt, String& errmsg);
    void unloadProg(Program& prog);

    static Context& getDefault(bool initialize = true);
    void* ptr() const;

    friend void initializeContextFromHandle(Context& ctx, void* platform, void* context, void* device);

    bool useSVM() const;
    void setUseSVM(bool enabled);

    struct Impl;
    inline Impl* getImpl() const { return (Impl*)p; }

    Impl* p;
};

class CV_EXPORTS Platform
{
public:
    Platform();
    ~Platform();
    Platform(const Platform& p);
    Platform& operator = (const Platform& p);

    void* ptr() const;
    static Platform& getDefault();

    friend void initializeContextFromHandle(Context& ctx, void* platform, void* context, void* device);
protected:
    struct Impl;
    Impl* p;
};


CV_EXPORTS void attachContext(const String& platformName, void* platformID, void* context, void* deviceID);


CV_EXPORTS void convertFromBuffer(void* cl_mem_buffer, size_t step, int rows, int cols, int type, UMat& dst);


CV_EXPORTS void convertFromImage(void* cl_mem_image, UMat& dst);


void initializeContextFromHandle(Context& ctx, void* platform, void* context, void* device);

class CV_EXPORTS Queue
{
public:
    Queue();
    explicit Queue(const Context& c, const Device& d=Device());
    ~Queue();
    Queue(const Queue& q);
    Queue& operator = (const Queue& q);

    bool create(const Context& c=Context(), const Device& d=Device());
    void finish();
    void* ptr() const;
    static Queue& getDefault();


    const Queue& getProfilingQueue() const;

    struct Impl; friend struct Impl;
    inline Impl* getImpl() const { return p; }
protected:
    Impl* p;
};


class CV_EXPORTS KernelArg
{
public:
    enum { LOCAL=1, READ_ONLY=2, WRITE_ONLY=4, READ_WRITE=6, CONSTANT=8, PTR_ONLY = 16, NO_SIZE=256 };
    KernelArg(int _flags, UMat* _m, int wscale=1, int iwscale=1, const void* _obj=0, size_t _sz=0);
    KernelArg();

    static KernelArg Local(size_t localMemSize)
    { return KernelArg(LOCAL, 0, 1, 1, 0, localMemSize); }
    static KernelArg PtrWriteOnly(const UMat& m)
    { return KernelArg(PTR_ONLY+WRITE_ONLY, (UMat*)&m); }
    static KernelArg PtrReadOnly(const UMat& m)
    { return KernelArg(PTR_ONLY+READ_ONLY, (UMat*)&m); }
    static KernelArg PtrReadWrite(const UMat& m)
    { return KernelArg(PTR_ONLY+READ_WRITE, (UMat*)&m); }
    static KernelArg ReadWrite(const UMat& m, int wscale=1, int iwscale=1)
    { return KernelArg(READ_WRITE, (UMat*)&m, wscale, iwscale); }
    static KernelArg ReadWriteNoSize(const UMat& m, int wscale=1, int iwscale=1)
    { return KernelArg(READ_WRITE+NO_SIZE, (UMat*)&m, wscale, iwscale); }
    static KernelArg ReadOnly(const UMat& m, int wscale=1, int iwscale=1)
    { return KernelArg(READ_ONLY, (UMat*)&m, wscale, iwscale); }
    static KernelArg WriteOnly(const UMat& m, int wscale=1, int iwscale=1)
    { return KernelArg(WRITE_ONLY, (UMat*)&m, wscale, iwscale); }
    static KernelArg ReadOnlyNoSize(const UMat& m, int wscale=1, int iwscale=1)
    { return KernelArg(READ_ONLY+NO_SIZE, (UMat*)&m, wscale, iwscale); }
    static KernelArg WriteOnlyNoSize(const UMat& m, int wscale=1, int iwscale=1)
    { return KernelArg(WRITE_ONLY+NO_SIZE, (UMat*)&m, wscale, iwscale); }
    static KernelArg Constant(const Mat& m);
    template<typename _Tp> static KernelArg Constant(const _Tp* arr, size_t n)
    { return KernelArg(CONSTANT, 0, 1, 1, (void*)arr, n); }

    int flags;
    UMat* m;
    const void* obj;
    size_t sz;
    int wscale, iwscale;
};


class CV_EXPORTS Kernel
{
public:
    Kernel();
    Kernel(const char* kname, const Program& prog);
    Kernel(const char* kname, const ProgramSource& prog,
           const String& buildopts = String(), String* errmsg=0);
    ~Kernel();
    Kernel(const Kernel& k);
    Kernel& operator = (const Kernel& k);

    bool empty() const;
    bool create(const char* kname, const Program& prog);
    bool create(const char* kname, const ProgramSource& prog,
                const String& buildopts, String* errmsg=0);

    int set(int i, const void* value, size_t sz);
    int set(int i, const Image2D& image2D);
    int set(int i, const UMat& m);
    int set(int i, const KernelArg& arg);
    template<typename _Tp> int set(int i, const _Tp& value)
    { return set(i, &value, sizeof(value)); }


protected:
    template<typename _Tp0> inline
    int set_args_(int i, const _Tp0& a0) { return set(i, a0); }
    template<typename _Tp0, typename... _Tps> inline
    int set_args_(int i, const _Tp0& a0, const _Tps&... rest_args) { i = set(i, a0); return set_args_(i, rest_args...); }
public:

    template<typename... _Tps> inline
    Kernel& args(const _Tps&... kernel_args) { set_args_(0, kernel_args...); return *this; }



    bool run(int dims, size_t globalsize[],
             size_t localsize[], bool sync, const Queue& q=Queue());
    bool runTask(bool sync, const Queue& q=Queue());


    int64 runProfiling(int dims, size_t globalsize[], size_t localsize[], const Queue& q=Queue());

    size_t workGroupSize() const;
    size_t preferedWorkGroupSizeMultiple() const;
    bool compileWorkGroupSize(size_t wsz[]) const;
    size_t localMemSize() const;

    void* ptr() const;
    struct Impl;

protected:
    Impl* p;
};

class CV_EXPORTS Program
{
public:
    Program();
    Program(const ProgramSource& src,
            const String& buildflags, String& errmsg);
    Program(const Program& prog);

    Program& operator = (const Program& prog);
    ~Program();

    bool create(const ProgramSource& src,
                const String& buildflags, String& errmsg);

    void* ptr() const;


    void getBinary(std::vector<char>& binary) const;

    struct Impl; friend struct Impl;
    inline Impl* getImpl() const { return (Impl*)p; }
protected:
    Impl* p;
public:
#ifndef OPENCV_REMOVE_DEPRECATED_API

    CV_DEPRECATED bool read(const String& buf, const String& buildflags);
    CV_DEPRECATED bool write(String& buf) const;
    CV_DEPRECATED const ProgramSource& source() const;
    CV_DEPRECATED String getPrefix() const;
    CV_DEPRECATED static String getPrefix(const String& buildflags);
#endif
};


class CV_EXPORTS ProgramSource
{
public:
    typedef uint64 hash_t;

    ProgramSource();
    explicit ProgramSource(const String& module, const String& name, const String& codeStr, const String& codeHash);
    explicit ProgramSource(const String& prog);
    explicit ProgramSource(const char* prog);
    ~ProgramSource();
    ProgramSource(const ProgramSource& prog);
    ProgramSource& operator = (const ProgramSource& prog);

    const String& source() const;
    hash_t hash() const;



    static ProgramSource fromBinary(const String& module, const String& name,
            const unsigned char* binary, const size_t size,
            const cv::String& buildOptions = cv::String());


    static ProgramSource fromSPIR(const String& module, const String& name,
            const unsigned char* binary, const size_t size,
            const cv::String& buildOptions = cv::String());






    struct Impl; friend struct Impl;
    inline Impl* getImpl() const { return (Impl*)p; }
protected:
    Impl* p;
};

class CV_EXPORTS PlatformInfo
{
public:
    PlatformInfo();
    explicit PlatformInfo(void* id);
    ~PlatformInfo();

    PlatformInfo(const PlatformInfo& i);
    PlatformInfo& operator =(const PlatformInfo& i);

    String name() const;
    String vendor() const;
    String version() const;
    int deviceNumber() const;
    void getDevice(Device& device, int d) const;

protected:
    struct Impl;
    Impl* p;
};

CV_EXPORTS const char* convertTypeStr(int sdepth, int ddepth, int cn, char* buf);
CV_EXPORTS const char* typeToStr(int t);
CV_EXPORTS const char* memopTypeToStr(int t);
CV_EXPORTS const char* vecopTypeToStr(int t);
CV_EXPORTS const char* getOpenCLErrorString(int errorCode);
CV_EXPORTS String kernelToStr(InputArray _kernel, int ddepth = -1, const char * name = NULL);
CV_EXPORTS void getPlatfomsInfo(std::vector<PlatformInfo>& platform_info);


enum OclVectorStrategy
{

    OCL_VECTOR_OWN = 0,


    OCL_VECTOR_MAX = 1,


    OCL_VECTOR_DEFAULT = OCL_VECTOR_OWN
};

CV_EXPORTS int predictOptimalVectorWidth(InputArray src1, InputArray src2 = noArray(), InputArray src3 = noArray(),
                                         InputArray src4 = noArray(), InputArray src5 = noArray(), InputArray src6 = noArray(),
                                         InputArray src7 = noArray(), InputArray src8 = noArray(), InputArray src9 = noArray(),
                                         OclVectorStrategy strat = OCL_VECTOR_DEFAULT);

CV_EXPORTS int checkOptimalVectorWidth(const int *vectorWidths,
                                       InputArray src1, InputArray src2 = noArray(), InputArray src3 = noArray(),
                                       InputArray src4 = noArray(), InputArray src5 = noArray(), InputArray src6 = noArray(),
                                       InputArray src7 = noArray(), InputArray src8 = noArray(), InputArray src9 = noArray(),
                                       OclVectorStrategy strat = OCL_VECTOR_DEFAULT);


CV_EXPORTS int predictOptimalVectorWidthMax(InputArray src1, InputArray src2 = noArray(), InputArray src3 = noArray(),
                                            InputArray src4 = noArray(), InputArray src5 = noArray(), InputArray src6 = noArray(),
                                            InputArray src7 = noArray(), InputArray src8 = noArray(), InputArray src9 = noArray());

CV_EXPORTS void buildOptionsAddMatrixDescription(String& buildOptions, const String& name, InputArray _m);

class CV_EXPORTS Image2D
{
public:
    Image2D();


    explicit Image2D(const UMat &src, bool norm = false, bool alias = false);
    Image2D(const Image2D & i);
    ~Image2D();

    Image2D & operator = (const Image2D & i);


    static bool canCreateAlias(const UMat &u);


    static bool isFormatSupported(int depth, int cn, bool norm);

    void* ptr() const;
protected:
    struct Impl;
    Impl* p;
};

class CV_EXPORTS Timer
{
public:
    Timer(const Queue& q);
    ~Timer();
    void start();
    void stop();

    uint64 durationNS() const;

protected:
    struct Impl;
    Impl* const p;

private:
    Timer(const Timer&);
    Timer& operator=(const Timer&);
};

CV_EXPORTS MatAllocator* getOpenCLAllocator();


#ifdef __OPENCV_BUILD
namespace internal {

CV_EXPORTS bool isOpenCLForced();
#define OCL_FORCE_CHECK(condition) (cv::ocl::internal::isOpenCLForced() || (condition))

CV_EXPORTS bool isPerformanceCheckBypassed();
#define OCL_PERFORMANCE_CHECK(condition) (cv::ocl::internal::isPerformanceCheckBypassed() || (condition))

CV_EXPORTS bool isCLBuffer(UMat& u);

}
#endif



}}

#endif
