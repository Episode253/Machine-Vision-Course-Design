

#ifndef OPENCV_CORE_OCL_RUNTIME_OPENCL_SVM_HSA_EXTENSION_HPP
#define OPENCV_CORE_OCL_RUNTIME_OPENCL_SVM_HSA_EXTENSION_HPP

#if defined(HAVE_OPENCL_SVM)
#include "opencl_core.hpp"

#ifndef CL_DEVICE_SVM_CAPABILITIES_AMD







typedef cl_bitfield                      cl_device_svm_capabilities_amd;
typedef cl_bitfield                      cl_svm_mem_flags_amd;
typedef cl_uint                          cl_kernel_exec_info_amd;


#define CL_DEVICE_SVM_CAPABILITIES_AMD                     0x1053
#define CL_DEVICE_PREFERRED_PLATFORM_ATOMIC_ALIGNMENT_AMD  0x1054


#define CL_DEVICE_SVM_COARSE_GRAIN_BUFFER_AMD             (1 << 0)
#define CL_DEVICE_SVM_FINE_GRAIN_BUFFER_AMD               (1 << 1)
#define CL_DEVICE_SVM_FINE_GRAIN_SYSTEM_AMD               (1 << 2)
#define CL_DEVICE_SVM_ATOMICS_AMD                         (1 << 3)


#define CL_MEM_SVM_FINE_GRAIN_BUFFER_AMD                  (1 << 10)
#define CL_MEM_SVM_ATOMICS_AMD                            (1 << 11)


#define CL_MEM_USES_SVM_POINTER_AMD                       0x1109


#define CL_KERNEL_EXEC_INFO_SVM_PTRS_AMD                  0x11B6
#define CL_KERNEL_EXEC_INFO_SVM_FINE_GRAIN_SYSTEM_AMD     0x11B7


#define CL_COMMAND_SVM_FREE_AMD                           0x1209
#define CL_COMMAND_SVM_MEMCPY_AMD                         0x120A
#define CL_COMMAND_SVM_MEMFILL_AMD                        0x120B
#define CL_COMMAND_SVM_MAP_AMD                            0x120C
#define CL_COMMAND_SVM_UNMAP_AMD                          0x120D

typedef CL_API_ENTRY void*
(CL_API_CALL * clSVMAllocAMD_fn)(
    cl_context            ,
    cl_svm_mem_flags_amd  ,
    size_t                ,
    unsigned int
) CL_EXT_SUFFIX__VERSION_1_2;

typedef CL_API_ENTRY void
(CL_API_CALL * clSVMFreeAMD_fn)(
    cl_context  ,
    void*
) CL_EXT_SUFFIX__VERSION_1_2;

typedef CL_API_ENTRY cl_int
(CL_API_CALL * clEnqueueSVMFreeAMD_fn)(
    cl_command_queue ,
    cl_uint          ,
    void**           ,
    void (CL_CALLBACK *)(
        cl_command_queue ,
        cl_uint          ,
        void**           ,
        void*            ),
    void*             ,
    cl_uint           ,
    const cl_event*   ,
    cl_event*
) CL_EXT_SUFFIX__VERSION_1_2;

typedef CL_API_ENTRY cl_int
(CL_API_CALL * clEnqueueSVMMemcpyAMD_fn)(
    cl_command_queue ,
    cl_bool          ,
    void*            ,
    const void*      ,
    size_t           ,
    cl_uint          ,
    const cl_event*  ,
    cl_event*
) CL_EXT_SUFFIX__VERSION_1_2;

typedef CL_API_ENTRY cl_int
(CL_API_CALL * clEnqueueSVMMemFillAMD_fn)(
    cl_command_queue ,
    void*            ,
    const void*      ,
    size_t           ,
    size_t           ,
    cl_uint          ,
    const cl_event*  ,
    cl_event*
) CL_EXT_SUFFIX__VERSION_1_2;

typedef CL_API_ENTRY cl_int
(CL_API_CALL * clEnqueueSVMMapAMD_fn)(
    cl_command_queue ,
    cl_bool          ,
    cl_map_flags     ,
    void*            ,
    size_t           ,
    cl_uint          ,
    const cl_event*  ,
    cl_event*
) CL_EXT_SUFFIX__VERSION_1_2;

typedef CL_API_ENTRY cl_int
(CL_API_CALL * clEnqueueSVMUnmapAMD_fn)(
    cl_command_queue ,
    void*            ,
    cl_uint          ,
    const cl_event*  ,
    cl_event*
) CL_EXT_SUFFIX__VERSION_1_2;

typedef CL_API_ENTRY cl_int
(CL_API_CALL * clSetKernelArgSVMPointerAMD_fn)(
    cl_kernel     ,
    cl_uint       ,
    const void *
) CL_EXT_SUFFIX__VERSION_1_2;

typedef CL_API_ENTRY cl_int
(CL_API_CALL * clSetKernelExecInfoAMD_fn)(
     cl_kernel                ,
     cl_kernel_exec_info_amd  ,
     size_t                   ,
     const void *
) CL_EXT_SUFFIX__VERSION_1_2;

#endif

#endif

#endif
