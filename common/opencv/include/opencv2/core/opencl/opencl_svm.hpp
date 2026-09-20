

#ifndef OPENCV_CORE_OPENCL_SVM_HPP
#define OPENCV_CORE_OPENCL_SVM_HPP




#ifndef __OPENCV_BUILD
#error Internal header file
#endif

#if defined(HAVE_OPENCL) && defined(HAVE_OPENCL_SVM)
#include "runtime/opencl_core.hpp"
#include "runtime/opencl_svm_20.hpp"
#include "runtime/opencl_svm_hsa_extension.hpp"

namespace cv { namespace ocl { namespace svm {

struct SVMCapabilities
{
    enum Value
    {
        SVM_COARSE_GRAIN_BUFFER = (1 << 0),
        SVM_FINE_GRAIN_BUFFER = (1 << 1),
        SVM_FINE_GRAIN_SYSTEM = (1 << 2),
        SVM_ATOMICS = (1 << 3),
    };
    int value_;

    SVMCapabilities(int capabilities = 0) : value_(capabilities) { }
    operator int() const { return value_; }

    inline bool isNoSVMSupport() const { return value_ == 0; }
    inline bool isSupportCoarseGrainBuffer() const { return (value_ & SVM_COARSE_GRAIN_BUFFER) != 0; }
    inline bool isSupportFineGrainBuffer() const { return (value_ & SVM_FINE_GRAIN_BUFFER) != 0; }
    inline bool isSupportFineGrainSystem() const { return (value_ & SVM_FINE_GRAIN_SYSTEM) != 0; }
    inline bool isSupportAtomics() const { return (value_ & SVM_ATOMICS) != 0; }
};

CV_EXPORTS const SVMCapabilities getSVMCapabilitites(const ocl::Context& context);

struct SVMFunctions
{
    clSVMAllocAMD_fn fn_clSVMAlloc;
    clSVMFreeAMD_fn fn_clSVMFree;
    clSetKernelArgSVMPointerAMD_fn fn_clSetKernelArgSVMPointer;


    clEnqueueSVMMemcpyAMD_fn fn_clEnqueueSVMMemcpy;
    clEnqueueSVMMemFillAMD_fn fn_clEnqueueSVMMemFill;
    clEnqueueSVMMapAMD_fn fn_clEnqueueSVMMap;
    clEnqueueSVMUnmapAMD_fn fn_clEnqueueSVMUnmap;

    inline SVMFunctions()
        : fn_clSVMAlloc(NULL), fn_clSVMFree(NULL),
          fn_clSetKernelArgSVMPointer(NULL),
           fn_clEnqueueSVMMemcpy(NULL), fn_clEnqueueSVMMemFill(NULL),
          fn_clEnqueueSVMMap(NULL), fn_clEnqueueSVMUnmap(NULL)
    {

    }

    inline bool isValid() const
    {
        return fn_clSVMAlloc != NULL && fn_clSVMFree && fn_clSetKernelArgSVMPointer &&
                 fn_clEnqueueSVMMemcpy &&
                fn_clEnqueueSVMMemFill && fn_clEnqueueSVMMap && fn_clEnqueueSVMUnmap;
    }
};


CV_EXPORTS const SVMFunctions* getSVMFunctions(const ocl::Context& context);

CV_EXPORTS bool useSVM(UMatUsageFlags usageFlags);

}}}
#endif

#endif

