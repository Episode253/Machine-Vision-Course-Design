

#ifndef OPENCV_CORE_OCL_RUNTIME_OPENCL_SVM_DEFINITIONS_HPP
#define OPENCV_CORE_OCL_RUNTIME_OPENCL_SVM_DEFINITIONS_HPP

#if defined(HAVE_OPENCL_SVM)
#if defined(CL_VERSION_2_0)



#else

typedef cl_bitfield cl_device_svm_capabilities;
typedef cl_bitfield cl_svm_mem_flags;
typedef cl_uint     cl_kernel_exec_info;





#ifndef CL_DEVICE_SVM_CAPABILITIES
#define CL_DEVICE_SVM_CAPABILITIES 0x1053

#define CL_DEVICE_SVM_COARSE_GRAIN_BUFFER             (1 << 0)
#define CL_DEVICE_SVM_FINE_GRAIN_BUFFER               (1 << 1)
#define CL_DEVICE_SVM_FINE_GRAIN_SYSTEM               (1 << 2)
#define CL_DEVICE_SVM_ATOMICS                         (1 << 3)
#endif

#ifndef CL_MEM_SVM_FINE_GRAIN_BUFFER
#define CL_MEM_SVM_FINE_GRAIN_BUFFER (1 << 10)
#endif

#ifndef CL_MEM_SVM_ATOMICS
#define CL_MEM_SVM_ATOMICS (1 << 11)
#endif


#endif
#endif

#endif
